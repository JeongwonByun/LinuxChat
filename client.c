#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include "queue.c"

int main(){
		int th_id;
		Message buff;
		pthread_t th_send;

		struct sockaddr_in addr;


		addr.sin_family = AF_INET;
		addr.sin_port = htons(36007);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		if ((sock_main = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			printf("Socket Open Failed\n");
			exit(1);
		}

		if (connect(sock_main, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
			printf("Connect Failed\n");
			exit(4);
		}

		th_id = pthread_create(&th_send, NULL, sendThreadClient, 0);  

		if (th_id < 0) {
			printf("Send Thread Creation Failed\n");
			exit(1);
		}


		while (1) {
			memset(&buff, 0, sizeof(buff));

			if (recv(sock_main, (char*)&buff, sizeof(buff), 0) > 0) {
				printf("User %d: %s\n", buff.user_id, buff.str);
			}
			else {
				printf("Disconnected\n");
				exit(5);
			}
		}
}
