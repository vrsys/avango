// $Header$
// 
// Utility for performance tests.
//
// Author:  Alexey Vaysburd, Sept. 1997.

#ifndef __MAESTRO_PERFTEST_H__
#define __MAESTRO_PERFTEST_H__

#include <iostream>

/* jsd: added cond */
#if defined(__GNUC__)
#pragma interface
#endif

#include <avango/ensemble/maestro/Maestro_Types.h>

#ifdef PERFTEST
#define perf_reset()       perf.reset()
#define perf_start(n,d)    perf.start(n,d)
#define perf_end(n)        perf.end(n)
#define perf_print()       perf.print()
#else
#define perf_reset()
#define perf_start(n,d)
#define perf_end(n)
#define perf_print()
#endif

#define PERFPTS 100

class Maestro_Perf {
public:
  Maestro_Perf() {
    reset();
  }

  void reset() {
    memset(total_latency, 0, sizeof(*total_latency) * PERFPTS);
    memset(ncalls, 0, sizeof(*ncalls) * PERFPTS);
    memset(before, 0, sizeof(*before) * PERFPTS);
    memset(after, 0, sizeof(*after) * PERFPTS);
  }

  void start(int point, char *_descr = NULL) {
    ncalls[point]++;
    if (descr[point].size == 1 && _descr != NULL) {
      descr[point] = _descr;
    }
    gettimeofday(&before[point], NULL);
  }

  void end(int point) {
    gettimeofday(&after[point], NULL);
    total_latency[point] += 
      (after[point].tv_sec - before[point].tv_sec) * 1000000 +
      (after[point].tv_usec - before[point].tv_usec);
  }

  void print() {
    int i;
    std::cout << "****************** Performance results *****************" << std::endl;
    for (i = 0; i < PERFPTS; i++) {
      if (ncalls[i] > 0) {
        std::cout << i << ": "
                  << descr[i] << " "
                  << (total_latency[i] / ncalls[i]) 
                  << " usec (over " 
                  << ncalls[i] 
                  << " calls)" 
                  << std::endl;
      }
    }
    std::cout << "********************************************************" << std::endl;
    reset();
  }

private:
  Maestro_String descr[PERFPTS];
  unsigned long total_latency[PERFPTS];
  unsigned long ncalls[PERFPTS];
  struct timeval before[PERFPTS], after[PERFPTS];
};

extern Maestro_Perf perf;

#endif // __MAESTRO_PERFTEST_H__
