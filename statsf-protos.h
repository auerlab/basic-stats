/* statsf.c */
void statsf_process_val(statsf_t *function, double x);
void statsf_init(statsf_t *function);
void statsf_quantiles(statsf_t *function, const char *row_col_name, unsigned row_col_value);
double statsf_sum_squares(statsf_t *function);
