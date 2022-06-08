#include <optional>
#include <user.h>
#include <task.h>
#include <QJsonDocument>

#ifndef TASKSTRATEGY_H
#define TASKSTRATEGY_H
class TaskStrategy {
    public:
        virtual std::optional<User> 
        executeRead(const QString userName, const QString password) 
            { return std::nullopt; };
        virtual bool executeWrite(const User &user) { return false;};
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

#endif // TASKSTRATEGY_H