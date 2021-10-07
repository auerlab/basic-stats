#ifndef _STDIO_H_
#include <stdio.h>
#endif

typedef enum
{
    STATSF_QUANTILE = 0,
    STATSF_MEDIAN,
    STATSF_QUARTILE,
    STATSF_MEAN,
    STATSF_MODE,
    STATSF_POP_VAR,
    STATSF_POP_STDDEV,
    STATSF_SAMPLE_VAR,
    STATSF_SAMPLE_STDDEV,
    STATSF_SAMPLE_STDERR,
    STATSF_RANGE,
    STATSF_IQ_RANGE,
    STATSF_BOX_PLOT,
    STATSF_Z_SCORES,
    STATSF_CHI_FIT,
    STATSF_CHI_IND
}   statsf_code_t;

typedef struct
{
    statsf_code_t   code;       // MEAN, MEDIAN, etc.
    unsigned        row;        // 1-based row in input
    unsigned        col;        // 1-based col in input
    unsigned        *rows;      // List of rows for multirow (e.g. chi-sq)
    unsigned        *cols;      // List of cols for multicol (e.g. chi-sq)
    double          sum;        // Mean
    double          *nums;      // Array for sorting for quantiles
    size_t          num_count;
    size_t          array_size;
    unsigned        partitions; // Quantile partitions

    // For variance use theorom sum[(x - u)^2] = sum[x^2] - (sum[x])^2 / n
    double          sum_x;
    double          sum_x_2;
    double          *row_sums;  // List of row sums for multirow (e.g. chi-sq)
    double          *col_sums;  // List of col sums for multirow (e.g. chi-sq)
}   statsf_t;


/* Return values for mutator functions */
#define STATSF_DATA_OK              0
#define STATSF_DATA_INVALID         -1      // Catch-all for non-specific error
#define STATSF_DATA_OUT_OF_RANGE    -2
    
/*
 *  Generated by /usr/local/bin/auto-gen-get-set
 *
 *  Accessor macros.  Use these to access structure members from functions
 *  outside the statsf_t class.
 *
 *  These generated macros are not expected to be perfect.  Check and edit
 *  as needed before adding to your code.
 */

#define STATSF_CODE(ptr)                ((ptr)->code)
#define STATSF_ROW(ptr)                 ((ptr)->row)
#define STATSF_COL(ptr)                 ((ptr)->col)
#define STATSF_SUM(ptr)                 ((ptr)->sum)
#define STATSF_NUMS(ptr)                ((ptr)->nums)
#define STATSF_NUMS_AE(ptr,c)           ((ptr)->nums[c])
#define STATSF_NUM_COUNT(ptr)           ((ptr)->num_count)
#define STATSF_ARRAY_SIZE(ptr)          ((ptr)->array_size)
#define STATSF_PARTITIONS(ptr)          ((ptr)->partitions)
#define STATSF_SUM_X(ptr)               ((ptr)->sum_x)
#define STATSF_SUM_X_2(ptr)             ((ptr)->sum_x_2)

/*
 *  Generated by /usr/local/bin/auto-gen-get-set
 *
 *  Mutator macros for setting with no sanity checking.  Use these to
 *  set structure members from functions outside the statsf_t
 *  class.  These macros perform no data validation.  Hence, they achieve
 *  maximum performance where data are guaranteed correct by other means.
 *  Use the mutator functions (same name as the macro, but lower case)
 *  for more robust code with a small performance penalty.
 *
 *  These generated macros are not expected to be perfect.  Check and edit
 *  as needed before adding to your code.
 */

#define STATSF_SET_CODE(ptr,val)                ((ptr)->code = (val))
#define STATSF_SET_ROW(ptr,val)                 ((ptr)->row = (val))
#define STATSF_SET_COL(ptr,val)                 ((ptr)->col = (val))
#define STATSF_SET_SUM(ptr,val)                 ((ptr)->sum = (val))
#define STATSF_SET_NUMS(ptr,val)                ((ptr)->nums = (val))
// FIXME: Assuming all elements should be copied
#define STATSF_SET_NUMS_CPY(ptr,val,array_size) \
    for (size_t c = 0; c < (array_size); ++c) (ptr)->nums[c] = val[c];
#define STATSF_SET_NUMS_AE(ptr,c,val)           ((ptr)->nums[c] = (val))
#define STATSF_SET_NUM_COUNT(ptr,val)           ((ptr)->num_count = (val))
#define STATSF_SET_ARRAY_SIZE(ptr,val)          ((ptr)->array_size = (val))
#define STATSF_SET_PARTITIONS(ptr,val)          ((ptr)->partitions = (val))
#define STATSF_SET_SUM_X(ptr,val)               ((ptr)->sum_x = (val))
#define STATSF_SET_SUM_X_2(ptr,val)             ((ptr)->sum_x_2 = (val))

#include "statsf-protos.h"
#include "statsf-mutators-protos.h"
