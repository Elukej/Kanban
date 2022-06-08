#include <QtCore>
#include <memory>

#include <qglobal.h>
#include <qjsengine.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qqmlengine.h>
#include "user.h"
#include "taskListModel.h"
#include "taskStrategy.h"

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
        TaskStrategy* strategy() const { return m_strategy.get(); }
        void setStrategy(TaskStrategy *s) 
            { m_strategy = std::unique_ptr<TaskStrategy>(s);}
        
        Q_INVOKABLE bool loginInit(QString username, QString pass);
        Q_INVOKABLE bool logout(/*QString fileName*/);

    signals:
        void modelChanged();
    public slots:
        void updateUser(QModelIndex index);
    private:
        explicit MainSingleton(QObject *parent = nullptr): QObject(parent) {}
        static MainSingleton *instance;
        std::unique_ptr<User> m_user;
        std::unique_ptr<TaskListModel> m_model;
        std::unique_ptr<TaskStrategy> m_strategy;
};

#endif // MAINSINGLETON_H

