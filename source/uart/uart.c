/************************************************************************
  > File Name: uart.c
  > Author: chengdong
  > Mail: zchengdongdong@gmail.com 
  > Created Time: Tue 08 Dec 2015 04:26:26 PM CST
 ***********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>//必须包含此头文件
#define TRUE 1
#define FALSE 0
#include <errno.h>
int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
          		   B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, 
		  		  115200, 38400, 19200, 9600, 4800, 2400, 1200,  300, };
void set_speed(int fd, int speed)
{
		int i;
		int status;
		struct
				termios opt;
		tcgetattr(fd, &opt);
		for( i = 0; i < sizeof(speed_arr)/sizeof(int); i++) {
				if(speed == name_arr[i]) {
						printf("set speed!\n");
						tcflush(fd, TCIOFLUSH);
						cfsetispeed(&opt, speed_arr[i]);
						cfsetospeed(&opt, speed_arr[i]);
						status = tcsetattr(fd, TCSANOW, &opt);
						if(status != 0)
								perror("tcsetattr fd1");
						return;
				}
				printf("no set speed!\n");
				tcflush(fd, TCIOFLUSH);
		}
}
/**
 *@brief 设置串口数据位,停止位和效验位
 *@param fd *@param databits 类型 int 数据位 取值 为 7 或者 8*
 *@param stopbits 类型 int 停止位 取值为 1 或者 2*
 *@param parity
 打开的串口文件句柄*
 int
 类型
 int
 效验类型 取值为 N,E,O,,S
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
/**
 *@breif 打开串口
 */
int OpenDev(char *Dev)
{
		int fd = open( Dev, O_RDWR );
		if (-1 == fd) //| O_NOCTTY | O_NDELAY
		{ /*设置数据位数*/
				perror("Can't Open Serial Port");
				return -1;
		} else
				return fd;
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
/**
 *@breif
 main()
 */
int main(int argc, char **argv)
{
		int fd;
		int nread;
		char buff[512];
		char *dev ="/dev/ttyS2";
		char write_info[1024];
		char write_ch[256];
		fd = OpenDev(dev); //根据设备节点,打开相应的 uart
		if (fd>0) //设备波特率
				set_speed(fd,115200);
		else {
				printf("Can't Open Serial Port!\n");
				exit(0);
		}
		
		//设置数据格式
		if (set_Parity(fd,8,1,'N')== FALSE) {
			printf("Set Parity Error\n");
			exit(1);
		}
		scanf("%s", write_info);
		changestr(write_info, write_ch);
		sprintf(write_ch,"%s\n",write_ch);
		printf("write_info:%s\n", write_info);
		nread = write(fd, write_ch, sizeof(write_ch)/sizeof(char));//写测试
		/*close(fd);*/
		while(1)
		{
				//读测试
				while((nread = read(fd,buff,512))>0)
				{
						printf("\nLen %d\n",nread);
						buff[nread+1]='\0';
						printf("\n%s",buff);

				}
		}
		/*[>close(fd);<]*/
		/*[>exit(0);<]*/
		return 0;
}
