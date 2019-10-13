#include "mainwindow.h"
#include <QApplication>

#include "simplegvfstreemodel.h"
#include <QTreeView>
#include <QHeaderView>

#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    SimpleGVfsTreeModel m;
    QTreeView v;
    v.header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    v.setModel(&m);

    w.setCentralWidget(&v);

    w.show();

    QTimer::singleShot(5000, [&](){
        m.setRootUri("file:///");
    });

    return a.exec();
}
