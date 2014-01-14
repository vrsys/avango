/**************************************************************/
/* MPIC.C: MPI C stubs */
/* Author: Mark Hayden, 11/97 */
/* Originally based on code by Jocelyn Serot. */
/**************************************************************/
#include <stdio.h>
#include <assert.h>
#include "mpi.h"
#include "caml/mlvalues.h"
#include "caml/alloc.h"
#include "caml/memory.h"
/**************************************************************/

value mpi_init(value arg_v) {	/* ML */
  int argc ;
  char **argv ;
  value ret_v ;
  int ret ;
  int i;

  argc = Wosize_val(arg_v);
  argv = (char **) stat_alloc((argc+1)*sizeof(char*));

  /* Copy args and put a NULL at the end.
   */
  for (i=0;i<argc;i++)
    argv[i] = String_val(Field(arg_v,i)) ;
  argv[argc] = NULL ;

  /* Initialize MPI.
   */
  MPI_Init(&argc, &argv);

  /* Enable the returning of error code to me.
   */
  ret = MPI_Errhandler_set(MPI_COMM_WORLD, MPI_ERRORS_RETURN) ;
  assert(ret == MPI_SUCCESS) ;

  /* MPI is not being a good citizen and putting a NULL at the end,
   * so we will do this ourselves.  This should be OK as mpich uses
   * the space we gave it.  
   */
  argv[argc] = NULL ;

  ret_v = copy_string_array(argv) ;
  
  /* This call to stat_free seems to be causing
   * a segfault.  Maybe MPI is keeping a copy (?).
   */
  /*stat_free((char*) argv) ;*/

  return ret_v ;
}

value mpi_finalize(value unit) { /* ML */
  MPI_Finalize();
  return Val_unit;
}

value mpi_abort(value unit) {	/* ML */
  MPI_Abort(MPI_COMM_WORLD, 1);
  return Val_unit;
}

value mpi_nmembers(value unit) { /* ML */
  int size;
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  return Val_int(size);
}

value mpi_rank(value unit) {	/* ML */
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  return Val_int(rank);
}

value mpi_get_processor_name(value unit) { /* ML */
  char buf[MPI_MAX_PROCESSOR_NAME+1] ;
  int len ;
  MPI_Get_processor_name(buf, &len) ;
  buf[len] = '\0' ;
  return copy_string(buf) ;
}

value mpi_buffer(value size_v) { /* ML */
  long size ;
  int ret ;
  void *buf ;
  size = Long_val(size_v) ;
  assert(size>0) ;
  buf = stat_alloc(size) ;
  ret = MPI_Buffer_attach(buf,size) ;
  assert(ret == MPI_SUCCESS) ;
  return Val_unit ;
}

value mpi_send(			/* ML */
        value dests_v,
	value buf_v,
	value ofs_v,
	value len_v
) {
  long dest ;
  long len ;
  long ndests ;
  long i ;
  char *buf ;
  int ret ;

  ndests = Wosize_val(dests_v) ;
  buf = (char*)&Byte(buf_v,Long_val(ofs_v)) ;
  len = Long_val(len_v) ;
  for (i=0;i<ndests;i++) {
    dest = Long_val(Field(dests_v,i)) ;

    /* We use the buffered send so that this code does not
     * wait for a matching recv (which can cause deadlock).
     */
    ret = MPI_Bsend(buf, len, MPI_BYTE, 
		    dest, 0/*MPI_ANY_TAG*/, MPI_COMM_WORLD) ;
    assert(ret == MPI_SUCCESS) ;
  }
  
  return Val_unit ;
}

value mpi_recv(			/* ML */
      value buf_v,
      value ofs_v,
      value len_v
) {
  long ofs ;
  long len ;
  int flag ;
  int rlen ;
  int ret ;
  MPI_Status st ;

  ret = MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &st) ;
  assert(ret == MPI_SUCCESS) ;
  if (!flag)
    return Val_int(0) ;
  ret = MPI_Get_count(&st, MPI_UNSIGNED_CHAR, &rlen) ;
  assert(ret == MPI_SUCCESS) ;
  len = Long_val(len_v) ;
  ofs = Long_val(ofs_v) ;
  if (rlen > len) return -1 ;
  ret = MPI_Recv(&Byte(buf_v,ofs), rlen, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st) ;
  assert(ret == MPI_SUCCESS) ;
  return Val_int(rlen) ;
}

/**************************************************************/
