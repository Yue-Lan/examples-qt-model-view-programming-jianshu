#include "mainwindow.h"
#include <QApplication>

#include "trashfileslistmodel1.h"
#include <QListView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    TrashFilesListModel1 m;

    QListView v;
    v.setModel(&m);

    w.setCentralWidget(&v);
    w.show();

    return a.exec();
}
