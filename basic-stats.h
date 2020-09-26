
#define INITIAL_LIST_SIZE   1024

#define MALLOC(nelem, type)         ((type *)malloc((nelem) * sizeof(type)))
#define REALLOC(buff, count, type)  ((type *)realloc((buff), (count) * sizeof(type)))

typedef double  variance_t;
#define POPULATION_VARIANCE 0.0
#define SAMPLE_VARIANCE     1.0

#include "basic-stats-protos.h"
