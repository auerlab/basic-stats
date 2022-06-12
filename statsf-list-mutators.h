
/*
 *  Generated by /usr/local/bin/auto-gen-get-set
 *
 *  Mutator functions for setting with no sanity checking.  Use these to
 *  set structure members from functions outside the statsf_list_t
 *  class.  These macros perform no data validation.  Hence, they achieve
 *  maximum performance where data are guaranteed correct by other means.
 *  Use the mutator functions (same name as the macro, but lower case)
 *  for more robust code with a small performance penalty.
 *
 *  These generated macros are not expected to be perfect.  Check and edit
 *  as needed before adding to your code.
 */

/* temp-statsf-list-mutators.c */
int statsf_list_set_count(statsf_list_t *statsf_list_ptr, unsigned new_count);
int statsf_list_set_functions_ae(statsf_list_t *statsf_list_ptr, size_t c, statsf_t new_functions_element);
int statsf_list_set_functions_cpy(statsf_list_t *statsf_list_ptr, statsf_t new_functions[], size_t array_size);
