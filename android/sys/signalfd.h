
#pragma once

#include <linux/types.h>
#include <sys/types.h>

#define signalfd(fd,mask,flags) syscall(__NR_signalfd4,fd,mask,sizeof(*mask),flags)
#define SFD_NONBLOCK 04000
#define SFD_CLOEXEC  02000000

struct signalfd_siginfo {
	__u32 ssi_signo;
	__s32 ssi_errno;
	__s32 ssi_code;
	__u32 ssi_pid;
	__u32 ssi_uid;
	__s32 ssi_fd;
	__u32 ssi_tid;
	__u32 ssi_band;
	__u32 ssi_overrun;
	__u32 ssi_trapno;
	__s32 ssi_status;
	__s32 ssi_int;
	__u64 ssi_ptr;
	__u64 ssi_utime;
	__u64 ssi_stime;
	__u64 ssi_addr;

	/*
	 * Pad strcture to 128 bytes. Remember to update the
	 * pad size when you add new members. We use a fixed
	 * size structure to avoid compatibility problems with
	 * future versions, and we leave extra space for additional
	 * members. We use fixed size members because this strcture
	 * comes out of a read(2) and we really don't want to have
	 * a compat on read(2).
	 */
	__u8 __pad[48];
};

/* 64-bit sigset_t type just like the kernel expects,
 * signalfd() aborts if 32-bit sigset_t is being used */
typedef struct {
	unsigned long sigs[2];
} udev_sigset_t;

/* get bionic's bogus sigset_t definition */
#include <asm/signal.h>

/* switch to the 64-bit definition */
#define sigset_t udev_sigset_t
#include <signal.h>

