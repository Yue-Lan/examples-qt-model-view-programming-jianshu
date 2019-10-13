#ifndef TRASHVFSFILEITEM_H
#define TRASHVFSFILEITEM_H

#include <QObject>
#include <QIcon>

class TrashVFSFileItem : public QObject
{
    Q_OBJECT
public:
    explicit TrashVFSFileItem(const QString &uri, QObject *parent = nullptr);
    const QString getUri() {return m_uri;}
    const QString getDisplayName() {return m_display_name;}
    const QIcon getIcon() {return m_icon;}

private:
    QString m_uri;
    QIcon m_icon;
    QString m_display_name;
};

#endif // TRASHVFSFILEITEM_H
