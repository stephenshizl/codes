#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>

#define INPUT_DIR "/dev/input"

int main(int argc, char *argv[])
{
	int fd;
	char *dev_name;
	char *dev_path;
	char buf[64];
	DIR *dirp;
	struct dirent *dirent;
	int length, ret;
	struct input_event event;

	/*if (argc < 2) {*/
		/*fprintf(stderr, "Usage: %s device_name\n", argv[0]);*/
		/*return -1;*/
	/*}*/

	dev_name = argv[1];

	dirp = opendir(INPUT_DIR);
	if (dirp == NULL)
		return -1;

	for (;;) {
		dirent = readdir(dirp);
		if (dirent == NULL) {
			ret = -1;
			break;
		}

		if (dirent->d_type == DT_CHR) {
			length = strlen(dirent->d_name);
			length += sizeof(INPUT_DIR) + 1;
			dev_path = malloc(length);
			if (dev_path == NULL)
				return -1;
			snprintf(dev_path, length,"%s/%s",
					INPUT_DIR, dirent->d_name);
			fd = open("/dev/input/event2"/*dev_path*/, O_RDONLY);
			if (fd <= 0)
				return -1;
			ret = ioctl(fd, EVIOCGNAME(64), buf);
			if (ret < 0)
				return -1;
			if (strcmp(dev_name, buf))
				continue;

			fprintf(stdout, "Found: %s <--> %s\n", dev_name, dev_path);
			while (1) {
				read(fd, &event, sizeof(struct input_event));
				fprintf(stdout, "Time:%lld Type:%u Code:%u Value:%d\n",
						(int64_t)(event.time.tv_sec * 1000 * 1000 + event.time.tv_usec),
						event.type, event.code, event.value);
			}
		}
	}

	closedir(dirp);
	close(fd);

	return 0;
}
