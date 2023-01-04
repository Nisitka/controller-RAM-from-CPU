#ifndef MANAGERFORMS_H
#define MANAGERFORMS_H

#include <QWidget>

namespace Ui {
class managerForms;
}

class managerForms : public QWidget
{
    Q_OBJECT

public slots:
    void addWindow(QWidget* window);
    void delWindow(QWidget* window);

public:
    explicit managerForms(QWidget *parent = nullptr);
    ~managerForms();

private:
    Ui::managerForms *ui;
};

#endif // MANAGERFORMS_H
