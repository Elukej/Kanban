#include "taskStrategy.h"
#include <optional>
#include <qdebug.h>
#include <qglobal.h>
#include <qjsonarray.h>
#include <qjsonobject.h>

LocalJsonStrategy::LocalJsonStrategy(QString fileName)
{
    if (!LocalJsonStrategy::readDocFromFile(fileName))
        qDebug() << "Error reading from the file";
}

bool LocalJsonStrategy::readDocFromFile(QString fileName)
{
    QFile usersFile(fileName); //need to be very careful since this path depends on the place from which we start the program
    if (!usersFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray usersAsBytes = usersFile.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(usersAsBytes));
    if (doc.isEmpty()) return false;
    this->setDoc(doc);
    return true;
}

bool LocalJsonStrategy::writeDocToFile(QString fileName)
{
    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    saveFile.write(this->doc().toJson());
    return true;
}

std::optional<User> LocalJsonStrategy::
executeRead(const QString user, const QString pass)
{
    if (!(doc()["users"].isArray()) ) return std::nullopt;
    auto users = doc()["users"].toArray();

    for (auto it = users.begin(); it != users.end(); it++) {
        const QJsonObject &currUser = (*it).toObject();
        if ( currUser["name"] == user && currUser["password"] == pass) {
            if (!currUser.contains("uuid")) return std::nullopt;
            auto uuid = currUser["uuid"].toInt();

            if (!currUser.contains("isAdmin")) return std::nullopt;
            bool isAdmin = currUser["isAdmin"].toBool();
            
            if(!currUser.contains("tasks") || !currUser["tasks"].isArray()) 
                return std::nullopt;            
            auto userArr = currUser["tasks"].toArray();

            QList<Task> tasks;
            for (auto it2 = userArr.begin(); it2 != userArr.end(); it2++ ) {
                const QJsonObject &currTask = (*it2).toObject();
                Task task;
                if (!currTask.contains("title")) return std::nullopt;
                task.setTitle(currTask["title"].toString());

                if (!currTask.contains("description")) return std::nullopt;
                task.setDescription(currTask["description"].toString());

                if (!currTask.contains("startDate")) return std::nullopt;
                task.setStartDate(QDate::fromString(currTask["startDate"].toString(),"dd.MM.yyyy"));

                if (!currTask.contains("estimation")) return std::nullopt;
                task.setEstimation(QDate::fromString(currTask["estimation"].toString(),"dd.MM.yyyy"));

                if (!currTask.contains("status")) return std::nullopt;
                task.setStatus(currTask["status"].toString());

                tasks.append(task);
            }

            return User(user, pass, uuid, isAdmin, tasks); 
        }
    }
    return std::nullopt;
}

bool LocalJsonStrategy::executeWrite(const User &user)
{
    QJsonObject jsonUser;
    jsonUser["name"] = user.name();
    jsonUser["password"] = user.password();
    jsonUser["uuid"] = user.uuid();
    jsonUser["isAdmin"] = user.isAdmin();
    QJsonArray jsonTasks;
    
    /*for (auto it = user.tasks().begin(); it != user.tasks().end(); it++) {
        QJsonObject task;
        qDebug() << it->title() << Qt::endl;
        task["title"] =  (it->title());
                            qDebug() << "I STILL WORK!" << Qt::endl;
                        
        task["description"] = it->description();
        task["startDate"] = it->startDate().toString("dd.MM.yyyy");
        task["estimation"] = it->estimation().toString("dd.MM.yyyy");
        task["status"] = it->status();
        jsonTasks.push_back(task);
    }*/
   
    for (int i = 0; i < user.tasks().length(); i++) {
        QJsonObject task;
        task["title"] = user.tasks()[i].title(); 
        task["description"] = user.tasks()[i].description();
        task["startDate"] = user.tasks()[i].startDate().toString("dd.MM.yyyy");
        task["estimation"] = user.tasks()[i].estimation().toString("dd.MM.yyyy");
        task["status"] = user.tasks()[i].status();
        jsonTasks.push_back(task);
    }

    jsonUser["tasks"] = jsonTasks;

    auto docUsers = this->doc()["users"].toArray();
    for (auto it = docUsers.begin(); it != docUsers.end(); it++) 
        if (it->toObject()["name"] == user.name()) 
            *it = jsonUser;
   
    auto finalObj = this->doc().object(); //////////////////////////
    finalObj["users"] = docUsers;
    this->setDoc(QJsonDocument(finalObj));

    return writeDocToFile("../user_data.json");
}