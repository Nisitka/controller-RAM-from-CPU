#include "ramemory.h"
#include "ui_ramemory.h"

#include <cmath>

#include <QDebug>

RAMemory::RAMemory(QPushButton* pButton,
                   vector* pOutVector_,
                   QObject* editerString_,
                   QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RAMemory)
{
    ui->setupUi(this);
    this->setWindowTitle("Оперативная память");

    senderEditer = editerString_;

    connect(
            pButton, SIGNAL(clicked()),
            this,    SLOT(show())
            );

    connect(
            ui->allRandomDataButton, SIGNAL(clicked()),
            this,                    SLOT(setAllRandomData())
            );

    pOutVector = pOutVector_;

    loadTimer = new QTimer;
    connect(
            loadTimer, SIGNAL(timeout()),
            this,      SLOT(output())
            );

    int numRow = (int)pow(2, stringBits::numBits);
    ui->tableWidget->setRowCount(numRow);
    QStringList verLabels;
    for (int i=0; i<numRow; i++) verLabels.append("");
        ui->tableWidget->setVerticalHeaderLabels(verLabels);

    QStringList horLabels = {"Данные", "Адресный тег"};
    ui->tableWidget->setColumnCount(horLabels.size());
    ui->tableWidget->setHorizontalHeaderLabels(horLabels);

    for (int i=0; i<numRow; i++)
    {
        data.append("Строка " + QString::number(i + 1));

        QTableWidgetItem* newItemData = new QTableWidgetItem(data.last());

        bool* bitTag = stringBits::toBites(i);
        addresses.append(bitTag);

        QTableWidgetItem* newItemTag = new QTableWidgetItem(stringBits::toString(bitTag));
        QColor colorBack = stringBits::toColor(bitTag);
        newItemTag->setBackground(QBrush(colorBack));
        newItemTag->setTextColor(stringBits::colorText(colorBack));

        ui->tableWidget->setItem(i, Data, newItemData);
        ui->tableWidget->setItem(i, Tag, newItemTag);
    }

    connect(
            ui->editDataButton, SIGNAL(clicked()),
            this,               SLOT(editString())
            );

    connect(
            ui->tableWidget, SIGNAL(cellClicked(int, int)),
            this,            SLOT(setValueLineEdit(int))
            );
}

void RAMemory::getString(bool* address)
{
    senderL = sender();

    int id = stringBits::toInt(address);

    if (sender() == senderEditer)
    {
        emit sendStrToEditer(data.at(id));
    }
    else
    {
        emit sendString(data.at(id), senderL);
    }
}


void RAMemory::getSrings(const QVector <bool*>& addresses)
{
    senderL = sender();

    QStringList outData;
    for (int i=0; i<addresses.size(); i++)
    {
        int id = stringBits::toInt(addresses.at(i));
        outData.append(data.at(id));
    }

    emit sendData(outData, senderL);
}

void RAMemory::getData(bool* tag)
{   
    senderContr = sender();
    idOutData = stringBits::toInt(tag);
    loadTimer->start(timeLoading);
}

void RAMemory::output()
{
    loadTimer->stop();

    emit statusData(addresses.at(idOutData),
                    data.at(idOutData),
                    senderContr);

    pOutVector->on(true);
}

void RAMemory::setAllRandomData()
{
    for (int i=0; i<data.size(); i++)
    {
        removeString(i);
        emit updateString(addresses[i], data[i]);
    }
    emit updateAllData();

    setValueLineEdit(ui->tableWidget->currentRow());
}

void RAMemory::setValueLineEdit(int row)
{
    ui->dataLineEdit->setText(data[row]);
}

void RAMemory::editString()
{
    int idStr = ui->tableWidget->currentRow();
    removeString(idStr, ui->dataLineEdit->text());
    emit updateString(addresses[idStr], data[idStr]);
}

void RAMemory::removeString(int id, const QString& strData)
{
    data[id] = strData;
    ui->tableWidget->item(id, Data)->setText(data[id]);
}

QString RAMemory::randData(int size)
{
    QString bigData =
        "1qazxsw23edcvfr45tgbnhy67ujm,ki89ol./;p0-[']=!@#$%^&*(){}_-=+";

    QString outData = "";

    for (int i=0; i<size; i++)
    {
        outData += bigData[rand()%bigData.size()];
    }

    return outData;
}

RAMemory::~RAMemory()
{
    delete ui;
}
