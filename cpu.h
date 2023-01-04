#ifndef CPU_H
#define CPU_H

#include <QObject>

#include <QPushButton>
#include <QComboBox>

#include "corecpuwidget.h"
#include "ramemory.h"

//процессор
class CPU : public QObject
{
    Q_OBJECT
signals:
    //void getDataMemory(bool* data, int tData, const QString& nameCore);

public slots:
    void switchCore(int id);
    void onVector();

    void showContrller();

    //открыть окно памяти (какой памяти, какого ядра)
    void showMemory(memoryLwidget*, coreCPUwidget*);

public:
    CPU(const QVector <vector*>& pDataVectors,        //cpu, L1data, L1instr, L2, L3
        const QVector <QPushButton*>& buttonsOpenGui, //cpu, L1data, L1instr, L2, L3
        RAMemory* RAM,
        QComboBox* coresComboBox,
        editString* editerString);

    coreCPUwidget* getCore(int numCore);

private slots:
    void showCore();

private:
    coreCPUwidget* enableCore;

    QVector <coreCPUwidget*> cores;
    int numCores = 4;
    QStringList nameCoresList;

    memoryLwidget* L3;

    QComboBox* coresComboBox;
    QPushButton* buttonOpenGui;

    vector* pDataVector;
};

#endif // CPU_H
