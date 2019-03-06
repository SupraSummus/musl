#include <sys/uio.h>
#include "syscall.h"

#ifndef SYS_readv
// we need read()
#include <unistd.h>
#endif

ssize_t readv(int fd, const struct iovec *iov, int count)
{
#ifdef SYS_readv
	return syscall_cp(SYS_readv, fd, iov, count);
#else
	ssize_t ret = 0;
	for (int i = 0; i < count; i ++) {
		ssize_t r = read(fd, iov[i].iov_base, iov[i].iov_len);
		if (r < 0) return r;
		ret += r;
		if (r < iov[i].iov_len) return ret;
	}
#endif
}
