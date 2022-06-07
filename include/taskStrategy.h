#include <optional>
#include <qjsondocument.h>
#include <user.h>
#include <task.h>
#include <QJsonDocument>

class TaskStrategy {
    public:
        virtual std::optional<User> executeRead(const QString userName, const QString password) = 0;
        virtual void executeWrite(const User &user) = 0;
};

class LocalJsonStrategy : public TaskStrategy {

    public:
        virtual std::optional<User> executeRead(const QString userName, const QString password) override;
        virtual void executeWrite(const User &user) override;

        LocalJsonStrategy(QString fileName);
        bool readDocFromFile(QString fileName);
        bool writeDocToFile(QString fileName);

        QJsonDocument doc() const { return m_doc; }
        void setDoc(QJsonDocument doc) { m_doc = doc; }
    private:
        QJsonDocument m_doc;
};