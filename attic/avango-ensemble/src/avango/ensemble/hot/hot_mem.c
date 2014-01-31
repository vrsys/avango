/****************************************************************************/
#include <assert.h>
#include <string.h>
#include <stdlib.h>
/****************************************************************************/
#include "hot_sys.h"
#include "hot_error.h"
#include "hot_thread.h"
#include "hot_msg.h"
/*#include "hot_ens.h"*/
#include "hot_mem.h"

#ifdef HOT_MEM_CHANNELS

/****************************************************************************/

#define HOT_MEM_MAGIC 0xa1b2c3d4

struct hot_mem {
    const char *name;
    unsigned magic;
    unsigned nalloc;
    unsigned nfree;
    unsigned ninuse;
    unsigned salloc, salloc_K, salloc_M;
    unsigned sfree, sfree_K, sfree_M;
    unsigned sinuse, sinuse_K, sinuse_M;
};
typedef struct hot_mem hot_mem;

static hot_mem memory = {"hot_mem", HOT_MEM_MAGIC};

typedef struct {
    unsigned magic;
    hot_mem *ch;
    unsigned size;
} mem_hdr_t;

static struct {
    int trace_rate;
    hot_lock_t mutex;
    int initialized;
    int op_cnt;
    int in_critical;
    
    hot_mem_t *channels;
    int nchannels;
} g;

static void begin_critical(void) {
    hot_err_t err;
    if ((err = hot_lck_Lock(g.mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err)) ;
    assert(!g.in_critical) ;
    g.in_critical = 1 ;
}

static void end_critical(void) {
    hot_err_t err;
    assert(g.in_critical);
    g.in_critical = 0;
    if ((err = hot_lck_Unlock(g.mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
}

static void init() {
    hot_err_t err;
    char *trace = getenv("HOT_MEM_TRACE");
    
    if (g.initialized)
	hot_sys_Panic("HOT_MEM: double initialization 1");
  
    if ((err = hot_lck_Create(&g.mutex)) != HOT_OK)
	hot_sys_Panic(hot_err_ErrString(err));
  
    begin_critical(); {
	if (g.initialized)
	    hot_sys_Panic("HOT_MEM: double initialization 2");    
	g.initialized = 1;
	if (trace) 
	    g.trace_rate = atoi(trace);
    } end_critical();

    g.channels = hot_mem_Alloc(&memory, 2*sizeof(hot_mem*));
    g.channels[0] = &memory;
    g.nchannels = 1;
}

static void print_stats(void) {
    int i;
    hot_mem *ch;

    for (i = 0; i < g.nchannels; i++) {
	ch = g.channels[i];
	fprintf(stderr,"HOT_MEM: channel: %s\n", ch->name);
	fprintf(stderr,"\tbytes allocated = %4uM + %4uK + %4u, nalloc = %u\n", 
		ch->salloc_M, 
		ch->salloc_K, 
		ch->salloc,
		ch->nalloc);
	fprintf(stderr,"\tbytes freed     = %4uM + %4uK + %4u, nfree  = %u\n", 
		ch->sfree_M, 
		ch->sfree_K, 
		ch->sfree,
		ch->nfree);
	fprintf(stderr,"\tbytes in use    = %4uM + %4uK + %4u, ninuse = %u\n", 
		ch->sinuse_M, 
		ch->sinuse_K, 
		ch->sinuse,
		ch->ninuse);
    }
}

/****************************************************************************/

static void stats_add(
    hot_mem *ch,
    unsigned size
) {
    ch->salloc += size;
    if (ch->salloc >= 1024) {
	ch->salloc_K += ch->salloc / 1024;
	ch->salloc %= 1024;
    }
    if (ch->salloc_K >= 1024) {
	ch->salloc_M += ch->salloc_K / 1024;
	ch->salloc_K %= 1024;
    }
    ch->sinuse += size;
    if (ch->sinuse >= 1024) {
	ch->sinuse_K += ch->sinuse / 1024;
	ch->sinuse %= 1024;
    }
    if (ch->sinuse_K >= 1024) {
	ch->sinuse_M += ch->sinuse_K / 1024;
	ch->sinuse_K %= 1024;
    }	
}

static void stats_release(
    hot_mem *ch,
    unsigned size
) {
    unsigned B,K,M;

    ch->sfree += size;
    if (ch->sfree >= 1024) {
	ch->sfree_K += ch->sfree / 1024;
	ch->sfree %= 1024;
    }
    if (ch->sfree_K >= 1024) {
	ch->sfree_M += ch->sfree_K / 1024;
	ch->sfree_K %= 1024;
    }
    
    B = size;
    K = 0;
    M = 0;
    if (B >= 1024) {
	K = B / 1024;
	B %= 1024;
    }
    if (K >= 1024) {
	M = K / 1024;
	K %= 1024;
    }
    assert(B <= 1023);
    assert(K <= 1023);
    if (ch->sinuse_M < M) {
	print_stats();
	hot_sys_Panic("HOT_MEM: %s: sanity (freeing %u bytes)",
		      ch->name, size);
    }
    ch->sinuse_M -= M;
    if (K > ch->sinuse_K) {
	if (ch->sinuse_M < 1) {
	    print_stats();
	    hot_sys_Panic("HOT_MEM: %s: sanity (freeing %u bytes)",
			  ch->name, size);
	}
	ch->sinuse_M--;
	ch->sinuse_K += 1024;
    }
    ch->sinuse_K -= K;
    if (B > ch->sinuse) {
	if (ch->sinuse_K == 0 && ch->sinuse_M == 0) {
	    print_stats();
	    hot_sys_Panic("HOT_MEM: %s: sanity (freeing %u bytes)",
			  ch->name, size);
	}
	if (ch->sinuse_K == 0) {
	    ch->sinuse_M--;
	    ch->sinuse_K = 1023;
	}
	else {
	    ch->sinuse_K--;
	}
	ch->sinuse += 1024;
    }
    ch->sinuse -= B;
}

/****************************************************************************/

void *hot_mem_Alloc(
    hot_mem_t ch,
    unsigned size
) {
    char *mem;
    unsigned len;
    mem_hdr_t *hdr;

    assert(ch != NULL);
    assert(ch->magic == HOT_MEM_MAGIC);
    assert(size > 0);
    
    if (!g.initialized)
	init();
    
    begin_critical(); {
	ch->nalloc++;
	ch->ninuse++;
	stats_add(ch, size);

	len = size + sizeof(mem_hdr_t);
	mem = (char*) malloc(len);
	if (mem == NULL) {
	    hot_sys_Panic("hot_mem_Alloc: %s: malloc returned NULL", ch->name);
	}
	memset(mem, 0, len);
	hdr = (mem_hdr_t*) mem;
	hdr->magic = HOT_MEM_MAGIC;
	hdr->ch = ch;
	hdr->size = size;
	
	if (g.trace_rate && ++g.op_cnt >= g.trace_rate) {
	    print_stats();
	    g.op_cnt = 0;
	}
    } end_critical();
    return mem + sizeof(mem_hdr_t);
}

void *hot_mem_Realloc(
    void *mem,
    unsigned size
) {
    char *buf = mem;
    mem_hdr_t *hdr;

    assert(mem);
    assert(size > 0);
    assert(g.initialized);
    
    begin_critical(); {  
	hdr = (mem_hdr_t*) (buf - sizeof(mem_hdr_t));
	if (hdr->magic != HOT_MEM_MAGIC ||
	    hdr->ch == NULL             || 
	    hdr->ch->magic != HOT_MEM_MAGIC)
	    hot_sys_Panic("hot_mem_Realloc: invalid or corrupted buffer");

	if (size > hdr->size)
	    stats_add(hdr->ch, size - hdr->size);
	else if (size < hdr->size)
	    stats_release(hdr->ch, hdr->size - size);
	
	hdr = (mem_hdr_t*) realloc(hdr, size + sizeof(mem_hdr_t));
	if (hdr == NULL) {
	    hot_sys_Panic("hot_mem_Relloc: %s: realloc returned NULL", 
			  hdr->ch->name);
	}
	assert(hdr->magic == HOT_MEM_MAGIC && 
	       hdr->ch != NULL             && 
	       hdr->ch->magic == HOT_MEM_MAGIC);
	if (size > hdr->size) {
	    buf = (char*) (hdr + 1); 
	    memset(buf + hdr->size, 0, size - hdr->size);
	}
	hdr->size = size;

	if (g.trace_rate && ++g.op_cnt >= g.trace_rate) {
	    print_stats();
	    g.op_cnt = 0;
	}
    } end_critical();
    return (hdr + 1);
}

void hot_mem_Free(void *mem) {
    char *buf = mem;
    mem_hdr_t *hdr;

    assert(mem != NULL);    
    if (!g.initialized)
	init();
    
    begin_critical(); {
	hdr = (mem_hdr_t*) (buf - sizeof(mem_hdr_t));
	if (hdr->magic != HOT_MEM_MAGIC ||
	    hdr->ch == NULL             || 
	    hdr->ch->magic != HOT_MEM_MAGIC)
	    hot_sys_Panic("hot_mem_Free: invalid or corrupted buffer");

	hdr->ch->nfree++;
	hdr->ch->ninuse--;
	stats_release(hdr->ch, hdr->size);	

	memset(buf - sizeof(mem_hdr_t), 0, hdr->size + sizeof(mem_hdr_t));
	free(buf - sizeof(mem_hdr_t));
	
	if (g.trace_rate && ++g.op_cnt >= g.trace_rate) {
	    print_stats();
	    g.op_cnt = 0;
	}
    } end_critical();
}

hot_mem_t hot_mem_AddChannel(const char *name) {
    hot_mem *ch;
    char *tmp_name ;
    
    assert(name);
    if (!g.initialized)
	init();

    ch = hot_mem_Alloc(&memory, sizeof(hot_mem));
    ch->magic = HOT_MEM_MAGIC;
    tmp_name = hot_mem_Alloc(&memory, strlen(name) + 1);
    strcpy(tmp_name, name);
    ch->name = tmp_name ;

    g.channels = hot_mem_Realloc(g.channels, 
        (g.nchannels + 1) * sizeof(hot_mem*));

    begin_critical(); {
	g.channels[g.nchannels] = ch;
	g.nchannels++;
    } end_critical();

    return ch;
}

#else /*HOT_MEM_CHANNELS*/
/* For these to work, the malloc/free need to be reentrant.
 */
#if 1
void *hot_mem_Alloc(hot_mem_t ch, unsigned size) { return malloc(size); }
void *hot_mem_Realloc(void *mem, unsigned size) { return realloc(mem,size); }
void hot_mem_Free(void *mem) { free(mem); }
#else
#include "gc.h"
void *hot_mem_Alloc(hot_mem_t ch, unsigned size) { return GC_MALLOC(size); }
void *hot_mem_Realloc(void *mem, unsigned size) { return GC_REALLOC(mem,size); }
void hot_mem_Free(void *mem) { GC_FREE(mem); }
#endif
hot_mem_t hot_mem_AddChannel(const char *name) { return NULL; }
#endif /*HOT_MEM_CHANNELS*/

/****************************************************************************/
