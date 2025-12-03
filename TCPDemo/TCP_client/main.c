#include<stdio.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 1. 创造套接字
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET)
    {
        printf("socket failed with error: %d\n", WSAGetLastError());
        return -1;
    }

    // 2. 连接服务器
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(8080);
    target.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr*)&target, sizeof(target)) == SOCKET_ERROR)
    {
        closesocket(client_socket);
        printf("connect failed with error: %d\n", WSAGetLastError());
        return -1;
    }

    // 3. 发送数据
    // 回显服务器

    while (1)
    {
        char buffer[1024] = { 0 };
        printf("Please input :");
        scanf("%s", buffer);

        send(client_socket, buffer, strlen(buffer), 0);

        char rbuffer[1024] = { 0 };
        int ret = recv(client_socket, rbuffer, 1024, 0);
        if (ret <= 0)
        {
            break;
        }
        printf("%s\n", rbuffer);
    }

    // 4. 关闭套接字
    closesocket(client_socket);
    return 0;
}