#include <QtCore>
#include <qdatetime.h>

#ifndef TASK_H
#define TASK_H

class Task {
    public:
        Task() {}
        Task(QString t, QString d, QDate sd, QDate e, QString s)
            : m_title(t), m_description(d), m_startDate(sd), 
              m_estimation(e), m_status(s) {}

        QString title() const { return m_title; }
        void setTitle(QString t) { m_title = t; }
        QString description() const { return m_description; }
        void setDescription(QString d) { m_description = d; }
        QDate startDate() const { return m_startDate; }
        void setStartDate(QDate d) { m_startDate = d; }
        QDate estimation() const { return m_estimation; }
        void setEstimation(QDate e) { m_estimation = e; }
        QString status() const { return m_status; }
        void setStatus(QString s) { m_status = s; }
    
    private:
        QString m_title;
        QString m_description;
        QDate m_startDate;
        QDate m_estimation;
        QString m_status; 
};
#endif //TASK_H