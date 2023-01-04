#include "vector.h"

vector::vector(QLabel* pLabel_, types type_, location loc)
{
    pLabel = pLabel_;

    type = type_;
    switch (loc)
    {
    case location::pc :
        nameImage = "pc";
        break;
    case location::cpu :
        nameImage = "cpu";
        break;
    case location::core :
        nameImage = "core";
        break;
    }

    switch (type)
    {
    case types::Left :
        nameImage += "Left";
        break;
    case types::Right :
        nameImage += "Right";
        break;
    case types::Up :
        nameImage += "Up";
        pLabel->setMaximumWidth(30);
        break;
    case types::Down :
        nameImage += "Down";
        pLabel->setMaximumWidth(30);
        break;
    }

    timerToOFF = new QTimer;
    timerToON = new QTimer;

    connect(timerToOFF, SIGNAL(timeout()),
            this,       SLOT(off())
            );
    connect(timerToON, SIGNAL(timeout()),
            this,      SLOT(onFromPause())
            );
    on(true);
}

void vector::pauseON(int msec, bool status)
{
    lastStatus = status;
    timerToON->start(msec);
}

void vector::onFromPause()
{
    timerToON->stop();
    on(lastStatus);
}

void vector::on(bool s)
{
    lastStatus = s;

    timerToOFF->start(timeDataOut);
    repaint(true, s);
}

void vector::off()
{
    timerToOFF->stop();
    repaint(false);
}

void vector::repaint(bool enable, bool statusData)
{
    QPixmap pix;

    if (enable)
    {
        QString strStatus;
        if (statusData)
            strStatus = "Green";
        else
            strStatus = "Red";

        if (type == types::Up || type == types::Down)
            pix = QPixmap(QApplication::applicationDirPath() + "/" + nameImage + strStatus + format);
        else
            pix = QPixmap(QApplication::applicationDirPath() + "/" + nameImage + strStatus + format);
    }
    else pix = QPixmap(QApplication::applicationDirPath() + "/" + nameImage + "OFF" + format);

    pLabel->setPixmap(pix.scaled(pLabel->size()));
}


