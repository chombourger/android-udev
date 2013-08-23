/* fix-ups for building udev on Android */

#pragma once

#define _GNU_SOURCE

#define FIRMWARE_PATH  "/system/etc/firmware"
#define UDEVLIBEXECDIR "/system/etc/udev"

#include <grp.h>
#include <sys/signalfd.h>

/* stpcpy */
#include <string.h>
char *stpcpy(char *dest, const char *src);
void *mempcpy(void *dest, const void *src, size_t n);

/* Return an alloca'd copy of at most N bytes of string.  */
# define strndupa(s,n)							\
  (__extension__							\
    ({									\
      const char *__old = (s);						\
      size_t __len = strnlen (__old, (n));				\
      char *__new = (char *) __builtin_alloca (__len + 1);		\
      __new[__len] = '\0';						\
      (char *) memcpy (__new, __old, __len);				\
    }))

/* Work-around redefinition of ALIGN */
#include <sys/param.h>
#ifdef ALIGN
#undef ALIGN
#endif

/* bionic has gettid */
#define HAVE_DECL_GETTID 1

/* bionic has pivot_root */
#define HAVE_DECL_PIVOT_ROOT 1

/* work-around for missing secure_getenv */
#define HAVE_SECURE_GETENV 1 /* claim we have it */
#define secure_getenv getenv /* but alias it to getenv() */

#define __WORDSIZE 32

#include <limits.h>
#define LINE_MAX 2048
#define HOST_NAME_MAX 64

/* get cpu_set_t defined */
#include <sched.h>
#define SCHED_IDLE 5 /* not defined in bionic */

/* bionic defines CPU_ALLOC with a trailing semi-colon... */
#undef CPU_ALLOC
#define CPU_ALLOC(count)   __sched_cpualloc((count))

/* dummy (and incorrect) locale_t definition */
typedef unsigned int locale_t;

/* noop locale "functions" */
#define newlocale(x,y,z) 0
#define uselocale(x) 0
#define freelocale(x) do {} while (0)

/* use gcc builtins for __bswap_XX */
#define __bswap_16(x) ((uint16_t)(__builtin_bswap32 (x << 16)))
#define __bswap_32(x) __builtin_bswap32(x)
#define __bswap_64(x) __builtin_bswap64(x)

/* work-arounds for duplicate definitions of endian functions */
#define htole16 sys_htole16
#define htole32 sys_htole32
#define htole64 sys_htole64
#define htobe16 sys_htobe16
#define htobe32 sys_htobe32
#define htobe64 sys_htobe64
#include <sys/endian.h>
#undef htole16
#undef htole32
#undef htole64
#undef htobe16
#undef htobe32
#undef htobe64

/* struct stat in bionic lacks st_atim, st_mtim, st_ctim */
#define stat sys_stat
#define fstat sys_fstat
#define fstatat sys_fstatat
#define lstat sys_lstat
#include <sys/stat.h>
#undef stat
#undef fstat
#undef fstatat
#undef lstat

struct stat {
    unsigned long long  st_dev;
    unsigned char       __pad0[4];

    unsigned long       __st_ino;
    unsigned int        st_mode;
    unsigned int        st_nlink;

    unsigned long       st_uid;
    unsigned long       st_gid;

    unsigned long long  st_rdev;
    unsigned char       __pad3[4];

    long long           st_size;
    unsigned long       st_blksize;
    unsigned long long  st_blocks;

# define st_atime st_atim.tv_sec
# define st_mtime st_mtim.tv_sec
# define st_ctime st_ctim.tv_sec

    struct timespec st_atim;
    struct timespec st_mtim;
    struct timespec st_ctim;

    unsigned long long  st_ino;
};

extern int stat(const char *, struct stat *);
extern int fstat(int, struct stat *);
extern int fstatat(int dirfd, const char *path, struct stat *buf, int flags);
extern int lstat(const char *, struct stat *);

/* bionic does not define those */
#define SOCK_CLOEXEC    02000000
#define SOCK_NONBLOCK   04000
#define IN_CLOEXEC      02000000
#define IN_NONBLOCK     04000
#define EPOLL_CLOEXEC   02000000
#define RAMFS_MAGIC     0x858458f6
#define	LOG_NFACILITIES	24
#define	IPTOS_LOWCOST   0x02

/* workaround for canonicalize_file_name */
extern char *canonicalize_file_name(const char *path);

#define program_invocation_name "udev"

#define nl_langinfo(x) "UTF-8"

/* for open_memstream() */
#include <cutils/open_memstream.h>

/* missing syscalls */
#include <sys/syscall.h>

#ifdef __arm__
#define __NR_readlinkat		332
#define __NR_signalfd4		355
#define __NR_epoll_create1	357
#define __NR_dup3		358
#define __NR_inotify_init1	360
#define __NR_accept4		366
#else
#endif

#define readlinkat(dirfd,pathname,buf,bufsiz) syscall(__NR_readlinkat,dirfd,pathname,buf,bufsiz)
#define epoll_create1(flags) syscall(__NR_epoll_create1,flags)
#define dup3(fd,fd2,flags) syscall(__NR_dup3,fd,fd2,flags)
#define inotify_init1(flags) syscall(__NR_inotify_init1, flags)
#define accept4(sockfd,addr,addrlen,flags) syscall(__NR_accept4,sockfd,addr,addrlen,flags)

/* work-around for missing mkostemp() */
#define mkostemp(t,f) mkstemp(t)

/* work-around for missing get_current_dir_name() */
#define get_current_dir_name() getcwd(0,0)

/* work-around for missing statvfs() */
#define statvfs(p,s) -1

/* for stuff imported from uClibc such as glob.[ch] */
#define __USE_GNU 1
#define __UCLIBC_HAS_GNU_GLOB__ 1
#define __THROW   __attribute__ ((__nothrow__))
#define libc_hidden_proto(x)
#define attribute_hidden __attribute__ ((visibility ("hidden")))
#define libc_hidden_def(name)
#define true 1

/* Nonzero if STATUS indicates the child continued after a stop. */
#define W_CONTINUED      8       /* Report continued child.  */
#define WIFCONTINUED(status) ((status) == W_CONTINUED)

typedef int (*__compar_fn_t) (__const void *, __const void *);
typedef int (*__compar_d_fn_t) (const void *, const void *, void *);

/* no systemd, no need to sd_notify() */
#include "sd-daemon.h"
#define sd_notify(a,b) do {} while(0)
#define sd_listen_fds(x) -1
#define sd_is_socket(a,b,c,d) 0

#include "cgroup-util.h"
#define cg_pid_get_path(a,b,c) -1
#define cg_kill(a,b,c,d,e,f) do {} while(0)

/* missing qsort_r */
void qsort_r(void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *),
		void *arg);

/* work-around issue of sysconf(_SC_GETPW_R_SIZE_MAX) returning -1 */
#include <sys/sysconf.h>
extern long udev_sysconf(int name);
#define sysconf(x) udev_sysconf(x)

