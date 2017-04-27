#pragma once
#include <cstdlib>
#include <time.h>
#include <QThread>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define MAX_DownLoadThread 5
class ListenSocket : public QThread
{
	Q_OBJECT

public:
	ListenSocket(QObject *parent = 0);
	~ListenSocket();
	SOCKET& ListenSocket::GetSocket();
	u_short listenport;
protected:
	void run();
private:
	SOCKET NewSocket;
};
