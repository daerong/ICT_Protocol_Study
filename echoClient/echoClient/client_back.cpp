//
//
//
//
//
//
//// =======================================================
//#define MAXLINE 1024 //buf ũ��
//#define TOTALFORK 5 //Ŭ���̾�Ʈ ��
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
//	int pids[TOTALFORK];			// process ID�� ����
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
//		//		LPCTSTR lpApplicationName,						// ���⿡ �̸��� ���� �� �ֽ��ϴ�.
//		//		LPTSTR lpCommandLine,							// commend �Է�
//		//		LPSECURITY_ATTRIBUTES lpProcessAttributes, 
//		//		LPSECUREITY_ATTRIBUTES lpThreadAttributes, 
//		//		BOOL bInheritHandles,							// �θ����μ����� ��Ӱ����� �ڵ� ���
//		//		DWORD dwCreationFlags,							// dwCreationFlags
//		//		LPVOID lpEnvironment, 
//		//		LPCTSTR lpCurrentDirctory, 
//		//		LPSTARTUPINFO lpStartupInfo,					//STARTUPINFO ����ü ������ ������ ���������.
//		//		LPPROCESS + _INFORMATION lpProcessInformation	//���� ���μ����� ������ �����ö� �� ����ü�� ���!
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
//		else { //�θ� ���μ���
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
//	int sockfd, buf, cNum;//cNum ���� ��ȣ
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
//	buf = rand() % 100 + 1; //rand �� ����
//	write(sockfd, &buf, 4); //server�� ����
//	printf("cleint value : %d\n", buf);
//	read(sockfd, &buf, 4); //server���� �޾� ��
//	printf("Server sum result : %d\n", buf);
//	close(sockfd);
//}
