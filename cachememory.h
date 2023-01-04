#ifndef CACHEMEMORY_H
#define CACHEMEMORY_H

#include <QObject>
#include <QVector>

#include "stringbits.h"

// Класс, харак-й память в компьютере
class CACHEmemory : public QObject
{
    Q_OBJECT
signals:
    void sendData(stringBits*);
    void notData();

public:
    CACHEmemory();

protected slots:
    //обработка запроса на поиск данных
    void getData(bool*);

protected:
    void setV(int volume);
    //размер памяти (кол-во ячеек)
    int volume;

    QVector <stringBits*> boxData;

private:
    const int rang = 32;

};

#endif // CACHEMEMORY_H
