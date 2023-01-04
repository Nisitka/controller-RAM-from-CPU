#include "managerforms.h"
#include "ui_managerforms.h"

#include <QDebug>

managerForms::managerForms(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::managerForms)
{
    ui->setupUi(this);
}

void managerForms::addWindow(QWidget* window)
{
    ui->horizontalLayout->addWidget(window);
}

void managerForms::delWindow(QWidget* window)
{
    ui->horizontalLayout->removeWidget(window);
    ui->horizontalLayout->update();
}

managerForms::~managerForms()
{
    delete ui;
}
