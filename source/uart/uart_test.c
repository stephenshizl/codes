/************************************************************************
    > File Name: uart_t.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Tue 08 Dec 2015 04:29:58 PM CST
***********************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#define BAUDRATE        B115200
#define UART_DEVICE     "/dev/ttyS2"

#define FALSE  -1
#define TRUE   0
////////////////////////////////////////////////////////////////////////////////
/**
*@brief  设置串口通信速率
*@param  fd     类型 int  打开串口的文件句柄
*@param  speed  类型 int  串口速度
*@return  void
*/
int bl_fd = -1;
int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
          		   B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, 
		  		  115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, };
void set_speed(int fd, int speed){
  int   i; 
  int   status; 
  struct termios   Opt;
  tcgetattr(fd, &Opt); 
  for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) { 
    if  (speed == name_arr[i]) {     
      tcflush(fd, TCIOFLUSH);     
      cfsetispeed(&Opt, speed_arr[i]);  
      cfsetospeed(&Opt, speed_arr[i]);   
      status = tcsetattr(fd, TCSANOW, &Opt);  
      if  (status != 0) {        
        perror("tcsetattr fd1");  
        return;     
      }    
      tcflush(fd,TCIOFLUSH);   
    }  
  }
}
////////////////////////////////////////////////////////////////////////////////
/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄
*@param  databits 类型  int 数据位   取值 为 7 或者8
*@param  stopbits 类型  int 停止位   取值为 1 或者2
*@param  parity  类型  int  效验类型 取值为N,E,O,,S
*/
int set_Parity(int fd,int databits,int stopbits,int parity)
{ 
	struct termios options; 
	if  ( tcgetattr( fd,&options)  !=  0) { 
		perror("SetupSerial 1");     
		return(FALSE);  
	}
	options.c_cflag &= ~CSIZE; 
	switch (databits) /*设置数据位数*/
	{   
	case 7:		
		options.c_cflag |= CS7; 
		break;
	case 8:     
		options.c_cflag |= CS8;
		break;   
	default:    
		fprintf(stderr,"Unsupported data size\n"); return (FALSE);  
	}
	switch (parity) 
	{   
		case 'n':
		case 'N':    
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */ 
			break;  
		case 'o':   
		case 'O':     
			options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/  
			options.c_iflag |= INPCK;             /* Disnable parity checking */ 
			break;  
		case 'e':  
		case 'E':   
			options.c_cflag |= PARENB;     /* Enable parity */    
			options.c_cflag &= ~PARODD;   /* 转换为偶效验*/     
			options.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		case 'S': 
		case 's':  /*as no parity*/   
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;break;  
		default:   
			fprintf(stderr,"Unsupported parity\n");    
			return (FALSE);  
		}  
	/* 设置停止位*/  
	switch (stopbits)
	{   
		case 1:    
			options.c_cflag &= ~CSTOPB;  
			break;  
		case 2:    
			options.c_cflag |= CSTOPB;  
		   break;
		default:    
			 fprintf(stderr,"Unsupported stop bits\n");  
			 return (FALSE); 
	} 
	/* Set input parity option */ 
	if (parity != 'n')   
		options.c_iflag |= INPCK; 
	tcflush(fd,TCIFLUSH);
	options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/   
	options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
	if (tcsetattr(fd,TCSANOW,&options) != 0)   
	{ 
		perror("SetupSerial 3");   
		return (FALSE);  
	} 
	options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
	options.c_oflag  &= ~OPOST;   /*Output*/
	return (TRUE);  
}
static void changestr(char *str, char * out_str)
{
		int i = 0;
		int j = 0;
		while((str[i++]) && (i <= 1024)) {
				if(str[i-1] == ',') {
						out_str[j] = ' ';
				}else
						out_str[j] = str[i-1];
				j++;
		}
}
int bl_scanf(void) 
{
	char bl_data[256] = "AT#CA\n";
	char buf[256];
	char cmd[10] = "CA";
	int res;
	char write_info[1024];
	char write_ch[256];

	/*memset(bl_data, '\0', 256);*/
	res = write(bl_fd, "AT#MX\n", sizeof("AT#CA\n")-1);
	/*printf("count:%d\n", sizeof("AT#CA\n")-1);*/
	/*printf("Reading...\n");*/
	/*memset(write_ch, '\n', 256);*/
	/*changestr(bl_data, write_ch);*/
	/*sprintf(write_ch,"%s\n",write_ch);*/
	/*printf("write_info:%s\n", bl_data);*/
	/*write(bl_fd, write_ch, sizeof(write_ch)/sizeof(char));//写测试*/
	while(1) {
		res = read(bl_fd, buf, 255);

		if(res==0)
			continue;
		buf[res]=0;

		printf("%s", buf);
		if (buf[0] == 0x0d) {
			printf("\n");
		}
		
		if (buf[0] == '@') break;

		if (strncmp (buf, "SH", 2) == 0) {
			break;
		}
	}
	return 0;
}

int bl_conn(char *device) 
{
	char bl_data[256];
	char buf[256];
	int res;
	char write_info[1024];
	char write_ch[256];

	memset(write_ch, '\n', 256);
	memset(bl_data, '\0', 256);
	sprintf(bl_data, "AT#%s\n", device);
	/*printf("send: %s, count:%d\n", bl_data, sizeof(bl_data)-1);*/
	/*write(bl_fd, bl_data, sizeof(bl_data)-1);*/
	changestr(bl_data, write_ch);
	sprintf(write_ch,"%s\n",write_ch);
	printf("write_info:%s\n", bl_data);
	write(bl_fd, write_ch, sizeof(write_ch)/sizeof(char));//写测试
	while(1) {
		res = read(bl_fd, buf, 255);

		if(res==0)
			continue;
		buf[res]=0;

		printf("%s", buf);
		if (buf[0] == 0x0d) {
			printf("\n");
		}
		
		if (buf[0] == '@') break;

		if (strncmp (buf, "OKIB", 4) == 0) {
			break;
		}

	}
	return 0;
}

int bl_play(void) 
{
	char buf[256];
	int res;

	write(bl_fd, "AT#MA\n", sizeof("AT#MA\n")-1);
	while(1) {
		res = read(bl_fd, buf, 255);

		if(res==0)
			continue;
		buf[res]=0;

		printf("%s", buf);
		if (buf[0] == 0x0d) {
			printf("\n");
		}
		
		if (buf[0] == '@') break;

		if (strncmp (buf, "OKMA", 4) == 0) {
			break;
		}

	}
	return 0;
}

int bl_play_stop(void) 
{
	char *bl_data;
	int res;
	char buf[256];

	write(bl_fd, "AT#MB\n", sizeof("AT#MA\n")-1);
	while(1) {
		res = read(bl_fd, buf, 255);

		if(res==0)
			continue;
		buf[res]=0;

		printf("%s", buf);
		if (buf[0] == 0x0d) {
			printf("\n");
		}
		
		if (buf[0] == '@') break;

		if (strncmp (buf, "OKMB", 4) == 0) {
			break;
		}

	}
	return 0;
}

int bl_define(char *device, int num) 
{
	char bl_data[256];
	char buf[256];
	int res;
	char blexit[256];

	sprintf(bl_data, "AT#%s\n", device);
	printf("send: %s\n", bl_data);
	/*printf("send: %s, count:%d\n", bl_data, sizeof(bl_data)-1);*/
	if (strncmp(device, "CC", 2) == 0)
		write(bl_fd, bl_data, 21);
	else if (strncmp(device, "CV", 2) == 0)
		write(bl_fd, bl_data, 10);
	else if (strncmp(device, "CV1", 2) == 0)
		write(bl_fd, bl_data, 11);
	else if (strncmp(device, "CV2", 2) == 0)
		write(bl_fd, bl_data, 11);
	else	
		write(bl_fd, bl_data, 9);
	while(1) {
		res = read(bl_fd, buf, 255);

		if(res==0)
			continue;
		buf[res]=0;

		printf("%s", buf);
		if (buf[0] == 0x0d) {
			printf("\n");
		}
		
		if (buf[0] == '@') break;

		sprintf(blexit, "OK%s", device);
		if (strncmp (buf, blexit, 4) == 0) {
			break;
		}
		if (strncmp (buf, "MU", 2) == 0) {
			break;
		}

	}
	return 0;
}

int main(int argc, char *argv[])
{

    int    fd, c=0, res, opt, num;

    char  buf[256];

	char cmd[256];

    printf("Start...\n");
    fd = open(UART_DEVICE, O_RDWR);

    if (fd < 0) {
        perror(UART_DEVICE);
		return -1;
    }
	bl_fd = fd;
    printf("Open...\n");
	set_speed(fd,115200);
	if (set_Parity(fd,8,1,'S') == FALSE)  {
		printf("Set Parity Error\n");
		return -1;
	}
	
	while ((opt = getopt(argc, argv, "sn:d:tpc:e:h")) != -1) {
		switch (opt) {
		case 's':
			bl_scanf();
			break;
		case 'c':
			cmd = strdup(optarg);
			bl_conn(cmd);
			break;
		case 'p':
			bl_play();
			break;
		case 't':
			bl_play_stop();
			break;
		case 'n':
			num = strtol(optarg, NULL, 0);
			break;
		case 'd':
			cmd = strdup(optarg);
			bl_define(cmd, (int )num);
			break;
		case 'h':
			return 0;
		}
	}

	/*write(fd, "AT#MX\n", sizeof("AT#CA\n")-1);*/
	/*while(1) {*/
		/*res = read(fd, buf, 255);*/

		/*if(res==0)*/
			/*continue;*/
		/*buf[res]=0;*/

		/*printf("%s", buf);*/
		/*[>break;<]*/
		/*if (buf[0] == 0x0d)*/
			/*printf("\n");*/
		
		/*if (buf[0] == '@') break;*/

		/*if (strncmp (buf, "SH", 2) == 0) {*/
			/*printf("SH\n");*/
			/*break;*/
		/*}*/

		/*if (strncmp (buf, "SH\n", 4) == 0) {*/
			/*printf("SHrn\n");*/
			/*break;*/
		/*}*/
	/*}*/

    printf("Close...\n");
    close(fd);

    return 0;
}
