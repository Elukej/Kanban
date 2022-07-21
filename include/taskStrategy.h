#include <optional>
#include <user.h>
#include <task.h>
#include <QJsonDocument>
#include <sqlite3.h>

#ifndef TASKSTRATEGY_H
#define TASKSTRATEGY_H
class TaskStrategy {
    public:
        virtual std::optional<User> 
        executeRead(const QString userName, const QString password) = 0; 
        virtual bool executeWrite(const User &user) = 0; 
};

class LocalJsonStrategy : public TaskStrategy {

    public:
        std::optional<User> executeRead(const QString userName, const QString password) override;
        bool executeWrite(const User &user) override;

        LocalJsonStrategy(QString fileName);
        bool readDocFromFile(QString fileName);
        bool writeDocToFile(QString fileName);

        QJsonDocument doc() const { return m_doc; }
        void setDoc(QJsonDocument doc) { m_doc = doc; }
    private:
        QJsonDocument m_doc;
};

class LocalSQLStrategy : public TaskStrategy {
    public:
        std::optional<User> executeRead(const QString userName, const QString password) override;
        bool executeWrite(const User &user) override;
        static int user_callback(void *NotUsed, int numCols, char **colContent, char **colName);
        static int task_callback(void *NotUsed, int numCols, char **colContent, char **colName);
        LocalSQLStrategy(QString fileName);
        ~LocalSQLStrategy();
    private:
        sqlite3 *db;
        static User *user;
};

#endif // TASKSTRATEGY_H