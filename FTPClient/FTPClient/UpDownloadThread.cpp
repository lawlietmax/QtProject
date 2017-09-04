#include "UpDownloadThread.h"
extern QMutex UpDownListMutex;
int UpDownloadThread::numberofthread = 0;
UpDownloadThread::UpDownloadThread(SOCKET &control, const char* Dest, const char* Sour, const char* hostip, u_short listenport, int TransmissionMode, QStandardItemModel &UpDownList_Model, int Mode, int row, long long offest, QObject *parent)
	:QThread(parent),control_sock(control), Model(UpDownList_Model), port(listenport)
{
	QMutexLocker locker(&NumOfThreadMutex);
	this->Sour = new char[strlen(Sour) + 1];
	if (this->Sour) strcpy(this->Sour, Sour);
	this->Dest = new char[strlen(Dest) + 1];
	if (this->Dest) strcpy(this->Dest, Dest);
	this->hostip = new char[strlen(hostip) + 1];
	if (this->hostip) strcpy(this->hostip, hostip);
	this->TransmissionMode = TransmissionMode;
	this->Mode = Mode;
	this->Row = row;
	this->Offest = offest;
	first = true;
	completesize = Model.item(Row, 3);
	kbpsitem = Model.item(Row, 4);
	percent = Model.item(Row, 5);
	status = Model.item(Row, 6);
	if (Mode == DownloadMode)
	{
		if(Offest!=0) fp = fopen(Dest, "ab");
		else fp = fopen(Dest, "wb");
		if (fp)
		{
			if (fseek(fp, Offest, SEEK_SET))
			{
				QMessageBox::information(NULL, QString::fromLocal8Bit("�ļ�д����"), QString::fromLocal8Bit("%1 fseek ���� offest %1").arg(Offest));
				false_flag = 1;
				return;
			}
		}
		else
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("�ļ�д����"), QString::fromLocal8Bit("%1д����").arg(QString::fromLocal8Bit(Dest)));
			false_flag = 1;
			return;
		}
	}
	else
	{
		fp = fopen(Sour, "rb");
		if (fp)
		{
			if (fseek(fp, Offest, SEEK_SET))
			{
				QMessageBox::information(NULL, QString::fromLocal8Bit("�ļ�������"), QString::fromLocal8Bit("%1 fseek ���� offest %1").arg(Offest));
				false_flag = 1;
				return;
			}
		}
		else
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("�ļ�������"), QString::fromLocal8Bit("%1������").arg(QString::fromLocal8Bit(Sour)));
			false_flag = 1;
			return;
		}
	}
	if (StatusFlag)
	{
		if (Mode == UploadMode)
		{
			Model.item(Row, 0)->setText(QString::fromLocal8Bit("�ϴ���"));
		}
		else
		{
			Model.item(Row, 0)->setText(QString::fromLocal8Bit("������"));
		}
	}
	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(UpdateModel()));
	timer->start(1000);
	numberofthread++;
}

UpDownloadThread::~UpDownloadThread()
{ 
	UpdateModel();
	if(fp) fclose(fp);
	delete[] timer;
	delete[] Sour;
	delete[] Dest;
	delete[] hostip;
	QMutexLocker locker(&NumOfThreadMutex);
	numberofthread--;
	quit();
	wait();
}

void UpDownloadThread::run()
{
	if (!fp) return;
	SOCKET CommandSocket(control_sock);
	SOCKET DownloadDataSocket;
	SOCKET ListenSocket;
	SOCKADDR_IN ServerAddr;//����˵�ַ
	SOCKADDR_IN ClientAddr;
	IN_ADDR hostaddr;

	/* ���� ��TYPE I\r\n�� */
	sprintf(send_buf, "TYPE I\r\n");
	/* �ͻ��˸��߷��������������ö��M�� */
	send(CommandSocket, send_buf, strlen(send_buf), 0);
	/*�ͻ��˽��շ���������Ӧ�룬	����Ϊ ��200 Type set to I.\r\n�� */
	int recv_bytes;
	if ((recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0)) == -1) return;
	recv_buf[recv_bytes] = '\0';

	if (TransmissionMode == PORT_MODE)
	{
		srand(time(NULL));
		ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
		ClientAddr.sin_family = AF_INET;
		ClientAddr.sin_addr.s_addr = htons(INADDR_ANY);
		port = (u_short)(rand() * 64510 + 1024);
		ClientAddr.sin_port = htons(port);
		char opt = 1;
		setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(char));
		int bind_ret = bind(ListenSocket, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr));
		int error_number = WSAGetLastError();
		/* �ͻ��˿�ʼ�����˿�port */
		int listen_ret = listen(ListenSocket, MAXThread);

		hostaddr.S_un.S_addr = inet_addr(hostip);
		/* ���� ��PORT \r\n�� */
		sprintf(send_buf, "PORT %d,%d,%d,%d,%d,%d\r\n", hostaddr.S_un.S_un_b.s_b1, hostaddr.S_un.S_un_b.s_b2, hostaddr.S_un.S_un_b.s_b3, hostaddr.S_un.S_un_b.s_b4, port / 256, port % 256);
		send(CommandSocket, send_buf, strlen(send_buf), 0);
		/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��200 Port command successful�� */
		if ((recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0)) == -1) return;
		recv_buf[recv_bytes] = '\0';
		int length = sizeof(ServerAddr);
		DownloadDataSocket = accept(ListenSocket, (SOCKADDR *)& ServerAddr, &length);
		closesocket(ListenSocket);
	}
	else if(TransmissionMode == PASV_MODE)
	{
		/* ���� ��PASV\r\n�� */
		sprintf(send_buf, "PASV\r\n");
		/* �ͻ��˸��߷������ñ���ģʽ */
		send(CommandSocket, send_buf, strlen(send_buf), 0);
		/*�ͻ��˽��շ���������Ӧ����¿��Ķ˿ںţ�
		* ����Ϊ ��227 Entering passive mode (<h1,h2,h3,h4,p1,p2>)�� */
		if ((recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0)) == -1) return;
		recv_buf[recv_bytes] = '\0';
		char serverIP[16];
		int serverIPv4[4];
		int port[2];
		u_short ports;
		sscanf(recv_buf, "%*[^(](%d,%d,%d,%d,%d,%d)", &serverIPv4[0], &serverIPv4[1], &serverIPv4[2], &serverIPv4[3], &port[0], &port[1]);
		sprintf(serverIP, "%d.%d.%d.%d", serverIPv4[0], serverIPv4[1], serverIPv4[2], serverIPv4[3]);
		ports = port[0]*256+ port[1];
		DownloadDataSocket = socket(AF_INET, SOCK_STREAM, 0);
		ServerAddr.sin_addr.S_un.S_addr = inet_addr(serverIP);
		ServerAddr.sin_family = AF_INET;
		ServerAddr.sin_port = htons(ports);//���Ӷ˿�
		::connect(DownloadDataSocket, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));/* ���ӷ������¿������ݶ˿� */
	}
	if (Mode == DownloadMode)
	{
		/* ���� ��SIZE filename\r\n�� */
		sprintf(send_buf, "SIZE %s\r\n", Sour);
		/* �ͻ��˷�������ӷ������˵õ������ļ��Ĵ�С */
		send(CommandSocket, send_buf, strlen(send_buf), 0);
		/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��213 <size>�� */
		if ((recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0)) == -1) return;
		recv_buf[recv_bytes] = '\0';
		int retcode;
		sscanf(recv_buf, "%d %lld", &retcode, &filesize);
		if (Offest != 0)
		{
			/* ���� ��REST offset\r\n�� */
			sprintf(send_buf, "REST %lld\r\n", Offest);
			/* �ͻ��˷�������ָ�������ļ���ƫ���� */
			send(CommandSocket, send_buf, strlen(send_buf), 0);
			/* �ͻ��˽��շ���������Ӧ�����Ϣ��
			*����Ϊ ��350 Restarting at <position>. Send STORE or RETRIEVE to initiate transfer.�� */
			if ((recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0)) == -1) return;
			recv_buf[recv_bytes] = '\0';
		}
		/* ���� ��RETR filename\r\n�� */
		sprintf(send_buf, "RETR %s\r\n", Sour);
		/* �ͻ��˷�������ӷ������������ļ� */
		send(CommandSocket, send_buf, strlen(send_buf), 0);
		/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��150 Opening data connection.�� */
		if ((recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0)) == -1) return;
		recv_buf[recv_bytes] = '\0';
		recvsize = filesize - Offest;//Ҫ���ܵĴ�С
		while (recvsize)
		{
			if (DownStopStatus == status->data(Qt::DisplayRole).toInt())//�Д��ǲ��Ǖ�ͣ���d
			{
				StatusFlag = false;
				closesocket(DownloadDataSocket);
				/* ���� ��QUIT\r\n�� */
				sprintf(send_buf, "QUIT\r\n");
				/*�ͻ��˷����˳������������ */
				send(CommandSocket, send_buf, strlen(send_buf), 0);
				/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��221 Good Bye�� */
				recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0);
				recv_buf[recv_bytes] = '\0';
				closesocket(CommandSocket);
				return;
			}
			/* �ͻ���ͨ���������� �ӷ����������ļ����� */
			if ((recv_bytes = recv(DownloadDataSocket, recv_buf, recv_len, 0)) == -1) return;
			/* �ͻ���д�ļ� */
			fwrite(recv_buf, recv_bytes, 1, fp);
			recvsize -= recv_bytes;
		}
		/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��150 Opening data connection.�� */
		recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0);
		recv_buf[recv_bytes] = '\0';
	}
	else if(Mode == UploadMode)
	{
		//�ς�
		std::string SourString(Sour);//Դ��ַ���ַ���
		int index = SourString.find_last_of('/');//�ҵ�����һ���ַ���'/'��λ��
		std::string FileName;
		FileName.assign(SourString, index+1);//�ļ������ַ���
		/* ���� ��CWD dirname\r\n�� */
		sprintf(send_buf, "CWD %s\r\n", Dest);
		/* �ͻ��˷��͸ı�������ϵĹ���Ŀ¼���������� */
		send(CommandSocket, send_buf, strlen(send_buf), 0);
		/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ���� */
		if ((recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0)) == -1) return;
		recv_buf[recv_bytes] = '\0';
		if (Offest != 0)
		{
			/* ���� ��REST offset\r\n�� */
			sprintf(send_buf, "REST %lld\r\n", Offest);
			/* �ͻ��˷�������ָ�������ļ���ƫ���� */
			send(CommandSocket, send_buf, strlen(send_buf), 0);
			/* �ͻ��˽��շ���������Ӧ�����Ϣ��
			*����Ϊ ��350 Restarting at <position>. Send STORE or RETRIEVE to initiate transfer.�� */
			if ((recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0)) == -1) return;
			recv_buf[recv_bytes] = '\0';
		}
		/* ���� ��STOR  filename\r\n�� */
		sprintf(send_buf, "STOR %s\r\n", FileName.data());
		/* �ͻ��˷�������ӷ������������ļ� */
		send(CommandSocket, send_buf, strlen(send_buf), 0);
		/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��150 Opening data connection.�� */
		if ((recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0)) == -1) return;
		recv_buf[recv_bytes] = '\0';
		long send_bytes = 0;//���ΰl�͵Ĵ�С
		long fread_bytes = 0;//�����x�ļ����ݵĴ�С
		filesize = filelength(fileno(fp));//�ļ���С
		sendsize = filesize - Offest;//Ҫ�l�͵Ĵ�С
		while (sendsize)
		{
			//QMutexLocker LR(&UpDownListMutex);
			if (UpStopStatus == status->data(Qt::DisplayRole).toInt())//�Д��ǲ��Ǖ�ͣ�ς�
			{
				StatusFlag = false;
				closesocket(DownloadDataSocket);
				/* ���� ��QUIT\r\n�� */
				sprintf(send_buf, "QUIT\r\n");
				/*�ͻ��˷����˳������������ */
				send(CommandSocket, send_buf, strlen(send_buf), 0);
				/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��221 Good Bye�� */
				recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0);
				recv_buf[recv_bytes] = '\0';
				closesocket(CommandSocket);
				return;
			}
			//LR.unlock();
			/* �ͻ����x�ļ� */
			fread_bytes = fread(send_buf, 1, send_len, fp);
			/* �ͻ���ͨ���������� ���������l���ļ����� */
			while (fread_bytes != send_bytes)
			{
				if ((send_bytes += send(DownloadDataSocket, send_buf + send_bytes, fread_bytes - send_bytes, 0)) == -1)
				{
					qDebug("send file content error!");
					return;
				}
			}
			sendsize -= send_bytes;
			send_bytes = 0;
		}
	}
	closesocket(DownloadDataSocket);
	/* ���� ��QUIT\r\n�� */
	sprintf(send_buf, "QUIT\r\n");
	/*�ͻ��˷����˳������������ */
	send(CommandSocket, send_buf, strlen(send_buf), 0);
	/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��221 Good Bye�� */
	recv_bytes = recv(CommandSocket, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';

	closesocket(CommandSocket);
}

int UpDownloadThread::getThreadNum()
{
	return numberofthread;
}

void UpDownloadThread::UpdateModel()
{
	QMutexLocker LR(&UpDownListMutex);
	long long previoussize = currentsize;//previoussize���ϴε������(�ς�/���d)�Ĵ�С
	//currentsize�鱾�ε������(�ς�/���d)�Ĵ�С
	if (Mode == DownloadMode)	 currentsize = filesize - recvsize;
	else currentsize = filesize - sendsize;
	completesize->setData(QVariant(currentsize), Qt::DisplayRole);
	if (filesize == 0) percent->setData(QVariant(0), Qt::DisplayRole);
	else percent->setData(QVariant(100.0 * currentsize / filesize), Qt::DisplayRole);
	if (currentsize == filesize) kbpsitem->setData(QVariant(0), Qt::DisplayRole);
	else kbpsitem->setData(QVariant((currentsize - previoussize) / 1024), Qt::DisplayRole);
	if (percent->data(Qt::DisplayRole).toDouble() == 100.0)
	{
		if(Mode == UploadMode) Model.item(Row, 0)->setText(QString::fromLocal8Bit("�ϴ����"));
		else Model.item(Row, 0)->setText(QString::fromLocal8Bit("�������"));
		status->setData(QVariant(FinishStatus), Qt::DisplayRole);
	}
}