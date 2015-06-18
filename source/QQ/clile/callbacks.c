#include "callbacks.h"

int wrilog = 0, wriadd = 0, jinji = 0;
int log_stut = 0, add_stut = 0;
int fdstatu = 1,dia=0;
int recourd=1;
int serverport;
char serverip[30];
char ch_dialog[100];
int opt = 1, maxfd = -1, k = 0, top = 1;
int xx = 0;
  GtkWidget *window1;
//+++++++++++++查找还没有使用的连接状态数组++++++++++++++++++++++++
void add_sockset(fd_set * sockset, int sockfd, int user_link, int add[messcount][2])
{
	int i;
	FD_ZERO(sockset);
	if (xx)
		FD_CLR(sockfd, &allset);
	else
		FD_SET(sockfd, sockset);
	FD_SET(user_link, sockset);
	FD_SET(1, sockset);
	for (i = 0; i < messcount; i++) {
		if (add[i][0] != -1) {
			FD_SET(add[i][1], sockset);
		}
	}
}

//+++++++++++++++++++++++++++++
int addr(void)
{
	int i;
	for (i = 0; i < messcount; i++)
		if (add[i][0] == -1)
			return i;
}

//+++++++++++++++++++++++++
int tu_mess(void)
{
	int i;
	for (i = 0; i < messcount; i++)
		if (allmess[i].tu == 0)
			return i;
}

//++++++++++++++++++++++++
int max_fd(int ab, int ac)
{
	if (ab > ac)
		return ab;
	else
		return ac;
}
//++++++++++++++++关闭文件描述符+++++++++++++++++
void func_close (void)
{
	close(sockfd);
	for (i = 0; i < messcount; i++) {
		if (add[i][0] != -1) {
			close(add[i][1]);
		}
	}

}
//++++++++++++++++退出函数+++++++++++++++++
void on_delete_event(GtkWidget * widget, gpointer date)
{
	printf("aaaaadddddd\n");
	func_close();
	gtk_main_quit();
	exit(0);
//return FALSE;
}
//++++++++++++++++创建与服务器的连接函数+++++++++++++++++
int initial_server(void)
{
	char *POPMessage[]={
			"USER myqqmail\r\n",
			"PASS 123456123\r\n",
			NULL};
	char buf[5000];
	int i,n;
	int num1,num2,num3,num4,num5,num6,num7,num8,num9,num10,num11,num12;
	int sockfd_mail,iLength;
	struct sockaddr_in server_mail;
	int iMsg=0;
	if((sockfd_mail=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror(" socket error");exit(1);
	}
	server_mail.sin_family=AF_INET;
	server_mail.sin_port=htons(110);
	server_mail.sin_addr.s_addr=inet_addr("123.125.50.23");

	bzero(&(server_mail.sin_zero),8);


	if(connect(sockfd_mail,(struct sockaddr *)(&server_mail),sizeof(struct sockaddr))!=-1){
		iLength=recv(sockfd_mail,buf,sizeof(buf),0);
		buf[iLength]='\0';
		printf("received:%s\n",buf);
		do
		{
			send(sockfd_mail,POPMessage[iMsg],strlen(POPMessage[iMsg]),0);
			printf("发送帐号:%s",POPMessage[iMsg]);
			memset(buf,'\0',1024);
			iLength=recv(sockfd_mail,buf,sizeof(buf),0);
			buf[iLength]='\0';
			iMsg++;
			printf("接收信息:%s\n",buf);

		}while(POPMessage[iMsg]);

		send(sockfd_mail,"RETR 1\r\n",strlen("RETR 1\r\n"),0);
		memset(buf,'\0',5000);
	//		iLength=recv(sockfd_mail,buf,1024,0);
	//		buf[iLength]='\0';
	//		iMsg++;
	//		printf("接收receive:%s\n",buf);
	//开始接收邮件
		num1=num2=num3=0;
		n=0;
		int k=0,j,x=0;
		char a[2],*token,b=20;
		for(i=0;i<5000;i++){
			n++;
			recv(sockfd_mail,a,1,0);
			if(strcmp(a,"S")==0) num1=n;
			if(strcmp(a,"u")==0) num2=n;
			if(strcmp(a,"b")==0) num3=n;
 			if(strcmp(a,"j")==0) num4=n;
			if(strcmp(a,"e")==0) num5=n;
			if(strcmp(a,"c")==0) num6=n;
			if(strcmp(a,"t")==0) num7=n;
			if(strcmp(a,":")==0) num8=n;
			if(strcmp(a,"m")==0) num9=n;
			if(strcmp(a,"y")==0) num10=n;
			if(strcmp(a,"q")==0) num11=n;
			if(strcmp(a,"q")==0) num12=n;
			if(((num12-num1)==11)&&((num10-num9)==1)&&((num8-num7)==1)&&((num6-num5)==1)&&((num4-num3)==1)&&((num2-num1)==1)){
				for(j=0;j<45;j++)
				{
    		     			recv(sockfd_mail,a,1,0);
         				strcat(buf,a);
					x=1;
				}
			}
			if(x) break;
   		 }
		token=strtok(buf,":");
		token=strtok(NULL,":");
		printf("从邮件是读出来的ip是：\n%s\n",token);
		memset(serverip,'\0',30);
		strcpy(serverip,token);
		token=strtok(NULL,":");
		printf("从邮件是读出来的port是：\n%s\n",token);
		serverport=atoi(token);
		//
		close(sockfd_mail);
	}
	else{
		//printf("采用默认ip和port\n");
		perror(" 采用默认ip:127.0.0.1和port:8000");
		memset(serverip,'\0',30);
		strcpy(serverip,"127.0.0.1");
		serverport=8000;
	}
return 0;
}
//
void close_win(GtkWidget * widget, gpointer date)
{
//GtkWidget *kk=(GtkWidget *)date;
gtk_widget_destroy(window1);

}
//+++++++++++++++++++++++
GtkWidget* func_dialog1(void)
{

  GtkWidget *vbox1;
  GtkWidget *label_ti;
  GtkWidget *hbox1;
  GtkWidget *label2;
  GtkWidget *button1;

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request (window1, 198, 128);
  gtk_window_set_title (GTK_WINDOW (window1), "window1");
  gtk_window_set_resizable (GTK_WINDOW (window1), FALSE);
  gtk_window_set_destroy_with_parent (GTK_WINDOW (window1), TRUE);
//	g_signal_connect(G_OBJECT(window1), "destroy",G_CALLBACK(), NULL);
//	gtk_window_set_position(GTK_WINDOW(window_clicked), GTK_WIN_POS_CENTER);
  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  label_ti = gtk_label_new (ch_dialog);
  gtk_widget_show (label_ti);
  gtk_box_pack_start (GTK_BOX (vbox1), label_ti, FALSE, FALSE, 0);
  gtk_widget_set_size_request (label_ti, 38, 98);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  label2 = gtk_label_new ("");
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (hbox1), label2, FALSE, FALSE, 0);
  gtk_widget_set_size_request (label2, 136, 0);

  button1 = gtk_button_new_with_mnemonic (" 确 定 ");
  gtk_widget_show (button1);
  gtk_box_pack_start (GTK_BOX (hbox1), button1, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(button1), "clicked",G_CALLBACK(close_win),(gpointer)&window1);

  return window1;
/*	while (1){
//		if(dia){
		gdk_threads_enter();
			GtkWidget *dialog;
			dia=0;
			dialog=gtk_message_dialog_new(NULL,
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_INFO,
				GTK_BUTTONS_OK,
				ch_dialog,
				NULL);
		//gtk_widget_destroy(GTK_WIDGET(dialog));
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		gdk_threads_leave();
		
//		break;
//		}
	}
*/
}
void func_dialog(void)
{
GtkWidget* aaa;
aaa=func_dialog1();
  gtk_widget_show (aaa);
}
//++++++++++++++++（go_to)函数++++++++++++++++++++++++
void go_to(void)
{
	printf("已经进入go_to\n");
	for (i = 0; i < 20; i++) {
		allmess[i].fd = -1;
		allmess[i].tu = 0;
		add[i][0] = -1;
		add[i][1] = -1;
		allmess[i].winstu = 0;
		allmess[i].ac = 1;
	}
	char *token;
	if (top) {
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//		g_thread_create(G_THREAD_FUNC(initial_server), NULL, FALSE, NULL);
		initial_server();
		bzero(&dest, sizeof(dest));
		dest.sin_family = AF_INET;
		dest.sin_port = htons(serverport);
		inet_aton(serverip, (struct in_addr *)&dest.sin_addr.s_addr);
		/* 连接服务器 */
		if (connect(sockfd, (struct sockaddr *)&dest, sizeof(dest)) != 0) {
			perror("Connect ");
			exit(errno);
		}
		while (1) {
			FD_ZERO(&allset);
			maxfd = 0;
			FD_SET(sockfd, &allset);
			if (sockfd >maxfd)
				maxfd = sockfd;
			while (wrilog || wriadd || 1 || jinji) {
				char wri[200];
				memset(wri, '\0', 200);
				if (wrilog) {
					sprintf(wri, "usercheck:%s:%s",alogsi.id, alogsi.pwd);
					write(sockfd, wri, strlen(wri));
					wrilog = 0;
					break;
				}
				if (wriadd) {
					sprintf(wri, "useradd:%s:%s:%s", aaddsi.name,aaddsi.pwd, aaddsi.email);
					printf("%s\n", wri);
					write(sockfd, wri, strlen(wri));
					wriadd = 0;
					break;
				}
				if (jinji) {
					jinji = 0;
					break;
				}
			}
			retval = select(maxfd + 1, &allset, NULL, NULL, NULL);
			if (retval == 0) {
				continue;
			} else {
				if (FD_ISSET(sockfd, &allset)) {
					bzero(buffer, MAXBUF + 1);
					len = read(sockfd, buffer, MAXBUF);
					if (len > 0) {
						printf("%s\n", buffer);
						token = strtok(buffer, ":");
						printf("token=%s\n", token);
						if ((strcmp(token, "sqlfailed"))
						    == 0) {
							printf("server the mysql =%s\n", token);
							memset(token, '\0', strlen(token));
							continue;
						}
						if ((strcmp(token, "ucount")) == 0) {
							token = strtok(NULL, ":");
							printf("listcount=%s\n", token);
							listcount = atoi(token);
							memset(token, '\0', 200);
							jinji = 1;
						}
						if ((strcmp(token, "sigin")) == 0){
							int x;
							x = tu_mess();
							//name
							token = strtok(NULL, ":");
							printf("sigin1=%s\n", token);
							strcpy(allmess[x].name, token);
							//id
							token = strtok(NULL, ":");
							printf("sigin1=%s\n", token);
							strcpy(allmess[x].id, token);
							//ip
							token = strtok(NULL, ":");
							printf("sigin1=%s\n", token);
							strcpy(allmess[x].ip, token);
							//port
							token = strtok(NULL, ":");
							printf("sigin1=%s\n", token);
							strcpy(allmess[x].port, token);
							//
							token = strtok(NULL, ":");
							printf("sigin1=%s\n", token);
							strcpy(allmess[x].statu, token);
							//email
							token = strtok(NULL, ":");
							printf("sigin1=%s\n", token);
							strcpy(allmess[x].mail,token);
							allmess[x].tu = 1;
							memset(token, '\0', 200);
							jinji = 1;
						}
						if ((strcmp(token, "over")) == 0) {
							log_stut = 1;
							memset(token, '\0', 200);
							break;
						}
						if ((strcmp(token, "chkoff")) == 0) {
							printf("用户名或密码不对，或已经登陆过不能重复登陆！\n");
						}
						if ((strcmp(token, "on")) == 0) {
							token = strtok(NULL, ":");
							add_stut = 1;
							printf("注册成功！\n");
							memset(ch_dialog,'\0',100);
							sprintf(ch_dialog,"请牢记您审请的号码\n\t%s",token);
							//g_thread_create(G_THREAD_FUNC(func_dialog),NULL, FALSE, NULL);
							func_dialog();
						}
						if ((strcmp(token, "off")) == 0) {
							printf("注册失败,换ID！\n");
						}
					}

					else {
						if (len < 0)
							printf("消息接收失败！错误信息是'%s'\n", errno, strerror(errno));
						else
							printf("对方退出了，聊天终止！\n");
						break;
					}
					//printf("k=%d\n",k);
				}
				if (FD_ISSET(0, &allset)){
					bzero(buffer, MAXBUF + 1);
					fgets(buffer, MAXBUF, stdin);
					if (!strncasecmp(buffer, "quit", 4)) {
						printf("自己请求终止聊天！\n");
						break;
					}
					len = send(sockfd, buffer, strlen(buffer) - 1, 0);
				}
			}
		}
		top = 0;
	}
	//++++++++++++开始当服务器使用++++++
	printf("现在进入服务器进行通信\n");
	listfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("listfd;%d\n", listfd);
	setsockopt(listfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	printf("SO_REUSEADDR=%d\n", SO_REUSEADDR);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(allmess[0].port));

//	printf("allmess[0].ip=%s\n", allmess[0].port);
	printf("port=%d\n", atoi(allmess[0].port));

	if (bind(listfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		printf("绑定时有问题\n");
	//      printf("444444\n");
	listen(listfd, 20);
	maxfd = max_fd(listfd, sockfd);
	printf("listfd=%d\n", listfd);
	FD_ZERO(&allset);
	FD_SET(listfd, &allset);
	FD_SET(sockfd, &allset);
	for (;;) {
		printf("!!!!!!!!!!\n");
		nready = select(maxfd + 1,&allset, NULL, NULL, NULL);
		printf("...........\n");
		if (nready < 0)
			printf("select error\n");
		else {
			if (FD_ISSET(listfd, &allset)) {
				int cli = addr();
				add[cli][0] = cli;
				cliaddr_len = sizeof(cliaddr[cli]);
				printf("有连接，连接前\n");
				connfd = accept(listfd, (struct sockaddr *)&cliaddr[cli], &cliaddr_len);
				printf("已有新连接\n");
				add[cli][1] = connfd;
				//add[cli][0] = 1;
				maxfd = max_fd(maxfd, connfd);
				add_sockset(&allset, sockfd, listfd, add);
				char funip[25];
				memset(funip, '\0', 25);
				sprintf(funip, "%s", inet_ntop(AF_INET, &cliaddr[cli].sin_addr, str, sizeof(str)));
				printf("连接我的ip是：%s\n", funip);
				char funport[7];
				memset(funport, '\0', 7);
				sprintf(funport, "%d", ntohs(cliaddr[cli].sin_port));
				printf("连接我的port是：%s\n", funport);
				int a;
				for (a = 0; a < listcount; a++){
					if ((strcmp(allmess[a].ip, funip) == 0) && (strcmp(allmess[a].port, funport)
						== 0)) {
						allmess[a].fd = connfd;
						printf("allmess[%d].fd=%d\n", a, allmess[a].fd);
					}
					break;
				}
				printf("已经有人链接我了fd=%d\n", connfd);
				continue;
			}
			if (FD_ISSET(0, &allset)){
			printf("quit=0\n");
			break;
			}
			if (FD_ISSET(sockfd, &allset)){
				printf("11111\n");
				bzero(buffer, MAXBUF + 1);
				len = read(sockfd, buffer, MAXBUF);
				printf("111112222-buffer=%s\n", buffer);
				printf("222222\n");
				if (len == 0){
					printf("3333\n");
					xx = 1;
					//FD_CLR(sockfd,&allset);
					//sockfd=-1;
					printf("44444\n");
					add_sockset(&allset, sockfd, listfd, add);
					printf("55555\n");
					continue;
				}
				token = strtok(buffer, ":");
				if (strcmp(token, "accet") == 0) {
					printf("accet****\n");
					add_sockset(&allset, sockfd, listfd, add);
					continue;
				}
				if (strcmp(token, "delu") == 0){
					printf("delu****\n");
					token = strtok(NULL, ":");
					for (j = 0; j < listcount; j++) {
						if (strcmp(allmess[j].id, token) == 0){
							token = strtok(NULL, ":");
							memset(allmess[j].ip, '\0', 25);
							strcpy(allmess[j].ip, token);
printf("rrr\n");
							token = strtok(NULL, ":");
							memset(allmess[j].port, '\0', 10);
							strcpy(allmess[j].port, token);
							strcpy(allmess[j].statu, "1");
							break;
						}
					}
				//更新列表
					gdk_threads_enter;
					gtk_list_store_clear((GtkListStore*)store1);
					gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_frend_list), createModel());
					gdk_threads_leave;
					add_sockset(&allset, sockfd, listfd, add);
					continue;
				}
				if ((strcmp(token, "sigin")) == 0) {
					int x;
					x = tu_mess();
					//name
					token = strtok(NULL, ":");
					printf("sigin1=%s\n", token);
					strcpy(allmess[x].name, token);
					//id
					token = strtok(NULL, ":");
					printf("sigin1=%s\n", token);
					strcpy(allmess[x].id, token);
					//ip
					token = strtok(NULL, ":");
					printf("sigin1=%s\n", token);
					strcpy(allmess[x].ip, token);
					//port
					token = strtok(NULL, ":");
					printf("sigin1=%s\n", token);
					strcpy(allmess[x].port, token);
					//
					token = strtok(NULL, ":");
					printf("sigin1=%s\n", token);
					strcpy(allmess[x].statu, token);
					token = strtok(NULL, ":");
					printf("sigin1=%s\n", token);
					strcpy(allmess[x].mail, token);
					allmess[x].tu = 1;
					//memset(token, '\0', 200);
					listcount++;
				//更新列表
					gdk_threads_enter;
					gtk_list_store_clear((GtkListStore*)store1);
					gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_frend_list), createModel());
					gdk_threads_leave;
					//add_sockset(&allset, sockfd, listfd, add);
					continue;
				}
				if (strcmp(token, "quit") == 0){
				printf("1 buffer=%s\n", token);
					char aaip[30];
					memset(aaip,'\0',30);
					token = strtok(NULL, ":");
					strcpy(aaip,token);
					token = strtok(NULL, ":");
					for (j = 0; j < listcount; j++) {
						if (strcmp(allmess[j].port,token) == 0 && strcmp(allmess[j].ip,aaip) == 0){
							strcpy(allmess[j].statu, "0");
								for (i = 0; i < messcount; i++)
									if (add[i][0] == allmess[j].fd){
										add[i][0] = -1;
										add[i][1] = -1;
									break;
									}
							allmess[j].fd = -1;
							allmess[j].ac = 1;
						}
					}
					//token = strtok(NULL, ":");
				//更新列表
					gdk_threads_enter;
					gtk_list_store_clear((GtkListStore*)store1);
					gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_frend_list), createModel());
					gdk_threads_leave;
					add_sockset(&allset, sockfd, listfd, add);
					continue;
				}

				memset(sysmsg, '\0', 1000);
				strcpy(sysmsg,buffer);
				add_sockset(&allset, sockfd, listfd, add);
				g_thread_create(G_THREAD_FUNC(dialog_sysmsg), NULL, FALSE, NULL);
				printf("aaa buffer=%s\n", buffer);
				continue;
			}
			for (i = 0; i < messcount; i++) {
				printf("i=%d\n",i);
				if ((add[i][0] != -1) && (FD_ISSET(add[i][1], &allset))) {
					bzero(buffer, MAXBUF + 1);
					printf("111222aaa\n");
					readcount = read(add[i][1],buffer, MAXBUF);
					//printf("readbuffer=%s\n",buffer);
					if (readcount == 0) {
						for (j = 0; j < listcount; j++) {
							if (allmess[j].fd == add[i][1]){
								strcpy(allmess[j].statu, "0");
								allmess[j].fd = -1;
								allmess[j].ac = 1;
								break;
							}
						}
						add[i][0] = -1;
						add[i][1] = -1;
						add_sockset(&allset, sockfd, listfd, add);
				//更新列表
						gdk_threads_enter;
						gtk_list_store_clear((GtkListStore*)store1);
						gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_frend_list), createModel());
						gdk_threads_leave;
						continue;
					} else {
						token = strtok(buffer, ":");
						//printf("第一次tok=%s\n",token);
						if (strcmp(token, "lian") == 0) {
							token = strtok(NULL, ":");
							printf("if(lian=%s)\n", token);
							for (j = 0; j < listcount; j++) {
								if (strcmp(allmess[j].id, token) == 0) {
									allmess[j].fd = add[i][1];
									//      char por[10];
									//      sprintf(por,"%d",ntohs(cliaddr[j].sin_port));
									//      memset(allmess[j].port,'\0',10);
									//      strcpy(allmess[j].port,por);
									printf("allmess[%d]=%d\n", j, allmess[j].fd);
									break;
								}
							}
							add_sockset(&allset, sockfd, listfd, add);
							continue;
						}
						if (strcmp(token, "file") == 0) {
							token = strtok(NULL, ":");
							memset(descfile,'\0',20);
							sprintf(descfile,"copy%s",token+1);
							to_fd=fopen(descfile,"a");
							
							add_sockset(&allset, sockfd, listfd, add);
							continue;
						}
						if (strcmp(token, "#f") == 0){
							int bytes_write,i;
							token = strtok(NULL, ":");
							//for(i=0;i<readcount;i++)
							//	fputc(token[i],to_fd);
							bytes_write=fputs(token,to_fd);
							printf("读取出:%s\n",token);
							//add_sockset(&allset, sockfd, listfd, add);
							continue;
						}
						if (strcmp(token, "fileover") == 0){
							fclose(to_fd);
							memset(ch_dialog,'\0',100);
							sprintf(ch_dialog,"文件接收成功，请在当前目录查看！\n\t");
							//g_thread_create(G_THREAD_FUNC(func_dialog),NULL, FALSE, NULL);
							func_dialog();
							continue;
						}
						for (j = 0; j < listcount; j++) {
							if (allmess[j].fd == add[i][1]) {
							printf("%s的%d来信息是%s\n",allmess[j].id,allmess[j].fd,buffer);
								if (allmess[j].winstu) {
									gdk_threads_enter();
									memset(amsg.msg, '\0', 1000);
									sprintf(amsg.msg, "%s:     ", allmess[j].name);
									strcat(amsg.msg, buffer);
									amsg.xb = j;
									combochange_log(amsg.xb, amsg.msg);
									gdk_threads_leave();
									//add_sockset(&allset, sockfd, listfd, add);
									break;
								} else {
									g_thread_create(G_THREAD_FUNC(func_action), (void *)j, FALSE, NULL);
									usleep(20000);
									gdk_threads_enter();
									memset(amsg.msg, '\0', 1000);
									sprintf(amsg.msg, "%s:     ", allmess[j].name);
									strcat(amsg.msg, buffer);
									amsg.xb = j;
									combochange_log(amsg.xb, amsg.msg);
									gdk_threads_leave();
									//add_sockset(&allset, sockfd, listfd, add);
									break;
								}
							}
						}
						//bzero(buffer,MAXBUF + 1);
						//len = read(add[i][1],buffer,MAXBUF);
						printf("acsbuffer=%s\n", buffer);
						add_sockset(&allset, sockfd, listfd, add);
						break;
					}
				}
			}
		}
	}
	close(sockfd);
//return 0;
}

//++++++++++++聊天信息显示+++++++++++++++++++
void combochange_log(int fs, char *msg)
{
	GtkTextIter iter;
	GString *order_string;
	GtkTextMark *tmp_mark;

	gtk_text_buffer_get_end_iter(allmess[fs].buffer_up, &iter);
	gtk_text_buffer_insert(allmess[fs].buffer_up, &iter,msg, -1);
	
	char mes[200];
	char date[25];
	memset(date,'\0',25);
	memset(mes,'\0',200);
printf("123456789=%s\n",msg);
	sprintf(mes,"echo '%s' >> ./recourd/recourd-%s.txt",msg,allmess[fs].name);
	if (recourd){
	system("mkdir ./recourd");
	sprintf(date,"date >> ./recourd/recourd-%s.txt",allmess[fs].name);
	system(date);
	memset(mes,'\0',200);
	sprintf(mes,"echo '%s' >> ./recourd/recourd-%s.txt",msg,allmess[fs].name);
printf("9999999999=%s\n",msg);
//	usleep(5000);
//	system(mes);
	recourd=0;
	}
	system(mes);

	order_string = g_string_new("\n");
	gtk_text_buffer_insert(allmess[fs].buffer_up, &iter, order_string->str, -1);
	g_string_free(order_string, TRUE);

//tmp_mark = gtk_text_buffer_create_mark (allmess[fs].listup, NULL, &iter, FALSE);
//gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW (allmess[fs].listup), tmp_mark);
//gtk_text_buffer_delete_mark (allmess[fs].buffer_up, tmp_mark);

//g_free (msg);
}

//+++++++++++++++++++++++++++++++++++++

void go_to_broadcast(int aas)
{
	char *text;
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(allmess[aas].buffer_down), &start, &end);	/*获得缓冲区开始和结束位置的Iter */
	const GtkTextIter s = start, e = end;
	text = (char *)gtk_text_buffer_get_text(GTK_TEXT_BUFFER(allmess[aas].buffer_down), &s, &e, FALSE);	/*获得文本框缓冲区文本 */
	gdk_threads_enter();
	memset(amsg.msg, '\0', 1000);
	sprintf(amsg.msg, "%s:     ", allmess[0].name);
	strcat(amsg.msg, text);
	amsg.xb = aas;
	combochange_log(amsg.xb, amsg.msg);
	gdk_threads_leave();
}

//+++++++++++++++创建与好友的联接+++++++
int accect_to(int w)
{
//      gdk_threads_enter();
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	printf("sock=%d\n", sock);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	int b = addr();
	add[b][0] = b;
	bzero(&cliaddr[b], sizeof(cliaddr[b]));
	cliaddr[b].sin_family = AF_INET;
	cliaddr[b].sin_port = htons(atoi(allmess[w].port));
	inet_aton(allmess[w].ip, (struct in_addr *)&cliaddr[b].sin_addr.s_addr);
	/* 连接客户端 */
	printf("要链接的客户IP是：%s\n", allmess[w].ip);
	printf("要链接的客户PORT是：%s\n", allmess[w].port);
	if (connect(sock, (struct sockaddr *)&cliaddr[b], sizeof(cliaddr[b])) == 0){
		add[b][1] = sock;
		//add[b][0] = 1;
		maxfd = max_fd(maxfd, sock);
		allmess[w].fd = sock;
	
		add_sockset(&allset, sockfd, listfd, add);
	//	usleep(600);
		char aaa[30];
		memset(aaa, '\0', 30);
		sprintf(aaa, "lian:%s", allmess[0].id);
		int cou;
		cou=strlen(aaa);
	
	//	strcat(user,"\r\n"); 
		printf("aaa11=%s", aaa);
		write(sock,aaa,strlen(aaa));

		memset(aaa, '\0', 30);
		sprintf(aaa, "accet");
		write(sockfd, aaa, strlen(aaa));
	//      gdk_threads_leave();
	}
	else{
	perror("Connect ");
	}
}

//+++++++++++++++发送消息+++++++++++
void wri_to(GtkWidget * widget, gpointer data)
{
	int ss = (int)data;
//	if (allmess[ss].fd == -1) {
//		accect_to(ss);
//	}
//	printf("allmess.fd=%d\n", allmess[ss].fd);
	char *text;
	GtkTextIter start, end;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(allmess[ss].buffer_down), &start, &end);	/*获得缓冲区开始和结束位置的Iter */
	const GtkTextIter s = start, e = end;
	text = (char *)gtk_text_buffer_get_text(GTK_TEXT_BUFFER(allmess[ss].buffer_down), &s, &e, FALSE);	/*获得文本框缓冲区文本 */
	printf("%s\n",text);
	if (strlen(text) != 0){
		printf("go_to_broadcase\n");
		g_thread_create(G_THREAD_FUNC(go_to_broadcast),(void *)ss, FALSE, NULL);
	}
//	usleep(200);
	write(allmess[ss].fd, text,strlen(text));
	printf("向%s的%d发送的信息是%s\n",allmess[ss].id,allmess[ss].fd,text);
}

//+++++++++++++++对话框退出事件++++++++++++
void delete_event(GtkWidget * widget, GdkEvent * event, gpointer data)
{
	int ss = (int)data;
	allmess[ss].winstu = 0;
	gtk_widget_destroy(widget);
	//gtk_main_quit();
}

//++++++++++++++++创建对话框++++++++++++++++
void func_action(int h)
{
	gdk_threads_enter();
	printf("allmess_func_111[%d].wintu＝%d\n", h, allmess[h].winstu);
	if (!allmess[h].winstu) {

//	int ss = (int)data;
	if (allmess[h].fd == -1) {
		accect_to(h);
	}
	printf("allmess.fd=%d\n", allmess[h].fd);

		printf("allmess_func_222[%d].wintu＝%d\n", h, allmess[h].winstu);
		GtkWidget *to_to;
		allmess[h].winstu = 1;
		to_to = create_toto(h);
//		gtk_widget_show_all(to_to);

		printf("allmess_func_333[%d].wintu＝%d\n",h, allmess[h].winstu);
//		exit(0);
	}
	gdk_threads_leave();
}

//++++++++++++++++系统提示框++++++++++++++++
void destroy_sysmsg(GtkWidget * widget, gpointer * data)
{

	gtk_widget_destroy(GTK_WIDGET(widget));

}

void dialog_sysmsg(GtkWidget * widget, gpointer * data)
{
	gdk_threads_enter();
	GtkWidget *sys;
	sys = create_sysmsg();
	gtk_widget_show_all(sys);
	gdk_threads_leave();
}

//++++++++++++++++对话框++++++++++++++++
void destroy(GtkWidget * widget, gpointer * data)
{

	gtk_widget_destroy(GTK_WIDGET(data));

}

void dialog(GtkWidget * widget, gpointer * data)
{
	gdk_threads_enter();

	GtkWidget *dialog;

	GtkWidget *label;

	gchar *title;

	dialog = gtk_dialog_new();

	label = gtk_label_new(NULL);

	title = "<span foreground=\"red\"><big>\n提示：\n\n该用户没有登陆，不能对其说话！！\n</big></span>";

	gtk_label_set_markup(GTK_LABEL(label), title);

	gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);

	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area), label, TRUE, TRUE, 0);

	gtk_widget_show(label);

	gtk_widget_show(dialog);

	gdk_threads_leave();

}

//++++++++++++++++选择事件++++++++++++++++
gboolean selection_changed(GtkWidget * widget, gpointer data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {
		gtk_tree_model_get(model, &iter, 1, &value, -1);
		printf("%s\n", value);
		int i;
		for (i = 0; i < listcount; i++){
			if (strcmp(allmess[i].name, value) == 0) {
	//			printf("i=%d\n", i);

				if (atoi(allmess[i].statu) == 0) {
					g_thread_create(G_THREAD_FUNC(dialog), NULL, FALSE, NULL);
					break;
				} else {
					g_thread_create(G_THREAD_FUNC(func_action), (void *)i, FALSE, NULL);
					break;
				}
			}
		}

	}
}

//++++++++++++++++好友列表+++++++++++++++++
GtkTreeModel *createModel()
{
	int unline;
	int online;

	const gchar *files[] = { "emoticon2.ico"};

	const gchar *file[] = { "emoticon3.ico"};

	GtkWidget *cellView;
///	GdkPixbuf *pixbuf;
	GtkTreeIter iter1, iter2;
//	GtkTreeStore *store1;
	gint i, j, s, k = 0;

	char ag[2]="";
	char line[2][20];
	int a1=1;
	int a2=1;
	memset(line, 0, 40);
	strcpy(line[0], "在线好友");
	strcpy(line[1], "不在线好友");

///	store1 = gtk_tree_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
	store1 = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
	cellView = gtk_cell_view_new();
	for (i = 0; i < 2; i++) {
///		pixbuf = gdk_pixbuf_new_from_file(files[0], NULL);
		gtk_tree_store_append(store1, &iter1, NULL);
//		gtk_tree_store_set(store1, &iter1, PIXBUF_COL, pixbuf, TEXT_COL, line[i], -1);
		gtk_tree_store_set(store1, &iter1, PIXBUF_COL,ag, TEXT_COL, line[i], -1);
///		gdk_pixbuf_unref(pixbuf);
//allmess[messcount]
		if (i == 0) {
			for (j = 1; j < listcount; j++) {
				if (atoi(allmess[j].statu) == 1) {
	printf("在线循环：%d次\n",j);
					a1=0;
					s = rand() % 5;
///					pixbuf = gdk_pixbuf_new_from_file(file[0], NULL);
					//pixbuf = gtk_widget_render_icon(cellView,stocks[s],GTK_ICON_SIZE_BUTTON, NULL);
					gtk_tree_store_append(store1, &iter2, &iter1);
///					gtk_tree_store_set(store1, &iter2, PIXBUF_COL, pixbuf, TEXT_COL, allmess[j].name, -1);
					gtk_tree_store_set(store1, &iter2, PIXBUF_COL,ag, TEXT_COL, allmess[j].name, -1);
					printf("%s\n", allmess[j].name);
				}
			}
			if(a1){
///			pixbuf = gdk_pixbuf_new_from_file(file[0], NULL);
			//pixbuf = gtk_widget_render_icon(cellView,stocks[s],GTK_ICON_SIZE_BUTTON, NULL);
			gtk_tree_store_append(store1, &iter2, &iter1);
///			gtk_tree_store_set(store1, &iter2, PIXBUF_COL, pixbuf, TEXT_COL,"无人上线", -1);
			gtk_tree_store_set(store1, &iter2, PIXBUF_COL,ag, TEXT_COL,"无人上线", -1);
			continue;
			}
			//online=unline;
///			gdk_pixbuf_unref(pixbuf);
		} else {
			for (j = 0; j < listcount; j++) {
				if (atoi(allmess[j].statu) == 0) {
					s = rand() % 5;
					a2=0;
///					pixbuf = gdk_pixbuf_new_from_file(file[0], NULL);
					//pixbuf = gtk_widget_render_icon(cellView,files[s],GTK_ICON_SIZE_BUTTON, NULL);
					gtk_tree_store_append(store1, &iter2, &iter1);

///					gtk_tree_store_set(store1, &iter2, PIXBUF_COL, pixbuf, TEXT_COL, allmess[j].name, -1);
					gtk_tree_store_set(store1, &iter2, PIXBUF_COL,ag, TEXT_COL, allmess[j].name, -1);
///					gdk_pixbuf_unref(pixbuf);
					printf("%s\n", allmess[j].name);
				}
			}
			if(a2){
///			pixbuf = gdk_pixbuf_new_from_file(file[0], NULL);
			//pixbuf = gtk_widget_render_icon(cellView,stocks[s],GTK_ICON_SIZE_BUTTON, NULL);
			gtk_tree_store_append(store1, &iter2, &iter1);
///			gtk_tree_store_set(store1, &iter2, PIXBUF_COL, pixbuf, TEXT_COL,"无人上线", -1);
			gtk_tree_store_set(store1, &iter2, PIXBUF_COL,ag, TEXT_COL,"无人离线", -1);
			}
		}
	}
	return GTK_TREE_MODEL(store1);
}

//++++++++++创建登陆，放入线程，根据条件创建+++++++
void create_log(void)
{
/*
	gdk_threads_enter();
	while (1){
		if (log_stut==1){
			GtkWidget *window_list;
			window_list =create_list();
			gtk_widget_show_all(window_list);
			gtk_widget_destroy(window_login);
			break;
			}
	}
	gdk_threads_leave();
*/
//gdk_threads_enter();
	while (1) {
		if (log_stut == 1) {
//			gtk_widget_hide(window_login);
			GtkWidget *window_list;
			window_list = create_list();
			gtk_widget_show_all(window_list);
			printf("kkkkkkkkkkkk\n");
			break;
		}
	}
	gtk_widget_hide(window_login);
//gdk_threads_leave();
}
//++++++++++++++++++发送文件++++++++++++++++++++++++++++
void on_file_select_ok(GtkWidget *button,GtkFileSelection *fs)
{
	GtkWidget *dialog;
	gchar message[1024];
	const gchar *filename;
	char name[20],*p,ch;
	char filebuffer[SENFILE];
	FILE *fp;
	filename=gtk_file_selection_get_filename(fs);
	if(g_file_test(filename,G_FILE_TEST_IS_DIR))
		sprintf(message,"你选择的目录是：%s,不能传输",filename);
	else{
		sprintf(message,"传送文件：%s",filename);
		if ((fp=fopen(filename,"r"))!=NULL){
			memset(name,'\0',20);
			p=strrchr(filename,'/');
			sprintf(name,"file:%s",p);
			write(allmess[filefd].fd,name,strlen(name));
			usleep(2000);

			rewind(fp);
			memset(filebuffer,'\0',SENFILE);
			while(1){
			memset(filebuffer,'\0',SENFILE);
			filebuffer[0]='#';
			filebuffer[1]='f';
			filebuffer[2]=':';
				//while ((ch=fgetc(fp))!=EOF){
				for (i=3;i<SENFILE-1;i++){
					if((ch=fgetc(fp))!=EOF)
						filebuffer[i]=ch;
					else
						break;
				}
			write(allmess[filefd].fd,filebuffer,SENFILE);
			printf("发送：%s\n",filebuffer);
			usleep(2000);
			if(ch==EOF) break;
			}
			write(allmess[filefd].fd,"fileover",strlen("fileover"));
			printf("over\n");
			fclose(fp);
		}
	}
	dialog=gtk_message_dialog_new(NULL,
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_INFO,
			GTK_BUTTONS_OK,
			message,
			NULL);
	gtk_widget_destroy(GTK_WIDGET(fs));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
	}
//+++++++++++++发送文件按键+++++++++++++++++++++++++++++
void on_button_clicked(GtkWidget *button,gpointer userdata)
{
	filefd=(int)userdata;
	GtkWidget *dialog;
	dialog=gtk_file_selection_new("请选择一个文件或目录：");
	gtk_window_set_position(GTK_WINDOW(dialog),GTK_WIN_POS_CENTER);
	gtk_signal_connect(GTK_OBJECT(dialog),"destroy",GTK_SIGNAL_FUNC(gtk_widget_destroy),&dialog);
	gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(dialog)->ok_button),
					"clicked",
					GTK_SIGNAL_FUNC(on_file_select_ok),
					GTK_FILE_SELECTION(dialog));
	gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION(dialog)->cancel_button),"clicked",
						GTK_SIGNAL_FUNC(gtk_widget_destroy),GTK_OBJECT(dialog));
	gtk_widget_show(dialog);
}
//++++++++++++++++登陆按键+++++++++++++++++
void func_button_login(GtkWidget * widget, gpointer data)
{
	struct log *aa = (struct log *)data;
	gchar *id = (gchar *) gtk_entry_get_text(GTK_ENTRY(aa->id));
	gchar *pwd = (gchar *) gtk_entry_get_text(GTK_ENTRY(aa->pwd));
	memset(alogsi.id,'\0',strlen(alogsi.id));
	memset(alogsi.id,'\0',strlen(alogsi.pwd));
	strcpy(alogsi.id,id);
	strcpy(alogsi.pwd, pwd);
	if (fdstatu) {
		g_thread_create(G_THREAD_FUNC(go_to), NULL, FALSE, NULL);
		fdstatu = 0;
	}
	wrilog = 1;
//	g_thread_create(G_THREAD_FUNC(create_log),NULL,FALSE, NULL) ;
	create_log();
//	printf("alogsi:%s\n", alogsi.id);
//	printf("%s\n", alogsi.pwd);
	//gtk_widget_hide(window_list1);
}

//++++++++++++++注册按钮+++++++++++++++++++++++++++
void func_button_adduser(GtkWidget * widget, gpointer data)
{
	gtk_widget_hide(window_login);
	//GtkWidget *window_adduser;
	window_adduser = create_adduser();
	gtk_widget_show_all(window_adduser);
}

//+++++++++++注册以后，返回登陆界面++++++++++++++
void fanhui_log(void)
{
//	gdk_threads_enter();
	while (1){
		if (add_stut) {
			gtk_widget_destroy(window_adduser);
			gtk_widget_show_all(window_login);
			break;
		}
	}
//	gdk_threads_leave();
}
//+++++++++++++++提交按钮++++++++++++++++++++++++++
void funt_button_update(GtkWidget * widget, gpointer data)
{
	struct widadd *aa = (struct widadd *)data;
	gchar *name = (gchar *) gtk_entry_get_text(GTK_ENTRY(aa->name));
//	gchar *id = (gchar *) gtk_entry_get_text(GTK_ENTRY(aa->id));
	gchar *pwd1 = (gchar *) gtk_entry_get_text(GTK_ENTRY(aa->pwd1));
	gchar *pwd2 = (gchar *) gtk_entry_get_text(GTK_ENTRY(aa->pwd2));
	gchar *email = (gchar *) gtk_entry_get_text(GTK_ENTRY(aa->email));
	strcpy(aaddsi.name, name);
//	strcpy(aaddsi.id, id);
	
	if (strcmp(email,"")==0)
		strcpy(aaddsi.email, "null");
	else
		strcpy(aaddsi.email,email);
	if (strcmp(pwd1, pwd2) == 0) {
		strcpy(aaddsi.pwd, pwd1);
		if (fdstatu) {
			g_thread_create(G_THREAD_FUNC(go_to), NULL, FALSE, NULL);
			fdstatu = 0;
		}
		wriadd = 1;
//		g_thread_create(G_THREAD_FUNC(fanhui_log), NULL, FALSE, NULL);
	fanhui_log();
	}
}
//++++++++++++++++++发送电子邮件++++++++++++++++++++++++++++
void button_send_mail (GtkWidget * widget, gpointer data)
{
	gchar *ch_mail = (gchar *) gtk_entry_get_text(GTK_ENTRY(entry_email));
	gchar *ch_subject = (gchar *) gtk_entry_get_text(GTK_ENTRY(entry_subject));
	printf("地址：%s\n",ch_mail);
	printf("标题：%s\n",ch_subject);
	char *text;
	GtkTextIter start,end;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(atextview_email), &start, &end);
	/*获得缓冲区开始和结束位置的Iter */
	const GtkTextIter s = start, e = end;
	text = (char *)gtk_text_buffer_get_text(GTK_TEXT_BUFFER(atextview_email), &s, &e, FALSE);
	/*获得文本框缓冲区文本 */
	printf("正文：%s\n",text);
	char count[10000];
	memset(count,'\0',10000);
	//echo aaa|mutt -s 'myqq:127.123.123.123:8000:' myqqmail@126.com
	sprintf(count,"echo '%s'|mutt -s '%s' %s",text,ch_subject,ch_mail);
	int of;
	of=system(count);
	if(!of){
	gtk_widget_destroy((GtkWidget *)data);
	memset(ch_dialog,'\0',50);
	sprintf(ch_dialog,"您的电子邮件已发送！");
	g_thread_create(G_THREAD_FUNC(func_dialog),NULL, FALSE, NULL);
	}
	else{
	printf("%s",count);
	}
}
//++++++++++++++++++聊天记录按键++++++++++++++++++++++++++++
void button_recourd (GtkWidget * widget, gpointer *data)
{
	int a=(int)data;
	GtkWidget *recoud;
	recoud=create_recourd(a);
}
//++++++++++++++++++聊天记录退出++++++++++++++++++++++++++++
void on_delete_recoud(GtkWidget * widget, gpointer date)
{
	gtk_widget_destroy(widget);
}
//++++++++++++++++++聊天记录窗口++++++++++++++++++++++++++++
GtkWidget* create_recourd (int a)
{

	GtkWidget *window;
	GdkPixbuf *window_icon_pixbuf;
	GtkWidget *scrolledwindow1;
	GtkWidget *textview_recourd;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(on_delete_recoud), NULL);
	gtk_widget_set_size_request (window, 350, 435);
	gtk_container_set_border_width (GTK_CONTAINER (window), 2);
	gtk_window_set_title (GTK_WINDOW (window), "聊天记录");
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

	scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow1);
	gtk_container_add (GTK_CONTAINER (window), scrolledwindow1);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);

	textview_recourd = gtk_text_view_new ();
	gtk_widget_show (textview_recourd);
	gtk_container_add (GTK_CONTAINER (scrolledwindow1), textview_recourd);
	gtk_text_view_set_editable (GTK_TEXT_VIEW (textview_recourd),FALSE);

//读取聊天记录文件

	FILE *fp;
	int i=0;
	char ch;
	char count[10000];
	char filename[25];
	sprintf(filename,"./recourd/recourd-%s.txt",allmess[a].name);
	if ((fp=fopen(filename,"r"))!=NULL){
		rewind(fp);
		memset(count,'\0',10000);
		while ((ch=fgetc(fp))!=EOF){
		count[i]=ch;
		i++;
		}
		fclose(fp);
	printf("%s",count);
	}
	else{
	perror("Open file file2\n");
	}

	GtkTextIter iter;
	GString *order_string;
	GtkTextMark *tmp_mark;

	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview_recourd));
	gtk_text_buffer_get_end_iter(buffer,&iter);
	gtk_text_buffer_insert(buffer, &iter,count,-1);

	order_string = g_string_new("\n");
	gtk_text_buffer_insert(buffer, &iter, order_string->str, -1);
	g_string_free(order_string, TRUE);

//

	gtk_widget_show_all(window);

	return window;

}
//++++++++++++++++++电子邮件按键++++++++++++++++++++++++++++
void button_email (GtkWidget * widget, gpointer *data)
{
	int a=(int)data;
	GtkWidget *email;
	email=create_email(a);
}
//++++++++++++++++++电子邮件窗口++++++++++++++++++++++++++++
GtkWidget* create_email (int a)
{
	GtkWidget *window;
	GtkWidget *vbox1;
	GtkWidget *vbox2;
	GtkWidget *hbox5;
	GtkWidget *label8;
	GtkWidget *hbox6;
	GtkWidget *label9;
//	GtkWidget *entry_email;
//	GtkWidget *entry_subject;
	GtkWidget *textview_email;
	GtkWidget *label10;
	GtkWidget *scrolledwindow1;
	GtkWidget *hbox4;
	GtkWidget *label11;
	GtkWidget *label13;
	GtkWidget *mail_send;
	GtkWidget *label14;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request (window, 478, 361);
	gtk_container_set_border_width (GTK_CONTAINER (window), 5);
	gtk_window_set_title (GTK_WINDOW (window), "发送电子邮件");
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
	g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(on_delete_recoud), NULL);

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (window), vbox1);

	vbox2 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox2);
	gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);

	hbox5 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox5);
	gtk_box_pack_start (GTK_BOX (vbox2), hbox5, TRUE, TRUE, 0);

	label8 = gtk_label_new ("\351\202\256\344\273\266\345\234\260\345\235\200\357\274\232");
	gtk_widget_show (label8);
	gtk_box_pack_start (GTK_BOX (hbox5), label8, FALSE, FALSE, 0);
	gtk_widget_set_size_request (label8, 79, -1);
	gtk_misc_set_alignment (GTK_MISC (label8), 0.81, 0.5);

	entry_email = gtk_entry_new ();
	gtk_widget_show (entry_email);
	gtk_box_pack_start (GTK_BOX (hbox5), entry_email, TRUE, TRUE, 0);
	gtk_entry_set_text (GTK_ENTRY (entry_email),allmess[a].mail);
	gtk_entry_set_invisible_char (GTK_ENTRY (entry_email), 8226);

	hbox6 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox6);
	gtk_box_pack_start (GTK_BOX (vbox2), hbox6, TRUE, TRUE, 0);

	label9 = gtk_label_new ("\346\240\207\351\242\230\357\274\232");
	gtk_widget_show (label9);
	gtk_box_pack_start (GTK_BOX (hbox6), label9, FALSE, FALSE, 0);
	gtk_widget_set_size_request (label9, 47, -1);
	gtk_misc_set_alignment (GTK_MISC (label9), 0.8, 0.5);

	entry_subject = gtk_entry_new ();
	gtk_widget_show (entry_subject);
	gtk_box_pack_start (GTK_BOX (hbox6), entry_subject, TRUE, TRUE, 0);
	gtk_entry_set_invisible_char (GTK_ENTRY (entry_subject), 8226);

	label10 = gtk_label_new ("");
	gtk_widget_show (label10);
	gtk_box_pack_start (GTK_BOX (vbox2), label10, FALSE, FALSE, 0);

	scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow1);
	gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);

	textview_email = gtk_text_view_new ();
	gtk_widget_show (textview_email);
	gtk_container_add (GTK_CONTAINER (scrolledwindow1), textview_email);
	gtk_widget_set_size_request (textview_email, -1, 173);
	gtk_text_buffer_set_text (gtk_text_view_get_buffer (GTK_TEXT_VIEW (textview_email)),"\346\255\243\346\226\207\357\274\232\n\t", -1);

	hbox4 = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox4);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox4, TRUE, TRUE, 0);
	gtk_widget_set_size_request (hbox4, -1, 0);

	label11 = gtk_label_new (" ");
	gtk_widget_show (label11);
	gtk_box_pack_start (GTK_BOX (hbox4), label11, FALSE, FALSE, 0);
	gtk_widget_set_size_request (label11, 349, 1);

	label13 = gtk_label_new (" ");
	gtk_widget_show (label13);
	gtk_box_pack_start (GTK_BOX (hbox4), label13, FALSE, FALSE, 0);
	gtk_widget_set_size_request (label13, 19, 1);

	mail_send = gtk_button_new_with_mnemonic ("\345\217\221\351\200\201\351\202\256\344\273\266");
	gtk_widget_show (mail_send);
	gtk_box_pack_start (GTK_BOX (hbox4), mail_send, FALSE, FALSE, 0);
	gtk_widget_set_size_request (mail_send, -1, 1);

	label14 = gtk_label_new (" ");
	gtk_widget_show (label14);
	gtk_box_pack_start (GTK_BOX (hbox4), label14, FALSE, FALSE, 0);
	gtk_widget_set_size_request (label14, 2, 1);

	gtk_widget_grab_focus (entry_email);
	gtk_widget_grab_default (entry_email);


	atextview_email = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview_email));

	g_signal_connect(G_OBJECT(mail_send), "clicked",G_CALLBACK(button_send_mail),(gpointer)window);

	gtk_widget_show_all(window);
	return window;
}
//++++++++++++++++++创建登陆窗口++++++++++++++++++++++++++++
GtkWidget *create_login(void)
{
	GtkWidget *window_login;
	GtkWidget *vbox2;
	GtkWidget *label1;
	GtkWidget *hbox1;
	GtkWidget *label_user;
	GtkWidget *label5;
	GtkWidget *hbox2;
	GtkWidget *label_pwd;
	GtkWidget *hbox4;
	GtkWidget *entry_user;
	GtkWidget *entry_pwd;
	GtkWidget *label12;
	GtkWidget *label7;
	GtkWidget *hbox3;
	GtkWidget *label8;
	GtkWidget *label9;
	GtkWidget *button_login;
	GtkWidget *button_adduser;
	GtkWidget *label10;
	GtkWidget *label3;

	GdkColor color;
	color.red = 0x4400;
	color.green = 0xbf00;
	color.blue = 0xf100;   

	window_login = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_login, 401, 264);
	gtk_window_set_title(GTK_WINDOW(window_login), "\347\231\273\351\231\206\347\225\214\351\235\242");
	gtk_window_set_resizable(GTK_WINDOW(window_login), FALSE);
	gtk_window_set_position(GTK_WINDOW(window_login), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window_login), "delete_event",G_CALLBACK(gtk_main_quit), NULL);

//	gtk_widget_modify_bg(window_login,GTK_STATE_NORMAL, &color);

	vbox2 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox2);
	gtk_container_add(GTK_CONTAINER(window_login), vbox2);

	label1 = gtk_label_new("\346\254\242\350\277\216\344\275\277\347\224\250\344\270\252\344\272\272\350\201\212\345\244\251\345\267\245\345\205\267");
	gtk_widget_show(label1);
	gtk_box_pack_start(GTK_BOX(vbox2), label1, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label1, -1, 68);

	hbox1 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox1);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox1, TRUE, TRUE, 0);

	label_user = gtk_label_new("\345\270\220\345\217\267\357\274\232");
	gtk_widget_show(label_user);
	gtk_box_pack_start(GTK_BOX(hbox1), label_user, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_user, 110, -1);
	gtk_label_set_justify(GTK_LABEL(label_user), GTK_JUSTIFY_RIGHT);
	gtk_misc_set_alignment(GTK_MISC(label_user), 1, 0.52);

	entry_user = gtk_entry_new();
	gtk_widget_show(entry_user);
	gtk_box_pack_start(GTK_BOX(hbox1), entry_user, TRUE, TRUE, 0);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_user), 8226);

	label5 = gtk_label_new(" ");
	gtk_widget_show(label5);
	gtk_box_pack_start(GTK_BOX(hbox1), label5, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label5, 90, -1);

	hbox2 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox2);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox2, TRUE, TRUE, 0);

	label_pwd = gtk_label_new("\345\257\206\347\240\201\357\274\232");
	gtk_widget_show(label_pwd);
	gtk_box_pack_start(GTK_BOX(hbox2), label_pwd, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label_pwd, 110, -1);
	gtk_label_set_justify(GTK_LABEL(label_pwd), GTK_JUSTIFY_RIGHT);
	gtk_misc_set_alignment(GTK_MISC(label_pwd), 1, 0.52);

	hbox4 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox4);
	gtk_box_pack_start(GTK_BOX(hbox2), hbox4, TRUE, TRUE, 0);

	entry_pwd = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(entry_pwd), FALSE);
	gtk_widget_show(entry_pwd);
	gtk_box_pack_start(GTK_BOX(hbox4), entry_pwd, TRUE, TRUE, 0);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_pwd), 8226);

	label12 = gtk_label_new(" ");
	gtk_widget_show(label12);
	gtk_box_pack_start(GTK_BOX(hbox4), label12, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label12, 90, -1);

	label7 = gtk_label_new("");
	gtk_widget_show(label7);
	gtk_box_pack_start(GTK_BOX(hbox2), label7, FALSE, FALSE, 0);

	hbox3 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox3);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox3, TRUE, TRUE, 0);
	gtk_widget_set_size_request(hbox3, -1, 3);

	label8 = gtk_label_new("");
	gtk_widget_show(label8);
	gtk_box_pack_start(GTK_BOX(hbox3), label8, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label8, 115, -1);

	button_login = gtk_button_new_with_mnemonic("\347\231\273\351\231\206");
	gtk_widget_show(button_login);
	gtk_box_pack_start(GTK_BOX(hbox3), button_login, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_login, 60, -1);

	label9 = gtk_label_new(" ");
	gtk_widget_show(label9);
	gtk_box_pack_start(GTK_BOX(hbox3), label9, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label9, 53, -1);

	button_adduser = gtk_button_new_with_mnemonic("\346\263\250\345\206\214");
	gtk_widget_show(button_adduser);
	gtk_box_pack_start(GTK_BOX(hbox3), button_adduser, FALSE, FALSE, 0);
	gtk_widget_set_size_request(button_adduser, 60, -1);

	label10 = gtk_label_new("");
	gtk_widget_show(label10);
	gtk_box_pack_start(GTK_BOX(hbox3), label10, FALSE, FALSE, 0);

	label3 = gtk_label_new("");
	gtk_widget_show(label3);
	gtk_box_pack_start(GTK_BOX(vbox2), label3, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label3, -1, 43);

//GtkWidget *entry_user;
//GtkWidget *entry_pwd;
//GtkWidget *button_login;
//GtkWidget *button_adduser;
	login.id = GTK_ENTRY(entry_user);
	login.pwd = GTK_ENTRY(entry_pwd);

	g_signal_connect(G_OBJECT(button_login), "clicked", G_CALLBACK(func_button_login), (gpointer) & login);

	g_signal_connect(G_OBJECT(button_adduser), "clicked", G_CALLBACK(func_button_adduser), NULL);

	return window_login;
}
//++++++++++++++++++创建注册用户窗口++++++++++++++++++++++++++++
GtkWidget *create_adduser(void)
{
	GtkWidget *window_adduser;
	GtkWidget *vbox4;
	GtkWidget *label19;
	GtkWidget *hbox5;
	GtkWidget *label21;
	GtkWidget *entry_name;
	GtkWidget *label22;
	GtkWidget *hbox6;
	GtkWidget *label31;
	GtkWidget *entry_id;
	GtkWidget *label25;
	GtkWidget *hbox7;
	GtkWidget *label23;
	GtkWidget *entry_pwd1;
	GtkWidget *label26;
	GtkWidget *hbox8;
	GtkWidget *label28;
	GtkWidget *entry_pwd2;
	GtkWidget *label27;
	GtkWidget *hbox9;
	GtkWidget *label29;
	GtkWidget *entry_email;
	GtkWidget *label30;
	GtkWidget *vbox5;
	GtkWidget *hbox10;
	GtkWidget *label34;
	GtkWidget *button_update;
	GtkWidget *label35;
	GtkWidget *button_reset;
	GtkWidget *label36;
	GtkWidget *label33;

	window_adduser = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_adduser, 345, 441);
	gtk_window_set_title(GTK_WINDOW(window_adduser), "\346\263\250\345\206\214\351\241\265\351\235\242");
	gtk_window_set_resizable(GTK_WINDOW(window_adduser), FALSE);
	gtk_window_set_icon_name(GTK_WINDOW(window_adduser), "stock_smiley-18");
	gtk_window_set_position(GTK_WINDOW(window_adduser), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window_adduser), "delete_event",G_CALLBACK(gtk_main_quit), NULL);

	vbox4 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox4);
	gtk_container_add(GTK_CONTAINER(window_adduser), vbox4);

	label19 = gtk_label_new("\346\254\242\350\277\216\346\202\250\346\263\250\345\206\214\346\210\221\344\273\254\350\275\257\344\273\266");
	gtk_widget_show(label19);
	gtk_box_pack_start(GTK_BOX(vbox4), label19, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label19, -1, 71);

	hbox5 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox5);
	gtk_box_pack_start(GTK_BOX(vbox4), hbox5, TRUE, TRUE, 0);

	label21 = gtk_label_new("\347\275\221\345\220\215\357\274\232");
	gtk_widget_show(label21);
	gtk_box_pack_start(GTK_BOX(hbox5), label21, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label21, 70, -1);
	gtk_misc_set_alignment(GTK_MISC(label21), 1, 0.5);

	entry_name = gtk_entry_new();
	gtk_widget_show(entry_name);
	gtk_box_pack_start(GTK_BOX(hbox5), entry_name, TRUE, TRUE, 0);
	gtk_widget_set_size_request(entry_name, 160, -1);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_name), 8226);

	label22 = gtk_label_new(" * ");
	gtk_widget_show(label22);
	gtk_box_pack_start(GTK_BOX(hbox5), label22, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label22, 70, -1);

	hbox6 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox6);
	gtk_box_pack_start(GTK_BOX(vbox4), hbox6, FALSE, FALSE, 0);

	label31 = gtk_label_new("\345\270\220\345\217\267\357\274\232");
	gtk_widget_show(label31);
	gtk_box_pack_start(GTK_BOX(hbox6), label31, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label31, 70, 51);
	gtk_misc_set_alignment(GTK_MISC(label31), 1, 0.5);

	entry_id = gtk_entry_new();
	gtk_widget_show(entry_id);
	gtk_box_pack_start(GTK_BOX(hbox6), entry_id, TRUE, TRUE, 0);
	gtk_widget_set_size_request(entry_id, -1, 29);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_id), 8226);

	label25 = gtk_label_new(" * ");
	gtk_widget_show(label25);
	gtk_box_pack_start(GTK_BOX(hbox6), label25, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label25, 70, -1);

	hbox7 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox7);
	gtk_box_pack_start(GTK_BOX(vbox4), hbox7, TRUE, TRUE, 0);

	label23 = gtk_label_new("\345\257\206\347\240\201\357\274\232");
	gtk_widget_show(label23);
	gtk_box_pack_start(GTK_BOX(hbox7), label23, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label23, 70, -1);
	gtk_misc_set_alignment(GTK_MISC(label23), 1, 0.5);

	entry_pwd1 = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(entry_pwd1), FALSE);
	gtk_widget_show(entry_pwd1);
	gtk_box_pack_start(GTK_BOX(hbox7), entry_pwd1, TRUE, TRUE, 0);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_pwd1), 8226);

	label26 = gtk_label_new("*");
	gtk_widget_show(label26);
	gtk_box_pack_start(GTK_BOX(hbox7), label26, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label26, 70, -1);

	hbox8 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox8);
	gtk_box_pack_start(GTK_BOX(vbox4), hbox8, TRUE, TRUE, 0);

	label28 = gtk_label_new("\345\257\206\347\240\201\357\274\232");
	gtk_widget_show(label28);
	gtk_box_pack_start(GTK_BOX(hbox8), label28, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label28, 70, 0);
	gtk_misc_set_alignment(GTK_MISC(label28), 1, 0.5);

	entry_pwd2 = gtk_entry_new();
	gtk_entry_set_visibility(GTK_ENTRY(entry_pwd2), FALSE);
	gtk_widget_show(entry_pwd2);
	gtk_box_pack_start(GTK_BOX(hbox8), entry_pwd2, TRUE, TRUE, 0);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_pwd2), 8226);

	label27 = gtk_label_new(" * ");
	gtk_widget_show(label27);
	gtk_box_pack_start(GTK_BOX(hbox8), label27, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label27, 70, -1);

	hbox9 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox9);
	gtk_box_pack_start(GTK_BOX(vbox4), hbox9, FALSE, FALSE, 0);

	label29 = gtk_label_new("\347\224\265\345\255\220\351\202\256\344\273\266\357\274\232");
	gtk_widget_show(label29);
	gtk_box_pack_start(GTK_BOX(hbox9), label29, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label29, 70, 56);
	gtk_misc_set_alignment(GTK_MISC(label29), 1, 0.5);

	entry_email = gtk_entry_new();
	gtk_widget_show(entry_email);
	gtk_box_pack_start(GTK_BOX(hbox9), entry_email, TRUE, TRUE, 0);
	gtk_widget_set_size_request(entry_email, 166, 29);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry_email), 8226);

	label30 = gtk_label_new(" * ");
	gtk_widget_show(label30);
	gtk_box_pack_start(GTK_BOX(hbox9), label30, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label30, 70, -1);

	vbox5 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox5);
	gtk_box_pack_start(GTK_BOX(vbox4), vbox5, FALSE, FALSE, 0);

	hbox10 = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(hbox10);
	gtk_box_pack_start(GTK_BOX(vbox5), hbox10, FALSE, FALSE, 0);

	label34 = gtk_label_new("");
	gtk_widget_show(label34);
	gtk_box_pack_start(GTK_BOX(hbox10), label34, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label34, 90, -1);

	button_update = gtk_button_new_with_mnemonic(" \346\217\220\344\272\244 ");
	gtk_widget_show(button_update);
	gtk_box_pack_start(GTK_BOX(hbox10), button_update, FALSE, FALSE, 0);

	label35 = gtk_label_new("");
	gtk_widget_show(label35);
	gtk_box_pack_start(GTK_BOX(hbox10), label35, FALSE, FALSE, 0);

	button_reset = gtk_button_new_with_mnemonic("\351\207\215\347\275\256");
	gtk_widget_show(button_reset);
	gtk_box_pack_start(GTK_BOX(hbox10), button_reset, FALSE, FALSE, 0);

	label36 = gtk_label_new("");
	gtk_widget_show(label36);
	gtk_box_pack_start(GTK_BOX(hbox10), label36, FALSE, FALSE, 0);

	label33 =gtk_label_new("\344\270\272\344\272\206\344\270\215\347\273\231\346\202\250\345\270\246\346\235\245\344\270\215\344\276\277\357\274\214\350\257\267\347\211\242\350\256\260\346\202\250\347\232\204\344\270\252\344\272\272\344\277\241\346\201\257\357\274\201\357\274\201\357\274\201");
	gtk_widget_show(label33);
	gtk_box_pack_start(GTK_BOX(vbox5), label33, FALSE, FALSE, 0);
	gtk_widget_set_size_request(label33, -1, 79);

//GtkWidget *entry_name;
//GtkWidget *entry_id;
//GtkWidget *entry_pwd1;
//GtkWidget *entry_pwd2;
//GtkWidget *entry_email;
//GtkWidget *button_update;
//GtkWidget *button_reset;

	wadd.name = GTK_ENTRY(entry_name);
	wadd.id = GTK_ENTRY(entry_id);
	wadd.pwd1 = GTK_ENTRY(entry_pwd1);
	wadd.pwd2 = GTK_ENTRY(entry_pwd2);
	wadd.email = GTK_ENTRY(entry_email);

	g_signal_connect(G_OBJECT(button_update), "clicked", G_CALLBACK(funt_button_update), (gpointer) & wadd);

	return window_adduser;
}
//++++++++++++++++++更新时间++++++++++++++++++++++++++++
/*
void update_time(void)
{
//	while (1){
	now=time((time_t *)NULL);
	l_time=localtime(&now);
	sprintf(data,"多人聊天工具，登陆时间  %d:%d:%d",l_time->tm_hour,l_time->tm_min,l_time->tm_sec);
//	sleep(1);
	printf("%s\n",data);
	memset(data,'\0',100);
//	}
}
*/
//++++++++++++++++++创建好友列表++++++++++++++++++++++++++++
GtkWidget *create_list(void)
{
	GtkWidget *window_clicked;
	GtkWidget *vbox2;
	GtkWidget *label_top;
	GtkWidget *scrolledwindow1;
//	GtkWidget *treeview_frend_list;
	GtkWidget *label_end;

	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	GtkTreeModel *model;
	GtkTreeSelection *selection;

	window_clicked = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_clicked, 241, 540);
	gtk_window_set_title(GTK_WINDOW(window_clicked), "\345\244\232\344\272\272\350\201\212\345\244\251\345\267\245\345\205\267\345\256\242\346\210\267\347\253\257");
	gtk_window_set_resizable(GTK_WINDOW(window_clicked), FALSE);
	gtk_window_set_icon_name(GTK_WINDOW(window_clicked), "stock_smiley-1");
	gtk_window_set_position(GTK_WINDOW(window_clicked), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window_clicked), "delete_event", G_CALLBACK(on_delete_event), NULL);

	vbox2 = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox2);
	gtk_container_add(GTK_CONTAINER(window_clicked), vbox2);
	char baoti[100];
	memset(baoti, '\0', 100);
	sprintf(baoti, " %s 您好,欢迎您登陆.", allmess[0].name);
	label_top = gtk_label_new(baoti);
	gtk_widget_show(label_top);
	gtk_box_pack_start(GTK_BOX(vbox2), label_top, FALSE, FALSE, 0);

	scrolledwindow1 = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(scrolledwindow1);
	gtk_box_pack_start(GTK_BOX(vbox2), scrolledwindow1, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledwindow1), GTK_SHADOW_IN);

//	treeview_frend_list = gtk_tree_view_new_with_model(createModel());

	treeview_frend_list = gtk_tree_view_new();
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_frend_list),(GtkTreeModel *) createModel());

	gtk_widget_show(treeview_frend_list);
	gtk_container_add(GTK_CONTAINER(scrolledwindow1), treeview_frend_list);
//好友列表
///	renderer = gtk_cell_renderer_pixbuf_new();
	renderer = gtk_cell_renderer_text_new();
///	column = gtk_tree_view_column_new_with_attributes("QQ", renderer, "pixbuf", PIXBUF_COL, NULL);
	column = gtk_tree_view_column_new_with_attributes("QQ", renderer, "text", PIXBUF_COL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_frend_list), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("好友列表", renderer, "text", TEXT_COL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_frend_list), column);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_frend_list));
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview_frend_list));
	g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(selection_changed), model);

	time_t now;
	struct tm *l_time;
	gchar buf[100];
	now=time((time_t *)NULL);
	l_time=localtime(&now);
//	g_thread_create(G_THREAD_FUNC(update_time),NULL, FALSE,NULL);
	sprintf(buf,"多人聊天工具，登陆时间是  %d:%d:%d",l_time->tm_hour,l_time->tm_min,l_time->tm_sec);
//	sleep(1);
	label_end = gtk_label_new(buf);
	gtk_widget_show(label_end);
	gtk_box_pack_start(GTK_BOX(vbox2), label_end, FALSE, FALSE, 0);
	return window_clicked;
}
//++++++++++++++++++消息对话框退出事件++++++++++++++++++++++++++++
void button_klose(GtkWidget * widget, gpointer *date)
{
	struct close *clo=(struct close *)date;
	printf("+++++++close=%d\n",clo->cl);
	gtk_widget_destroy(clo->wcl);
	allmess[clo->cl].winstu=0;
}
//++++++++++++++++++消息对话框++++++++++++++++++++++++++++
GtkWidget *create_toto(int h)
{
	GtkWidget *window;
	GtkWidget *label1;
	GtkWidget *list1;
	GtkWidget *label2;
	GtkWidget *list2;
	GtkWidget *label3;
	GtkWidget *button_send;
	GtkWidget *button_close;
	GtkWidget *table;

	GtkWidget *send_file;
	GtkWidget *image4;
	GtkWidget *xiexin;
	GtkWidget *image5;
	GtkWidget *xiaoxi;
	GtkWidget *image6;

	GdkColor color;
     color.red = 0x9e00;
     color.green = 0xcf00;
     color.blue = 0xff00;   
//	gtk_widget_modify_bg(window_login,GTK_STATE_NORMAL, &color);

	GtkWidget *scrolledwindow1;
	char acp[100];
	sprintf(acp, "%s正在与 %s 进行对话聊天",allmess[0].name, allmess[h].name);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_widget_set_size_request(window, 401, 350);
//	gtk_window_set_default_size(GTK_WINDOW(window), 200, 180);
	gtk_window_set_title(GTK_WINDOW(window), acp);
	gtk_container_set_border_width(GTK_CONTAINER(window),3);
	gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(delete_event), (gpointer) h);

	gtk_widget_modify_bg(window,GTK_STATE_NORMAL, &color);

	table = gtk_table_new(9, 10, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);
	gtk_widget_show(table);

	label1 = gtk_label_new("提示: 水平有限,有错误请谅解,交流请加 376726632！ ");
	gtk_table_attach_defaults(GTK_TABLE(table), label1, 0, 10, 0, 1);
	gtk_widget_show(label1);

	list1 = gtk_text_view_new();
	gtk_table_attach_defaults(GTK_TABLE(table), list1, 0, 10, 1, 5);
	gtk_widget_show(list1);
	gtk_text_view_set_editable(GTK_TEXT_VIEW(list1), FALSE);

	label2 = gtk_label_new("");
	gtk_table_attach_defaults(GTK_TABLE(table), label2,0,4, 5, 6);
	gtk_widget_show(label2);
	//中间按钮工具栏+++++++++++
	send_file = gtk_button_new ();
	gtk_widget_show (send_file);

	image4 =gtk_image_new_from_file("./ico/QQ截图未命名.gif");
	gtk_widget_show (image4);

	gtk_container_add (GTK_CONTAINER (send_file), image4);
	gtk_widget_set_size_request (image4, 20, -1);
	gtk_table_attach_defaults(GTK_TABLE(table),send_file,4,6, 5, 6);

	xiexin = gtk_button_new ();
	gtk_widget_show (xiexin);
	gtk_widget_set_size_request (xiexin, 25, 32);

//	image5 = create_pixmap (window, "写信.gif");
	image5 =gtk_image_new_from_file("./ico/写信.gif");
	gtk_widget_show (image5);
	gtk_container_add (GTK_CONTAINER (xiexin), image5);
	gtk_widget_set_size_request (image5,-1, 20);
	gtk_table_attach_defaults(GTK_TABLE(table),xiexin, 6,8, 5, 6);

	xiaoxi = gtk_button_new ();
	gtk_widget_show (xiaoxi);
	gtk_widget_set_size_request (xiaoxi, -1, 32);

//	image6 = create_pixmap (window, "聊天记录.jpg");
	image6 =gtk_image_new_from_file("./ico/聊天记录.jpg");
	gtk_widget_show (image6);
	gtk_container_add (GTK_CONTAINER (xiaoxi), image6);
	gtk_widget_set_size_request (image6, -1, 20);
	gtk_table_attach_defaults(GTK_TABLE(table),xiaoxi, 8,10, 5, 6);
//+++++++++++

	scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow1);
///	gtk_container_add (GTK_CONTAINER (window2), scrolledwindow1);
//	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);

	list2 = gtk_text_view_new();
	gtk_table_attach_defaults(GTK_TABLE(table), list2, 0, 10, 6, 8);
	gtk_widget_show(list2);

	gtk_container_add (GTK_CONTAINER (scrolledwindow1),list2);

	gtk_table_attach_defaults(GTK_TABLE(table),scrolledwindow1, 0, 10, 6, 8);
//++++++++++++++
	label3 = gtk_label_new("");
	gtk_table_attach_defaults(GTK_TABLE(table), label3, 0, 6, 8, 9);
	gtk_widget_show(label3);

	button_send = gtk_button_new_with_mnemonic(" 发送(_s) ");
	gtk_table_attach_defaults(GTK_TABLE(table), button_send, 6, 8, 8, 9);
	gtk_widget_show(button_send);

	button_close = gtk_button_new_with_mnemonic(" 关闭(_c) ");
	gtk_table_attach_defaults(GTK_TABLE(table), button_close, 8, 10, 8, 9);
	gtk_widget_show(button_close);

	allmess[h].listup = list1;
	allmess[h].listdown = list2;
	allmess[h].buffer_up = gtk_text_view_get_buffer(GTK_TEXT_VIEW(list1));
	allmess[h].buffer_down = gtk_text_view_get_buffer(GTK_TEXT_VIEW(list2));
//gtk_signal_connect (GTK_OBJECT (button_close),"clicked",GTK_SIGNAL_FUNC(delete_event),(gpointer)h);

	struct close klose;
	klose.wcl=window;
	klose.cl=h;

	gtk_signal_connect(GTK_OBJECT(button_send), "clicked", GTK_SIGNAL_FUNC(wri_to),(gpointer) h);
	gtk_signal_connect(GTK_OBJECT(xiaoxi), "clicked", GTK_SIGNAL_FUNC(button_recourd),(gpointer) h);
	gtk_signal_connect(GTK_OBJECT(xiexin), "clicked", GTK_SIGNAL_FUNC(button_email),(gpointer) h);
	gtk_signal_connect(GTK_OBJECT(button_close), "clicked", GTK_SIGNAL_FUNC(button_klose),(gpointer)&klose);
	gtk_signal_connect(GTK_OBJECT(send_file), "clicked", GTK_SIGNAL_FUNC(on_button_clicked),(gpointer) h);

	gtk_widget_show_all(window);

	gtk_main();

	return window;
}

//++++++++++++++++++系统消息++++++++++++++++++++++++++++
GtkWidget *create_sysmsg(void)
{
	GtkWidget *window;
	GtkWidget *box;
	GtkWidget *label;
	GtkWidget *frame;
	char *qu;
	char title[1000];
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(destroy_sysmsg), NULL);
	gtk_window_set_title(GTK_WINDOW(window), "系统发过来的消息");
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 170);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 20);
	box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), box);

	label = gtk_label_new(NULL);
	printf("a\n");
	qu = strtok(sysmsg, "：");
	qu = strtok(NULL, "：");
	sprintf(title, "<span><big>系统提示：\n \n\n%s</big></span>", qu);
	printf("aa\n");
//title="<span><big>系统提示：\n \nadsfadfadfadfadsfadfadfadsfaf</big></span>";
	gtk_label_set_markup(GTK_LABEL(label), title);
	printf("aaa\n");
	gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
	printf("aaaa\n");
	gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 5);

	printf("aaaaaaaaaa\n");

//gtk_widget_show_all(window);

	return window;
}
