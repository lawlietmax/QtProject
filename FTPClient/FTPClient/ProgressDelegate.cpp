#include "ProgressDelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDialog>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QProgressBar>

ProgressDelegate::ProgressDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

ProgressDelegate::~ProgressDelegate()
{
}

void ProgressDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() ==5) 
	{
		double progress = index.data().toDouble();
		QStyleOptionProgressBar progressBarOption;
		progressBarOption.rect = option.rect;
		progressBarOption.minimum = 0;
		progressBarOption.maximum = 100;
		progressBarOption.progress = (int)progress;
		progressBarOption.text = QString::number(progress, 'f', 1) + "%";
		progressBarOption.textVisible = true;
		QApplication::style()->drawControl(QStyle::CE_ProgressBar,	&progressBarOption, painter);
	}
	else if (index.column() == 6)
	{
		int Status = index.data().toInt();
		QStyleOptionButton INTButton;
		INTButton.rect = option.rect;
		INTButton.iconSize = QSize(20, 20);
		INTButton.state |= QStyle::State_Enabled;
		if ((DownStopStatus == Status) || (UpStopStatus == Status))
		{
			INTButton.text = QString::fromLocal8Bit("继续");
			if (DownStopStatus == Status)INTButton.icon = QIcon(QString(":/FTPClient/download.png"));
			else if (UpStopStatus == Status)INTButton.icon = QIcon(QString(":/FTPClient/upload.png"));
		}
		else if ((DownLoadStatus == Status) || (UpLoadStatus == Status))
		{
			INTButton.text = QString::fromLocal8Bit("暂停");
			if (DownLoadStatus == Status) INTButton.icon = QIcon(QString(":/FTPClient/downloadrel.png"));
			else if (UpLoadStatus == Status)INTButton.icon = QIcon(QString(":/FTPClient/uploadrel.png"));
		}
		else if(FinishStatus == Status)
		{
			INTButton.text = QString::fromLocal8Bit("已完成");
			INTButton.icon = QIcon(QString(":/FTPClient/finish.png"));
		}
		else if (ReadyStatus == Status)
		{
			INTButton.text = QString::fromLocal8Bit("等待");
			INTButton.icon = QIcon(QString(":/FTPClient/wait.png"));
		}
		QApplication::style()->drawControl(QStyle::CE_PushButton, &INTButton, painter);
	}
	else	QStyledItemDelegate::paint(painter, option, index);
}

bool ProgressDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
	if (index.column() != 6) return QStyledItemDelegate::editorEvent(event, model, option, index);
	bool bRepaint = false;
	QMouseEvent *pEvent = static_cast<QMouseEvent*> (event);
	QPoint m_mousePoint = pEvent->pos();
	// 还原鼠标样式
	QApplication::restoreOverrideCursor();
	QStyleOptionButton button;
	button.rect = option.rect;
	// 鼠标位于按钮之上
	if (button.rect.contains(m_mousePoint))
	{
		bRepaint = true;
		switch (event->type())
		{
			// 鼠标释放
			case QEvent::MouseButtonRelease:
			{
				emit clicked(index);
				break;
			}
			default: break;
		}
	}
	return bRepaint;
}