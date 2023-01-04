#include "controllermemory.h"
#include "ui_controllermemory.h"

#include <QDebug>

#include "stringbits.h"

controllerMemory::controllerMemory(const QVector <vector*>& outVectors_, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controllerMemory)
{
    ui->setupUi(this);
    pI = this;

    outVectors = outVectors_;

    loadTimer = new QTimer;
    connect(
            loadTimer, SIGNAL(timeout()),
            this,      SLOT(output())
            );

    this->setWindowTitle("Контроллер кеш памяти");
}

void controllerMemory::setDataFromCore(bool* tag_, int typeData)
{
    tag = tag_;
    if (typeData)
    {
        typeCurrentData = data;
        toSignal(L1data);
    }
    else
    {
        typeCurrentData = instr;
        toSignal(L1instr);
    }
}

void controllerMemory::setStatusFromL1data(bool status)
{
    if (status) toSignal(ALD);
    else toSignal(L2);
}

void controllerMemory::setStatusFromL1instr(bool status)
{
    if (status) toSignal(ALD);
    else toSignal(L2);
}

void controllerMemory::setStatusFromL2(bool status)
{
    if (status) toSignal(ALD);
    else toSignal(L3);
}

void controllerMemory::setStatusFromL3(bool status, QObject* pObj)
{
    // если данные адресованы именно этому котроллеру:
    if (pI == pObj)
    {
        if (status) toSignal(ALD);
        else toSignal(RAM);
    }
}

void controllerMemory::setDataFromRAM(bool* tag_, const QString& strData_,
                                      QObject* pObj)
{
    tag = tag_;
    strData = strData_;

    outVectors.at(ALD)->pauseON(1000, true);

    if (pI == pObj)
    {
         //toSignal(ALD);

         switch (typeCurrentData)
         {
            case data:
                toSignal(L1data, set);
                break;

            case instr:
                toSignal(L1instr, set);
                break;
        }
    }
}

void controllerMemory::toSignal(device nameDevice, typeSignal tSignal)
{
    receiver = nameDevice;
    typeCurrentSignal = tSignal;

    loadTimer->start(timeLoading);
}

void controllerMemory::setDataFromL1data(bool* tag_, const QString& strData_)
{
    bool* copyTag = new bool[stringBits::numBits];
    for (int i=0; i<stringBits::numBits; i++) copyTag[i] = tag_[i];

    tag = copyTag;
    strData = strData_;

    toSignal(L2, set);
}

void controllerMemory::setDataFromL1instr(bool* tag_, const QString& strData_)
{
    bool* copyTag = new bool[stringBits::numBits];
    for (int i=0; i<stringBits::numBits; i++) copyTag[i] = tag_[i];

    tag = copyTag;
    strData = strData_;

    toSignal(L2, set);
}

void controllerMemory::setDataFromL2(bool* tag_, const QString& strData_)
{
    bool* copyTag = new bool[stringBits::numBits];
    for (int i=0; i<stringBits::numBits; i++) copyTag[i] = tag_[i];

    tag = copyTag;
    strData = strData_;

    toSignal(L3, set);
}

void controllerMemory::output()
{
    loadTimer->stop();

    switch (receiver)
    {
    case ALD:
        emit sendALD(tag);
        break;

    case L1data:
        switch (typeCurrentSignal)
        {
            case set:
                emit setDataL1data(tag, strData);
                break;
            case get:
                emit sendL1data(tag);
                break;
        }
        break;

    case L1instr:
        switch (typeCurrentSignal)
        {
            case set:
                emit setDataL1instr(tag, strData);
                break;
            case get:
                emit sendL1instr(tag);
                break;
        }
        break;

    case L2:
        switch (typeCurrentSignal)
        {
            case set:
                emit setDataL2(tag, strData);
                break;
            case get:
                emit sendL2(tag);
                break;
        }
        break;

    case L3:
        switch (typeCurrentSignal)
        {
            case set:
                emit setDataL3(tag, strData);
                break;
            case get:
                emit sendL3(tag);
                break;
        }
        break;

    case RAM:
        emit sendRAM(tag);
        break;
    }
    outVectors.at(receiver)->on(true);
}

controllerMemory::~controllerMemory()
{
    delete ui;
}
