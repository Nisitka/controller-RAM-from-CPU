#ifndef STRINGBITS_H
#define STRINGBITS_H

#include <QColor>
#include <QTableWidgetItem>
#include <QTime>

/* Для удобства восприятия строчки будут подсвечиваться
 * разными цветами в зависимости от значения
 */
//строчка битов
class stringBits
{
public:
    static QColor toColor(bool* bites);
    static bool* toBites(long int number);
    static bool* toBites(const QString& charsBite);
    static long int toInt(bool*, int rang = numBits);
    static const QString toString(bool* bites);
    static QColor colorText(const QColor& colorBackgraund);

    stringBits(QTableWidgetItem* itemData,
               QTableWidgetItem* itemTag,
               QTableWidgetItem* pItemTime);

    void setRandData();
    void setTag(bool* data);
    void setData(const QString& data);

    void usedData();

    bool* getTag();
    bool readyReset();

    QString getData();

    QTime getTimeUsed();

    static const int numBits = 8; // разрядность
private:
    void updateItems();

    QTableWidgetItem* pItemData;
    QTableWidgetItem* pItemTag;
    QTableWidgetItem* pItemTime;

    int volumeMemory;
    static const int dColor = (255*3) / (numBits + 2);

    // время, когда последний раз обращались
    QTime timeUsed;

    QString data;
    bool* pBits; // тег
};

#endif // STRINGBITS_H
