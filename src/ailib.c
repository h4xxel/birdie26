#include "ailib.h"
#include <string.h>

static struct AILibEntry ailib[] = {
	{ NULL, NULL }
};


void *ailib_get(const char *str) {
	int i;

	for (i = 0; ailib[i].symbol; i++) {
		if (!strcmp(ailib[i].symbol, str))
			return (void *) ailib[i].symbol;
	}

	return NULL;
}
