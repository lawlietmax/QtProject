#include "LoginWin.h"
extern bool login_bool;
LoginWin::LoginWin(SOCKET w,QWidget *parent)
	: QWidget(parent), socket(w)
{
	ui.setupUi(this);
	if (ui.anonymous_check->checkState() == Qt::Checked)
	{
		ui.ID_Line->setText("anonymous");
		ui.ID_Line->setEnabled(false);
		ui.PW_Line->setFocus();
	}
	else
	{
		ui.ID_Line->setEnabled(true);
		ui.ID_Line->setText("");
		ui.PW_Line->setText("");
		ui.ID_Line->setFocus();
	}
	connect(ui.anonymous_check, SIGNAL(clicked()), this, SLOT(anonymous_checked()));
	connect(ui.Login_Button, SIGNAL(clicked()), this, SLOT(Login_Clicked()));
	this->setAttribute(Qt::WA_DeleteOnClose, true);
}

LoginWin::~LoginWin()
{
	
}

void LoginWin::anonymous_checked()
{
	if (ui.anonymous_check->checkState() == Qt::Checked)
	{
		ui.ID_Line->setText("anonymous");
		ui.ID_Line->setEnabled(false);
		ui.PW_Line->setFocus();
	}
	else
	{
		ui.ID_Line->setEnabled(true);
		ui.ID_Line->setText("");
		ui.ID_Line->setFocus();
	}
}

void LoginWin::init(char* ID, char* PW)
{
	ID_ptr = ID;
	PW_ptr = PW;
}
void LoginWin::Login_Clicked()
{
	char send_buf[send_len], recv_buf[recv_len];
	strcpy_s(ID_ptr, 256,ui.ID_Line->text().toLocal8Bit().data());
	strcpy_s(PW_ptr, 256, ui.PW_Line->text().toLocal8Bit().data());
	/* 命令 ”USER username\r\n” */
	sprintf(send_buf, "USER %s\r\n", ID_ptr);
	/*客户端发送用户名到服务器端 */
	send(socket, send_buf, strlen(send_buf), 0);
	/* 客户端接收服务器的响应码和信息，正常为 ”331 Password required” */
	int recv_bytes = recv(socket, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';
	if (GetRetCode(recv_buf) != 331)
	{
		//"501 Invalid number of parameters.\r\n"
		QMessageBox::information(NULL, QString::fromLocal8Bit("登入错误"), QString(recv_buf));
		login_bool = false;
		return;
	}
	/* 命令 ”PASS password\r\n” */
	sprintf(send_buf, "PASS %s\r\n", PW_ptr);
	/* 客户端发送密码到服务器端 */
	send(socket, send_buf, strlen(send_buf), 0);
	do
	{
		/* 客户端接收服务器的响应码和信息，正常为 ”230 User logged in.” */
		recv_bytes = recv(socket, recv_buf, recv_len, 0);
		recv_buf[recv_bytes] = '\0';
		if (GetRetCode(recv_buf) != 230)
		{
			/*
			530-User cannot log in.
			Win32 error:   The user name or password is incorrect.
			Error details: An error occurred during the authentication process.
			530 End 
			*/
			recv_bytes = recv(socket, recv_buf, recv_len, 0);
			recv_buf[recv_bytes] = '\0';
			QMessageBox::information(NULL, QString::fromLocal8Bit("登入错误"), QString(recv_buf));
			login_bool = false;
			return;
		}
	} while (strcmp(recv_buf, "230 User logged in.\r\n"));
	login_bool = true;
	this->close();
}