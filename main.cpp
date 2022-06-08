#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <cstdlib>
#include <qdebug.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>

#include "mainSingleton.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    
/////////////////////////////////////////////////////////////////////// my code
    auto mainSingleton= MainSingleton::getInstance();
    QList<Task> tasks = { Task( QString("Main work"), 
                            QString("just some ordinary work"), 
                            QDate(2022,5,23),
                            QDate(2022,5,31), 
                            QString("In Progress") ),
                          Task( QString("Learn Qt"), 
                            QString("difficult and tedious task"), 
                            QDate(2022,4,11),
                            QDate(2080,4,22), 
                            QString("To Do") ),
                          Task( QString("Learn Qt more"), 
                            QString("more difficult and tedious task"), 
                            QDate(2022,4,11),
                            QDate(2080,4,22), 
                            QString("Blocked") ),
                          Task( QString("Become a wizard"), 
                            QString("more difficult and tedious task"), 
                            QDate(2024,4,11),
                            QDate(2060,4,22), 
                            QString("Done") ),
                          Task( QString("Heal the world"), 
                            QString("more difficult and tedious task"), 
                            QDate(2022,5,20),
                            QDate(2099,4,22), 
                            QString("To Do") ),
                          Task( QString("Wrestle with QML"), 
                            QString("Doing declarative daily duties diligently"), 
                            QDate(2022,4,20),
                            QDate(2022,6,28), 
                            QString("Blocked") )
                        };
    
    mainSingleton->setStrategy(new LocalJsonStrategy("../user_data.json"));
    //mainSingleton->setUser(new User(QString("Luka"), QString("bonbon"), quint32(1), true, tasks));

    /* qDebug() << mainSingleton->user()->name() << Qt::endl << 
               mainSingleton->user()->tasks()[2].description() << Qt::endl; */


 /*   QFile usersFile("../user_data.json"); //need to be very careful since this path depends on the place from which we start the program
    if (!usersFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return EXIT_FAILURE;
    }

    QByteArray usersAsBytes = usersFile.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(usersAsBytes));
    QJsonArray array = doc["users"].toArray();
    QJsonObject obj = array[1].toObject();
    qDebug() <<  obj["tasks"].toArray()[1].toObject()["title"].toString() << Qt::endl;*/

/////////////////////////////////////////////////////////////////////////////
    
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance("MainSingleton", 1, 0, "MainSingleton", mainSingleton);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    //QQuickStyle::setStyle("Material");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);



    return app.exec();
}
