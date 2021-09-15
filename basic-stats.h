
#define INITIAL_LIST_SIZE   1024

#define MALLOC(nelem, type)         ((type *)malloc((nelem) * sizeof(type)))
#define REALLOC(buff, count, type)  ((type *)realloc((buff), (count) * sizeof(type)))

typedef double  variance_t;

/* 
 *  These are subtracted from the denominator when computing sample or
 *  population variance, so they must be 0 and 1.
 */
#define POPULATION_VARIANCE_ADJUST 0.0
#define SAMPLE_VARIANCE_ADJUST     1.0

#define MAX_DIGITS          64

#define MAX_FUNCTIONS       64  // Number of functions computed in 1 run
#define FUNCTION_LIST_INIT  { 0 }
typedef enum
{
    MEDIAN,
    MEAN,
    MODE,
    POPULATION_VARIANCE,
    SAMPLE_VARIANCE,
    POPULATION_STDDEV,
    SAMPLE_STDDEV,
    RANGE,
    IQ_RANGE,
    BOX_PLOT,
    Z_SCORES
}   function_t;
    
typedef struct
{
    unsigned    count;
    function_t  functions[MAX_FUNCTIONS];
    unsigned    rows[MAX_FUNCTIONS];
    unsigned    cols[MAX_FUNCTIONS];
    unsigned    n[MAX_FUNCTIONS];
    double      sum[MAX_FUNCTIONS];
}   function_list_t;

#include "basic-stats-protos.h"
