/* basic-stats.c */
int main(int argc, char *argv[]);
void usage(char *argv[]);
int add_function(function_list_t *flist, function_t new_function, int *c, char *argv[]);
int process_data(function_list_t *flist, const char *delims);
void process_val(function_list_t *flist, size_t c, double x);
void function_list_init(function_list_t *flist);
double median(double list[], size_t list_size);
int double_cmp(const double *d1, const double *d2);
double variance(variance_t variance_adjust);
