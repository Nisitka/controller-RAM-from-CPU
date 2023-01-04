#ifndef CORE_H
#define CORE_H

#include <QApplication>
#include <QObject>
#include <QPixmap>

#include "contmemorywidget.h"

// ядро всего приложения
class Core
{
public:
    Core();

    void start(); //запуск приложения

private:
    contMemoryWidget* contMemWin;
};

#endif // CORE_H
