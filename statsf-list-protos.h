/* statsf-list.c */
int statsf_list_add_func(statsf_list_t *flist, statsf_code_t new_code, int *c, char *argv[]);
int statsf_list_process_stream(statsf_list_t *flist, FILE *stream, const char *delims, int flags);
void statsf_list_init(statsf_list_t *flist);
