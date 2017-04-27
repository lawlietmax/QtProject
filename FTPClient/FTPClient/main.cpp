#include "FTPClient.h"
#include <QtWidgets/QApplication>
bool login_bool;
QMutex UpDownListMutex;
int GetRetCode(const char *recv_buf)
{
	int ret;
	sscanf(recv_buf, "%d", &ret);
	return ret;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FTPClient w;
    w.show();
    return a.exec();
}
