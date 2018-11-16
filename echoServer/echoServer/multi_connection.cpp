/*
  g++ server.cpp -o server -pthread
  g++ server.cpp -o server -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define LISTENQ 10
#define THREAD_NUM 5

void *thrfunc(void *arg);

int result = 0;
int cntNum = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	struct sockaddr_in servaddr, cliaddr;
	int listen_sock, accp_sock[THREAD_NUM];
	socklen_t addrlen = sizeof(servaddr);
	int i, status;
	pthread_t tid[THREAD_NUM];
	pid_t pid;

	if (argc != 2) {
		printf("Use %s PortNumber\n", argv[0]);
		exit(0);
	}

	if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket Fail");
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	if (bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind Fail");
		exit(0);
	}

	while (1) {
		listen(listen_sock, LISTENQ);
		puts("client wait....");
		accp_sock[cntNum] = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);

		if (accp_sock[cntNum] < 0) {
			perror("accept fail");
			exit(0);
		}

		if ((status = pthread_create(&tid[cntNum], NULL, &thrfunc, (void *)&accp_sock[cntNum])) != 0) {
			printf("%d thread create error: %s\n", cntNum, strerror(status));
			exit(0);
		}

		pthread_join(tid[cntNum], NULL);
		cntNum++;
		if (cntNum == 5)
			cntNum = 0;
	}

	return 0;
}

void *thrfunc(void *arg) {
	int accp_sock = (int) *((int*)arg);
	int buf;

	read(accp_sock, &buf, 4);

	printf("client send value = %d\n", buf);
	pthread_mutex_lock(&lock);

	result += buf;
	printf("accp_sock = %d", accp_sock);
	printf("result = %d\n", result);
	pthread_mutex_unlock(&lock);

	write(accp_sock, &result, 4);

	close(accp_sock);
}