#include <QtCore>
#include <QtGui>
#include <QtQml>
#include <QtGlobal>
#include <qobjectdefs.h>
#include "task.h"

#ifndef TASKLISTMODEL_H
#define TASKLISTMODEL_H
/*
class MyTask; // moja implementacija za Task klasu koju cu napraviti u drugom fajlu,
// ako se bude zvala Task takodje, samo cu promeniti u Task i obrisati typedef ispod
typedef MyTask Task;*/

class TaskListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    public:
        enum RoleNames {
            TitleRole = Qt::UserRole,
            DescriptionRole = Qt::UserRole+1,
            StartDateRole = Qt::UserRole+2,
            EstimationRole = Qt::UserRole+3,
            StatusRole = Qt::UserRole+4
        };
    
    explicit TaskListModel(QObject *parent = 0);
    ~TaskListModel() override;

    Q_INVOKABLE void insert(int index, const QVariantMap &task);
    //Q_INVOKABLE void append(const Task &task);
    Q_INVOKABLE void append(const QVariantMap &task);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void update(int index, const QVariantMap &task);
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void clear();
    Q_INVOKABLE Task get(int index);
    
    public: //QAbstractItemModel interface
        virtual int rowCount(const QModelIndex &parent) const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;
        int count() const;
        void setData(QList<Task> t) { m_data = t; }
        virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    signals:
        void countChanged(int arg);
    
    protected: //interface QAbstractListModel
        virtual QHash<int, QByteArray> roleNames() const override;
    private:
        QList<Task> m_data;
        QHash<int, QByteArray> m_roleNames;
        int m_count;
};


#endif // TASKLISTMODEL_H