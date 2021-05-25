typedef struct yt_field
{
	char *field_name;
	char *field_define_type;
};
int create_libyt_module();
int init_libyt_module(const char *fname);
int init_python(int, char **);
int yt_set_parameter(const char *prop_name, const int *data_array, const int len);
int yt_set_field_list(const char *prop_name, int num, struct yt_field **field_list);
int yt_inline(const char *fname);