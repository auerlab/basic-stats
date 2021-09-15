/* basic-stats.c */
int main(int argc, char *argv[]);
void usage(char *argv[]);
int add_function(function_list_t *function_list, function_t new_function, int *c, char *argv[]);
int process_data(function_list_t *function_list, const char *delims);
void function_list_init(function_list_t *function_list);
double median(void);
int double_cmp(const double *d1, const double *d2);
double variance(variance_t variance_type);
double average(void);
