/* statsf-mutators.c */
int statsf_set_code(statsf_t *statsf_ptr, statsf_code_t new_code);
int statsf_set_row(statsf_t *statsf_ptr, unsigned new_row);
int statsf_set_col(statsf_t *statsf_ptr, unsigned new_col);
int statsf_set_sum(statsf_t *statsf_ptr, double new_sum);
int statsf_set_nums(statsf_t *statsf_ptr, double *new_nums);
int statsf_set_nums_ae(statsf_t *statsf_ptr, size_t c, double new_nums_element);
int statsf_set_nums_cpy(statsf_t *statsf_ptr, double *new_nums, size_t array_size);
int statsf_set_num_count(statsf_t *statsf_ptr, size_t new_num_count);
int statsf_set_array_size(statsf_t *statsf_ptr, size_t new_array_size);
int statsf_set_partitions(statsf_t *statsf_ptr, unsigned new_partitions);
int statsf_set_sum_x(statsf_t *statsf_ptr, double new_sum_x);
int statsf_set_sum_x_2(statsf_t *statsf_ptr, double new_sum_x_2);
