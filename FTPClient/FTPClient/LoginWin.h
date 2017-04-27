#pragma once
#include "ui_LoginWin.h"
#include <string>
#include <QString>
#include <QWidget>
#include <QMessageBox>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define recv_len 512
#define send_len 512
int GetRetCode(const char *recv_buf);
class LoginWin : public QWidget
{
	Q_OBJECT

public:
	LoginWin(SOCKET w,QWidget *parent = Q_NULLPTR);
	~LoginWin();
	void init(char* ID,char* PW);
private:
	char* ID_ptr;
	char* PW_ptr;
	SOCKET socket;
	Ui::LoginWin ui;
	private slots:
	void anonymous_checked();
	void Login_Clicked();
};
