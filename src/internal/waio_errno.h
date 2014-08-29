#ifndef _WAIO_ERRNO_H_
#define _WAIO_ERRNO_H_

#define EPERM            1
#define ENOENT           2
#define EINTR            4
#define EIO              5
#define EBADF            9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define EBUSY           16
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define ENOSYS          38
#define ENOTEMPTY       39
#define ELOOP           40
#define EWOULDBLOCK     EAGAIN
#define ENOMSG          42
#define EBADMSG         74
#define EOVERFLOW       75
#define EOPNOTSUPP      95
#define ENOTSUP         EOPNOTSUPP
#define EPFNOSUPPORT    96
#define EAFNOSUPPORT    97
#define EADDRINUSE      98
#define EADDRNOTAVAIL   99

#endif /* _WAIO_ERRNO_H_ */
