#include <QtCore>
#include "task.h"

#ifndef USER_H
#define USER_H

class User {
    public:
        User(QString n, QString p, quint32 u, bool is, const QList<Task> &t)
            : m_name(n), m_password(p), m_uuid(u), m_isAdmin(is) {
                m_tasks.clear();
                m_tasks = t;
            }
        QString name() const { return m_name; }
        void setName(QString s) { m_name = s; }
        QString password() const { return m_password; }
        void setPassword(QString p) { m_password = p; }
        quint32 uuid() const { return m_uuid; } 
        void setUuid(quint32 num) { m_uuid = num; }
        bool isAdmin() const { return m_isAdmin; }
        QList<Task> tasks() const { return m_tasks; }
        void setTasks(QList<Task> &t) { m_tasks = t; }
        void appendTask(Task &t) { m_tasks.append(t); }
        void insertTask(int index, const Task &t) { m_tasks.insert(index, t); }
        void replaceTask(int index, const Task &t) { m_tasks.replace(index, t); }
        void removeTask(int index) { m_tasks.removeAt(index); }

    private:
        QString m_name;
        QString m_password;
        quint32 m_uuid;
        bool m_isAdmin;
        QList<Task> m_tasks;
};

#endif //USER_H