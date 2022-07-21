#include <QtCore>
#include "task.h"

#ifndef USER_H
#define USER_H

class User {
    public:
        User() {}
        User(QString n, QString p, quint32 u, bool is, const QList<Task> &t)
            : m_name(n), m_password(p), m_uuid(u), m_isAdmin(is) {
                m_tasks.clear();
                m_tasks = t;
            }
        QString name() const { return m_name; }
        void setName(QString s) { m_name = s; }
        QString password() const { return m_password; }
        void setPassword(QString p) { m_password = p; }
        int uuid() const { return m_uuid; } 
        void setUuid(int num) { m_uuid = num; }
        bool isAdmin() const { return m_isAdmin; }
        void setIsAdmin(bool val) { m_isAdmin = val; }
        const QList<Task> tasks() const { return m_tasks; }
        void setTasks(QList<Task> &t) { m_tasks = t; }
        void appendTask(Task &t) { m_tasks.append(t); }
        void insertTask(int index, const Task &t) { m_tasks.insert(index, t); }
        void replaceTask(int index, const Task &t) { m_tasks.replace(index, t); }
        void removeTask(int index) { m_tasks.removeAt(index); }

    private:
        QString m_name;
        QString m_password;
        int m_uuid;
        bool m_isAdmin;
        QList<Task> m_tasks;
};

#endif //USER_H