#include "stdio_impl.h"
#include <sys/uio.h>

size_t __stdio_read(FILE *f, unsigned char *buf, size_t len)
{
	struct iovec iov[2] = {
		{ .iov_base = buf, .iov_len = len - !!f->buf_size },
		{ .iov_base = f->buf, .iov_len = f->buf_size }
	};
	ssize_t cnt;

	if (iov[0].iov_len) {
#ifdef SYS_readv
		cnt = syscall(SYS_readv, f->fd, iov, 2);
#else
		cnt = readv(f->fd, iov, 2);
#endif
	} else {
		cnt = syscall(SYS_read, f->fd, iov[1].iov_base, iov[1].iov_len);
	}
	if (cnt <= 0) {
		f->flags |= cnt ? F_ERR : F_EOF;
		return 0;
	}
	if (cnt <= iov[0].iov_len) return cnt;
	cnt -= iov[0].iov_len;
	f->rpos = f->buf;
	f->rend = f->buf + cnt;
	if (f->buf_size) buf[len-1] = *f->rpos++;
	return len;
}
