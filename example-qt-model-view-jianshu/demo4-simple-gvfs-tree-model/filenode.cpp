#include "filenode.h"
#include "simplegvfstreemodel.h"

#include <gio/gio.h>
#include <QFileIconProvider>
#include <QDateTime>

#include <QDebug>

const QStringList enumerateDirectory(const QString &uri)
{
    QStringList uris;
    GFile *gfile = g_file_new_for_uri(uri.toUtf8().constData());
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

FileInfo::FileInfo(const QString &uri)
{
    this->uri = uri;
    GFile *gfile = g_file_new_for_uri(uri.toUtf8().constData());
    GFileInfo *ginfo = g_file_query_info(gfile,
                                         "standard::*, time::*",
                                         G_FILE_QUERY_INFO_NONE,
                                         nullptr,
                                         nullptr);
    const char *display_name = g_file_info_get_display_name(ginfo);
    this->displayName = display_name;

    goffset offset = g_file_info_get_size(ginfo);
    char *size_format = g_format_size(offset);
    size = size_format;
    g_free(size_format);

    const char *mime_type = g_file_info_get_content_type(ginfo);
    char *description = g_content_type_get_description(mime_type);
    this->mimeType = description;
    g_free(description);

    quint64 modified_date = g_file_info_get_attribute_uint64(ginfo, G_FILE_ATTRIBUTE_TIME_MODIFIED);
    qDebug()<<modified_date;
    QDateTime date = QDateTime::fromMSecsSinceEpoch(modified_date*1000);
    this->modifiedDate = date.toString();

    GFileType type = g_file_query_file_type(gfile, G_FILE_QUERY_INFO_NONE, nullptr);
    QFileIconProvider p;
    if (type == G_FILE_TYPE_DIRECTORY) {
        this->icon = p.icon(QFileIconProvider::Folder);
    } else {
        this->icon = p.icon(QFileIconProvider::File);
    }

    GFileEnumerator *test = g_file_enumerate_children(gfile,
                                                      G_FILE_ATTRIBUTE_STANDARD_NAME,
                                                      G_FILE_QUERY_INFO_NONE,
                                                      nullptr,
                                                      nullptr);
    if (test) {
        hasChildren = true;
        g_object_unref(test);
    }

    g_object_unref(ginfo);
    g_object_unref(gfile);
}

FileNode::FileNode(const QString &uri, FileNode *parentNode, SimpleGVfsTreeModel *model, QObject *parent) : QObject(parent)
{
    m_info = new FileInfo(uri);
    m_parent_node = parentNode;
    m_model = model;
}

FileNode::~FileNode()
{
    for (auto child : m_children) {
        delete child;
    }
    delete m_info;
}

QModelIndex FileNode::firstColumnIndex()
{
    return m_model->firstColumnIndex(this);
}

bool FileNode::hasChildren()
{
    return m_info->hasChildren;
}

bool FileNode::canFetchMore()
{
    //qDebug()<<"can fetch more";
    return m_children.isEmpty();
}

void FileNode::fetchMore()
{
    //qDebug()<<"fetchMore"<<m_info->uri;
    QStringList uris = enumerateDirectory(this->m_info->uri);
    //qDebug()<<uris;
    for (auto uri : uris) {
        FileNode *node = new FileNode(uri, this, m_model);
        m_children.append(node);
    }
    qDebug()<<"ok";
}
