#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int bin2hex(char *buf, int len)
{
        int i = 0;
        while (len-- > 0){
                /*fprintf(stdout, "0x%02x, ", buf[i++]);*/
				printf( "0x%02lx,", buf[i++] & 0xFFl );
                if ((i%16) == 0)
                        fprintf(stdout, "\n");
        }

        return 0;
}


int hex2bin(char *buf, int len)
{
        int i = 0;

        while (len-- > 0)
                fprintf(stdout, "%c", buf[i++]);

        return 0;
}

char hexfile[] = {
        #include "hex.bin"
};

int main(int argc, char *argv[])
{
        int opt;
        int fd;
        int rflag;
        char buf[1024];
        int len;

        rflag = 0;
        while ((opt=getopt(argc, argv, "ri:")) != -1) {
                switch (opt) {
                case 'r':
                        rflag = 1;
                        break;
                case 'i':
                        fd = open(optarg, O_RDONLY);
                        if (fd < 0) {
                                perror("open");
                                exit(-1);
                        }
                        break;
                default:
                        exit(-1);
                }
        }

        if (rflag) {
                hex2bin(hexfile, sizeof(hexfile));
        } else {
                while ((len=read(fd, buf, sizeof(buf))) > 0)
                        bin2hex(buf, len);
        }

        return 0;
}
