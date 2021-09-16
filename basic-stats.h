
#define INITIAL_LIST_SIZE   1024

#define MALLOC(nelem, type)         ((type *)malloc((nelem) * sizeof(type)))
#define REALLOC(buff, count, type)  ((type *)realloc((buff), (count) * sizeof(type)))

//typedef double  variance_t;

#define MAX_DIGITS          4096    // Big enough for string data

#define MAX_FUNCTIONS       64  // Number of functions computed in 1 run
#define FUNCTION_LIST_INIT  { 0 }
typedef enum
{
    QUANTILE = 0,
    MEDIAN,
    MEAN,
    MODE,
    POP_VAR,
    SAMPLE_VAR,
    POP_STDDEV,
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
    unsigned        partitions; // Quantile partitions

    // For variance use theorom sum[(x - u)^2] = sum[x^2] - (sum[x])^2 / n
    double          sum_x;
    double          sum_x_2;
}   function_t;

typedef struct
{
    unsigned        count;
    function_t  functions[MAX_FUNCTIONS];
}   function_list_t;

#include "basic-stats-protos.h"
