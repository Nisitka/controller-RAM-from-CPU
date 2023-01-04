#ifndef CORECPUWIDGET_H
#define CORECPUWIDGET_H

#include <QDebug>

#include <QWidget>
#include <QTimer>

#include "memorylwidget.h"
#include "controllermemory.h"
#include "vector.h"

#include <cmath>

/*
    Класс, характеризующий ядро
    процессора
*/
namespace Ui {
class coreCPUwidget;
}

class coreCPUwidget : public QWidget
{
    Q_OBJECT 
public slots:
    void showWinMemory(memoryLwidget*);

signals:
    void getDataSignal(bool* data, int typeData);

    // запрос на открытия окна КЕШ памяти у процессора
    void toShowMemory(memoryLwidget*, coreCPUwidget*);

public slots:
    void setBlocked(bool status = false);

public:
    explicit coreCPUwidget(const QString& name,
                           const QVector <vector*>& vectors,
                           const QVector <QPushButton*>& buttons,
                           RAMemory* RAM,
                           editString* editerString,
                           QWidget *parent = nullptr);

    controllerMemory* getController();

    ~coreCPUwidget();

private slots:
    void getData();
    void setRandData();
    void setOutDataSlider(int part);
    void setOutDataText(const QString& data);

private:
    void updateGUI();

    QStringList typesData = {"команда", "информация"};
    bool* pBitesOut;

    const int maxValueSlider = pow(2,  stringBits::numBits) - 1;

    memoryLwidget* L1data;
    memoryLwidget* L1instr;
    memoryLwidget* L2;

    controllerMemory* contMemory;

    Ui::coreCPUwidget *ui;
};

#endif // CORECPUWIDGET_H
