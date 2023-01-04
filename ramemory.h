#ifndef RAMEMORY_H
#define RAMEMORY_H

#include <QWidget>
#include <QPushButton>
#include <QTimer>
#include <QVector>

#include "stringbits.h"
#include "vector.h"

namespace Ui {
class RAMemory;
}

class RAMemory : public QWidget
{
    Q_OBJECT

signals:
    // данные там всегда есть, поэтому указы-ся какие именно
    void statusData(bool* tag, const QString& data, QObject*);

    // отправка данных исходя из запроса
    void sendData(const QStringList& data, QObject* sender);
    void sendString(const QString& data, QObject* sender);
    void sendStrToEditer(const QString& data);

    // изменение строки
    void updateString(bool* tag, const QString& data);

    //
    void updateAllData();

public slots:
    void getData(bool* tag); // запрос данных по тегу

    // запрос данных по этим строкам
    void getSrings(const QVector <bool*>& addresses);
    void getString(bool* address);

public:
    RAMemory(QPushButton* pButton,
                      vector* pOutVector,
                      QObject* editerString,
                      QWidget *parent = nullptr);

    static QString randData(int size = 10);

    ~RAMemory();

private slots:
    void output();
    void setAllRandomData();
    void editString();
    void setValueLineEdit(int row);
    void removeString(int id, const QString& strData = randData());

private:
    vector* pOutVector;   // стрелка выхода инф.
    int idOutData; // индекс данных для контроллера

    QTimer* loadTimer;
    const int timeLoading = 1500; // в мсек

    QVector <QString> data;
    QVector <bool*> addresses;

    enum Column{Data, Tag};

    QObject* senderContr;
    QObject* senderL;

    QObject* senderEditer;

    Ui::RAMemory *ui;
};

#endif // RAMEMORY_H
