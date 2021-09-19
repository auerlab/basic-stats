/* statsf-list-mutators.c */
int statsf_list_set_count(statsf_list_t *statsf_list_ptr, unsigned new_count);
int statsf_list_set_functions_ae(statsf_list_t *statsf_list_ptr, size_t c, statsf_t new_functions_element);
int statsf_list_set_functions_cpy(statsf_list_t *statsf_list_ptr, statsf_t new_functions[], size_t array_size);
