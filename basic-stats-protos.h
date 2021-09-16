/* basic-stats.c */
int main(int argc, char *argv[]);
void usage(char *argv[]);
int add_function(function_list_t *flist, function_code_t new_code, int *c, char *argv[]);
int process_data(function_list_t *flist, const char *delims);
void process_val(function_list_t *flist, size_t c, double x);
void function_list_init(function_list_t *flist);
void quantiles(function_list_t *flist, size_t c, const char *row_col_name, unsigned row_col_value);
int double_cmp(const double *d1, const double *d2);
double variance(variance_t variance_adjust);
