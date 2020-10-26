
#define INITIAL_LIST_SIZE   1024

#define MALLOC(nelem, type)         ((type *)malloc((nelem) * sizeof(type)))
#define REALLOC(buff, count, type)  ((type *)realloc((buff), (count) * sizeof(type)))

typedef double  variance_t;

/* 
 *  These are subtracted from the denominator when computing sample or
 *  population variance, so they must be 0 and 1.
 */
#define POPULATION_VARIANCE 0.0
#define SAMPLE_VARIANCE     1.0

#include "basic-stats-protos.h"
