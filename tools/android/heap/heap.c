/************************************************************************
    > File Name: heap.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Mon 13 Jun 2016 02:14:47 PM CST
***********************************************************************/
#include <stdio.h>
#include <math.h>

int h[100] = {0,99,5,36,7,22,17,46,12,2,19,25,28,1,92};
int n;

void n_init(void)
{
	int i;
	for (i=1; h[i]!=0; i++)
		continue;
	n = --i;
}

void swap(int x, int y)
{
	int t;

	t = h[x];
	h[x] = h[y];
	h[y] = t;
}

void print_h(void)
{
	int i;

	for (i=1; h[i]!=0; i++)
		printf("h[%d]:%d\n", i, h[i]);
}

void siftdown(int i)
{
	int t, flag=0;
	
	while (i*2<=n && flag==0) {
		if (h[i] > h[2*i])
			t = i*2;
		else
			t = i;

		if (2*i+1<=n) {
			if (h[t] > h[2*i+1])
				t = i*2+1;
		}

		if (t!=i) {
			swap(t,i);
			i = t;
		} else
			flag = 1;
	}
}

void siftup(int i)
{
	int t, flag=0;
	if (i==1) return;

	while (i!=1 && flag==0) {
		if (h[i/2] > h[i])
			swap(i, i/2);
		else
			flag = 1;
		i = i/2;
	}
}

void creat(void)
{
	int i;	
	for (i=n/2;i>=1;i--) {
		siftdown(i);
	}
}

int delete(void)
{
	int t;

	t = h[1];
	h[1] = h[n];
	n--;
	siftdown(1);

	return t;
}

void del(int val)
{
	int count = log(n+1)/log(2);
	int i=1, flag=0, t;
	
	if (h[i] == val) {
		delete();
	}

	while (i*2<=n && flag==0) {
		if ()
	}




	for (i=1; i<=count; i++) {
		if (h[i] == val) {
			del_heap(i);
		}
		if (i*2<=n) {
			if (h[i*2] == val) {
				del_heap(i*2);
			}
		}
		if (i*2+1<=n) {
			if (h[i*2] == val) {
				del_heap(i*2);
			}
		}
	}
}

void del_heap(int i)
{
	h[i] = h[n];
	n--;
	siftdown(i);
}

void ins(int val)
{
	h[n+1] = val;
	n++;
	siftup(n);
}

int main(int argc, char *argv[])
{
	int i,num;

	n_init();
	creat();
	print_h();
	printf("sd=:%f\n", );
	return 0;
}
