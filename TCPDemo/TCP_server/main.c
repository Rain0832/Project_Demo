#include<stdio.h>
#include<string.h>
// 包含WinSock2.h头文件和库文件才能使用网络功能
#include<WinSock2.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")

DWORD WINAPI thread_func(LPVOID lpThreadParameter)
{
	SOCKET client_socket = *(SOCKET*)lpThreadParameter;
	free(lpThreadParameter);

	while (1)
	{
		// 5.开始通讯（B/S）通过浏览器来访问一个服务器
		char buffer[1024] = { 0 };

		/*
		recv(
			SOCKET s,			// 客户端socket
			char FAR* buf,		// 接受的数据存到哪里
			int len,			// 接受的数据长度
			int flags			// 标志位：0表示不等待，1表示等待
		);
		*/

		int ret = recv(client_socket, buffer, 1024, 0);
		if (ret <= 0)
		{
			break;
		}
		printf("Recv from %llu: %s\n", client_socket, buffer);

		send(client_socket, buffer, strlen(buffer), 0);
	}

	printf("Client %llu disconnected.\n", client_socket);

	// 6.关闭套接字
	closesocket(client_socket);
	return 0;
}


int main()
{
	// 0. windows上使用网络功能需要开始网络权限

	/*
	WSAStartup(
		_In_ WORD wVersionRequested,
		_Out_ LPWSADATA lpWSAData
	);
	*/

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);


	// 1.创建套接字

	/*
	socket(
		int af,
		int type,
		int protocol
	);
	*/		

	SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	printf("listen_socket = %llu\n", listen_socket);
	
	// INVALID_SOCKET == -1
	if (listen_socket == INVALID_SOCKET)
	{
		printf("Create listen socket failed!!! Error code: %d\n", GetLastError());
		return -1;
	}

	unsigned short a = 8080;
	unsigned short *p = &a;

	// 2.绑定端口号

	/*
	typedef struct sockaddr_in {
		short   sin_family;
		USHORT sin_port;
		IN_ADDR sin_addr;
	} SOCKADDR_IN, * PSOCKADDR_IN;
	*/

	struct sockaddr_in local = {0};
	local.sin_family = AF_INET;
	local.sin_port = htons(8080);	// htons 解决大小端问题
	// 服务端 网卡 一般写 0.0.0.0 表示监听所有网卡的连接
	// local.sin_addr.s_addr = htonl(INADDR_ANY);	// htonl 解决大小端问题
	local.sin_addr.s_addr = inet_addr("0.0.0.0");	// 字符串IP地址转换为整数IP

	/*
	int bind(
		SOCKET s,
		const struct sockaddr * name,
		int namelen
	);
	*/

	// 转换为 sockaddr_in 结构体类型
	if (bind(listen_socket, (struct sockaddr*)&local, sizeof(local)) == -1)
	{
		printf("Bind listen socket failed!!! Error code: %d\n", GetLastError());
		return -1;
	}

	// 3.给socket开启监听属性

	/*
	listen(
		SOCKET s,
		int backlog
	);
	*/

	if (listen(listen_socket, 10) == -1)
	{
		printf("Start listen failed!!! Error code: %d\n", GetLastError());
		return -1;
	}

	// 4.等待客户端连接请求
	// 返回的SOCKET类型表示客户端的套接字，只能用这个来接收和发送数据
	// 阻塞函数：等到客户端连接进来就接受连接，然后返回，否则就阻塞
	
	// 而不是listen_socket
	/*accept(
		SOCKET s,						// 监听socket
		struct sockaddr FAR * addr,		// 返回客户端IP地址和端口号
		int FAR * addrlen				// 结构的大小（指针类型）
	);*/

	
	while (1)
	{
		SOCKET client_socket = accept(listen_socket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
		{
			printf("Accept client socket failed!!! Error code: %d\n", GetLastError());
			continue;
		}

		printf("new client socket = %llu\n", client_socket);

		SOCKET* sockfd = (SOCKET*)malloc(sizeof(SOCKET));
		*sockfd = client_socket;

		// 创建线程
		CreateThread(NULL, 0, thread_func, sockfd, 0, NULL);
		
	}
	return 0;
}