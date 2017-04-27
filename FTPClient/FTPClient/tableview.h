#pragma once
#include <qdebug.h>
#include <QDrag>
#include <QMimeData>
#include <QDropEvent>
#include <QTableView>
#include <QApplication>
#include <QStandardItem>
class TableView : public QTableView
{
	Q_OBJECT
signals:
	void dropedsomething(QString& file_name);
public:
	TableView(QWidget *parent = 0);
	~TableView();
	void setDir(const char* dir);
protected:
	void dropEvent(QDropEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
private:
	QString Dir;
	void performDrag();
	QPoint startPos;
};
