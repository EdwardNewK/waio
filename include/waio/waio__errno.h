#ifndef _WAIO__ERRNO_H_
#define _WAIO__ERRNO_H_

#define WAIO_EPERM             1
#define WAIO_ENOENT            2
#define WAIO_EINTR             4
#define WAIO_EIO               5
#define WAIO_ENXIO             6
#define WAIO_EBADF             9
#define WAIO_ECHILD           10
#define WAIO_EAGAIN           11
#define WAIO_ENOMEM           12
#define WAIO_EACCES           13
#define WAIO_EFAULT           14
#define WAIO_EBUSY            16
#define WAIO_ENODEV           19
#define WAIO_ENOTDIR          20
#define WAIO_EISDIR           21
#define WAIO_EINVAL           22
#define WAIO_ENFILE           23
#define WAIO_EMFILE           24
#define WAIO_ENOTTY           25
#define WAIO_EFBIG            27
#define WAIO_ENOSPC           28
#define WAIO_ESPIPE           29
#define WAIO_EROFS            30
#define WAIO_EMLINK           31
#define WAIO_EPIPE            32
#define WAIO_ENOSYS           38
#define WAIO_ENOTEMPTY        39
#define WAIO_ELOOP            40
#define WAIO_EWOULDBLOCK      WAIO_EAGAIN
#define WAIO_ENOMSG           42
#define WAIO_EBADMSG          74
#define WAIO_EOVERFLOW        75
#define WAIO_EOPNOTSUPP       95
#define WAIO_ENOTSUP          WAIO_EOPNOTSUPP
#define WAIO_EPFNOSUPPORT     96
#define WAIO_EAFNOSUPPORT     97
#define WAIO_EADDRINUSE       98
#define WAIO_EADDRNOTAVAIL    99
#define WAIO_EINPROGRESS     115
#define WAIO_ECANCELED       125

#endif /* _WAIO__ERRNO_H_ */
