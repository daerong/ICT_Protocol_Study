//
//
//
//
//
//
//// =======================================================
//#define MAXLINE 1024 //buf 크기
//#define TOTALFORK 5 //클라이언트 수
//
//
//
//void createClient(char *port, char *serverIP);
//int main(int argc, char *argv[]) {
//	if (argc != 3) {
//		printf("Use %s ip_addr port\n", argv[0]);
//		exit(0);
//	}
//
//	int pids[TOTALFORK];			// process ID를 담음
//	// pid_t -> int 
//
//	int runProcess = 0;
//
//	STARTUPINFO sinfo;
//	PROCESS_INFORMATION pinfo;
//
//	ZeroMemory(&sinfo, sizeof(sinfo));
//	sinfo.cb = sizeof(sinfo);
//	ZeroMemory(&pinfo, sizeof(pinfo));
//
//	while (runProcess < TOTALFORK) {
//		Sleep(1);
//
//		pids[runProcess] = CreateProcess(
//			L".\\foo.exe",
//			NULL,
//			NULL,
//			NULL,
//			FALSE,
//			0,
//			NULL,
//			NULL,
//			&sinfo,
//			&pinfo
//		);
//		// BOOL CreateProcess(
//		//		LPCTSTR lpApplicationName,						// 여기에 이름을 넣을 수 있습니다.
//		//		LPTSTR lpCommandLine,							// commend 입력
//		//		LPSECURITY_ATTRIBUTES lpProcessAttributes, 
//		//		LPSECUREITY_ATTRIBUTES lpThreadAttributes, 
//		//		BOOL bInheritHandles,							// 부모프로세스중 상속가능한 핸들 상속
//		//		DWORD dwCreationFlags,							// dwCreationFlags
//		//		LPVOID lpEnvironment, 
//		//		LPCTSTR lpCurrentDirctory, 
//		//		LPSTARTUPINFO lpStartupInfo,					//STARTUPINFO 구조체 정보를 위에서 만들어줬죠.
//		//		LPPROCESS + _INFORMATION lpProcessInformation	//이젠 프로세스의 정보를 가져올때 이 구조체를 사용!
//		// );
//
//		// 
//
//
//		if (pids[runProcess] < 0) {
//			return -1;
//		}
//
//		if (pids[runProcess] == 0) {
//			createClient(argv[2], argv[1]);
//			exit(0);
//		}
//		else { //부모 프로세스
//			printf("parent %ld, child %ld\n", (long)getpid(), (long)pids[runProcess]);
//		}
//		runProcess++;
//	}
//	return 0;
//}
//
//void createClient(char *port, char *serverIP) {
//	struct sockaddr_in servaddr;
//	int strlen = sizeof(servaddr);
//	int sockfd, buf, cNum;//cNum 연결 번호
//
//	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
//		perror("socket fail");
//		exit(0);
//	}
//
//	memset(&servaddr, 0, strlen);
//	servaddr.sin_family = AF_INET;
//	inet_pton(AF_INET, serverIP, &servaddr.sin_addr);
//	servaddr.sin_port = htons(atoi(port));
//
//	if (connect(sockfd, (struct sockaddr *)&servaddr, strlen) < 0) {
//		perror("connect fail");
//		exit(0);
//	}
//
//	srand((unsigned)time(NULL));
//	buf = rand() % 100 + 1; //rand 값 생성
//	write(sockfd, &buf, 4); //server로 전송
//	printf("cleint value : %d\n", buf);
//	read(sockfd, &buf, 4); //server에서 받아 옴
//	printf("Server sum result : %d\n", buf);
//	close(sockfd);
//}
