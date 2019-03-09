#include <sys/uio.h>
#include "syscall.h"

#ifndef SYS_writev
// we need write()
#include <unistd.h>
#endif

ssize_t writev(int fd, const struct iovec *iov, int count)
{
#ifdef SYS_writev
	return syscall_cp(SYS_writev, fd, iov, count);
#else
	ssize_t ret = 0;
	for (int i = 0; i < count; i ++) {
		ssize_t r = write(fd, iov[i].iov_base, iov[i].iov_len);
		if (r < 0) return r;
		ret += r;
		if (r < iov[i].iov_len) return ret;
	}
	return ret;
#endif
}
