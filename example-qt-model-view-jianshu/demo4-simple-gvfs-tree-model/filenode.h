#ifndef FILENODE_H
#define FILENODE_H

#include <QObject>
#include <QIcon>

class SimpleGVfsTreeModel;
class FileInfo
{
public:
    FileInfo(const QString &uri);

    QString uri;
    QString displayName;
    QString mimeType;
    QString size;
    QString modifiedDate;
    QIcon icon;

    bool hasChildren;
};

class FileNode : public QObject
{
    friend class SimpleGVfsTreeModel;

    Q_OBJECT
public:
    explicit FileNode(const QString &uri, FileNode *parentNode, SimpleGVfsTreeModel *model, QObject *parent = nullptr);
    ~FileNode() override;

    FileNode *getParentNode() const {return m_parent_node;}

    //model
    QModelIndex firstColumnIndex();

    bool hasChildren();
    bool canFetchMore();
    void fetchMore();

private:
    SimpleGVfsTreeModel *m_model;

    FileInfo *m_info;
    FileNode *m_parent_node;
    QList<FileNode *> m_children;
};

#endif // FILENODE_H
