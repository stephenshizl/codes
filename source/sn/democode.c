typedef		unsigned short	    uint16;
typedef		unsigned long	    uint32;
typedef		unsigned char	    uint8;

#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define NEWTON_GET_UID           0x6001

#define RKNAND_DIASBLE_SECURE_BOOT _IOW('d', 127, unsigned int)
#define RKNAND_ENASBLE_SECURE_BOOT _IOW('d', 126, unsigned int)
#define RKNAND_GET_SN_SECTOR       _IOW('d', 3, unsigned int)

#define RKNAND_GET_VENDOR_SECTOR0       _IOW('v', 16, unsigned int)
#define RKNAND_STORE_VENDOR_SECTOR0     _IOW('v', 17, unsigned int)

#define RKNAND_GET_VENDOR_SECTOR1       _IOW('v', 18, unsigned int)
#define RKNAND_STORE_VENDOR_SECTOR1     _IOW('v', 19, unsigned int)

#define RKNAND_DEV_CACHE_FLUSH          _IOW('c', 20, unsigned int)

#define DRM_KEY_OP_TAG              0x4B4D5244 // "DRMK" 
#define SN_SECTOR_OP_TAG            0x41444E53 // "SNDA"
#define DIASBLE_SECURE_BOOT_OP_TAG  0x42534444 // "DDSB"
#define ENASBLE_SECURE_BOOT_OP_TAG  0x42534E45 // "ENSB"
#define VENDOR_SECTOR_OP_TAG        0x444E4556 // "VEND"

#define RKNAND_SYS_STORGAE_DATA_LEN 512

typedef struct tagRKNAND_SYS_STORGAE
{
    uint32  tag;
    uint32  len;
	uint16  sn_len;
    uint8   data[RKNAND_SYS_STORGAE_DATA_LEN];
}RKNAND_SYS_STORGAE;


typedef struct tagSN_SECTOR_INFO
{
    uint32 snSectag;           // "SNDA" 0x41444E53
    uint32 snSecLen;           // 512
    uint16 snLen;              // 0:no sn , 0~30,sn len
    uint8 snData[30];          // sn data
    uint32 reserved2[(0x200-0x20)/4];
}SN_SECTOR_INFO,*pSN_SECTOR_INFO;

void rknand_print_hex_data(uint8 *s,uint32 * buf,uint32 len)
{
    uint32 i,j,count;
    fprintf(stderr, "%s",s);
	for(i=0;i<len;i+=4)
	{
	   fprintf(stderr, "%x %x %x %x",buf[i],buf[i+1],buf[i+2],buf[i+3]);
	}
}

void mem_dump(char *buf, int len)
{
		int i;

		for (i = 0; i < len; i++) {
				printf("0x%x ", buf[i]);
		}

	printf("\n");
		return;
}

int rknand_sys_storage_test_sn(void)
{
    uint32 i;
    int ret ;
    RKNAND_SYS_STORGAE sysData;
	int sys_fd = open("/dev/rknand_sys_storage",O_RDWR,0);
	/*int sys_fd = open("/dev/newton",O_RDWR,0);*/
    if(sys_fd < 0){
        fprintf(stderr, "rknand_sys_storage open fail\n");
        return -1;
    }
    //sn
    sysData.tag = SN_SECTOR_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN;
	ret = ioctl(sys_fd, RKNAND_GET_SN_SECTOR, &sysData);
	mem_dump(&sysData, sizeof(sysData));
	rknand_print_hex_data("sndata:",(uint32*)sysData.data,16);
    if(ret){
        fprintf(stderr, "get sn error\n");
        return -1;
    }
    return 0;
}


int rknand_sys_storage_secure_boot_disable(void)
{
    uint32 i;
    int ret ;
    RKNAND_SYS_STORGAE sysData;

    int sys_fd = open("/dev/rknand_sys_storage",O_RDWR,0);
    if(sys_fd < 0){
        fprintf(stderr, "rknand_sys_storage open fail\n");
        return -1;
    }
    sysData.tag = DIASBLE_SECURE_BOOT_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN;
    
    ret = ioctl(sys_fd, RKNAND_DIASBLE_SECURE_BOOT, &sysData);
    if(ret){
        fprintf(stderr, "disable secure boot error\n");
        return -1;
    }
    return 0;
}

int rknand_sys_storage_secure_boot_enable(void)
{
    uint32 i;
    int ret ;
    RKNAND_SYS_STORGAE sysData;

    int sys_fd = open("/dev/rknand_sys_storage",O_RDWR,0);
    if(sys_fd < 0){
        fprintf(stderr, "rknand_sys_storage open fail\n");
        return -1;
    }
    sysData.tag = ENASBLE_SECURE_BOOT_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN;
    
    ret = ioctl(sys_fd, RKNAND_ENASBLE_SECURE_BOOT, &sysData);
    if(ret){
        fprintf(stderr, "enable secure boot error\n");
        return -1;
    }
    return 0;
}


int rknand_sys_storage_vendor_sector_load(void)
{
    uint32 i;
    int ret ;
    RKNAND_SYS_STORGAE sysData;

    int sys_fd = open("/dev/rknand_sys_storage",O_RDWR,0);
    if(sys_fd < 0){
        fprintf(stderr, "rknand_sys_storage open fail\n");
        return -1;
    }

    sysData.tag = VENDOR_SECTOR_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN-8;

    ret = ioctl(sys_fd, RKNAND_GET_VENDOR_SECTOR0, &sysData);
    rknand_print_hex_data("vendor_sector load:",(uint32*)sysData.data,32);
    if(ret){
        fprintf(stderr, "get vendor_sector error\n");
        return -1;
    }
    return 0;
}

int rknand_sys_storage_vendor_sector_store(void)
{
    uint32 i;
    int ret ;
    RKNAND_SYS_STORGAE sysData;

    int sys_fd = open("/dev/rknand_sys_storage",O_RDWR,0);
    if(sys_fd < 0){
        fprintf(stderr, "rknand_sys_storage open fail\n");
        return -1;
    }
    sysData.tag = VENDOR_SECTOR_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;
    for(i=0;i<126;i++)
    {
        sysData.data[i] = i;
    }
    rknand_print_hex_data("vendor_sector save:",(uint32*)sysData.data,32);
    ret = ioctl(sys_fd, RKNAND_STORE_VENDOR_SECTOR0, &sysData);
    if(ret){
        fprintf(stderr, "save vendor_sector error\n");
        return -1;
    }
    return 0;
}

int rknand_cache_flush(void)
{
    uint32 i;
    int ret ;
    uint8 sysData[512];
    int sys_fd = open("/dev/rknand_sys_storage",O_RDWR,0);
    if(sys_fd < 0){
        fprintf(stderr, "rknand_sys_storage open fail\n");
        return -1;
    }
    /*memset(sysData, 0 , sizeof(sysData));*/
    ret = ioctl(sys_fd, RKNAND_DEV_CACHE_FLUSH, &sysData);
    if(ret){
        fprintf(stderr, "rknand cache flush error\n");
        return -1;
    }
    return 0;
}


void rknand_sys_storage_test(void)
{
    rknand_sys_storage_test_sn();
    /*rknand_sys_storage_vendor_sector_store();*/
    /*rknand_sys_storage_vendor_sector_load();*/
}

int main (int argc , char *argv[]) {

	rknand_sys_storage_test();
	return 0;
}






