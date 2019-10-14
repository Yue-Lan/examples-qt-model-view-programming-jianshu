#ifndef SIMPLEGVFSTREEMODEL_H
#define SIMPLEGVFSTREEMODEL_H

#include <QAbstractItemModel>

class FileNode;

class SimpleGVfsTreeModel : public QAbstractItemModel
{
    friend class SortFilterProxyModel;
    Q_OBJECT

public:
    explicit SimpleGVfsTreeModel(QObject *parent = nullptr);
    ~SimpleGVfsTreeModel() override;

    void setRootUri(const QString &uri);

    QModelIndex firstColumnIndex(FileNode *node);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    FileNode *m_root_node;
};

#endif // SIMPLEGVFSTREEMODEL_H
