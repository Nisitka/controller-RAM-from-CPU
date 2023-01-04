#include "editstring.h"
#include "ui_editstring.h"

#include <cmath>

#include <QDebug>

editString::editString(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editString)
{
    ui->setupUi(this);

    this->setWindowTitle("Редактор строк данных");

    connect(
            ui->editButton, SIGNAL(clicked()),
            this,           SLOT(buildData())
            );
    connect(
            ui->randDataButton, SIGNAL(clicked()),
            this,               SLOT(setRandData())
            );
    connect(
            ui->tagLineEdit, SIGNAL(textEdited(const QString&)),
            this,            SLOT(update(const QString&))
            );

    lastTag = new bool;

    // установка нач. значений
    ui->dataValueLabel->clear();
    ui->tagLineEdit->clear();
    setColorLabel(ui->colorDataLabel, Qt::white);
    ui->editButton->setEnabled(false);
    ui->randDataButton->setEnabled(false);
}

void editString::setDataRAM(const QString& data)
{
    ui->dataValueLabel->setText(data);
}

void editString::setStrFromMemory(stringBits* strBits)
{
    ui->randDataButton->setEnabled(true);

    lastSender = sender();

    lastTag = strBits->getTag();

    setData(strBits->getData());

    QString strData = stringBits::toString(strBits->getTag());
    ui->tagLineEdit->setText(strData);
    update(strData);

    ui->editButton->setEnabled(false);
}

void editString::setData(const QString& strTitle)
{
    ui->dataValueLabel->setText(strTitle);
}

void editString::setRandData()
{
    bool* tag = stringBits::toBites(rand()%(int)pow(2, stringBits::numBits) - 1);

    QString strTag = stringBits::toString(tag);
    ui->tagLineEdit->setText(strTag);
    emit getDataRAM(tag);

    update(strTag);
}

void editString::update(const QString& strBits_)
{
    if (strBits_.size() == stringBits::numBits)
    {
        ui->editButton->setEnabled(true);

        strBits = strBits_;
        bool* tag = stringBits::toBites(strBits);

        // меняем цвет тэга
        setColorLabel(ui->colorDataLabel, stringBits::toColor(tag));
        // отправляем запрос на данные по тегу в ОП
        emit getDataRAM(tag);
    }
    else
    {
        ui->editButton->setEnabled(false);
        setColorLabel(ui->colorDataLabel, Qt::white);
    }
}

void editString::updateData()
{
    emit getDataRAM(lastTag);
}

void editString::buildData()
{
    bool* outTag = new bool;
    outTag = stringBits::toBites(strBits);
    for (int i=0; i<stringBits::numBits; i++) lastTag[i] = outTag[i];

    emit sendData(outTag, lastSender);
    emit getDataRAM(outTag);

    ui->editButton->setEnabled(false);
}

void editString::setColorLabel(QLabel* label, const QColor& color)
{
    label->clear();

    int r = color.red();
    int g = color.green();
    int b = color.blue();

    label->setStyleSheet(
                "QLabel {"
                    "background-color: rgb("
                    + QString::number(r) + ","
                    + QString::number(g) + ","
                    + QString::number(b) + ");"
                    "color: rgb(0,0,0);"

                    "border-style: outset;"
                    "border-radius: 3px;"
                    "border-width: 2px;"
                    "border-color: rgb(0,0,0);"
                "}"
                        );
}

editString::~editString()
{
    delete ui;
}
