/*************************************************************************
    > File Name: test.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Mon 05 Aug 2013 05:17:02 PM CST
 ************************************************************************/

#include <stdio.h>

#define PM_SUSPEND_ON		0
#define PM_SUSPEND_STANDBY	1
#define PM_SUSPEND_IDLE		2	
#define PM_SUSPEND_MEM		3
#define PM_SUSPEND_MAX		4

typedef int suspend_state_t;

const char *const pm_states[PM_SUSPEND_MAX] = {
	[PM_SUSPEND_ON]		= "on",
	[PM_SUSPEND_STANDBY]	= "standby",
	[PM_SUSPEND_IDLE]	= "idle",
	[PM_SUSPEND_MEM]	= "mem",
};

int a[2][2] = {{1, 2}, {3, 4}};
const int * const *p = a;

int main(int argc, char *argv[]) {

	const char * const *s;
	int len = 4;
	char buf[] = "idle";
		
	printf("*p:%d, a:%d\n", *(&p[1]), a);
	/*suspend_state_t state = PM_SUSPEND_ON;*/
	/*for (s = &pm_states[state]; state < PM_SUSPEND_MAX; s++, state++) {*/
		/*printf("s:%d ,state:%d pm_states[%d]:%d\n", **s, state, pm_states[state]);*/
		/*if (*s && len == strlen(*s) && !strncmp(buf, *s, len))*/
			/*break;*/
	/*}*/

	return 0;
}

