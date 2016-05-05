#include <stdlib.h>
#include <string.h>


char *util_binrel_path(const char *file) {
	char *path;


	asprintf(&path, "%s/%s", d_fs_exec_path(), file);
	return path;
}
