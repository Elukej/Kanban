#include "taskStrategy.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <optional>
#include <qdatetime.h>
#include <qdebug.h>
#include <qglobal.h>
#include <qjsonarray.h>
#include <qjsonobject.h>

User* LocalSQLStrategy::user = nullptr; // static class members always have to be declared somewhere outside of the class that has it

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


LocalSQLStrategy::LocalSQLStrategy(QString fileName)
{
    int rc;
    rc = sqlite3_open("../kanban.db", &db);
    if (rc) {
        qDebug() << "Cant open the database! " << sqlite3_errmsg(db);
        sqlite3_close(db);
        return;
    }
    qDebug() << "I opened the database!";
}

LocalSQLStrategy::~LocalSQLStrategy()
{
    sqlite3_close(db);
    if (user != NULL) delete user;
}



int LocalSQLStrategy::user_callback(void *NotUsed, int numCols, char **colContent, char **colName)
{
    LocalSQLStrategy::user = new User();
    for (int i = 0; i < numCols; i++) {
        if (!strcmp(colName[i], "name")) LocalSQLStrategy::user->setName(QString(colContent[i]));
        else if (!strcmp(colName[i], "password")) LocalSQLStrategy::user->setPassword(QString(colContent[i]));
        else if (!strcmp(colName[i], "uuid")) LocalSQLStrategy::user->setUuid(std::stoi(colContent[i]));
        else if (!strcmp(colName[i], "isAdmin")) LocalSQLStrategy::user->setIsAdmin(colContent[i]);
    }
    return 0;
}


int LocalSQLStrategy::task_callback(void *NotUsed, int numCols, char **colContent, char **colName)
{
    Task task;
    for (int i = 0; i < numCols; i++) {
        //qDebug() << colContent[i];
        if (!strcmp(colName[i], "title")) task.setTitle(QString(colContent[i]));
        else if (!strcmp(colName[i], "description")) task.setDescription(QString(colContent[i]));
        else if (!strcmp(colName[i], "startDate")) task.setStartDate(QDate::fromString(colContent[i], "dd.MM.yyyy"));
        else if (!strcmp(colName[i], "estimation")) task.setEstimation(QDate::fromString(colContent[i], "dd.MM.yyyy"));
        else if (!strcmp(colName[i], "status")) task.setStatus(QString(colContent[i]));
        else if (!strcmp(colName[i], "taskId")) task.setTaskId(std::stoi(colContent[i]));
    }
    LocalSQLStrategy::user->appendTask(task);
    return 0;
}

std::optional<User> LocalSQLStrategy::executeRead(const QString userName, const QString password)
{
    char sql_querry[1000];
    sprintf(sql_querry, "SELECT * FROM user "
                        "WHERE user.name = '%s' AND user.password = '%s';", 
                        userName.toStdString().c_str(), password.toStdString().c_str());
    
    int rc;
    rc = sqlite3_exec(db, sql_querry, &LocalSQLStrategy::user_callback, 0, 0);
    
    if (LocalSQLStrategy::user == NULL) return std::nullopt;

    sprintf(sql_querry, "SELECT task.* FROM user "
                        "LEFT JOIN user_task ON user.uuid = user_task.userId "
                        "LEFT JOIN task ON task.taskId = user_task.taskId "
                        "WHERE user.name = '%s';", 
                        userName.toStdString().c_str());
    rc = sqlite3_exec(db, sql_querry, &LocalSQLStrategy::task_callback, 0, 0);
    if (!rc) return *user;
    else return std::nullopt;
}

bool LocalSQLStrategy::executeWrite(const User &newUser) 
{ 
    // this function will take advantage of the fact that tasks cant be erased inside the application and that they can only be appended to the end
    char sql_querry[1000]; int rc;
    for (int i = 0; i < newUser.tasks().size(); i++) {
        if (i >= user->tasks().size()) {
            qDebug() << "Im in insert part!";
            const Task &task = newUser.tasks().at(i);           
            sprintf(sql_querry, "INSERT INTO task (title, description, startDate, estimation, status) "
                                "VALUES ('%s', '%s', '%s', '%s', '%s');",
                                task.title().toStdString().c_str(), 
                                task.description().toStdString().c_str(),
                                task.startDate().toString("dd.MM.yyyy").toStdString().c_str(),
                                task.estimation().toString("dd.MM.yyyy").toStdString().c_str(),
                                task.status().toStdString().c_str());
            rc = sqlite3_exec(db, sql_querry, NULL, 0,0);           
            if (rc) return false;
            qDebug() << "I inserted into user!";
            sprintf(sql_querry, "INSERT INTO user_task (userId, taskId) "
                                "VALUES ((SELECT uuid FROM user WHERE name = '%s'), "
                                "(SELECT taskId FROM task WHERE title = '%s'));",
                                newUser.name().toStdString().c_str(),
                                task.title().toStdString().c_str());
            rc = sqlite3_exec(db, sql_querry, NULL, 0,0);            
            if (rc) return false;
            qDebug() << "I inserted into user_task!";
        }
        if (user->tasks().at(i) == newUser.tasks().at(i)) continue;
        else {
            const Task &task = newUser.tasks().at(i); 
            sprintf(sql_querry, "UPDATE task "
                                "SET title = '%s', description = '%s', " 
                                "startDate = '%s', estimation = '%s', status = '%s' "
                                "WHERE taskId = '%d';",
                                task.title().toStdString().c_str(), 
                                task.description().toStdString().c_str(),
                                task.startDate().toString("dd.MM.yyyy").toStdString().c_str(),
                                task.estimation().toString("dd.MM.yyyy").toStdString().c_str(),
                                task.status().toStdString().c_str(), 
                                task.taskId());
            rc = sqlite3_exec(db, sql_querry, NULL, 0,0);
            if (rc) return false;
        }
    }
    return true;
}