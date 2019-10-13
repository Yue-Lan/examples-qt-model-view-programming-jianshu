#include "mainwindow.h"
#include <QApplication>

#include <QFileSystemModel>
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QHeaderView>

#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFileSystemModel m;
    m.setRootPath("/");//to use the QFileSystemModel we have to setRootPath() first.
    m.setReadOnly(false);//editable

    QListView v1;
    v1.setViewMode(QListView::IconMode);//icon view
    v1.setGridSize(QSize(100, 100));
    v1.setIconSize(QSize(64, 64));
    v1.setResizeMode(QListView::Adjust);//automaticly layout when view resized
    v1.setEditTriggers(QListView::DoubleClicked);//when double click, start an edition.
    v1.setSelectionMode(QListView::ExtendedSelection);//extend selection

    QTreeView v2;
    v2.setEditTriggers(QListView::DoubleClicked);
    v2.setSelectionMode(QTreeView::SingleSelection);//single selection
    v2.setSortingEnabled(true);//enable sorting

    v1.setModel(&m);
    v2.setModel(&m);

    QSplitter *p = new QSplitter(&w);
    p->addWidget(&v1);
    p->addWidget(&v2);

    w.setCentralWidget(p);

    v1.setRootIndex(m.index(QDir::currentPath()));
    v2.setRootIndex(m.index(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)));

    v2.header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);//ensure that we can see all content of file name.
    w.resize(800, 600);
    w.show();

    return a.exec();
}
