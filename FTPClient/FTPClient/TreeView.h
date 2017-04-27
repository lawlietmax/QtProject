#pragma once
#include <qdebug.h>
#include <QTreeView>
#include <QMimeData>
#include <QDropEvent>
#include <QFileSystemModel>
class TreeView : public QTreeView
{
	Q_OBJECT
signals :
	void dropedsomething(QString& Dest, QString& Sour);
public:
	TreeView(QWidget *parent);
	~TreeView();
protected:
	void dropEvent(QDropEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
private:
	QPoint Mouse;
	QModelIndex lastindex;
	QFileInfo fileinfo;
};
