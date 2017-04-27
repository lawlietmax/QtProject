#include "ListenSocket.h"

ListenSocket::ListenSocket(QObject *parent)
	: QThread(parent)
{
}

ListenSocket::~ListenSocket()
{
}

void ListenSocket::run()
{
	srand(time(NULL));
	SOCKADDR_IN ClientADDR;//客戶端地址
	SOCKET Listen = socket(AF_INET, SOCK_STREAM, 0);
	ClientADDR.sin_family = AF_INET;
	ClientADDR.sin_addr.s_addr = htons(INADDR_ANY);
	listenport = (u_short)(rand() * 64510 + 1024);
	ClientADDR.sin_port = htons(listenport);
	int bind_ret = bind(Listen, (struct sockaddr *)&ClientADDR, sizeof(ClientADDR));
	int error_number = WSAGetLastError();
	/* 客户端开始监听端口port */
	int listen_ret = listen(Listen, MAX_DownLoadThread);
	/* ftp客户端接受服务器端的连接请求 */
	while (true)
	{
		NewSocket = accept(Listen, NULL, 0);
	}
}
SOCKET& ListenSocket::GetSocket()
{
	return this->NewSocket;
}