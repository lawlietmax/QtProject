/********************************************************************************
** Form generated from reading UI file 'FTPClient.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FTPCLIENT_H
#define UI_FTPCLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <TreeView.h>
#include <tableview.h>

QT_BEGIN_NAMESPACE

class Ui_FTPClientClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *IP_Text;
    QLabel *label_2;
    QLineEdit *Port_Text;
    QPushButton *connect_button;
    QPushButton *disconnect_button;
    QHBoxLayout *horizontalLayout_3;
    QLabel *dir_label;
    QLineEdit *Dir_Text;
    QCommandLinkButton *RefreshDirButton;
    QCommandLinkButton *BackButton;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QWidget *layoutWidget_1;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    TableView *FilesView;
    QPushButton *download_button;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    TreeView *FileSysView;
    QPushButton *upload_button;
    QTabWidget *Tab;
    QWidget *MsgTab;
    QGridLayout *gridLayout_2;
    QListView *msgView;
    QWidget *ListTab;
    QGridLayout *gridLayout_3;
    QTableView *UpDownListView;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FTPClientClass)
    {
        if (FTPClientClass->objectName().isEmpty())
            FTPClientClass->setObjectName(QStringLiteral("FTPClientClass"));
        FTPClientClass->resize(1230, 704);
        centralWidget = new QWidget(FTPClientClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(10);
        label->setFont(font);

        horizontalLayout_2->addWidget(label);

        IP_Text = new QLineEdit(centralWidget);
        IP_Text->setObjectName(QStringLiteral("IP_Text"));

        horizontalLayout_2->addWidget(IP_Text);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        Port_Text = new QLineEdit(centralWidget);
        Port_Text->setObjectName(QStringLiteral("Port_Text"));

        horizontalLayout_2->addWidget(Port_Text);

        connect_button = new QPushButton(centralWidget);
        connect_button->setObjectName(QStringLiteral("connect_button"));

        horizontalLayout_2->addWidget(connect_button);

        disconnect_button = new QPushButton(centralWidget);
        disconnect_button->setObjectName(QStringLiteral("disconnect_button"));
        disconnect_button->setEnabled(false);

        horizontalLayout_2->addWidget(disconnect_button);

        horizontalLayout_2->setStretch(1, 8);
        horizontalLayout_2->setStretch(3, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        dir_label = new QLabel(centralWidget);
        dir_label->setObjectName(QStringLiteral("dir_label"));
        dir_label->setMaximumSize(QSize(100, 16777215));
        dir_label->setFont(font);

        horizontalLayout_3->addWidget(dir_label);

        Dir_Text = new QLineEdit(centralWidget);
        Dir_Text->setObjectName(QStringLiteral("Dir_Text"));
        Dir_Text->setReadOnly(true);

        horizontalLayout_3->addWidget(Dir_Text);

        RefreshDirButton = new QCommandLinkButton(centralWidget);
        RefreshDirButton->setObjectName(QStringLiteral("RefreshDirButton"));
        RefreshDirButton->setEnabled(false);
        RefreshDirButton->setMaximumSize(QSize(70, 100));
        QFont font1;
        font1.setFamily(QStringLiteral("Segoe UI"));
        font1.setPointSize(10);
        RefreshDirButton->setFont(font1);
        QIcon icon;
        icon.addFile(QStringLiteral(":/FTPClient/update.png"), QSize(), QIcon::Normal, QIcon::Off);
        RefreshDirButton->setIcon(icon);

        horizontalLayout_3->addWidget(RefreshDirButton);

        BackButton = new QCommandLinkButton(centralWidget);
        BackButton->setObjectName(QStringLiteral("BackButton"));
        BackButton->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BackButton->sizePolicy().hasHeightForWidth());
        BackButton->setSizePolicy(sizePolicy);
        BackButton->setMaximumSize(QSize(80, 100));
        BackButton->setFont(font1);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/FTPClient/timg.png"), QSize(), QIcon::Normal, QIcon::Off);
        BackButton->setIcon(icon1);

        horizontalLayout_3->addWidget(BackButton);

        horizontalLayout_3->setStretch(1, 10);

        verticalLayout->addLayout(horizontalLayout_3);

        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget_1 = new QWidget(splitter);
        layoutWidget_1->setObjectName(QStringLiteral("layoutWidget_1"));
        verticalLayout_3 = new QVBoxLayout(layoutWidget_1);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget_1);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setFont(font);

        verticalLayout_3->addWidget(label_4);

        FilesView = new TableView(layoutWidget_1);
        FilesView->setObjectName(QStringLiteral("FilesView"));
        FilesView->setContextMenuPolicy(Qt::DefaultContextMenu);
        FilesView->setAcceptDrops(true);
        FilesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        FilesView->setDragEnabled(true);
        FilesView->setDragDropOverwriteMode(false);
        FilesView->setDragDropMode(QAbstractItemView::DragDrop);
        FilesView->setDefaultDropAction(Qt::MoveAction);
        FilesView->setSelectionMode(QAbstractItemView::SingleSelection);
        FilesView->setSelectionBehavior(QAbstractItemView::SelectRows);
        FilesView->setGridStyle(Qt::NoPen);
        FilesView->setSortingEnabled(true);
        FilesView->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        FilesView->verticalHeader()->setVisible(false);
        FilesView->verticalHeader()->setDefaultSectionSize(20);
        FilesView->verticalHeader()->setMinimumSectionSize(20);

        verticalLayout_3->addWidget(FilesView);

        download_button = new QPushButton(layoutWidget_1);
        download_button->setObjectName(QStringLiteral("download_button"));
        download_button->setEnabled(false);

        verticalLayout_3->addWidget(download_button);

        splitter->addWidget(layoutWidget_1);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setFont(font);

        verticalLayout_2->addWidget(label_3);

        FileSysView = new TreeView(layoutWidget);
        FileSysView->setObjectName(QStringLiteral("FileSysView"));
        FileSysView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        FileSysView->setDragEnabled(true);
        FileSysView->setDragDropMode(QAbstractItemView::DragDrop);
        FileSysView->setDefaultDropAction(Qt::MoveAction);
        FileSysView->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_2->addWidget(FileSysView);

        upload_button = new QPushButton(layoutWidget);
        upload_button->setObjectName(QStringLiteral("upload_button"));
        upload_button->setEnabled(false);

        verticalLayout_2->addWidget(upload_button);

        verticalLayout_2->setStretch(2, 1);
        splitter->addWidget(layoutWidget);
        splitter_2->addWidget(splitter);
        Tab = new QTabWidget(splitter_2);
        Tab->setObjectName(QStringLiteral("Tab"));
        Tab->setTabPosition(QTabWidget::South);
        Tab->setTabShape(QTabWidget::Rounded);
        Tab->setElideMode(Qt::ElideMiddle);
        Tab->setDocumentMode(false);
        Tab->setTabsClosable(true);
        Tab->setMovable(false);
        Tab->setTabBarAutoHide(false);
        MsgTab = new QWidget();
        MsgTab->setObjectName(QStringLiteral("MsgTab"));
        MsgTab->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(MsgTab->sizePolicy().hasHeightForWidth());
        MsgTab->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(MsgTab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        msgView = new QListView(MsgTab);
        msgView->setObjectName(QStringLiteral("msgView"));
        sizePolicy1.setHeightForWidth(msgView->sizePolicy().hasHeightForWidth());
        msgView->setSizePolicy(sizePolicy1);
        msgView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_2->addWidget(msgView, 0, 0, 1, 1);

        Tab->addTab(MsgTab, QString());
        ListTab = new QWidget();
        ListTab->setObjectName(QStringLiteral("ListTab"));
        sizePolicy1.setHeightForWidth(ListTab->sizePolicy().hasHeightForWidth());
        ListTab->setSizePolicy(sizePolicy1);
        gridLayout_3 = new QGridLayout(ListTab);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        UpDownListView = new QTableView(ListTab);
        UpDownListView->setObjectName(QStringLiteral("UpDownListView"));
        UpDownListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        UpDownListView->setSelectionMode(QAbstractItemView::SingleSelection);
        UpDownListView->setSelectionBehavior(QAbstractItemView::SelectRows);
        UpDownListView->verticalHeader()->setVisible(false);
        UpDownListView->verticalHeader()->setDefaultSectionSize(30);
        UpDownListView->verticalHeader()->setMinimumSectionSize(20);

        gridLayout_3->addWidget(UpDownListView, 0, 0, 1, 1);

        Tab->addTab(ListTab, QString());
        splitter_2->addWidget(Tab);

        verticalLayout->addWidget(splitter_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        FTPClientClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FTPClientClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1230, 21));
        FTPClientClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FTPClientClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FTPClientClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FTPClientClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FTPClientClass->setStatusBar(statusBar);

        retranslateUi(FTPClientClass);

        Tab->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(FTPClientClass);
    } // setupUi

    void retranslateUi(QMainWindow *FTPClientClass)
    {
        FTPClientClass->setWindowTitle(QApplication::translate("FTPClientClass", "FTPClient", Q_NULLPTR));
        label->setText(QApplication::translate("FTPClientClass", "\346\234\215\345\212\241\345\231\250IP:", Q_NULLPTR));
        IP_Text->setText(QApplication::translate("FTPClientClass", "192.168.31.100", Q_NULLPTR));
        label_2->setText(QApplication::translate("FTPClientClass", "\347\253\257\345\217\243:", Q_NULLPTR));
        Port_Text->setText(QApplication::translate("FTPClientClass", "21", Q_NULLPTR));
        connect_button->setText(QApplication::translate("FTPClientClass", "\350\277\236\346\216\245", Q_NULLPTR));
        disconnect_button->setText(QApplication::translate("FTPClientClass", "\344\270\255\346\226\255\350\277\236\346\216\245", Q_NULLPTR));
        dir_label->setText(QApplication::translate("FTPClientClass", "\345\275\223\345\211\215\347\233\256\345\275\225:", Q_NULLPTR));
        RefreshDirButton->setText(QApplication::translate("FTPClientClass", "\345\210\267\346\226\260", Q_NULLPTR));
        BackButton->setText(QApplication::translate("FTPClientClass", "\350\277\224\345\233\236", Q_NULLPTR));
        label_4->setText(QApplication::translate("FTPClientClass", "FTP\346\234\215\345\212\241\345\231\250\346\226\207\344\273\266\346\265\217\350\247\210", Q_NULLPTR));
        download_button->setText(QApplication::translate("FTPClientClass", "\344\270\213\350\275\275", Q_NULLPTR));
        label_3->setText(QApplication::translate("FTPClientClass", "\346\234\254\346\234\272\347\233\256\345\275\225\346\265\217\350\247\210", Q_NULLPTR));
        upload_button->setText(QApplication::translate("FTPClientClass", "\344\270\212\344\274\240", Q_NULLPTR));
        Tab->setTabText(Tab->indexOf(MsgTab), QApplication::translate("FTPClientClass", "\345\223\215\345\272\224\344\277\241\346\201\257", Q_NULLPTR));
        Tab->setTabText(Tab->indexOf(ListTab), QApplication::translate("FTPClientClass", "\344\270\212\344\274\240/\344\270\213\350\274\211\345\210\227\350\241\250", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FTPClientClass: public Ui_FTPClientClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FTPCLIENT_H
