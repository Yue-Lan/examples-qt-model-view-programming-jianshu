#include "mainwindow.h"
#include <QApplication>

#include "simplegvfstreemodel.h"
#include "sortfilterproxymodel.h"

#include <QTreeView>
#include <QHeaderView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    SimpleGVfsTreeModel m;
    SortFilterProxyModel p;
    p.setSourceModel(&m);

    QTreeView v;

    v.setSortingEnabled(true); //enable proxy model sort/filter;

    v.header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    v.setModel(&p);

    w.setCentralWidget(&v);
    w.show();

    return a.exec();
}
