/* statsf.c */
void statsf_process_val(statsf_t *function, double x);
void statsf_init(statsf_t *function);
void statsf_quantiles(statsf_t *function, const char *row_col_name, unsigned row_col_value);
double statsf_sum_squares(statsf_t *function);
double z_score(double x, double mean, double stddev);
double z_equivalent(double x, double mean, double stddev, double new_mean, double new_stddev);
double z_cdf(double x, double mean, double stddev);
double t_score(double x_bar, double expected_mean, double sample_stddev, unsigned n);
double hypergeometric(double a, double b, double c, double x);
double t_cdf(double x_bar, double expected_mean, double sample_stddev, unsigned n);
