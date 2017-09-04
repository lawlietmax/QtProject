#include "TreeView.h"

TreeView::TreeView(QWidget *parent)
	: QTreeView(parent)
{
	setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDragEnabled(true);
	setDropIndicatorShown(true);
}

TreeView::~TreeView()
{
}

void TreeView::dropEvent(QDropEvent * event)
{
	if (event->mimeData()->hasText())
	{
		QString Sour = event->mimeData()->text();
		std::string SourString(Sour.toLocal8Bit());//源地址的字符串
		int index = SourString.find_last_of('/');//找到最後一個字符為'/'的位置
		std::string FileName;
		FileName.assign(SourString, index + 1);//文件名的字符串
		if (fileinfo.isDir())
		{
			QString Dest = fileinfo.absoluteFilePath();
			Dest.append(QString::fromLocal8Bit(FileName.data()));
			qDebug() << Sour << "to" << Dest;
			emit dropedsomething(Dest, Sour);
		}
		else if (fileinfo.isFile())
		{
			QString Dest = fileinfo.absolutePath();
			Dest.append(QString::fromLocal8Bit(FileName.data()));
			qDebug() << Sour << "to" << Dest;
			emit dropedsomething(Dest, Sour);
		}
	}
}

void TreeView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasText())  	event->acceptProposedAction();
	qDebug() << "drag enter";
}

void TreeView::dragMoveEvent(QDragMoveEvent *event)
{
	Mouse = event->pos();
	QModelIndex index = indexAt(Mouse);
	if (index.isValid())
	{
		const QFileSystemModel* model = static_cast <const QFileSystemModel *>(index.model());
		fileinfo = model->fileInfo(index);
		if (!isExpanded(index))
		{
			_sleep(300);
			QPoint Current= event->pos();
			if (Mouse == Current) expand(index);
			if (lastindex != index.parent()) collapse(lastindex);
		}
		lastindex = index;
	}
}