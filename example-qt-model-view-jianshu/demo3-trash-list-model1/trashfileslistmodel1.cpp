#include "trashfileslistmodel1.h"
#include <gio/gio.h>
#include <QDebug>

#include "trashvfsfileitem.h"

const QStringList enumerateTrash()
{
    QStringList uris;
    GFile *gfile = g_file_new_for_uri("trash:///");
    GFileEnumerator *g_enumerator = g_file_enumerate_children(gfile,
                                                              G_FILE_ATTRIBUTE_STANDARD_NAME,
                                                              G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,
                                                              nullptr,
                                                              nullptr);
    GFileInfo *ginfo = g_file_enumerator_next_file(g_enumerator,
                                                   nullptr,
                                                   nullptr);
    while (ginfo) {
        GFile *file = g_file_enumerator_get_child(g_enumerator, ginfo);
        char *uri = g_file_get_uri(file);
        uris<<uri; //add file uri to list.
        g_free(uri);
        g_object_unref(file);
        g_object_unref(ginfo);

        ginfo = g_file_enumerator_next_file(g_enumerator,
                                            nullptr,
                                            nullptr); //next file
    }

    return uris;
}

TrashFilesListModel1::TrashFilesListModel1(QObject *parent)
    : QAbstractListModel(parent)
{
    QStringList uris = enumerateTrash();
    for (auto uri : uris) {
        TrashVFSFileItem *item = new TrashVFSFileItem(uri);
        m_children.append(item);
    }

    GFile *trash = g_file_new_for_uri("trash:///");
    trash_monitor = g_file_monitor_directory(trash,
                                             G_FILE_MONITOR_SEND_MOVED,
                                             nullptr,
                                             nullptr);

    signal_handler = g_signal_connect(trash_monitor, "changed", GCallback(direcoty_changed_cb), this);

    connect(this, &TrashFilesListModel1::itemAdded, [=](const QString &uri){
        TrashVFSFileItem *newItem = new TrashVFSFileItem(uri);
        m_children.append(newItem);
        this->insertRow(m_children.indexOf(newItem), QModelIndex());//trigger the view change.
    });

    connect(this, &TrashFilesListModel1::itemRemoved, [=](const QString &uri){
        for (auto child : m_children) {
            if (child->getUri() == uri) {
                this->removeRow(m_children.indexOf(child), QModelIndex());//trigger the view change.
                return;
            }
        }
    });
}

TrashFilesListModel1::~TrashFilesListModel1()
{
    g_signal_handler_disconnect(trash_monitor, signal_handler);
    g_object_unref(trash_monitor);
    for (auto child: m_children) {
        delete child;
    }
}

GCallback TrashFilesListModel1::direcoty_changed_cb(GFileMonitor *monitor,
                                                    GFile *file,
                                                    GFile *other_file,
                                                    GFileMonitorEvent event_type,
                                                    gpointer user_data)
{
    TrashFilesListModel1 *p_this = static_cast<TrashFilesListModel1*>(user_data);
    char *changed_uri = g_file_get_uri(file);
    QString uri = changed_uri;
    g_free(changed_uri);
    switch (event_type) {
    case G_FILE_MONITOR_EVENT_CREATED:
        Q_EMIT p_this->itemAdded(uri);
        break;
    case G_FILE_MONITOR_EVENT_DELETED:
        Q_EMIT p_this->itemRemoved(uri);
        break;
    default:
        break;
    }
}

int TrashFilesListModel1::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_children.count();//return the list count
}

QVariant TrashFilesListModel1::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    switch (role) {
    case Qt::DecorationRole:
        return m_children.at(index.row())->getIcon();
    case Qt::DisplayRole:
        return m_children.at(index.row())->getDisplayName();
    case Qt::ToolTipRole:
        return m_children.at(index.row())->getDisplayName();
    default:
        break;
    }
    return QVariant();
}

bool TrashFilesListModel1::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endInsertRows();
    return true;
}

bool TrashFilesListModel1::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    auto deletedItem = m_children.takeAt(row);
    deletedItem->deleteLater();
    endRemoveRows();
    return true;
}
