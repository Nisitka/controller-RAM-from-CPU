#include "contmemorywidget.h"
#include "ui_contmemorywidget.h"

#include <QDebug>

contMemoryWidget::contMemoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contMemoryWidget)
{
    ui->setupUi(this);

    //инициализация стрелок
    setVectors();

    // создаем процессор, ярда создаются автоматически
    QVector <vector*> pVectorsCPU;
    pVectorsCPU.append(vectors.at(outALD)); // из АЛУ

    pVectorsCPU.append(vectors.at(outL3));  // из L3

    pVectorsCPU.append(vectors.at(outL1data));  // из L1data памяти
    pVectorsCPU.append(vectors.at(outL1instr)); // из L1insert памяти
    pVectorsCPU.append(vectors.at(outL2));      // из L2 памяти

    // из контроллера
    pVectorsCPU.append(vectors.at(inALD));
    pVectorsCPU.append(vectors.at(inL1data));
    pVectorsCPU.append(vectors.at(inL1instr));
    pVectorsCPU.append(vectors.at(inL2));
    pVectorsCPU.append(vectors.at(inL3));
    pVectorsCPU.append(vectors.at(inRAM));

    QVector <QPushButton*> pButtons;
    pButtons.append(ui->coresButton);
    pButtons.append(ui->L1dataButton);
    pButtons.append(ui->L1instrButton);
    pButtons.append(ui->L2Button);
    pButtons.append(ui->L3Button);

    editerString = new editString;
    RAM = new RAMemory(ui->ramButton, vectors.at(11), editerString);
    connect(
            editerString, SIGNAL(getDataRAM(bool*)),
            RAM,          SLOT(getString(bool*))
            );
    connect(
            RAM,          SIGNAL(sendStrToEditer(const QString&)),
            editerString, SLOT(setDataRAM(const QString&))
            );
    connect(
            RAM,          SIGNAL(updateAllData()),
            editerString, SLOT(updateData())
            );

    cpu = new CPU(pVectorsCPU, pButtons, RAM, ui->coresBox, editerString);
    connect(
            ui->contrMemoryButton, SIGNAL(clicked()),
            cpu,                   SLOT(showContrller())
            );

    // настройка визуала
    setVizual();
}

// покраска фона в белый цвет
void contMemoryWidget::paintEvent(QPaintEvent* paintEvent)
{
    QPainter painter(this);
    painter.setPen(Qt::white);
    painter.setBrush(QBrush(Qt::white));

    painter.drawRect(
                     0,                     0,
                     this->size().width(),  this->size().height()
                     );
}

// настройка визуала
void contMemoryWidget::setDesineWidget(QWidget *widget, typeWidget tW, const QColor& color)
{
    int r, g, b;
    r = color.red();
    g = color.green();
    b = color.blue();

    QString strStyle;
    if (tW)
    {
        strStyle =
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
                "}";
    }
    else
    {
        strStyle =
                "QPushButton {"
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

                "QPushButton:hover {"
                    "background-color: rgb("
                        + QString::number(lC(r)) + ","
                        + QString::number(lC(g)) + ","
                        + QString::number(lC(b)) + ");"
                    "color: rgb(0,0,0);"
                    "border-color: rgb(0,0,0);"
                    "}"
                ;
    }
    widget->setStyleSheet(strStyle);
}

int contMemoryWidget::lC(int rgb)
{
    int outValue = rgb + 20; // приращение яркости
    if (outValue > 255) return 255;
    else return outValue;
}

void contMemoryWidget::setVizual()
{
    this->setWindowTitle("Контроллер кэш памяти");
    this->setFixedSize(1150, 550);

    ui->coreLabel->clear();
    setDesineWidget(ui->coreLabel, label, QColor(35, 129, 203));

    ui->cpuLabel->clear();
    setDesineWidget(ui->cpuLabel, label, QColor(96, 255, 232));

    QList <QWidget*> buttons = {ui->coresButton,
                                ui->contrMemoryButton,
                                ui->L2Button,
                                ui->L3Button,
                                ui->L1dataButton,
                                ui->L1instrButton,
                                ui->ramButton
                               };
    QList <QColor> colors = {QColor(239, 228, 176),
                             QColor(168,210,84),
                             QColor(255,204,0),
                             QColor(235,235,20),
                             QColor(242,133,0),
                             QColor(242,133,0),
                             QColor(125,125,255),
                            };
    for (int i=0; i < buttons.size(); i++)
        setDesineWidget(buttons[i], button, colors[i]);
}

void contMemoryWidget::setVectors()
{
    vectors.append(new vector(ui->outALD,     vector::Right, vector::core));
    vectors.append(new vector(ui->inALD,      vector::Left,  vector::core));
    vectors.append(new vector(ui->inL1data,   vector::Up,    vector::core));
    vectors.append(new vector(ui->outL1data,  vector::Down,  vector::core));
    vectors.append(new vector(ui->inL1instr,  vector::Up,    vector::core));
    vectors.append(new vector(ui->outL1instr, vector::Down,  vector::core));
    vectors.append(new vector(ui->inL2,       vector::Up,    vector::core));
    vectors.append(new vector(ui->outL2,      vector::Down,  vector::core));
    vectors.append(new vector(ui->inL3,       vector::Up,    vector::cpu));
    vectors.append(new vector(ui->outL3,      vector::Down,  vector::cpu));
    vectors.append(new vector(ui->inRAM,      vector::Right, vector::pc));
    vectors.append(new vector(ui->outRAM,     vector::Left,  vector::pc));
}

contMemoryWidget::~contMemoryWidget()
{
    delete ui;
}
