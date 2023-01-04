#ifndef CONTROLLERMEMORY_H
#define CONTROLLERMEMORY_H

#include <QWidget>
#include <QVector>

#include <QTimer>

#include "vector.h"

namespace Ui {
class controllerMemory;
}

// контроллер памяти в ядре
class controllerMemory : public QWidget
{
    Q_OBJECT

signals:
    void sendL1data(bool*);
    void sendL1instr(bool*);
    void sendL2(bool*);
    void sendL3(bool*);
    void sendRAM(bool*);
    void sendALD(bool*);

    void setDataL1data(bool* tag, const QString& dataStr);
    void setDataL1instr(bool* tag, const QString& dataStr);
    void setDataL2(bool* tag, const QString& dataStr);
    void setDataL3(bool* tag, const QString& dataStr);

public slots:
    void setDataFromCore(bool*, int);

    // принятие ответа об статусе данных
    void setStatusFromL1data(bool status);
    void setStatusFromL1instr(bool status);
    void setStatusFromL2(bool status);
    void setStatusFromL3(bool status, QObject* pObj);

    // там всегда имееются данные
    void setDataFromRAM(bool* tag,
                        const QString& data,
                        QObject* sender);

    // принятие данных для записи в кеш на ур. выше
    void setDataFromL1data(bool* tag, const QString& dataStr);
    void setDataFromL1instr(bool* tag, const QString& dataStr);
    void setDataFromL2(bool* tag, const QString& dataStr);

public:
    explicit controllerMemory(const QVector <vector*>& vectors, QWidget *parent = nullptr);

    enum device {ALD, L1data, L1instr, L2, L3, RAM};

    ~controllerMemory();

private slots:
    void output();

private:
    enum typeSignal {set, get};
    typeSignal typeCurrentSignal;

    enum typeData {data, instr};
    typeData typeCurrentData;

    // с кем и как взаимодействовать
    void toSignal(device, typeSignal = get);

    bool* tag;
    device receiver; // кому будут адресованы данные (запрос)
    QString strData;

    QVector <vector*> outVectors;

    QTimer* loadTimer;
    const int timeLoading = 1500; // в мсек

    QObject* pI;
    Ui::controllerMemory *ui;
};

#endif // CONTROLLERMEMORY_H
