#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termio.h>
#define pwd printf("%d\n",__LINE__);

int scanKeyboard();

// 当检测到键盘输入<q>时退出
void quitLoop(){
	while(1)
		if(scanKeyboard() == 113)
			break;
}


int main(){
	int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server_sock == -1){
		printf("Call <socket> failed!\n");
		return -1;
	}
	
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(1234);
	bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

	listen(server_sock, 10);

	while(1){
		struct sockaddr_in client_addr;
		socklen_t client_addr_size = sizeof(client_addr);
		int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);
		printf("[Client IP] %s\n", inet_ntoa(client_addr.sin_addr));
		int rev_len = 1024;
		char rev_buf[rev_len] = { 0 };
		while (1) {
			read(client_sock, rev_buf, rev_len);
			printf("[Client Data] %s\n", rev_buf);
			if (!strcmp(rev_buf, "quit")) {
				char quit_msg[] = "Close the connection from remote!";
				write(client_sock, quit_msg, strlen(quit_msg));
				close(client_sock);
				break;
			}
			else
				write(client_sock, rev_buf, rev_len);
			memset(rev_buf, 0, rev_len);
		}
	}
	close(server_sock);

	return 0;
}

int scanKeyboard(){
	int in;
	struct termios new_settings;
	struct termios stored_settings;
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(0,&stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
	 
	in = getchar();
	 
	tcsetattr(0,TCSANOW,&stored_settings);
	return in;
}
