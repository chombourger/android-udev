
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

int getgrnam_r(const char *name, struct group *grp, char *buf, size_t buflen, struct group **result)
{
	struct group *g = getgrnam(name);

	if (g != NULL) {
		grp->gr_gid = g->gr_gid;
		strncpy(buf, g->gr_name, buflen);
		grp->gr_name = buf;
		grp->gr_passwd = NULL; /* not used in udev */
		grp->gr_mem = NULL; /* not used in udev */
		*result = grp;
		return 0;
	}
	*result = NULL;
	return -1;
}

#undef sysconf
long udev_sysconf(int name)
{
	switch(name) {
		case _SC_GETPW_R_SIZE_MAX: return 1024;
		default: return sysconf(name);
	}
}

