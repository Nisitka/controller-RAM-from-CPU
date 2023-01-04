#ifndef MEMORYLWIDGET_H
#define MEMORYLWIDGET_H

#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>

#include "stringbits.h"
#include "vector.h"
#include "editstring.h"
#include "ramemory.h"
#include "editstring.h"

namespace Ui {
class memoryLwidget;
}

// Класс, харак-й память в процессоре
class memoryLwidget : public QWidget
{
    Q_OBJECT
signals:
    // есть ли данные в памяти; кому отправить
    void statusData(bool, QObject*);

    // запрос на открытия интерфейса
    void toShow(memoryLwidget*);

    // изменение визуала в редакторе строк
    void updateStrEditer(stringBits* strBits);

    // запросить данные по тегам
    void getDataFromTags(const QVector <bool*>& addresses);
    void getStringFromTag(bool* address);

    // отправить данные на перезапись в кеш другого уровня
    void dataSendL(bool* tag_, const QString& strData_);

public slots:
    void Show();

    // обновить данные строки
    void updateString(bool* tag, const QString& data);

    //обработка запроса на поиск данных
    void getData(bool* tag);

    // по запросу от ОП
    void setData(bool* tag, QObject* sender);
    // от контроллера (моделирование как вреальности)
    void setDataFromContr(bool* tag_, const QString& strData_);

    // обновить данные у всех строк
    void update(const QStringList& dataList, QObject* sender);
    void removeData(const QString& data, QObject* sender);

public:
    explicit memoryLwidget(const QString& nameWindow,
                           QPushButton* toShowButton,
                           vector* pDataVector,
                           int volume,
                           RAMemory* RAM,
                           editString* editerString,
                           bool readyNextL = true,
                           QWidget *parent = nullptr);

    void setTimeLoading(int msec);

    static bool XAND(bool x1, bool x2);

    ~memoryLwidget();

private slots:
    void sending();
    void sendStatusData();
    void sendDataToL();

    void sendStrToEditer(int indexStr);

private:
    bool u(bool* a, bool* b);

    QObject* pI;

    enum typeSignal {toL, status};
    typeSignal tSignal;

    int idRemoveString();

    QTimer* updateTimer;
    int timeLoading = 1500;

    //размер памяти (кол-во ячеек)
    int volume;
    //сами ячейки
    QVector <stringBits*> boxStrings;
    const int rang = stringBits::numBits; //кол-во ячеек

    vector* pVector;

    bool outStatus;

    bool* outTag;
    QString outData;

    enum Column{Data, Tag, Time};

    bool readyUpdate;

    QObject* senderContr;

    // есть кеш уровня выше
    bool readyNextL;

    Ui::memoryLwidget *ui;
};

#endif // MEMORYLWIDGET_H
