#include "mainSingleton.h"

MainSingleton* MainSingleton::instance = nullptr;

MainSingleton* MainSingleton::getInstance() 
{
    if (instance == nullptr) instance = new MainSingleton();
    return instance;
}

void MainSingleton::setModel(TaskListModel *m) 
{
    m_model = std::unique_ptr<TaskListModel>(m); 
    m->setParent(this); // this needs to be added so the model becomes a proper part of the hieararchy as a child of singleton
}


bool MainSingleton::loginInit(QString username, QString pass)
{
    // if (this->user = this->strategy->execute(username, pass)) return true;
    // gornja linija treba da bude rezultat uspesno implementiranog backenda
    if (this->user()->name() == username && this->user()->password() == pass) { // for now, to test frontend
        this->setModel(new TaskListModel());
        this->model()->setData(this->user()->tasks());
        connect(this->model(), &TaskListModel::dataChanged, this, &MainSingleton::updateUser);
        return true;
    }
    return false;
}

void MainSingleton::updateUser(QModelIndex index)
{
    int row = index.row();
    if (m_user->tasks().count() < m_model->count()) 
        m_user->insertTask(row, m_model->get(row));
    else if (m_user->tasks().count() > m_model->count())
        m_user->removeTask(row);
    else m_user->replaceTask(row, m_model->get(row));
}

/*QObject* MainSingleton::componentByName(QObject *parent, QString name)
{

}*/


/*QObject* MainSingleton::singletonTypeProvider(QQmlEngine *, QJSEngine *) 
{

}*/ //ovo mi izgleda nece trebati posto cu probati registerSingletonInstance