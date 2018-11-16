/*
  명령어 : gcc server.c -o server -pthread
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>		// pthread_t와 같은 변수타입을 지정
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>		// pthread_mutex_lock(), pthread_mutex_trylock(), pthread_mutex_unlock(), pthread_mutex_destroy()

#define BUF_SIZE 1024		// buf 크기
#define LISTENQ 10			// Listen Queue
#define THREAD_NUM 5		// 연결 가능한 Client 수

void *thrfunc(void *arg);	//쓰레드 시작 함수

int result = 0;
int cntNum = 0;				// client count


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// mutex : Mutual Exclusive(상호배제)
// 쓰레드간 공유하는 데이터 영역을 보호하기 위해서 사용한다.
// 데이터 영역의 보호는 Critical Section(임계 영역)을 만들고 임계 영역내에 단하나의 쓰레드만이 진입가능 하도록 하는 방식을 사용한다.
// 보통 이 임계영역에는 보호하고자 하는 데이터에 대한 접근-수정 루틴이 들어간다.
// 데이터에 대한 접근-수정 루틴에 오직 하나의 쓰레드만 접근 가능하게 되므로 결국 데이터를 보호할 수 있게 된다.

// 뮤텍스는 단지 2개의 가능한 행동(unlock와 lock)만이 정의되어 있다.
// lock는 임계영역은 진입하기 위한 요청, unlock는 임계영역을 빠져나오면서 다른 쓰레드에게 임계영역을 되돌려주기 위해서 사용한다.
// 만약 쓰레드가 임계영역이 진입하기 위해서 lock를 시도 했는데, 
// 다른 쓰레드가 이미 임계영역에 진입했다면 해당 쓰레드가 unlock를 해서 임계영역을 빠져나오기 전까지 기다리게 된다.

// mutex는 fast와 recursive의 2가지 종류가 지원된다. 기본적으로 mutex의 종류는 fast 상태로 시작된다. 
// 이것은 lock을 얻은 쓰레드가 다시 lock를 얻을 수 있도록 할 것인지를 결정하기 위해서 사용한다. 

// pthread_mutex_t는 뮤텍스의 특징을 결정하기 위해 아래 세개의 상수를 사용할 수 있다. 이중 하나를 선택하여 선언한다.
// PTHREAD_MUTEX_INITIALIZER(fast mutex)
// PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP(recursive mutex)
// PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP(mutex 에러 체크용)

// pthread_mutex_lock()는(임계영역에 진입하기 위함)뮤텍스 잠금을 요청한다.
// 만약 뮤텍스의 최근 상태가 unlocked라면 쓰레드는 잠금을 얻고 임계영역에 진입하게 되고 리턴한다.
// 다른 쓰레드가 뮤텍스 잠금을 얻은 상태라면 잠금을 얻을 수 있을 때까지 기다리게 된다.

// pthread_mutex_unlock()는 뮤텍스잠금을 되돌려준다.
// 만약 fast 뮤텍스라면 pthread_mutex_unlock()는 언제나 unlocked 상태를 되돌려준다.
// recursive 뮤텍스라면 잠겨있는 뮤텍스의 수를 감소시키고 이 수가 0이 된다면 뮤텍스잠금을 되돌려주게 된다.

// pthread_mutex_destory()는 뮤텍스 객체를 삭제하고 자원을 되돌려준다.
// 더이상 사용하지 않는 뮤텍스는 반드시 이 함수를 이용해서 삭제하도록 하자.
// 리눅스에서 쓰레드는 뮤텍스 객체와 별개로 되어 있다.
// 그러므로 쓰레드가 종료되었다고 하더라도 뮤텍스 객체는 여전히 남아 있게 된다.
// 이 함수를 호출해야지만 뮤텍스 객체가 삭제 된다.


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

	int listen_sock;					// server단 socket 생성

	int accp_sock[THREAD_NUM];				// thread 갯수만큼 socket을 담을 배열 생성

	socklen_t addrlen = sizeof(servaddr);	// 16 Bytes (2 + 2 + 4 + 8)
	// typedef UINT32 socklen_t;

	int i, status;

	pthread_t tid[THREAD_NUM];					// socket 갯수만큼 thread를 담을 배열을 생성
	// typedef __uint32_t pthread_t

	pid_t pid;									// process ID를 담음
	// typedef int __pid_t;

	if (argc != 2) {							// 프로그램 실행 시 PORT 입력 필수
		printf("argc[1] use only port number : %s [PortNumber]\n", argv[0]);
		exit(0);
	}

	if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		// 소켓 생성
		// int socket(int domain, int type, int protocol);
		// domain : 인터넷 통신인 지, 시스템 내 프로세스 간 통신인 지 여부(PF_INET, AF_INET는 IPv4 인터넷 프로토콜)
		// type : 데이터의 전송 형태 (SOCK_STREAM / SOCK_DGRAM, 각각 TCP/IP 프로토콜과 UDP/IP 프로토콜)
		// protocol : 특정 프로토콜을 지정하기 위한 변수이며 대개 0을 사용한다.

		// socket 옵션 사용하는 이유 : 프로세스 좀비, 소켓을 사용하는 포트가 close 되지 않고 나가는 경우. 


		perror("socket Fail");
		exit(0);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	// Byte 단위로 쓰기를 시작함
	// void * memset ( void * ptr, int value, size_t num );
	// ptr : 데이터 쓰기를 시작할 메모리 주소
	// value : 반복적으로 쓰여질 값
	// num : 채우고자 하는 Byte 크기. 즉, 메모리 크기

	servaddr.sin_family = AF_INET;					// AF_INET은 IPv4 인터넷 프로토콜
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	
	// INADDR_ANY를 사용하는 이유.
	// 특정 NIC의 IP주소를 sin_addr.s_addr에 지정하면 다른 NIC에서 요청한 연결은 서비스 할 수 없다.
	// 이때, INADDR_ANY를 사용하면 두 NIC를 모두 바인딩 할 수 있어 어느 IP로 접속해도 정상적인서비스가 가능하다.
	// 또한, IP를 특정지으면 서버 컴퓨터에서 주소값을 수정해야하는 데 INADDR_ANY를 사용하면 소스의 수정없이 사용가능하다.
	// IP와 무관해진 프로그램은 포트번호로 접근하는 모든 연결에 대해 처리가 가능하다.
	servaddr.sin_port = htons(atoi(argv[1]));		// PORT를 프로그램 실행 시 얻은 argv[1]의 값으로 지정한다.

	//bind 호출
	if (bind(listen_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		// 소켓에 IP주소와 포트번호를 지정
		// int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
		// sockfd : 소켓 디스크립터(파일이나 소켓을 대표하는 정수)
		// myaddr : 인터넷 통신인 AF_INET인 지, 시스템 내 통신인 AF_UNIX인 지에 따라 다름. AF_INET의 경우, sockaddr_in을 사용
		// addrlen : myaddr 구조체의 크기
		perror("bind Fail");
		exit(0);
	}

	while (1) {
		listen(listen_sock, LISTENQ);
		// 클라이언트 접속 요청을 수신하도록 설정
		// int listen(int s, int backlog);
		// s : 소켓 디스크립터(파일이나 소켓을 대표하는 정수)
		// backlog : 대기 중인 메시지 큐의 개수


		puts("client wait....");

		accp_sock[cntNum] = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);
		// 클라이언트 접속 요청 승인
		// int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
		// s : 소켓 디스크립터(파일이나 소켓을 대표하는 정수)
		// addr : 클라이언트의 주소
		// addrlen : addr 포인터가 가르키고 있는 구조체 크기

		if (accp_sock[cntNum] < 0) {
			// 소켓 식별자는 양의 정수이므로 예외처리 진행
			perror("accept fail");
			exit(0);
		}

		if ((status = pthread_create(&tid[cntNum], NULL, &thrfunc, (void *)&accp_sock[cntNum])) != 0) {
			// 새로운 쓰레드를 생성한다. 성공 시, 0을 반환
			// int pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
			// thread : 스레드 식별자
			// attr : 스레드와 관련된 특징을 지정. NULL로 할 경우 기본 특성.
			// start_routine : 실행될 함수
			// arg : argument. 즉, start_routine에 인자로 넘어갈 값.
			printf("%d thread create error: %s\n", cntNum, strerror(status));
			exit(0);
		}

		pthread_join(tid[cntNum], NULL);
		// Sub 스레드 종료까지 Main 스레드(프로세스) 종료를 대기. 즉, 보조스레드의 처리를 모두 끝내고 종료될 수 있도록 함
		// int pthread_join(pthread_t th, void **thread_return);
		// th : thread 식별자
		// thread_return : 종료대기 및 리턴값을 받을 스레드의 리턴 값이다(?). 여기에 리턴값을 저장할 수 있다.
		// 스레드의 기본 형식에서 리턴값은 void *이므로 이 리턴값을 reference 형식의 주소로 받아오기 위해 이중포인터 형식을 사용.
		
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
	// 파일 읽기
	// ssize_t read (int fd, void *buf, size_t nbytes)
	// fd : 파일 디스크립터(파일이나 소켓을 대표하는 정수)
	// buf : 파일을 읽어 저장할 버퍼
	// nbytes : 버퍼의 크기

	printf("client send value = %d\n", buf);
	pthread_mutex_lock(&lock);
	// int pthread_mutex_lock(pthread_mutex_t *mutex);
	// (임계영역에 진입하기 위함)뮤텍스 잠금을 요청한다. 
	// 만약 뮤텍스의 최근 상태가 unlocked라면 쓰레드는 잠금을 얻고 임계영역에 진입하게 되고 리턴한다. 
	// 다른 쓰레드가 뮤텍스 잠금을 얻은 상태라면 잠금을 얻을 수 있을 때까지 기다리게 된다.


	result += buf;
	printf("accp_sock = %d", accp_sock);
	printf("result = %d\n", result);
	pthread_mutex_unlock(&lock);
	// int pthread_mutex_unlock(pthread_mutex_t *mutex);
	// 뮤텍스잠금을 되돌려준다. 
	// 만약 fast 뮤텍스라면 pthread_mutex_unlock()는 언제나 unlocked 상태를 되돌려준다. 
	// recursive 뮤텍스라면 잠겨있는 뮤텍스의 수를 감소시키고 이 수가 0이 된다면 뮤텍스잠금을 되돌려주게 된다.

	write(accp_sock, &result, 4);
	// 파일 쓰기
	// ssize_t write (int fd, const void *buf, size_t n)
	// fd : 파일 디스크립터(파일이나 소켓을 대표하는 정수)
	// buf : 파일에 쓸 내용을 담고있는 버퍼
	// n : 버퍼의 크기

	close(accp_sock);
	// 소켓 닫기
	// void Close ();
}