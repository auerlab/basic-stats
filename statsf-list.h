
#include "statsf.h"

#define SFL_INITIAL_LIST_SIZE   1024
#define SFL_MAX_FUNCTIONS       64      // Max functions computed in 1 pass
#define SFL_MAX_FIELD_LEN       4096    // Any field, not just numeric

#define SFL_FLAG_NONE           0x00
#define SFL_FLAG_VERBOSE        0x01

typedef struct
{
    unsigned    count;
    statsf_t    functions[SFL_MAX_FUNCTIONS];
}   statsf_list_t;

#include "statsf-list-rvs.h"
#include "statsf-list-accessors.h"
#include "statsf-list-mutators.h"

#include "statsf-list-protos.h"
#include "statsf-list-mutators-protos.h"
