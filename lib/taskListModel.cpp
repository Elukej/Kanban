#include "taskListModel.h"

TaskListModel::TaskListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames[TitleRole] = "title";
    m_roleNames[DescriptionRole] = "description";
    m_roleNames[StartDateRole] = "startDate";
    m_roleNames[EstimationRole] = "estimation";
    m_roleNames[StatusRole] = "status";
}

TaskListModel::~TaskListModel() {}


void TaskListModel::insert(int index, const QVariantMap &task) 
{
    if (index < 0 || index > m_data.count()) return;

    Task tempTask;
    tempTask.setTitle(task["title"].toString());
    tempTask.setDescription(task["description"].toString());
    tempTask.setEstimation(task["estimation"].toDate());
    tempTask.setStartDate(task["startDate"].toDate());
    tempTask.setStatus(task["status"].toString());

    // if (!task.isValid()) return;

    beginInsertRows(QModelIndex(), index, index);
    m_data.insert(index, tempTask);
    endInsertRows();
    
    emit countChanged(m_data.count());
    auto qindex = QAbstractItemModel::createIndex(index,index, this);
    emit dataChanged(qindex, qindex,
              { Qt::UserRole, Qt::UserRole+1, Qt::UserRole+2, Qt::UserRole+3, Qt::UserRole+4});
    //the way i use this signal here is a bit of a fluke, but it serves purpose since i dont move stuff inside the model, just edit and append
}


void TaskListModel::append(const QVariantMap &task)
{
    insert(count(), task);
}


void TaskListModel::update(int index, const QVariantMap &task)
{
    if (index < 0 || index > m_data.count()) return;

    m_data[index].setTitle(task["title"].toString());
    m_data[index].setDescription(task["description"].toString());
    m_data[index].setStartDate(task["startDate"].toDate());
    m_data[index].setEstimation(task["estimation"].toDate());
    m_data[index].setStatus(task["status"].toString());

    auto qindex = QAbstractItemModel::createIndex(index,index, this);
    emit dataChanged(qindex, qindex,
              { Qt::UserRole, Qt::UserRole+1, Qt::UserRole+2, Qt::UserRole+3, Qt::UserRole+4});
}


void TaskListModel::move(int from, int to)
{
    if (from < 0 || to < 0 || from > m_data.count() || to > m_data.count()) 
        return;
    
    beginMoveRows(QModelIndex(), from, from, QModelIndex(), to + 1);
    m_data.move(from, to);
    endMoveRows();
}

void TaskListModel::remove(int index)
{
    if (index < 0 || index > m_data.count()) return;

    beginRemoveRows(QModelIndex(), index, index);
    m_data.removeAt(index);
    endRemoveRows();
    emit countChanged(m_data.count());
}

void TaskListModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}


Task TaskListModel::get(int index)
{
    if (index < 0 || index >= m_data.count()) {
        return Task();
    }
    return m_data.at(index);
}


int TaskListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

QVariant TaskListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_data.count()) {
        return QVariant();
    }

    const Task &task = m_data.at(row);

    switch(role) {
        case TitleRole:
            return task.title();
        case DescriptionRole:
            return task.description();
        case EstimationRole:
            return task.estimation();
        case StartDateRole:
            return task.startDate();
        case StatusRole:
            return task.status();
    }
    return QVariant();
}

bool TaskListModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
    int row = index.row();
    if (row < 0 || row > m_data.count()) {
        return false;
    }

    Task &task = m_data[row];

    switch(role) {
        case TitleRole: {
            task.setTitle(value.toString());
            emit dataChanged(index, index, {Qt::UserRole});
            return true;
        };
        case DescriptionRole: {
            task.setDescription(value.toString());
            emit dataChanged(index, index, {Qt::UserRole+1});
            return true;
        };
        case StartDateRole: {
            task.setStartDate(value.toDate());
            emit dataChanged(index, index, {Qt::UserRole+2});
            return true;
        };
        case EstimationRole: {
            task.setStartDate(value.toDate());
            emit dataChanged(index, index, {Qt::UserRole+3});
            return true;
        };
        case StatusRole: {
            task.setStatus(value.toString());
            emit dataChanged(index, index, {Qt::UserRole+4});
            return true;
        };
    }
    return false;
} 


int TaskListModel::count() const
{
    return rowCount(QModelIndex());
}

QHash<int, QByteArray> TaskListModel::roleNames() const
{
    return m_roleNames;
}