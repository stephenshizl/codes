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
#include <jni.h>
#include <assert.h>

// 引入log头文件
#include <android/log.h>  

#define BAUDRATE        B115200
#define UART_DEVICE     "/dev/ttyS2"

#define FALSE  -1
#define TRUE   0

// 获取数组的大小
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#define JNIREG_CLASS "com/skyworth/ndk/libuart_bl"

// log标签
#define  TAG    "ndk_basic_param"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
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

void Bl_set_speed(int fd, int speed){
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
int Bl_set_Parity(int fd,int databits,int stopbits,int parity)
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

JNIEXPORT jint JNICALL 
Bl_Md103_Open(JNIEnv *env, jobject clazz)
{
    int    fd, c=0, res;
    char  buf[256];

	printf("Start...\n");
    fd = open(UART_DEVICE, O_RDWR);

    if (fd < 0) {
		return (jint)fd;
    }
	bl_fd = fd;

	printf("Open...\n");
    Bl_set_speed(fd,115200);
	if (Bl_set_Parity(fd,8,1,'S') == FALSE)  {
		printf("Set Parity Error\n");
		return (jint)-1;
	}
	return (jint)0;
}

JNIEXPORT jint JNICALL 
Bl_Md103_Write(JNIEnv *env, jobject clazz)
{
    int    res;
    char  buf[256];

	strncpy(buf, "AT#CA\r\n", 9);
	res = write(bl_fd, buf, 9);
	if (res < 0)
		return (jint)-10086;
	return (jint)10086;

}

JNIEXPORT void JNICALL 
Bl_Md103_Close(JNIEnv *env, jobject clazz)
{
	printf("Close...\n");
    close(bl_fd);
	bl_fd = -1;
}

static JNINativeMethod method_table[] = {
	{ "Bl_Md103_Open"		, "()I"					, (void*)Bl_Md103_Open		},
	{ "Bl_Md103_Write"		, "()I"					, (void*)Bl_Md103_Write		},
	{ "Bl_Md103_Close"		, "()V"					, (void*)Bl_Md103_Close		},
};

// 注册native方法到java中
static int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods)
{
	jclass clazz;
	clazz = (*env)->FindClass(env, className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}
	if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}
int register_basic_ndk_param(JNIEnv *env)
{
	// 调用注册方法
    return registerNativeMethods(env, JNIREG_CLASS,
            method_table, NELEM(method_table));
}

// JNI_OnLoad在jni注册时，会被回调执行。
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1; 

    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }   

    register_basic_ndk_param(env);

	// 返回jni的版本
    return JNI_VERSION_1_4;
}
