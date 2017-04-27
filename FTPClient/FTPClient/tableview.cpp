#include "tableview.h"

TableView::TableView(QWidget *parent)
	: QTableView(parent)
{
	setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDragEnabled(true);
	setDropIndicatorShown(true);
}

TableView::~TableView()
{
}

void TableView::dropEvent(QDropEvent * event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty())
		return;

	foreach(QUrl url, urls) {
		QString file_name = url.toLocalFile();
		emit dropedsomething(file_name);
		qDebug() << file_name;
	}
}

void TableView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/uri-list"))  	event->acceptProposedAction();
		qDebug() << "drag enter";
}

void TableView::dragMoveEvent(QDragMoveEvent *event)
{
	qDebug() << "drag move";
}

void TableView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		startPos = event->pos();
	QTableView::mousePressEvent(event);
}

void TableView::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		int distance = (event->pos() - startPos).manhattanLength();
		if (distance >= QApplication::startDragDistance())
			performDrag();
	}
	QTableView::mouseMoveEvent(event);
}

void TableView::performDrag()
{
	QModelIndex *index = &currentIndex();
	if (index) 
	{
		QMimeData *mimeData = new QMimeData;
		QString url(Dir);
		url.append(index->sibling(index->row(), 3).data(Qt::DisplayRole).toString());
		mimeData->setText(url);
		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData);
		if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
			delete index;
	}
}
void TableView::setDir(const char* dir)
{
	Dir.clear();
	if (!dir) return;
	Dir.append(dir);
	if (strcmp(dir, "/")) Dir.append("/");
}