#ifndef EDITSTRING_H
#define EDITSTRING_H

#include <QWidget>
#include <QLabel>

#include "stringbits.h"

namespace Ui {
class editString;
}

class editString : public QWidget
{
    Q_OBJECT

signals:
    // отправить новые данные
    void sendData(bool*, QObject*);

    // запросить данные по тегу
    void getDataRAM(bool* tag);

public slots:
    void setStrFromMemory(stringBits* strBits);

    // получить данные от ОП
    void setDataRAM(const QString& data);

    // проверить правильность данных в ОП
    void updateData();

public:
    static void setColorLabel(QLabel*, const QColor&);

    explicit editString(QWidget *parent = nullptr);
    ~editString();

private slots:
    // установить случайную строку данных
    void setRandData();

    void buildData();
    void update(const QString& strBits);

private:
    void setData(const QString&);

    QString strBits;

    bool* lastTag;

    QObject* lastSender;

    Ui::editString *ui;
};

#endif // EDITSTRING_H
