#ifndef TRASHFILESLISTMODEL1_H
#define TRASHFILESLISTMODEL1_H

#include <QAbstractListModel>
#include <gio/gio.h>

class TrashVFSFileItem;

class TrashFilesListModel1 : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TrashFilesListModel1(QObject *parent = nullptr);
    ~TrashFilesListModel1() override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

Q_SIGNALS:
    void itemAdded(const QString &uri);
    void itemRemoved(const QString &uri);

private:
    static GCallback direcoty_changed_cb(GFileMonitor     *monitor,
                                         GFile            *file,
                                         GFile            *other_file,
                                         GFileMonitorEvent event_type,
                                         gpointer          user_data);

    GFileMonitor *trash_monitor = nullptr;
    gulong signal_handler = 0;

    QList<TrashVFSFileItem*> m_children;
};

#endif // TRASHFILESLISTMODEL1_H
