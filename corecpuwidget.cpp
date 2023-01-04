#include "corecpuwidget.h"
#include "ui_corecpuwidget.h"

coreCPUwidget::coreCPUwidget(const QString& name,
                             const QVector <vector*>& vectors,
                             const QVector <QPushButton*>& buttons,
                             RAMemory* RAM_,
                             editString* editerString,
                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::coreCPUwidget)
{
    ui->setupUi(this);

    // инициализация памяти L1 и L2
    L1data = new memoryLwidget("L1 для данных",
                               buttons.at(0),
                               vectors.at(0),
                               2,
                               RAM_,
                               editerString);
    L1data->setTimeLoading(1200);

    L1instr = new memoryLwidget("L1 для команд",
                                buttons.at(1),
                                vectors.at(1),
                                2,
                                RAM_,
                                editerString);
    L1instr->setTimeLoading(1200);

    L2 = new memoryLwidget("L2",
                           buttons.at(2),
                           vectors.at(2),
                           4,
                           RAM_,
                           editerString);
    L2->setTimeLoading(2000);

    connect(
            RAM_,     SIGNAL(updateString(bool*, const QString&)),
            L1data,   SLOT(updateString(bool*, const QString&))
            );
    connect(
            RAM_,      SIGNAL(updateString(bool*, const QString&)),
            L1instr,   SLOT(updateString(bool*, const QString&))
            );
    connect(
            RAM_, SIGNAL(updateString(bool*, const QString&)),
            L2,   SLOT(updateString(bool*, const QString&))
            );

    QList <memoryLwidget*> Lbox = {L1data, L1instr, L2};

    // создаем котроллер памяти ядра
    contMemory = new controllerMemory(vectors.mid(3));
    connect(
            this,       SIGNAL(getDataSignal(bool*, int)),
            contMemory, SLOT(setDataFromCore(bool*, int))
            );

    // Установка связи между памятью и её контроллером
        // поиск данных:
            // запрос данных от контроллера
    connect(
            contMemory, SIGNAL(sendL1data(bool*)),
            L1data,     SLOT(getData(bool*))
            );
    connect(
            contMemory,  SIGNAL(sendL1instr(bool*)),
            L1instr,     SLOT(getData(bool*))
            );
    connect(
            contMemory,  SIGNAL(sendL2(bool*)),
            L2,     SLOT(getData(bool*))
            );
            // ответ кеш памяти контроллеру
    connect(
            L1data,     SIGNAL(statusData(bool, QObject*)),
            contMemory, SLOT(setStatusFromL1data(bool))
            );
    connect(
            L1instr,    SIGNAL(statusData(bool, QObject*)),
            contMemory, SLOT(setStatusFromL1instr(bool))
            );
    connect(
            L2,         SIGNAL(statusData(bool, QObject*)),
            contMemory, SLOT(setStatusFromL2(bool))
            );
        // установка данных:
            // перезапись в кеш
    connect(
            contMemory, SIGNAL(setDataL1data(bool*, const QString&)),
            L1data,     SLOT(setDataFromContr(bool*, const QString&))
            );
    connect(
            contMemory, SIGNAL(setDataL1instr(bool*, const QString&)),
            L1instr,    SLOT(setDataFromContr(bool*, const QString&))
            );
    connect(
            contMemory, SIGNAL(setDataL2(bool*, const QString&)),
            L2,         SLOT(setDataFromContr(bool*, const QString&))
            );
            // ответ на перезапись в кеш
    connect(
            L1data,     SIGNAL(dataSendL(bool*, const QString&)),
            contMemory, SLOT(setDataFromL1data(bool*, const QString&))
            );
    connect(
            L1instr,    SIGNAL(dataSendL(bool*, const QString&)),
            contMemory, SLOT(setDataFromL1instr(bool*, const QString&))
            );
    connect(
            L2,         SIGNAL(dataSendL(bool*, const QString&)),
            contMemory, SLOT(setDataFromL2(bool*, const QString&))
            );

    for (int i=0; i<Lbox.size(); i++)
    {
        connect(
                Lbox.at(i), SIGNAL(toShow(memoryLwidget*)),
                this,       SLOT(showWinMemory(memoryLwidget*))
                );
        Lbox.at(i)->setWindowTitle(name);  // в титуле окна будет написан номер ядра
    }

    this->setObjectName(name);
    ui->nameCoreLabel->setText(name);
    this->setWindowTitle(name);

    connect(
            ui->sendButton, SIGNAL(clicked()),
            this,           SLOT(getData())
            );

    connect(
            ui->randButton, SIGNAL(clicked()),
            this,           SLOT(setRandData())
            );

    ui->buildDataSlider->setMaximum(maxValueSlider);
    connect(
            ui->buildDataSlider, SIGNAL(sliderMoved(int)),
            this,                SLOT(setOutDataSlider(int))
            );

    connect(
            ui->dataOutLineEdit, SIGNAL(textEdited(const QString&)),
            this,                SLOT(setOutDataText(const QString&))
            );

    ui->colorLabel->clear();

    ui->typeDataComboBox->addItems(typesData);

    setRandData();
}

void coreCPUwidget::setBlocked(bool status)
{
    if (status)
    {
        ui->sendButton->setEnabled(false);
    }
    else
    {
        ui->sendButton->setEnabled(true);
    }
}

void coreCPUwidget::setOutDataText(const QString& data)
{
    if (data.size() == stringBits::numBits)
    {
        pBitesOut = stringBits::toBites(data);
        ui->buildDataSlider->setValue(stringBits::toInt(pBitesOut));
        updateGUI();
    }
    else
    {
        editString::setColorLabel(ui->colorLabel, Qt::white);
        ui->sendButton->setEnabled(false);
    }
}

void coreCPUwidget::showWinMemory(memoryLwidget* memWin)
{
    emit toShowMemory(memWin, this);
}

//установка значения по умолчанию
void coreCPUwidget::setRandData()
{
    int randValue = rand()%maxValueSlider;
    ui->buildDataSlider->setValue(randValue);
    setOutDataSlider(randValue);
}

void coreCPUwidget::setOutDataSlider(int part)
{
    double d;
    if (part == 0)
    {
        d = 0;
    }
    else
    {
        d = (double) part / maxValueSlider;
    }
    int value = d * pow(2, stringBits::numBits);

    pBitesOut = stringBits::toBites(value);

    // отображение отправляемой информации
    updateGUI();
}

void coreCPUwidget::updateGUI()
{
    ui->dataOutLineEdit->setText(stringBits::toString(pBitesOut));

    editString::setColorLabel(ui->colorLabel, stringBits::toColor(pBitesOut));

    ui->sendButton->setEnabled(true);
}

// запрос данных у котроллера
void coreCPUwidget::getData()
{
    emit getDataSignal(pBitesOut, ui->typeDataComboBox->currentIndex());
}

controllerMemory* coreCPUwidget::getController()
{
    return contMemory;
}

coreCPUwidget::~coreCPUwidget()
{
    delete ui;
}
