/* statsf-mutators.c */
int statsf_set_code(statsf_t *statsf_ptr, statsf_code_t new_code);
int statsf_set_row(statsf_t *statsf_ptr, unsigned new_row);
int statsf_set_col(statsf_t *statsf_ptr, unsigned new_col);
int statsf_set_rows(statsf_t *statsf_ptr, unsigned *new_rows);
int statsf_set_rows_ae(statsf_t *statsf_ptr, size_t c, unsigned new_rows_element);
int statsf_set_rows_cpy(statsf_t *statsf_ptr, unsigned *new_rows, size_t array_size);
int statsf_set_cols(statsf_t *statsf_ptr, unsigned *new_cols);
int statsf_set_cols_ae(statsf_t *statsf_ptr, size_t c, unsigned new_cols_element);
int statsf_set_cols_cpy(statsf_t *statsf_ptr, unsigned *new_cols, size_t array_size);
int statsf_set_sum(statsf_t *statsf_ptr, double new_sum);
int statsf_set_nums(statsf_t *statsf_ptr, double *new_nums);
int statsf_set_nums_ae(statsf_t *statsf_ptr, size_t c, double new_nums_element);
int statsf_set_nums_cpy(statsf_t *statsf_ptr, double *new_nums, size_t array_size);
int statsf_set_num_count(statsf_t *statsf_ptr, size_t new_num_count);
int statsf_set_array_size(statsf_t *statsf_ptr, size_t new_array_size);
int statsf_set_partitions(statsf_t *statsf_ptr, unsigned new_partitions);
int statsf_set_sum_x(statsf_t *statsf_ptr, double new_sum_x);
int statsf_set_sum_x_2(statsf_t *statsf_ptr, double new_sum_x_2);
int statsf_set_row_sums(statsf_t *statsf_ptr, double *new_row_sums);
int statsf_set_row_sums_ae(statsf_t *statsf_ptr, size_t c, double new_row_sums_element);
int statsf_set_row_sums_cpy(statsf_t *statsf_ptr, double *new_row_sums, size_t array_size);
int statsf_set_col_sums(statsf_t *statsf_ptr, double *new_col_sums);
int statsf_set_col_sums_ae(statsf_t *statsf_ptr, size_t c, double new_col_sums_element);
int statsf_set_col_sums_cpy(statsf_t *statsf_ptr, double *new_col_sums, size_t array_size);
int statsf_set_expected_mean(statsf_t *statsf_ptr, double new_expected_mean);
int statsf_set_tmp_file(statsf_t *statsf_ptr, FILE *new_tmp_file);
int statsf_set_tmp_file_ae(statsf_t *statsf_ptr, size_t c, FILE new_tmp_file_element);
int statsf_set_tmp_file_cpy(statsf_t *statsf_ptr, FILE *new_tmp_file, size_t array_size);
