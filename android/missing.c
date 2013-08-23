
#include <string.h>

void *mempcpy(void * __restrict s1, const void * __restrict s2, size_t n)
{
	register char *r1 = s1;
	register const char *r2 = s2;

	while (n) {
		*r1++ = *r2++;
		--n;
	}

	return r1;
}

char *stpcpy(register char * __restrict s1, const char * __restrict s2)
{
	while ((*s1++ = *s2++) != 0);
	return s1 - 1;
}

char *canonicalize_file_name(const char *path)
{
	char *resolved, *result=NULL;
	
	resolved = malloc(PATH_MAX);
	if (resolved != NULL) {
		result = realpath(path, resolved);
		if (result == NULL) free(resolved);
	}
	return result;
}

