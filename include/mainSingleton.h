#include <QtCore>
#include <memory>

#include <qglobal.h>
#include <qjsengine.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qqmlengine.h>
#include "user.h"
#include "taskListModel.h"

#ifndef MAINSINGLETON_H
#define MAINSINGLETON_H

class MainSingleton : public QObject { //singleton dizajn patern
        Q_OBJECT
        Q_PROPERTY(TaskListModel* model READ model CONSTANT) //constant ide zato sto se nece menjati u qmlu
    public:
        MainSingleton(MainSingleton&) = delete;
        ~MainSingleton() override{}
        static MainSingleton* getInstance();
        //static QObject *singletonTypeProvider(QQmlEngine *, QJSEngine *);
        User* user() const { return m_user.get(); }
        void setUser(User *u) { m_user = std::unique_ptr<User>(u); } 
        TaskListModel* model() const { return m_model.get(); }
        void setModel(TaskListModel *m);
        
        Q_INVOKABLE bool loginInit(QString username, QString pass);
        //Q_INVOKABLE QObject* componentByName(QObject *parent, QString name);

    signals:
        void modelChanged();
    public slots:
        void updateUser(QModelIndex index);
    private:
        explicit MainSingleton(QObject *parent = nullptr): QObject(parent) {}
        static MainSingleton *instance;
        std::unique_ptr<User> m_user;
        std::unique_ptr<TaskListModel> m_model;
};

#endif // MAINSINGLETON_H

