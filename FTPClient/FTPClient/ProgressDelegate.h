#pragma once

#include <QMouseEvent>
#include <QPushButton>
#include <QStyledItemDelegate>
#include "defineword.h"
class ProgressDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	explicit ProgressDelegate(QObject *parent = 0);
	~ProgressDelegate();
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	bool ProgressDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);
signals:
	void clicked(const QModelIndex &index);
private:
};
