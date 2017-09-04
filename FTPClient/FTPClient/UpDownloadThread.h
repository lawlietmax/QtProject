#pragma once

#include <cstdlib>
#include <time.h>
#include <io.h>
#include <iostream> 
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <QMutexLocker>
#include <QMessageBox>
#include <QStandardItemModel>
#include <winsock2.h>
#include "defineword.h"
#pragma comment(lib,"ws2_32.lib")

int GetRetCode(const char *recv_buf);
class UpDownloadThread : public QThread
{
	Q_OBJECT
signals:
	void ViewFresh(QModelIndex& index);
public:
	bool false_flag = 0;
	explicit UpDownloadThread(SOCKET &control, const char* Dest, const char* Sour, const char* hostip, u_short listenport, int TransmissionMode, QStandardItemModel &UpDownList_Model , int  Mode,  int row, long long offest, QObject *parent = 0);
	~UpDownloadThread();
	static int getThreadNum();
protected:
	void run();
private:
	FILE *fp;
	QTimer *timer;
	bool first;
	int TransmissionMode;
	int Mode;
	int Row;
	int StatusFlag;
	char send_buf[send_len];
	char recv_buf[recv_len];
	char *Sour;
	char *Dest;
	char *hostip;
	u_short port;
	long long filesize = 0;
	long long recvsize = 0;
	long long sendsize = 0;
	long long currentsize = 0;
	long long Offest = 0;
	SOCKET control_sock;
	SOCKET Listen_Socket;
	QMutex NumOfThreadMutex;
	QStandardItemModel &Model;
	QStandardItem *completesize;
	QStandardItem *status;
	QStandardItem *percent;
	QStandardItem *kbpsitem;
	static int numberofthread;
private slots:
	void UpdateModel();
};