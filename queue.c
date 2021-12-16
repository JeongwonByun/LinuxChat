
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

#define MAX_BUFF 100
#define MAX_MESSAGE_LEN 256


typedef struct Message {
	int user_id;
	char str[MAX_MESSAGE_LEN];
}Message;


void *sendThread();
void *recvThread(void *data);
void *sendThreadClient();

int isFull();
int isEmpty();
int enqueue(Message item);
Message* dequeue();

int sock_main, sock_client[10];
Message *msgbuff;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int front = -1;
int rear = -1;

//Send
void *sendThread() { 
	Message *tmp; 

	printf("Send Thread Start\n");

	while (1) {		
		if ((tmp = dequeue()) != NULL) { 
			for (int i = 0; i < 10; i++) {
				if (i != tmp->user_id) { 
					send(sock_client[i], (char*)tmp, sizeof(Message), 0);
				}
			}
		}

		usleep(1000);
	}
}
//Receive
void *recvThread(void *data) {
	Message buff;
	int thread_id = *((int*)data);

	printf("Receive Thread %d Start\n", thread_id);

	memset(&buff, 0, sizeof(Message)); 

	while (recv(sock_client[thread_id], (char*)&buff, sizeof(buff), 0) > 0) {
		buff.user_id = thread_id;
		if (enqueue(buff) == -1) {
			printf("Messag Buffer Full\n");
		}
	}
}

void *sendThreadClient() {
	Message tmp;
	int count;

	while (1) {
		
		memset(&tmp, 0, sizeof(tmp)); 
		//scanf("%[^\n]s", tmp.str);
		fgets(tmp.str, MAX_MESSAGE_LEN, stdin);
		tmp.str[strlen(tmp.str) - 1] = '\0';
		tmp.user_id = -1;

		count = send(sock_main, (char *)&tmp, sizeof(Message), 0);
	}
}

int isFull() { 
 	if ((front == rear + 1) || (front == 0 && rear == MAX_BUFF - 1)) {
		return 1;
	}
	return 0;
}

int isEmpty() {  
	if (front == -1) {
		return 1;
	}
	return 0;
}

int enqueue(Message item) {  

	if (isFull()) {
		return -1;
	}
	else {
		pthread_mutex_lock(&mutex);
		if (front == -1) {
			front = 0;
		}
		rear = (rear + 1) % MAX_BUFF;
		msgbuff[rear].user_id = item.user_id;
		strcpy(msgbuff[rear].str, item.str);
		pthread_mutex_unlock(&mutex);
	}
	return 0;
}

Message* dequeue() { 
	Message *item;

	if (isEmpty()) { 
		return NULL;
	}
	else {  
		pthread_mutex_lock(&mutex);
		item = &msgbuff[front];

		if (front == rear) {
			front = -1;
			rear = -1;
		}
		else {
			front = (front + 1) % MAX_BUFF;
		}
		pthread_mutex_unlock(&mutex);
		return item;
	}

}
