#include "sortfilterproxymodel.h"

#include "filenode.h"
#include "simplegvfstreemodel.h"

#include <QDebug>

SortFilterProxyModel::SortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

bool SortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    //qDebug()<<sourceParent;
    //return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);

    FileNode *node = static_cast<FileNode*>(sourceParent.internalPointer());
    if (node) {
        QString displayName = node->m_children.at(sourceRow)->m_info->displayName;
        return !displayName.startsWith(".");
    } else {
        SimpleGVfsTreeModel *model = static_cast<SimpleGVfsTreeModel*>(sourceModel());
        QString displayName = model->m_root_node->m_children.at(sourceRow)->m_info->displayName;
        return !displayName.startsWith(".");
    }
}

bool SortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    qDebug()<<"lessThan"<<source_left.column();
    FileNode *left = static_cast<FileNode*>(source_left.internalPointer());
    FileNode *right = static_cast<FileNode*>(source_right.internalPointer());
    switch (sortColumn()) {
    case 0: {
        if (!left->hasChildren() && !right->hasChildren()) {
            return left->m_info->displayName < right->m_info->displayName;
        }
        if (left->hasChildren() && right->hasChildren()) {
            return left->m_info->displayName < right->m_info->displayName;
        }
        bool isAscendingOrder = sortOrder()==Qt::AscendingOrder;
        if (left->hasChildren()) {
            return isAscendingOrder?true:false;
        }
        if (right->hasChildren()) {
            return isAscendingOrder?false:true;
        }
        return left->m_info->displayName < right->m_info->displayName;
    }
    default:
        return QSortFilterProxyModel::lessThan(source_left, source_right);
    }
}
