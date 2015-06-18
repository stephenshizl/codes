CC = gcc
all:
	$(CC) -o server main.c callbacks.c `pkg-config --cflags --libs gtk+-2.0 gthread-2.0`  -I /usr/include/mysql -L /usr/lib/mysql -lmysqlclient
