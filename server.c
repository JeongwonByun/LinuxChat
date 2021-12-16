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

int main()
{
	int count = 0;
	int th_id;
	Message buff;
	pthread_t th_send;

	struct sockaddr_in addr; 
	 
		pthread_t th_recv[10];

		msgbuff = (Message *)malloc(sizeof(Message) * MAX_BUFF);

		th_id = pthread_create(&th_send, NULL, sendThread, 0);  

		if (th_id < 0) {
			printf("Send Thread Creation Failed\n");
			exit(1);
		}
		
		addr.sin_family = AF_INET; 
		addr.sin_port = htons(36007); 
		addr.sin_addr.s_addr = INADDR_ANY; 

		
		if ((sock_main = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			printf("Socket Open Failed\n");
			exit(1);
		}

		 
		if (bind(sock_main, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
			printf("Bind Failed\n");
			exit(2);
		}

		
		if (listen(sock_main, 5) == -1) {
			printf("Listen Failed\n");
			exit(3);
		}

		while (1) {
			
			if ((sock_client[count] = accept(sock_main, NULL, NULL)) == -1) {
				printf("Accept Failed\n");
				continue;
			}
			else {
				if (count < 10) {
					int idx = count;

					th_id = pthread_create(&th_recv[count], NULL, recvThread, (void *)&idx);

					if (th_id < 0) {
						printf("Receive Thread #%d Creation Failed\n", count + 1);
						exit(1);
					}

					count++;
				}
			}
		}
	
}

