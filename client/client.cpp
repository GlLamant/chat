#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <iostream>

#define BUF_SIZE 1024

int main(){

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr ;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(1234);
	if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
		std::cout << "Connect failed.\n";
		return -1;
		}

	//Send data what inputted from console to server 
	std::string str ;
	while (1) {
		getline(std::cin, str);
		write(sock, str.c_str(), str.length());
		char buffer[BUF_SIZE] = { 0 };
		if (read(sock, buffer, BUF_SIZE) == -1) {
			std::cout << "[Error] Read data failed.\n";
			continue;
		}

		//printf("[Server IP] %s\n", buffer);
		std::cout << "[Server IP] " << inet_ntoa(server_addr.sin_addr) << std::endl;
		std::cout << "[Server Data] " << buffer << std::endl;
		if (str.compare("quit") == 0) {
			std::cout << "str is \"" << str << "\"\n";
			break;
		}
	}

	close(sock);

	return 0;
}
