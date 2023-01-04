#ifndef CONTMEMORYWIDGET_H
#define CONTMEMORYWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

#include "vector.h"
#include "corecpuwidget.h"
#include "cpu.h"
#include "ramemory.h"

#include "managerforms.h"

namespace Ui {
class contMemoryWidget;
}
/*  Класс, который осуществляет отображение всего
 * визуала
 */
class contMemoryWidget : public QWidget
{
    Q_OBJECT
public:
    static int lC(int rgb);

    // стрелочки
    enum nameVector{
        outALD, inALD,
        inL1data, outL1data,
        inL1instr, outL1instr,
        inL2, outL2,
        inL3, outL3,
        inRAM, outRAM
    };

    enum typeWidget{button, label};
    static void setDesineWidget(QWidget* widget, typeWidget, const QColor&);

public:
    explicit contMemoryWidget(QWidget *parent = nullptr);
    ~contMemoryWidget();

protected:
    void paintEvent(QPaintEvent* pEvent);

private:
    void setVizual();
    void setVectors();

    CPU* cpu;      // ЦП
    RAMemory* RAM; // ОП

    QVector <vector*> vectors;
    const int numVectors = 12;

    editString* editerString;  // редактор данных в кеш

    Ui::contMemoryWidget *ui;
};

#endif // CONTMEMORYWIDGET_H
