#include "FTPClient.h"
extern bool login_bool;
extern QMutex UpDownListMutex;
FTPClient::FTPClient(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	int nRc = WSAStartup(0x0202, &wsaData);//��ʼ��socket
	if (nRc)
	{
		return;
	}
	TransmissionMode = PASV_MODE;//����ģʽ
	//TransmissionMode = PORT_MODE;//����ģʽ
	// ��ñ���������  
	char hostname[MAX_PATH] = { 0 };
	gethostname(hostname, MAX_PATH);
	struct hostent FAR* lpHostEnt = gethostbyname(hostname);
	struct in_addr inAddr;
	if (lpHostEnt == NULL) strcpy(hostip,"127.0.0.1");
	else
	{
		strcpy(hostip, "0.0.0.0");
		SelectConnectMode = ui.menuBar->addMenu(QString::fromLocal8Bit("����ģʽ"));
		QActionGroup *SelectIPAction = new QActionGroup(this);
		QMenu *PORTMode;
		QAction* ActPoint;
		ActPoint = SelectIPAction->addAction(QString::fromLocal8Bit("������PASV��ģʽ"));
		ActPoint->setCheckable(true);
		if (TransmissionMode == PASV_MODE) ActPoint->setChecked(true);
		SelectConnectMode->addAction(ActPoint);
		PORTMode = SelectConnectMode->addMenu(QString::fromLocal8Bit("������PORT��ģʽ"));
		for (int i = 0; lpHostEnt->h_addr_list[i] != NULL; i++)
		{
			ActPoint = SelectIPAction->addAction(inet_ntoa(*(IN_ADDR*)lpHostEnt->h_addr_list[i]));
			ActPoint->setCheckable(true);
			PORTMode->addAction(ActPoint);
		}
		SelectIPAction->setExclusive(true);
		connect(SelectIPAction, &QActionGroup::triggered, this, &FTPClient::SetHostIP);
	}
	login_bool = false;
	ui.BackButton->setEnabled(false);
	code_label.setText("Response Code : ");
	ui.statusBar->addWidget(&code_label);
	ui.msgView->setModel(&Msg_Model);
	Files_Model.setColumnCount(4);
	Files_Model.setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("����"));
	Files_Model.setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("ʱ��"));
	Files_Model.setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("�ļ���С(Byte)"));
	Files_Model.setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("�ļ����Q"));
	ui.FilesView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.FilesView->setModel(&Files_Model);
	ui.disconnect_button->setEnabled(false);
	connect(ui.connect_button, SIGNAL(clicked()), this, SLOT(ftpconnect()));
	connect(ui.disconnect_button, SIGNAL(clicked()), this, SLOT(ftpdisconnect()));
	connect(ui.BackButton, SIGNAL(clicked()), this, SLOT(BackButtonClicked()));
	connect(ui.download_button, SIGNAL(clicked()), this, SLOT(DownloadButtonClicked()));
	connect(ui.upload_button, SIGNAL(clicked()), this, SLOT(UploadButtonClicked()));
	connect(ui.FilesView, SIGNAL(doubleClicked(const QModelIndex)), this,SLOT(DoubleClickItem(const QModelIndex)));
	connect(ui.FilesView, &TableView::dropedsomething, this, &FTPClient::dropUpload);
	//����Ŀ¼���Model
	FileSystem_Model.setRootPath(QDir::currentPath());
	FileSystem_Model.setReadOnly(false);
	ui.FileSysView->setModel(&FileSystem_Model);
	ui.FileSysView->setColumnWidth(0, 200);
	connect(ui.FileSysView, &TreeView::dropedsomething, this, &FTPClient::dropDownload);
	connect(ui.RefreshDirButton, &QPushButton::clicked, this, &FTPClient::RefreshButtonClicked);
	ui.splitter_2->setStretchFactor(0, 3);
	ui.splitter_2->setStretchFactor(1, 1);
	UpDownList_Model.setColumnCount(7);
	UpDownList_Model.setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("����"));
	UpDownList_Model.setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("��Դ"));
	UpDownList_Model.setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("Ŀ��"));
	UpDownList_Model.setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("�Ѵ���(Btye)"));
	UpDownList_Model.setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("�ٶ�(KB/s)"));
	UpDownList_Model.setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("����"));
	UpDownList_Model.setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("״̬"));
	ui.UpDownListView->setModel(&UpDownList_Model);
	ui.UpDownListView->setColumnWidth(0, 70);
	ui.UpDownListView->setColumnWidth(1, 300);
	ui.UpDownListView->setColumnWidth(2, 400);
	Progress = new ProgressDelegate(ui.UpDownListView);
	ui.UpDownListView->setItemDelegate(Progress);
	connect(Progress, &ProgressDelegate::clicked, this, &FTPClient::ChangeStatus);
	ui.UpDownListView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
	ui.FilesView->setDir("/");
	//ui.FileSysView->setRootIndex(FileSystem_Model.index(QDir::currentPath()));
}

FTPClient::~FTPClient()
{
	closesocket(control_sock);
	WSACleanup();
}

void FTPClient::ftpconnect()
{
	strcpy(serverIP, ui.IP_Text->text().toLocal8Bit().data());
	serverPort = (u_short)ui.Port_Text->text().toInt();
	if (login_bool == false) ui.connect_button->setText(QString::fromLocal8Bit("������..."));
	if(!socket_build(control_sock, serverIP, serverPort)) return;
	int timeout = 3000; //3s
	int ret = setsockopt(control_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	LoginWin *loginwin = new LoginWin(control_sock);
	loginwin->setWindowModality(Qt::ApplicationModal);
	loginwin->init(ID, PW);
	loginwin->show();
	connect(loginwin, &LoginWin::destroyed, this, &FTPClient::Logined);
	this->setFocus();
}

int FTPClient::socket_build(SOCKET& s, const char* serverIP, const u_short ports)
{
	SOCKET newsocket;
	memset(&serveraddr_control, 0, sizeof(struct sockaddr_in));
	newsocket = socket(AF_INET, SOCK_STREAM, 0);//�½��ͻ���socket
	serveraddr_control.sin_addr.S_un.S_addr = inet_addr(serverIP);//����Ҫ���ӵķ���˵�ַ
	serveraddr_control.sin_family = AF_INET;
	serveraddr_control.sin_port = htons(ports);//���Ӷ˿�
	::connect(newsocket, (struct sockaddr *)&serveraddr_control, sizeof(serveraddr_control));/* ���ӵ��������� */
	/* �ͻ��˽��շ������˵�һЩ��ӭ��Ϣ */
	int recv_bytes = recv(newsocket, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';
	if (GetRetCode(recv_buf) != 220 || -1 == recv_bytes)
	{
		if (-1 == recv_bytes)	QMessageBox::information(NULL, QString::fromLocal8Bit("����ʧ��"), QString::fromLocal8Bit("%1����ʧ��").arg(serverIP));
		else QMessageBox::information(NULL, QString::fromLocal8Bit("����ʧ��"), QString::fromLocal8Bit("����:%1").arg(recv_buf));
		ui.connect_button->setText(QString::fromLocal8Bit("����"));
		closesocket(newsocket);
		return 0;
	}
	s = newsocket;
	return 1;
}

int FTPClient::status_msg(const char* recv_buf)
{
	char showbuf[recv_len + 10];
	int ret;
	sscanf(recv_buf, "%d", &ret);
	QStandardItem *Qs = new QStandardItem(QString(recv_buf));
	Msg_Model.appendRow(Qs);
	sprintf(showbuf, "Response Code : %d", ret);
	code_label.setText(showbuf);
	ui.msgView->scrollToBottom();
	return ret;
}

bool FTPClient::login(SOCKET& sock,char *ID,char *PW)
{
	/* ���� ��USER username\r\n�� */
	sprintf(send_buf, "USER %s\r\n", ID);
	/*�ͻ��˷����û������������� */
	send(sock, send_buf, strlen(send_buf), 0);
	/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��331 User name okay, need password.�� */
	int recv_bytes = recv(sock, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';
	if (!login_bool) if (status_msg(recv_buf) != 331) return false;
	/* ���� ��PASS password\r\n�� */
	sprintf(send_buf, "PASS %s\r\n", PW);
	/* �ͻ��˷������뵽�������� */
	send(sock, send_buf, strlen(send_buf), 0);
	do
	{
		/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��230 User logged in.�� */
		recv_bytes = recv(sock, recv_buf, recv_len, 0);
		recv_buf[recv_bytes] = '\0';
	} while (strcmp(recv_buf, "230 User logged in.\r\n"));
	return true;
}

void FTPClient::Logined()//�ĳɵ��봰���P�]��
{
	if (!login_bool) return;
	ui.connect_button->setText(QString::fromLocal8Bit("������"));
	PrintfDir();
	ui.disconnect_button->setEnabled(true);
	ui.IP_Text->setEnabled(false);
	ui.Port_Text->setEnabled(false);
	ui.upload_button->setEnabled(true);
	ui.download_button->setEnabled(true);
	ui.disconnect_button->setEnabled(true);
	ui.connect_button->setEnabled(false);
	ui.BackButton->setEnabled(true);
	ui.RefreshDirButton->setEnabled(true);
}

void FTPClient::closeEvent(QCloseEvent *event)
{
	switch (QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��ȷ��Ҫ�˳�FTPClient?"), QString::fromLocal8Bit("ȷ��"), QString::fromLocal8Bit("ȡ��"), 0, 1))
	{
	case 0:
		event->accept();
		break;
	case 1:
	default:
		event->ignore();
		break;
	}
}

void FTPClient::PrintfDir()
{
	AutoConnect();
	//QMutexLocker LR(&ConnectMutex);
	Files_Model.removeRows(0, Files_Model.rowCount());
	/* ���� ��PASV\r\n�� */
	sprintf(send_buf, "PASV\r\n");
	/* �ͻ��˸��߷������ñ���ģʽ */
	send(control_sock, send_buf, strlen(send_buf), 0);
	/*�ͻ��˽��շ���������Ӧ����¿��Ķ˿ںţ�
	* ����Ϊ ��227 Entering passive mode (<h1,h2,h3,h4,p1,p2>)�� */
	int recv_bytes = recv(control_sock, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';
	status_msg(recv_buf);
	char serverIP[16];
	int serverIPv4[4];
	int port[2];
	u_short ports;
	sscanf(recv_buf, "%*[^(](%d,%d,%d,%d,%d,%d)", &serverIPv4[0], &serverIPv4[1], &serverIPv4[2], &serverIPv4[3], &port[0], &port[1]);
	sprintf(serverIP, "%d.%d.%d.%d", serverIPv4[0], serverIPv4[1], serverIPv4[2], serverIPv4[3]);
	ports = port[0] * 256 + port[1];
	data_sock = socket(AF_INET, SOCK_STREAM, 0);
	serveraddr_data.sin_addr.S_un.S_addr = inet_addr(serverIP);
	serveraddr_data.sin_family = AF_INET;
	serveraddr_data.sin_port = htons(ports);//���Ӷ˿�
	/* ���ӷ������¿������ݶ˿� */
	::connect(data_sock, (struct sockaddr *)&serveraddr_data, sizeof(serveraddr_data));

	/* ���� ��PWD \r\n�� */
	sprintf(send_buf, "PWD \r\n");
	/*�ͻ��˷���ݔ����ǰĿ������������ */
	send(control_sock, send_buf, strlen(send_buf), 0);
	/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��257 "%directory" is current directory.�� */
	recv_bytes = recv(control_sock, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';
	status_msg(recv_buf);

	char *p = strtok(recv_buf, "\"");
	p = strtok(NULL, "\"");
	ui.Dir_Text->setText(p);
	ui.FilesView->setDir(p);
	/* ���� ��LIST \r\n�� */
	sprintf(send_buf, "LIST \r\n");
	/*�ͻ��˷���ݔ��Ŀ������������ */
	send(control_sock, send_buf, strlen(send_buf), 0);
	/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��125 Data connection already open; Transfer starting.�� */
	recv_bytes = recv(control_sock, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';
	status_msg(recv_buf);

	/* �ͻ��˽���Ŀ䛱� */
	memset(&recv_buf, 0, sizeof(recv_buf));
	recv(data_sock, recv_buf, recv_len, 0);
	char* ap= recv_buf;
	char* bp;
	int row = 0;
	QStandardItem* item;
	while (bp=strstr(ap, "\n"))
	{
		int num = bp - ap;
		char *buf = (char*)malloc(1 + sizeof(char)*num);
		strncpy(buf, ap, num+1);
		char date[16], time[8], size[32], name[256];
		sscanf(buf, "%s %s %s %[^\r]",date,time,size,name);
		Files_Model.setItem(row, 0, new QStandardItem(QString::fromLocal8Bit(date)));
		Files_Model.item(row, 0)->setTextAlignment(Qt::AlignCenter);
		Files_Model.setItem(row, 1, new QStandardItem(QString::fromLocal8Bit(time)));
		Files_Model.item(row, 1)->setTextAlignment(Qt::AlignCenter);
		if (strcmp(size, "<DIR>"))
		{
			item = new QStandardItem();
			item->setData(QVariant(atoll(size)), Qt::DisplayRole);
			Files_Model.setItem(row, 2, item);
		}
		else Files_Model.setItem(row, 2, new QStandardItem(QString::fromLocal8Bit(size)));
		Files_Model.item(row, 2)->setTextAlignment(Qt::AlignRight);
		Files_Model.setItem(row, 3, new QStandardItem(QString::fromLocal8Bit(name)));
		free(buf);
		ap = bp + 1;
		row++;
	}
	/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��226 Transfer complete.�� */
	recv_bytes = recv(control_sock, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';
	/* �ͻ��˹ر��������� */
	closesocket(data_sock);
	status_msg(recv_buf);
}

void FTPClient::ftpdisconnect()
{
	/* ���� ��QUIT\r\n�� */
	sprintf(send_buf, "QUIT\r\n");
	/*�ͻ��˷����˳������������ */
	send(control_sock, send_buf, strlen(send_buf), 0);
	/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��221 Good Bye�� */
	int recv_bytes = recv(control_sock, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';
	status_msg(recv_buf);
	ui.Dir_Text->setText("");
	Files_Model.removeRows(0, Files_Model.rowCount());
	ui.connect_button->setText(QString::fromLocal8Bit("����"));
	ui.upload_button->setEnabled(false);
	ui.download_button->setEnabled(false);
	ui.disconnect_button->setEnabled(false);
	ui.connect_button->setEnabled(true);
	ui.BackButton->setEnabled(false);
	ui.IP_Text->setEnabled(true);
	ui.Port_Text->setEnabled(true);
	ui.RefreshDirButton->setEnabled(false);
	login_bool = false;
}

void FTPClient::DoubleClickItem(const QModelIndex & curr_model)
{
	AutoConnect();
	if (TransmissionMode == PORT_MODE && !strcmp(hostip, "0.0.0.0"))
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("ȡ������"), QString::fromLocal8Bit("��ָ��ʹ���Ǹ�����ȥ����\r\n�ڣ�����ģʽ������������ģʽ���趨"));
		return;
	}
	int selectrow = ui.FilesView->currentIndex().row();
	QModelIndex FileTypeIndex = curr_model.model()->index(selectrow, 2);//ѡ���е�3�е�����
	QModelIndex FileNameIndex = curr_model.model()->index(selectrow, 3);//ѡ���е�4�е�����
	QVariant FileType = curr_model.model()->data(FileTypeIndex);
	QVariant FileName = curr_model.model()->data(FileNameIndex);
	if (!strcmp(FileType.toString().toLocal8Bit().data(), "<DIR>"))
	{
		std::string EnterDir(ui.Dir_Text->text().toStdString());
		EnterDir.append("/");
		EnterDir.append(FileName.toString().toStdString());
		ChangeDir(EnterDir.data());
	}
	else
	{
		//���d����
		QFileDialog *fileDialog = new QFileDialog(this);
		fileDialog->setWindowTitle(QString::fromLocal8Bit("ѡ�����ص�λ��"));
		fileDialog->setAcceptMode(QFileDialog::AcceptSave);
		fileDialog->setDirectory("/");
		fileDialog->selectFile(FileName.toString());
		if (fileDialog->exec() == QDialog::Accepted) 
		{
			QString path = fileDialog->selectedFiles()[0];
			QString sourpath(ui.Dir_Text->text());
			if (sourpath.compare("/")) sourpath.append('/');
			sourpath.append(FileName.toString());
			//QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
			if(!DownloadFile(path.toLocal8Bit().data(), sourpath.toLocal8Bit().data())) QMessageBox::information(NULL, QString::fromLocal8Bit("����ʧ��"), QString::fromLocal8Bit("����ʧ��"));
		}
		else 
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("ȡ������"), QString::fromLocal8Bit("û��ѡ�����ص�λ��"));
		}
	}
}

void FTPClient::DownloadButtonClicked()
{
	if (TransmissionMode == PORT_MODE && !strcmp(hostip, "0.0.0.0"))
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("ȡ������"), QString::fromLocal8Bit("��ָ��ʹ���Ǹ�����ȥ����\r\n�ڣ�����ģʽ������������ģʽ���趨"));
		return;
	}
	int selectrow = ui.FilesView->currentIndex().row();
	QModelIndex FileTypeIndex = ui.FilesView->model()->index(selectrow, 2);//ѡ���е�3�е�����
	QModelIndex FileNameIndex = ui.FilesView->model()->index(selectrow, 3);//ѡ���е�4�е�����
	QVariant FileType = ui.FilesView->model()->data(FileTypeIndex);
	QVariant FileName = ui.FilesView->model()->data(FileNameIndex);
	if (!strcmp(FileType.toString().toLocal8Bit().data(), "<DIR>"))
	{
		/*std::string EnterDir(ui.Dir_Text->text().toStdString());
		EnterDir.append("/");
		EnterDir.append(FileName.toString().toStdString());
		ChangeDir(EnterDir.data());*/
		//����Ŀ����d
	}
	else
	{
		//���d����
		QFileDialog *fileDialog = new QFileDialog(this);
		fileDialog->setWindowTitle(QString::fromLocal8Bit("ѡ�����ص�λ��"));
		fileDialog->setAcceptMode(QFileDialog::AcceptSave);
		fileDialog->setDirectory("/");
		fileDialog->selectFile(FileName.toString());
		if (fileDialog->exec() == QDialog::Accepted)
		{
			QString path = fileDialog->selectedFiles()[0];
			QString sourpath(ui.Dir_Text->text());
			if (sourpath.compare("/")) sourpath.append('/');
			sourpath.append(FileName.toString());
			//QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
			if (!DownloadFile(path.toLocal8Bit().data(), sourpath.toLocal8Bit().data())) QMessageBox::information(NULL, QString::fromLocal8Bit("����ʧ��"), QString::fromLocal8Bit("����ʧ��"));
		}
		else 
		{
			QMessageBox::information(NULL, QString::fromLocal8Bit("ȡ������"), QString::fromLocal8Bit("û��ѡ�����ص�λ��"));
		}
	}
}
void FTPClient::UploadButtonClicked()
{
	if (TransmissionMode == PORT_MODE && !strcmp(hostip, "0.0.0.0"))
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("ȡ���ϴ�"), QString::fromLocal8Bit("��ָ��ʹ���Ǹ�����ȥ����\r\n�ڣ�����ģʽ������������ģʽ���趨"));
		return;
	}
	QFileSystemModel* model = static_cast < QFileSystemModel *>(ui.FileSysView->model());
	QModelIndex   currentIndex = ui.FileSysView->currentIndex();
	QFileInfo currentFileInfo(model->fileInfo(currentIndex));
	QString Path(currentFileInfo.absolutePath());
	Path.append('/');
	Path.append(currentFileInfo.fileName());
	if(currentFileInfo.isFile())
	{
		//�ϴ�����
		if (!UploadFile(ui.Dir_Text->text().toLocal8Bit().data(),Path.toLocal8Bit().data())) QMessageBox::information(NULL, QString::fromLocal8Bit("�ϴ�ʧ��"), QString::fromLocal8Bit("�ϴ�ʧ��"));
	}
}
bool FTPClient::DownloadFile(const char* Dest, const char* Sour)
{
	if (login_bool == false) return false;
	QMutexLocker LR(&UpDownListMutex);
	if (!Dest)return false;
	if (UpDownloadThread::getThreadNum() < MAXThread)
	{
		int row = UpDownList_Model.rowCount();
		UpDownList_Model.setItem(row, 0, new QStandardItem(QString::fromLocal8Bit("׼������")));
		UpDownList_Model.setItem(row, 1, new QStandardItem(Sour));
		UpDownList_Model.setItem(row, 2, new QStandardItem(Dest));
		UpDownList_Model.setItem(row, 3, new QStandardItem(0));
		UpDownList_Model.setItem(row, 4, new QStandardItem(0));
		UpDownList_Model.setItem(row, 5, new QStandardItem(0));
		QStandardItem *status = new QStandardItem();
		status->setData(QVariant(DownLoadStatus), Qt::DisplayRole);
		UpDownList_Model.setItem(row, 6, status);
		SOCKET newsocket;
		socket_build(newsocket, serverIP, serverPort);
		login(newsocket, ID, PW);
		UpDownloadThread *DT = new UpDownloadThread(newsocket, Dest, Sour, hostip, listenport, TransmissionMode, UpDownList_Model, DownloadMode, row, 0);
		connect(DT, &QThread::finished, DT, &QThread::deleteLater);
		connect(DT, &QThread::finished, this, &FTPClient::CheckWaitQueue);
		//connect(DT, &UpDownloadThread::ViewFresh, Progress, &ProgressDelegate::paint);
		DT->start();
	}
	else
	{//���^��󾀳̔�.�M��ȴ����
		Task *newtask = new Task(hostip, Dest, Sour, ID, PW, UpDownList_Model.rowCount(), DownloadMode, TransmissionMode, serverIP, serverPort, 0);
		UpDownList_Model.setItem(newtask->Row, 0, new QStandardItem(QString::fromLocal8Bit("�ȴ�����")));
		UpDownList_Model.setItem(newtask->Row, 1, new QStandardItem(Sour));
		UpDownList_Model.setItem(newtask->Row, 2, new QStandardItem(Dest));
		UpDownList_Model.setItem(newtask->Row, 3, new QStandardItem(0));
		UpDownList_Model.setItem(newtask->Row, 4, new QStandardItem(0));
		UpDownList_Model.setItem(newtask->Row, 5, new QStandardItem(0));
		QStandardItem *status = new QStandardItem();
		status->setData(QVariant(ReadyStatus), Qt::DisplayRole);
		UpDownList_Model.setItem(newtask->Row, 6, status);
		WaitQueue.push(newtask);
	}
	return true;
}
bool FTPClient::UploadFile(const char* Dest, const char* Sour)
{
	if (login_bool == false) return false;
	QFileInfo SourFileInfo(Sour);
	if (SourFileInfo.isDir()) return false;
	QMutexLocker LR(&UpDownListMutex);
	if (!Dest)return false;
	if (UpDownloadThread::getThreadNum() < MAXThread)
	{
		int row = UpDownList_Model.rowCount();
		UpDownList_Model.setItem(row, 0, new QStandardItem(QString::fromLocal8Bit("׼���ϴ�")));
		UpDownList_Model.setItem(row, 1, new QStandardItem(Sour));
		UpDownList_Model.setItem(row, 2, new QStandardItem(Dest));
		UpDownList_Model.setItem(row, 3, new QStandardItem(0));
		UpDownList_Model.setItem(row, 4, new QStandardItem(0));
		UpDownList_Model.setItem(row, 5, new QStandardItem(0));
		QStandardItem *status = new QStandardItem();
		status->setData(QVariant(UpLoadStatus), Qt::DisplayRole);
		UpDownList_Model.setItem(row, 6, status);
		SOCKET newsocket;
		socket_build(newsocket, serverIP, serverPort);
		login(newsocket, ID, PW);
		UpDownloadThread *DT = new UpDownloadThread(newsocket, Dest, Sour, hostip, listenport, TransmissionMode, UpDownList_Model, UploadMode, row, 0);
		connect(DT, &QThread::finished, DT, &QThread::deleteLater);
		connect(DT, &QThread::finished, this, &FTPClient::CheckWaitQueue);
		DT->start();
	}
	else
	{//���^��󾀳̔�.�M��ȴ����
		Task *newtask = new Task(hostip, Dest, Sour, ID, PW, UpDownList_Model.rowCount(), UploadMode, TransmissionMode, serverIP, serverPort, 0);
		UpDownList_Model.setItem(newtask->Row, 0, new QStandardItem(QString::fromLocal8Bit("�ȴ��ϴ�")));
		UpDownList_Model.setItem(newtask->Row, 1, new QStandardItem(Sour));
		UpDownList_Model.setItem(newtask->Row, 2, new QStandardItem(Dest));
		UpDownList_Model.setItem(newtask->Row, 3, new QStandardItem(0));
		UpDownList_Model.setItem(newtask->Row, 4, new QStandardItem(0));
		UpDownList_Model.setItem(newtask->Row, 5, new QStandardItem(0));
		QStandardItem *status = new QStandardItem();
		status->setData(QVariant(ReadyStatus), Qt::DisplayRole);
		UpDownList_Model.setItem(newtask->Row, 6, status);
		WaitQueue.push(newtask);
	}
	return true;
}
void FTPClient::CheckWaitQueue()//���̽Y����
{
	QMutexLocker LR(&UpDownListMutex);
	QMutexLocker Locker(&QueueMutex);//�ȴ���еĻ����i
	if (!WaitQueue.empty())
	{
		Task *temp = WaitQueue.front();//ȡ��е���Ԫ��
		WaitQueue.pop();//�����
		SOCKET newsocket;
		socket_build(newsocket, serverIP, serverPort);//�¾����õ�socket
		login(newsocket, temp->ID, temp->PW);//ʹ���µ�socket����FTP
		//�����µ���/���d����
		UpDownloadThread *DT = new UpDownloadThread(newsocket, temp->Dest, temp->Sour, temp->hostip, listenport, temp->TransmissionMode, UpDownList_Model, temp->Mode, temp->Row, temp->offest);
		//�Ѿ��̽Y����̖�B�ӵ������Ĳ�
		if (temp->Mode == UploadMode) UpDownList_Model.setData(UpDownList_Model.index(temp->Row, 6), UpLoadStatus, Qt::DisplayRole);
		else UpDownList_Model.setData(UpDownList_Model.index(temp->Row, 6), DownLoadStatus, Qt::DisplayRole);
		connect(DT, &QThread::finished, DT, &QThread::deleteLater);
		connect(DT, &QThread::finished, this, &FTPClient::CheckWaitQueue);
		DT->start();//�����_ʼ�\��
		delete temp;
	}
}

bool FTPClient::ChangeDir(const char* EnterDir)
{
	AutoConnect();
	if (!EnterDir) return  false;
	/* ���� ��CWD dirname\r\n�� */
	sprintf(send_buf, "CWD %s\r\n", EnterDir);
	/* �ͻ��˷�������ı乤��Ŀ¼ */
	send(control_sock, send_buf, strlen(send_buf), 0);
	/* �ͻ��˽��շ���������Ӧ�����Ϣ������Ϊ ��250 CWD command successful.�� */
	int recv_bytes = recv(control_sock, recv_buf, recv_len, 0);
	recv_buf[recv_bytes] = '\0';
	status_msg(recv_buf);
	PrintfDir();
	return true;
}

void FTPClient::BackButtonClicked()
{
	AutoConnect();
	int dir_len = strlen(ui.Dir_Text->text().toLocal8Bit().data());
	if (dir_len == 1) return;
	ChangeDir("../");
}

void FTPClient::SetHostIP(QAction* A)
{
	if (A->isChecked())
	{
		if (QString::compare(A->text(), "������PASV��ģʽ")) TransmissionMode = PASV_MODE;
		else TransmissionMode = PORT_MODE;
		strcpy(hostip, A->text().toLocal8Bit().data());
	}
}

void FTPClient::ChangeStatus(const QModelIndex &index)
{
	QMutexLocker LR(&UpDownListMutex);
	int row = index.row();
	switch(index.data().toInt())
	{
		case DownStopStatus:
		{
			ui.UpDownListView->model()->setData(index, QVariant(DownLoadStatus), Qt::DisplayRole);
			//�^�m���dδ��ɵă���
			if (UpDownloadThread::getThreadNum() < MAXThread)
			{
				SOCKET newsocket;
				socket_build(newsocket, serverIP, serverPort);
				login(newsocket, ID, PW);
				UpDownloadThread *DT = new UpDownloadThread(newsocket, index.sibling(row, 2).data().toString().toLocal8Bit().data(), index.sibling(row, 1).data().toString().toLocal8Bit().data(), hostip, listenport, TransmissionMode, UpDownList_Model, DownloadMode, row, index.sibling(row, 3).data().toLongLong());
				UpDownList_Model.itemFromIndex(index.sibling(row, 0))->setText(QString::fromLocal8Bit("������"));
				connect(DT, &QThread::finished, DT, &QThread::deleteLater);
				connect(DT, &QThread::finished, this, &FTPClient::CheckWaitQueue);
				DT->start();
			}
			else
			{//���^��󾀳̔�.�M��ȴ����
				Task *newtask = new Task(hostip, index.sibling(row, 2).data().toString().toLocal8Bit().data(), index.sibling(row, 1).data().toString().toLocal8Bit().data(), ID, PW, row, DownloadMode, TransmissionMode, serverIP, serverPort, index.sibling(row, 3).data().toLongLong());
				UpDownList_Model.item(newtask->Row, 0)->setText(QString::fromLocal8Bit("�ȴ�����"));
				UpDownList_Model.setData(index, 6, ReadyStatus);
				WaitQueue.push(newtask);
			}
			break;
		}
		case UpStopStatus:
		{
			ui.UpDownListView->model()->setData(index, QVariant(UpLoadStatus), Qt::DisplayRole);
			//�^�m�ς�δ��ɵă���
			if (UpDownloadThread::getThreadNum() < MAXThread)
			{
				SOCKET newsocket;
				socket_build(newsocket, serverIP, serverPort);
				login(newsocket, ID, PW);
				UpDownloadThread *DT = new UpDownloadThread(newsocket, index.sibling(row, 2).data().toString().toLocal8Bit().data(), index.sibling(row, 1).data().toString().toLocal8Bit().data(), hostip, listenport, TransmissionMode, UpDownList_Model, UploadMode, row, index.sibling(row, 3).data().toLongLong());
				UpDownList_Model.itemFromIndex(index.sibling(row, 0))->setText(QString::fromLocal8Bit("�ϴ���")); 
				connect(DT, &QThread::finished, DT, &QThread::deleteLater);
				connect(DT, &QThread::finished, this, &FTPClient::CheckWaitQueue);
				DT->start();
			}
			else
			{//���^��󾀳̔�.�M��ȴ����
				Task *newtask = new Task(hostip, index.sibling(row, 2).data().toString().toLocal8Bit().data(), index.sibling(row, 1).data().toString().toLocal8Bit().data(), ID, PW, row, UploadMode, TransmissionMode, serverIP, serverPort, index.sibling(row, 3).data().toLongLong());
				UpDownList_Model.item(newtask->Row, 0)->setText(QString::fromLocal8Bit("�ȴ��ϴ�"));
				UpDownList_Model.setData(index, 6, ReadyStatus);
				WaitQueue.push(newtask);
			}
			break;
		}
		case DownLoadStatus:
		{
			ui.UpDownListView->model()->setData(index, QVariant(DownStopStatus), Qt::DisplayRole);
			UpDownList_Model.itemFromIndex(index.sibling(row,0))->setText(QString::fromLocal8Bit("��ͣ����"));
			break;
		}
		case UpLoadStatus:
		{
			ui.UpDownListView->model()->setData(index, QVariant(UpStopStatus), Qt::DisplayRole);
			UpDownList_Model.itemFromIndex(index.sibling(row, 0))->setText(QString::fromLocal8Bit("��ͣ�ϴ�"));
			break;
		}
		default: break;
	}
}

void FTPClient::AutoConnect()
{
	if (login_bool)
	{
		char buf[send_len];
		strcpy(buf, "PWD \r\n");
		if (-1 == send(control_sock, buf, strlen(buf), 0) || -1 == recv(control_sock, buf, send_len, 0))
		{
			closesocket(control_sock);
			if (!socket_build(control_sock, serverIP, serverPort))QMessageBox::information(NULL, QString::fromLocal8Bit("�Ԅ��B��ʧ��"), QString::fromLocal8Bit("�Ԅ��B��ʧ��@(socket_build)"));
			if (!login(control_sock, ID, PW)) QMessageBox::information(NULL, QString::fromLocal8Bit("�Ԅ��B��ʧ��"), QString::fromLocal8Bit("�Ԅ��B��ʧ��@(login)"));
			int timeout = 3000; //3s
			int ret = setsockopt(control_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
		}
	}
}

void FTPClient::dropUpload(QString& Sour)
{
	UploadFile(ui.Dir_Text->text().toLocal8Bit().data(), Sour.toLocal8Bit().data());
	PrintfDir();
}

void FTPClient::dropDownload(QString& Dest, QString& Sour)
{
	FTPClient::DownloadFile(Dest.toLocal8Bit().data(), Sour.toLocal8Bit().data());
}

void FTPClient::RefreshButtonClicked()
{
	PrintfDir();
}