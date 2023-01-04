#include "memorylwidget.h"
#include "ui_memorylwidget.h"

#include <QDebug>

memoryLwidget::memoryLwidget(const QString& nameWindow,
                             QPushButton* toShowButton,
                             vector* pDataVector,
                             int volume_,
                             RAMemory* RAM,
                             editString* editerString,
                             bool readyNextL_,
                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::memoryLwidget)
{
    ui->setupUi(this);

    pI = this;

    readyNextL = readyNextL_;
    readyUpdate = false;

    //
    connect(
            this, SIGNAL(getDataFromTags(const QVector <bool*>&)),
            RAM,  SLOT(getSrings(const QVector <bool*>&))
            );
    connect(
            RAM,  SIGNAL(sendData(const QStringList&, QObject*)),
            this, SLOT(update(const QStringList&, QObject*))
            );

    connect(
            this, SIGNAL(getStringFromTag(bool*)),
            RAM,  SLOT(getString(bool*))
            );
    connect(
            RAM,  SIGNAL(sendString(const QString&, QObject*)),
            this, SLOT(removeData(const QString&, QObject*))
            );

    //соеденяем кнопку с сигналом запроса на открытия окна
    connect(
            toShowButton, SIGNAL(clicked()),
            this,         SLOT(Show())
            );
    ui->titleLabel->setText(nameWindow);

    pVector = pDataVector;

    volume = volume_;

    // таблица строк битев
    ui->bitesTableWidget->setRowCount(volume);
    QStringList verLabels;
    for (int i=0; i<volume; i++) verLabels.append("");
    ui->bitesTableWidget->setVerticalHeaderLabels(verLabels);

    QStringList horLabels = {"данные", "Адресный тег", "Время обращения"};
    ui->bitesTableWidget->setColumnCount(horLabels.size());
    ui->bitesTableWidget->setHorizontalHeaderLabels(horLabels);

    for (int i=0; i<volume; i++)
    {
        QTableWidgetItem* newItemData = new QTableWidgetItem;
        QTableWidgetItem* newItemTag = new QTableWidgetItem;
        QTableWidgetItem* newItemTime = new QTableWidgetItem;

        ui->bitesTableWidget->setItem(i, Data, newItemData);
        ui->bitesTableWidget->setItem(i, Tag, newItemTag);
        ui->bitesTableWidget->setItem(i, Time, newItemTime);

        boxStrings.append(new stringBits(newItemData, newItemTag, newItemTime));

    }
    ui->bitesTableWidget->setColumnWidth(Data, 160);

    // искуственная задержка отправки ответа
    updateTimer = new QTimer;
    connect(
            updateTimer, SIGNAL(timeout()),
            this,        SLOT(sending())
            );

    // подключаем редактор строки
    connect(
            ui->editStringBitesButton, SIGNAL(clicked()),
            editerString,              SLOT(show())
            );
    connect(
            editerString, SIGNAL(sendData(bool*, QObject*)),
            this,         SLOT(setData(bool*, QObject*))
            );

    // изменить визуал редактора строки
    connect(
            ui->bitesTableWidget, SIGNAL(cellClicked(int, int)),
            this,                 SLOT(sendStrToEditer(int))
            );
    connect(
            this,         SIGNAL(updateStrEditer(stringBits*)),
            editerString, SLOT(setStrFromMemory(stringBits*))
            );

    QVector <bool*> tags;
    for (int i=0; i<volume; i++)
    {
        tags.append(boxStrings.at(i)->getTag());
    }
    getDataFromTags(tags);
}

void memoryLwidget::update(const QStringList& dataList, QObject* sender)
{
    if (sender == pI)
        for (int i=0; i<dataList.size(); i++)
            boxStrings.at(i)->setData(dataList.at(i));
}

void memoryLwidget::removeData(const QString& data, QObject* sender)
{
    if (sender == pI) boxStrings.at(ui->bitesTableWidget->currentRow())->setData(data);
}

void memoryLwidget::sendStrToEditer(int idStr)
{
    emit updateStrEditer(boxStrings.at(idStr));
}

void memoryLwidget::setData(bool* tag, QObject* sender)
{
    if (pI == sender)
    {
        boxStrings.at(ui->bitesTableWidget->currentRow())->setTag(tag);

        // отпраляем запрос данных по тегу
        emit getStringFromTag(tag);
    }
}

void memoryLwidget::setDataFromContr(bool* tag_, const QString& strData_)
{
    int idStr = idRemoveString();

    outTag = boxStrings[idStr]->getTag();
    outData = boxStrings[idStr]->getData();

    boxStrings[idStr]->setTag(tag_);
    boxStrings[idStr]->setData(strData_);

    if (readyNextL)
    {
        tSignal = toL;
        updateTimer->start(timeLoading);
    }
}

bool memoryLwidget::u(bool *a, bool *b)
{
    bool outV = true;
    for (int j=0; j<rang; j++)
    {
        outV = XAND(a[j], b[j]);
        // если хоть раз не совпало выйти с проверки строки
        if (!outV) break;
    }

    return outV;
}

void memoryLwidget::updateString(bool* tag, const QString& data)
{
    for (int i=0; i<volume; i++)
    {
        stringBits* string = boxStrings.at(i);
        if (u(tag, string->getTag()))
        {
            string->setData(data);
            // если нашли строку то выходим из цикла поиска
            break;
        }
    }
}

int memoryLwidget::idRemoveString()
{
    QTime t = QTime::currentTime();
    QTime timeUsed;
    int idStr;
    for (int i=0; i<boxStrings.size(); i++)
    {
        timeUsed = boxStrings[i]->getTimeUsed();
        if (t > timeUsed)
        {
            idStr = i;
            t = timeUsed;
        }
    }

    return idStr;
}

void memoryLwidget::Show()
{
    emit toShow(this);
}

void memoryLwidget::getData(bool* data)
{
    senderContr = sender();

    outStatus = true;
    for (int i=0; i<volume; i++)
    {
        bool* memData = boxStrings.at(i)->getTag();
        for (int j=0; j<rang; j++)
        {
            outStatus = XAND(data[j], memData[j]);
            // если хоть раз не совпало выйти с проверки строки
            if (!outStatus) break;
        }

        // если уже нашли строку то прекратить поиск
        if (outStatus)
        {
            boxStrings.at(i)->usedData();
            break;
        }
    }

    tSignal = status;
    updateTimer->start(timeLoading);
}

void memoryLwidget::setTimeLoading(int msec)
{
    timeLoading = msec;
}

void memoryLwidget::sending()
{
    updateTimer->stop();
    switch (tSignal)
    {
        case toL:
            sendDataToL();
            break;

        case status:
            sendStatusData();
            break;
    }
}

void memoryLwidget::sendDataToL()
{
    emit dataSendL(outTag, outData);

    // очищаем память от времменного значения
    delete [] outTag;

    pVector->on(true);
}

void memoryLwidget::sendStatusData()
{
    emit statusData(outStatus, senderContr);
    pVector->on(outStatus);
}

bool memoryLwidget::XAND(bool x1, bool x2)
{
    return !(x1 || x2) || (x1 && x2);
}

memoryLwidget::~memoryLwidget()
{
    delete ui;
}
