#include "cpu.h"

CPU::CPU(const QVector <vector*>& pDataVectors_,
         const QVector <QPushButton*>& buttonsOpenGui_,
         RAMemory* RAM,
         QComboBox* coresComboBox_,
         editString* editerString)
{
    coresComboBox = coresComboBox_;
    buttonOpenGui = buttonsOpenGui_.at(0);
    pDataVector = pDataVectors_.at(0);

    // создаем память L3
    L3 = new memoryLwidget("L3",
                           buttonsOpenGui_.last(),
                           pDataVectors_.at(1),
                           8,       // строк данных
                           RAM,
                           editerString,
                           false);  // есть ли кеш выше уровня чем этот
    L3->setWindowTitle("L3 (процессор)");
    connect(
            L3, SIGNAL(toShow(memoryLwidget*)),
            L3, SLOT(show())
            );

    // создаем ядра процессора
    coreCPUwidget* pNewCore;
    QString strNameNewCore;
    for (int i=0; i<numCores; i++)
    {
        strNameNewCore = "Ядро " + QString::number(i + 1);
        nameCoresList.append(strNameNewCore);

        QVector <vector*> coreVectors = pDataVectors_.mid(2);
        QVector <QPushButton*> coreButtons = buttonsOpenGui_.mid(1);

        pNewCore = new coreCPUwidget(strNameNewCore, coreVectors, coreButtons, RAM, editerString);

        cores.append(pNewCore);
        connect(
                pNewCore, SIGNAL(getDataSignal(bool*, int)),
                this,     SLOT(onVector())
                );
        connect(
                pNewCore, SIGNAL(toShowMemory(memoryLwidget*, coreCPUwidget*)),
                this,     SLOT(showMemory(memoryLwidget*, coreCPUwidget*))
                );

        connect(
                pNewCore->getController(), SIGNAL(sendL3(bool*)),
                L3,                        SLOT(getData(bool*))
                );
        connect(
                pNewCore->getController(), SIGNAL(setDataL3(bool*, const QString&)),
                L3,                        SLOT(setDataFromContr(bool*, const QString&))
                );

        connect(                           // этот сигнал соединен со всеми контроллерами
                L3,                        SIGNAL(statusData(bool, QObject*)),
                pNewCore->getController(), SLOT(setStatusFromL3(bool, QObject*))
                );

        connect(
                pNewCore->getController(), SIGNAL(sendRAM(bool*)),
                RAM,                       SLOT(getData(bool*))
                );
        connect(                           // этот сигнал соединен со всеми контроллерами
                RAM,                       SIGNAL(statusData(bool*, const QString&, QObject*)),
                pNewCore->getController(), SLOT(setDataFromRAM(bool*, const QString&, QObject*))
                );
        connect(
                RAM, SIGNAL(updateString(bool*, const QString&)),
                L3,  SLOT(updateString(bool*, const QString&))
                );
    }

    //
    coresComboBox->addItems(nameCoresList);
    enableCore = cores.at(0);
    connect(
            coresComboBox, SIGNAL(currentIndexChanged(int)),
            this,          SLOT(switchCore(int))
            );

    connect(
            buttonOpenGui, SIGNAL(clicked()),
            this,          SLOT(showCore())
            );
}

void CPU::showContrller()
{
    enableCore->getController()->show();
}

void CPU::showMemory(memoryLwidget* winMemory, coreCPUwidget* core)
{
    if (core == enableCore) winMemory->show();
}

void CPU::onVector()
{
    pDataVector->on(true);
}

coreCPUwidget* CPU::getCore(int numCore)
{
    return cores.at(numCore);
}

//смена ядра
void CPU::switchCore(int id)
{
    enableCore = cores[id];

    for (int i=0; i<cores.size(); i++)
    {
        cores[i]->setBlocked(true);
    }
    enableCore->setBlocked(false);
}

void CPU::showCore()
{
    enableCore->show();
}
