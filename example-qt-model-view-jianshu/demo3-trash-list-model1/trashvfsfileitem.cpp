#include "trashvfsfileitem.h"

#include <gio/gio.h>
#include <QFileIconProvider>

TrashVFSFileItem::TrashVFSFileItem(const QString &uri, QObject *parent) : QObject(parent)
{
    m_uri = uri;
    GFile *gfile = g_file_new_for_uri(uri.toUtf8().constData());
    GFileInfo *ginfo = g_file_query_info(gfile,
                                         G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME,
                                         G_FILE_QUERY_INFO_NONE,
                                         nullptr,
                                         nullptr);
    const char *display_name = g_file_info_get_display_name(ginfo);
    m_display_name = display_name;

    GFileType type = g_file_query_file_type(gfile, G_FILE_QUERY_INFO_NONE, nullptr);
    QFileIconProvider p;
    if (type == G_FILE_TYPE_DIRECTORY) {
        m_icon = p.icon(QFileIconProvider::Folder);
    } else {
        m_icon = p.icon(QFileIconProvider::File);
    }

    g_object_unref(ginfo);
    g_object_unref(gfile);
}
