/*
  ��ɾ� : gcc server.c -o server -pthread
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>		// pthread_t�� ���� ����Ÿ���� ����
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>		// pthread_mutex_lock(), pthread_mutex_trylock(), pthread_mutex_unlock(), pthread_mutex_destroy()

#define BUF_SIZE 1024		// buf ũ��
#define LISTENQ 10			// Listen Queue
#define THREAD_NUM 5		// ���� ������ Client ��

void *thrfunc(void *arg);	//������ ���� �Լ�

int result = 0;
int cntNum = 0;				// client count


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// mutex : Mutual Exclusive(��ȣ����)
// �����尣 �����ϴ� ������ ������ ��ȣ�ϱ� ���ؼ� ����Ѵ�.
// ������ ������ ��ȣ�� Critical Section(�Ӱ� ����)�� ����� �Ӱ� �������� ���ϳ��� �����常�� ���԰��� �ϵ��� �ϴ� ����� ����Ѵ�.
// ���� �� �Ӱ迵������ ��ȣ�ϰ��� �ϴ� �����Ϳ� ���� ����-���� ��ƾ�� ����.
// �����Ϳ� ���� ����-���� ��ƾ�� ���� �ϳ��� �����常 ���� �����ϰ� �ǹǷ� �ᱹ �����͸� ��ȣ�� �� �ְ� �ȴ�.

// ���ؽ��� ���� 2���� ������ �ൿ(unlock�� lock)���� ���ǵǾ� �ִ�.
// lock�� �Ӱ迵���� �����ϱ� ���� ��û, unlock�� �Ӱ迵���� ���������鼭 �ٸ� �����忡�� �Ӱ迵���� �ǵ����ֱ� ���ؼ� ����Ѵ�.
// ���� �����尡 �Ӱ迵���� �����ϱ� ���ؼ� lock�� �õ� �ߴµ�, 
// �ٸ� �����尡 �̹� �Ӱ迵���� �����ߴٸ� �ش� �����尡 unlock�� �ؼ� �Ӱ迵���� ���������� ������ ��ٸ��� �ȴ�.

// mutex�� fast�� recursive�� 2���� ������ �����ȴ�. �⺻������ mutex�� ������ fast ���·� ���۵ȴ�. 
// �̰��� lock�� ���� �����尡 �ٽ� lock�� ���� �� �ֵ��� �� �������� �����ϱ� ���ؼ� ����Ѵ�. 

// pthread_mutex_t�� ���ؽ��� Ư¡�� �����ϱ� ���� �Ʒ� ������ ����� ����� �� �ִ�. ���� �ϳ��� �����Ͽ� �����Ѵ�.
// PTHREAD_MUTEX_INITIALIZER(fast mutex)
// PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP(recursive mutex)
// PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP(mutex ���� üũ��)

// pthread_mutex_lock()��(�Ӱ迵���� �����ϱ� ����)���ؽ� ����� ��û�Ѵ�.
// ���� ���ؽ��� �ֱ� ���°� unlocked��� ������� ����� ��� �Ӱ迵���� �����ϰ� �ǰ� �����Ѵ�.
// �ٸ� �����尡 ���ؽ� ����� ���� ���¶�� ����� ���� �� ���� ������ ��ٸ��� �ȴ�.

// pthread_mutex_unlock()�� ���ؽ������ �ǵ����ش�.
// ���� fast ���ؽ���� pthread_mutex_unlock()�� ������ unlocked ���¸� �ǵ����ش�.
// recursive ���ؽ���� ����ִ� ���ؽ��� ���� ���ҽ�Ű�� �� ���� 0�� �ȴٸ� ���ؽ������ �ǵ����ְ� �ȴ�.

// pthread_mutex_destory()�� ���ؽ� ��ü�� �����ϰ� �ڿ��� �ǵ����ش�.
// ���̻� ������� �ʴ� ���ؽ��� �ݵ�� �� �Լ��� �̿��ؼ� �����ϵ��� ����.
// ���������� ������� ���ؽ� ��ü�� ������ �Ǿ� �ִ�.
// �׷��Ƿ� �����尡 ����Ǿ��ٰ� �ϴ��� ���ؽ� ��ü�� ������ ���� �ְ� �ȴ�.
// �� �Լ��� ȣ���ؾ����� ���ؽ� ��ü�� ���� �ȴ�.


int main(int argc, char *argv[]) {
	struct sockaddr_in servaddr, cliaddr;
	//struct sockaddr_in {
	//	short sin_family;
	//	unsigned short sin_port;
	//	struct in_addr sin_addr;
	//	char sin_zero[8];
	//};

	//typedef struct _in_addr_t
	//{
	//	UINT32 s_addr;                   // load with inet_aton()
	//} in_addr;

	int listen_sock;					// server�� socket ����

	int accp_sock[THREAD_NUM];				// thread ������ŭ socket�� ���� �迭 ����

	socklen_t addrlen = sizeof(servaddr);	// 16 Bytes (2 + 2 + 4 + 8)
	// typedef UINT32 socklen_t;

	int i, status;

	pthread_t tid[THREAD_NUM];					// socket ������ŭ thread�� ���� �迭�� ����
	// typedef __uint32_t pthread_t

	pid_t pid;									// process ID�� ����
	// typedef int __pid_t;

	if (argc != 2) {							// ���α׷� ���� �� PORT �Է� �ʼ�
		printf("argc[1] use only port number : %s [PortNumber]\n", argv[0]);
		exit(0);
	}

	if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		// ���� ����
		// int socket(int domain, int type, int protocol);
		// domain : ���ͳ� ����� ��, �ý��� �� ���μ��� �� ����� �� ����(PF_INET, AF_INET�� IPv4 ���ͳ� ��������)
		// type : �������� ���� ���� (SOCK_STREAM / SOCK_DGRAM, ���� TCP/IP �������ݰ� UDP/IP ��������)
		// protocol : Ư�� ���������� �����ϱ� ���� �����̸� �밳 0�� ����Ѵ�.

		// socket �ɼ� ����ϴ� ���� : ���μ��� ����, ������ ����ϴ� ��Ʈ�� close ���� �ʰ� ������ ���. 


		perror("socket Fail");
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	// Byte ������ ���⸦ ������
	// void * memset ( void * ptr, int value, size_t num );
	// ptr : ������ ���⸦ ������ �޸� �ּ�
	// value : �ݺ������� ������ ��
	// num : ä����� �ϴ� Byte ũ��. ��, �޸� ũ��

	servaddr.sin_family = AF_INET;					// AF_INET�� IPv4 ���ͳ� ��������
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	
	// INADDR_ANY�� ����ϴ� ����.
	// Ư�� NIC�� IP�ּҸ� sin_addr.s_addr�� �����ϸ� �ٸ� NIC���� ��û�� ������ ���� �� �� ����.
	// �̶�, INADDR_ANY�� ����ϸ� �� NIC�� ��� ���ε� �� �� �־� ��� IP�� �����ص� �������μ��񽺰� �����ϴ�.
	// ����, IP�� Ư�������� ���� ��ǻ�Ϳ��� �ּҰ��� �����ؾ��ϴ� �� INADDR_ANY�� ����ϸ� �ҽ��� �������� ��밡���ϴ�.
	// IP�� �������� ���α׷��� ��Ʈ��ȣ�� �����ϴ� ��� ���ῡ ���� ó���� �����ϴ�.
	servaddr.sin_port = htons(atoi(argv[1]));		// PORT�� ���α׷� ���� �� ���� argv[1]�� ������ �����Ѵ�.

	//bind ȣ��
	if (bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		// ���Ͽ� IP�ּҿ� ��Ʈ��ȣ�� ����
		// int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
		// sockfd : ���� ��ũ����(�����̳� ������ ��ǥ�ϴ� ����)
		// myaddr : ���ͳ� ����� AF_INET�� ��, �ý��� �� ����� AF_UNIX�� ���� ���� �ٸ�. AF_INET�� ���, sockaddr_in�� ���
		// addrlen : myaddr ����ü�� ũ��
		perror("bind Fail");
		exit(0);
	}

	while (1) {
		listen(listen_sock, LISTENQ);
		// Ŭ���̾�Ʈ ���� ��û�� �����ϵ��� ����
		// int listen(int s, int backlog);
		// s : ���� ��ũ����(�����̳� ������ ��ǥ�ϴ� ����)
		// backlog : ��� ���� �޽��� ť�� ����


		puts("client wait....");

		accp_sock[cntNum] = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
		// Ŭ���̾�Ʈ ���� ��û ����
		// int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
		// s : ���� ��ũ����(�����̳� ������ ��ǥ�ϴ� ����)
		// addr : Ŭ���̾�Ʈ�� �ּ�
		// addrlen : addr �����Ͱ� ����Ű�� �ִ� ����ü ũ��

		if (accp_sock[cntNum] < 0) {
			// ���� �ĺ��ڴ� ���� �����̹Ƿ� ����ó�� ����
			perror("accept fail");
			exit(0);
		}

		if ((status = pthread_create(&tid[cntNum], NULL, &thrfunc, (void *)&accp_sock[cntNum])) != 0) {
			// ���ο� �����带 �����Ѵ�. ���� ��, 0�� ��ȯ
			// int pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
			// thread : ������ �ĺ���
			// attr : ������� ���õ� Ư¡�� ����. NULL�� �� ��� �⺻ Ư��.
			// start_routine : ����� �Լ�
			// arg : argument. ��, start_routine�� ���ڷ� �Ѿ ��.
			printf("%d thread create error: %s\n", cntNum, strerror(status));
			exit(0);
		}

		pthread_join(tid[cntNum], NULL);
		// Sub ������ ������� Main ������(���μ���) ���Ḧ ���. ��, ������������ ó���� ��� ������ ����� �� �ֵ��� ��
		// int pthread_join(pthread_t th, void **thread_return);
		// th : thread �ĺ���
		// thread_return : ������ �� ���ϰ��� ���� �������� ���� ���̴�(?). ���⿡ ���ϰ��� ������ �� �ִ�.
		// �������� �⺻ ���Ŀ��� ���ϰ��� void *�̹Ƿ� �� ���ϰ��� reference ������ �ּҷ� �޾ƿ��� ���� ���������� ������ ���.
		
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
	// ���� �б�
	// ssize_t read (int fd, void *buf, size_t nbytes)
	// fd : ���� ��ũ����(�����̳� ������ ��ǥ�ϴ� ����)
	// buf : ������ �о� ������ ����
	// nbytes : ������ ũ��

	printf("client send value = %d\n", buf);
	pthread_mutex_lock(&lock);
	// int pthread_mutex_lock(pthread_mutex_t *mutex);
	// (�Ӱ迵���� �����ϱ� ����)���ؽ� ����� ��û�Ѵ�. 
	// ���� ���ؽ��� �ֱ� ���°� unlocked��� ������� ����� ��� �Ӱ迵���� �����ϰ� �ǰ� �����Ѵ�. 
	// �ٸ� �����尡 ���ؽ� ����� ���� ���¶�� ����� ���� �� ���� ������ ��ٸ��� �ȴ�.


	result += buf;
	printf("accp_sock = %d", accp_sock);
	printf("result = %d\n", result);
	pthread_mutex_unlock(&lock);
	// int pthread_mutex_unlock(pthread_mutex_t *mutex);
	// ���ؽ������ �ǵ����ش�. 
	// ���� fast ���ؽ���� pthread_mutex_unlock()�� ������ unlocked ���¸� �ǵ����ش�. 
	// recursive ���ؽ���� ����ִ� ���ؽ��� ���� ���ҽ�Ű�� �� ���� 0�� �ȴٸ� ���ؽ������ �ǵ����ְ� �ȴ�.

	write(accp_sock, &result, 4);
	// ���� ����
	// ssize_t write (int fd, const void *buf, size_t n)
	// fd : ���� ��ũ����(�����̳� ������ ��ǥ�ϴ� ����)
	// buf : ���Ͽ� �� ������ ����ִ� ����
	// n : ������ ũ��

	close(accp_sock);
	// ���� �ݱ�
	// void Close ();
}