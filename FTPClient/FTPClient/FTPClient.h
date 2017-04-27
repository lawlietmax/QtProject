#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FTPClient.h"
#include <iostream>
#include <string>
#include <queue>
#include <winsock2.h>
#include <QTimer>
#include <QFileDialog>
#include <QCloseEvent>
#include <QProgressBar>
#include <QFileSystemModel>
#include "LoginWin.h"
#include "TreeView.h"
#include "ProgressDelegate.h"
#include "UpDownloadThread.h"

#pragma comment(lib,"ws2_32.lib")

class Task
{
public:
	Task(char *hostip, const char *Dest, const  char *Sour, char *ID, char *PW, int Row, int Mode, int TransmissionMode, char *serverIP, u_short serverPort,long long offest)
	{
		this->Dest = (char*)malloc(strlen(Dest) + 1);
		if (this->Dest) strcpy(this->Dest, Dest);
		this->Sour = (char*)malloc(strlen(Sour) + 1);
		if (this->Sour) strcpy(this->Sour, Sour);
		this->ID = (char*)malloc(strlen(ID) + 1);
		if (this->ID) strcpy(this->ID, ID);
		this->PW = (char*)malloc(strlen(PW) + 1);
		if (this->PW) strcpy(this->PW, PW);
		this->hostip = (char*)malloc(strlen(hostip) + 1);
		if (this->hostip) strcpy(this->hostip, hostip);
		this->serverIP = (char*)malloc(strlen(serverIP) + 1);
		if (this->serverIP) strcpy(this->serverIP, serverIP);
		this->serverPort = serverPort;
		this->Mode = Mode;
		this->TransmissionMode = TransmissionMode;
		this->Row = Row;
		this->offest = offest;
	}
	~Task()
	{
		delete[] Dest;
		delete[] Sour;
		delete[] ID;
		delete[] PW;
		delete[] hostip;
		delete[] serverIP;
	}
	char *hostip;
	char *Dest;
	char *Sour;
	char *ID;
	char *PW;
	int Row;
	int Mode;
	int TransmissionMode;
	char *serverIP;
	u_short serverPort;
	long long offest;
};

class FTPClient : public QMainWindow
{
    Q_OBJECT

public:
    FTPClient(QWidget *parent = Q_NULLPTR);
	FTPClient::~FTPClient();
	std::queue<Task*> WaitQueue;
	QStandardItemModel UpDownList_Model;
private:
	QStandardItemModel Msg_Model;
	QStandardItemModel Files_Model;
	QFileSystemModel FileSystem_Model;
	char serverIP[16];
	u_short serverPort;
	char ID[256];
	char PW[256];
	char send_buf[send_len];
	char recv_buf[recv_len];
	u_short listenport;
	QLabel code_label;
	WSADATA wsaData;
	SOCKET control_sock;
	SOCKET data_sock;
	SOCKADDR_IN serveraddr_control;//服务端地址
	SOCKADDR_IN serveraddr_data;//服务端地址
	int  socket_build(SOCKET&,const char* serverIP, const u_short ports);
	bool FTPClient::login(SOCKET& sock, char *ID, char *PW);
	int status_msg(const char* recv_buf);
	int TransmissionMode;
	void closeEvent(QCloseEvent *event);
	bool DownloadFile(const char* Dest, const char* Sour);
	bool UploadFile(const char* Dest, const char* Sour);
	void PrintfDir();
	bool ChangeDir(const char* EnterDir);
    Ui::FTPClientClass ui;
	QMutex QueueMutex;
	//QMutex ConnectMutex;
	ProgressDelegate *Progress;
	QMenu *SelectConnectMode;
	void AutoConnect();
	char hostip[16];
	QTimer *ConnectTimer;
private slots:
	void Logined();
	void CheckWaitQueue();
	void DownloadButtonClicked();
	void UploadButtonClicked();
	void ftpconnect();
	void ftpdisconnect();
	void BackButtonClicked();
	void SetHostIP(QAction*);
	void dropUpload(QString&);
	void RefreshButtonClicked();
	void DoubleClickItem(const QModelIndex &);
	void ChangeStatus(const QModelIndex &index);
	void dropDownload(QString& Dest, QString& Sour);
};