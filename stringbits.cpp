#include "stringbits.h"

#include <cmath>

#include <QDebug>

stringBits::stringBits(QTableWidgetItem* itemData_,
                       QTableWidgetItem* itemTag_,
                       QTableWidgetItem* pItemTime_)
{
    pItemData = itemData_;
    pItemTag = itemTag_;
    pItemTime = pItemTime_;

    // вероятность того, что время совпадет 1:3600
    QTime curTime = QTime::currentTime();
    timeUsed = QTime(curTime.hour(), rand()%curTime.minute(), rand()%curTime.second());

    pBits = new bool[numBits];
    setRandData();
    updateItems();
}

void stringBits::updateItems()
{
    QColor colorBack = toColor(pBits);

    pItemTag->setText(toString(pBits));

    pItemTag->setBackground(QBrush(colorBack));
    pItemTag->setTextColor(colorText(colorBack));

    pItemTime->setText(timeUsed.toString());
}

QColor stringBits::colorText(const QColor& colorBackgraund)
{
    int sumRGB = colorBackgraund.red() +
                 colorBackgraund.green() +
                 colorBackgraund.blue();

    // если цвет темнее опр. уровня то изменить цвет текста
    if (sumRGB < 255*1.3) return Qt::white;
    else return Qt::black;
}

//заполнение строки битов случайными значениями
void stringBits::setRandData()
{
    for (int i=0; i<numBits; i++) pBits[i] = rand()%100 >= 50;
}

const QString stringBits::toString(bool *bites)
{
    QString outStr;
    for (int i=0; i<numBits; i++)
    {
        if (bites[i])
        {
            outStr.append('1');
        }
        else
        {
            outStr.append('0');
        }
    }
    return outStr;
}

bool* stringBits::toBites(const QString& charsBite)
{
    int numBites = charsBite.size();
    bool* Bites = new bool[numBites];

    for (int i=0; i<numBits; i++)
    {
        if (charsBite.at(i) == '1') Bites[i] = true;
        else Bites[i] = false;
    }

    return Bites;
}

// перевод в 2-ю СИ
bool* stringBits::toBites(long int value)
{
    bool* out = new bool[numBits];
    long int d = pow(2, numBits - 1);

    for (int i=0; i<numBits; i++)
    {
        if (value - d >= 0)
        {
            value -= d;
            out[i] = true;
        }
        else
        {
            out[i] = false;
        }
        d = (d+1)/2;
    }

    return out;
}
// говно код
void stringBits::setData(const QString &data_)
{
    data = data_;
    pItemData->setText(data_);
}

QString stringBits::getData()
{
    return data;
}

// использовать данные
void stringBits::usedData()
{
   timeUsed = QTime::currentTime();
   pItemTime->setText(timeUsed.toString());
}

QTime stringBits::getTimeUsed()
{
    return timeUsed;
}

long int stringBits::toInt(bool* bits, int rang)
{
    long int number = 0;

    for (int i=0; i<rang; i++)
    {
        if (bits[rang-1-i]) number += pow(2, i);
    }

    return number;
}

void stringBits::setTag(bool* tag)
{
    timeUsed = QTime::currentTime(); // обновление времени исп.

    for (int i=0; i<numBits; i++) pBits[i] = tag[i];

    // обязательно обновить визуал своей строки в табл.
    updateItems();
}

QColor stringBits::toColor(bool* bits)
{
    int* rgb = new int[3];
    rgb[0] = 0; rgb[1] = 0; rgb[2] = 0;

    for (int i=0; i<numBits; i++)
    {
        if (bits[i]) rgb[i%3] += dColor;
    }

    return QColor(rgb[0], rgb[1], rgb[2]);
}

bool* stringBits::getTag()
{
    bool* copyTag = new bool[numBits];
    for (int i=0; i<numBits; i++) copyTag[i] = pBits[i];

    return copyTag;
}
