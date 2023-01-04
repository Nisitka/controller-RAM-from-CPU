#include "core.h"

#include <QDebug>

Core::Core()
{
    contMemWin = new contMemoryWidget;
}

void Core::start()
{
    contMemWin->show();
}
