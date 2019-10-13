#include "simplegvfstreemodel.h"
#include "filenode.h"

#include <QStandardPaths>
#include <QDebug>

SimpleGVfsTreeModel::SimpleGVfsTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_root_node = new FileNode("file://" + QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                               nullptr,
                               this);
    beginResetModel();
    m_root_node->fetchMore();
    endResetModel();
}

SimpleGVfsTreeModel::~SimpleGVfsTreeModel()
{
    delete m_root_node;
}

void SimpleGVfsTreeModel::setRootUri(const QString &uri)
{
    beginResetModel();
    delete m_root_node;
    m_root_node = new FileNode(uri, nullptr, this);
    m_root_node->fetchMore();
    endResetModel();
}

QModelIndex SimpleGVfsTreeModel::firstColumnIndex(FileNode *node)
{
    if (node->getParentNode()) {
        int row = node->getParentNode()->m_children.indexOf(node);
        return createIndex(row, 0, node);
    } else {
        int row = this->m_root_node->m_children.indexOf(node);
        return createIndex(row, 0, node);
    }
    return QModelIndex();
}

QVariant SimpleGVfsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (orientation == Qt::Vertical)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Size");
        case 2:
            return tr("File Type");
        case 3:
            return tr("Modified Date");
        default:
            break;
        }
    }
    return QVariant();
}

QModelIndex SimpleGVfsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    //qDebug()<<"index()"<<parent<<row;
    // FIXME: Implement me!
    if (!parent.isValid()) {
        return createIndex(row, column, m_root_node->m_children.at(row));
    } else {
        FileNode *parentNode = static_cast<FileNode*>(parent.internalPointer());
        return createIndex(row, column, parentNode->m_children.at(row));
    }
}

QModelIndex SimpleGVfsTreeModel::parent(const QModelIndex &index) const
{
    //qDebug()<<"parent()"<<index;
    // FIXME: Implement me!
    FileNode *childNode = static_cast<FileNode*>(index.internalPointer());
    if (childNode->getParentNode())
        return childNode->getParentNode()->firstColumnIndex();
    return QModelIndex();
}

int SimpleGVfsTreeModel::rowCount(const QModelIndex &parent) const
{
    //qDebug()<<"rowCount"<<parent;
    //qDebug()<<m_root_node->m_children.count();
    if (!parent.isValid())
        return m_root_node->m_children.count();

    // FIXME: Implement me!
    FileNode *parentNode = static_cast<FileNode*>(parent.internalPointer());
    return parentNode->m_children.count();
}

int SimpleGVfsTreeModel::columnCount(const QModelIndex &parent) const
{
    return 4;
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

bool SimpleGVfsTreeModel::hasChildren(const QModelIndex &parent) const
{
    //qDebug()<<"hasChildren"<<parent;
    if (!parent.isValid())
        return true;

    FileNode *parentNode = static_cast<FileNode*>(parent.internalPointer());
    //qDebug()<<parentNode->hasChildren();
    return parentNode->hasChildren();
    // FIXME: Implement me!
}

bool SimpleGVfsTreeModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_root_node->canFetchMore();

    FileNode *parentNode = static_cast<FileNode*>(parent.internalPointer());
    return parentNode->canFetchMore();
    // FIXME: Implement me!
    return false;
}

void SimpleGVfsTreeModel::fetchMore(const QModelIndex &parent)
{
    //qDebug()<<"fetch more";
    if (!parent.isValid())
        m_root_node->fetchMore();

    FileNode *parentNode = static_cast<FileNode*>(parent.internalPointer());
    if (parentNode) {
        parentNode->fetchMore();
    } else {
        m_root_node->fetchMore();
    }
    // FIXME: Implement me!
}

QVariant SimpleGVfsTreeModel::data(const QModelIndex &index, int role) const
{
    //qDebug()<<"data"<<index;
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    FileNode *node = static_cast<FileNode*>(index.internalPointer());
    //qDebug()<<index;

    switch (index.column()) {
    case 0: {
        switch (role) {
        case Qt::DecorationRole:
            return node->m_info->icon;
        case Qt::DisplayRole:
            return node->m_info->displayName;
        default:
            break;
        }
        break;
    }
    case 1: {
        if (role == Qt::DisplayRole) {
            return node->m_info->size;
        }
        break;
    }
    case 2: {
        if (role == Qt::DisplayRole) {
            return node->m_info->mimeType;
        }
        break;
    }
    case 3: {
        if (role == Qt::DisplayRole) {
            return node->m_info->modifiedDate;
        }
        break;
    }
    default:
        break;
    }
    return QVariant();
}
