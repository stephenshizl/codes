#include <string.h>
#include <poll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <linux/netlink.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


static int uevent_init()
{
	struct sockaddr_nl addr;
	int sz = 64 * 1024;
        int on = 1;
	int fd;

	memset(&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;
	addr.nl_pid = getpid();
	addr.nl_groups = 0xffffffff;

	fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (fd < 0) {
		printf("Failed to create socket !\n");
		return -1;
	}
	setsockopt(fd, SOL_SOCKET, SO_RCVBUFFORCE, &sz, sizeof(sz));
        setsockopt(fd, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on));


	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		printf("Failed to bind socket !\n");
		close(fd);
		return -1;
	}
	return fd;
}

ssize_t uevent_kernel_multicast_uid_recv(int socket, void *buffer,
                                         size_t length, uid_t *user)
{
    struct iovec iov = { buffer, length };
    struct sockaddr_nl addr;
    char control[CMSG_SPACE(sizeof(struct ucred))];
    struct msghdr hdr = {
        &addr,
        sizeof(addr),
        &iov,
        1,
        control,
        sizeof(control),
        0,
    };

    *user = -1;
    ssize_t n = recvmsg(socket, &hdr, 0);
    if (n <= 0) {
        return n;
    }

    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&hdr);
    if (cmsg == NULL || cmsg->cmsg_type != SCM_CREDENTIALS) {
        /* ignoring netlink message with no sender credentials */
        goto out;
    }

    struct ucred *cred = (struct ucred *)CMSG_DATA(cmsg);
    *user = cred->uid;
    if (cred->uid != 0) {
        /* ignoring netlink message from non-root user */
        goto out;
    }

    if (addr.nl_groups == 0 || addr.nl_pid != 0) {
        /* ignoring non-kernel or unicast netlink message */
        goto out;
    }

    return n;

out:
    /* clear residual potentially malicious data */
    bzero(buffer, length);
    errno = EIO;
    return -1;
}

int main()
{
	int fd, n;
	char buf[1024] = { 0 };

	fd = uevent_init();
	if(fd < 0) {
		printf("Failed to exec uevent_init !\n");
		return -1;
	}

	while ((n = recv(fd, buf, sizeof(buf), 0)) > 0) {
		printf("socket (%d): %s\n", n, buf);
		memset(buf, 0, sizeof(buf));
	}

	return 0;
}

