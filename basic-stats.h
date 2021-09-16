
#define INITIAL_LIST_SIZE   1024

#define MALLOC(nelem, type)         ((type *)malloc((nelem) * sizeof(type)))
#define REALLOC(buff, count, type)  ((type *)realloc((buff), (count) * sizeof(type)))

typedef double  variance_t;

/* 
 *  These are subtracted from the denominator when computing sample or
 *  population variance, so they must be 0 and 1.
 */
#define POPULATION_VARIANCE_ADJUST  0.0
#define SAMPLE_VARIANCE_ADJUST      1.0

#define MAX_DIGITS          4096    // Big enough for string data

#define MAX_FUNCTIONS       64  // Number of functions computed in 1 run
#define FUNCTION_LIST_INIT  { 0 }
typedef enum
{
    QUANTILE = 0,
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
}   function_code_t;

typedef struct
{
    function_code_t code;       // MEAN, MEDIAN, etc.
    unsigned        row;        // 1-based row in input
    unsigned        col;        // 1-based col in input
    double          sum;        // Mean
    double          *nums;      // Array for sorting for quantiles
    size_t          num_count;
    size_t          array_size;
    FILE            *temp_file; // Second pass for variance relevant data only
    unsigned        partitions; // Quantile partitions
}   function_t;

typedef struct
{
    unsigned        count;
    function_t  functions[MAX_FUNCTIONS];
}   function_list_t;

#include "basic-stats-protos.h"
