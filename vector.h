#ifndef VECTOR_H
#define VECTOR_H

#include <QApplication>

#include <QLabel>
#include <QPixmap>

#include <QTimer>

class vector : public QObject
{
    Q_OBJECT
public:
    // включить через опред. время
    void pauseON(int msec, bool status);

    // типы направлений
    enum types{Left, Right, Up, Down};

    //
    enum location{core, cpu, pc};

    vector(QLabel* pLabel, types, location);

public slots:
    void on(bool s);

private slots:
    void off();
    void onFromPause();

private:
    void repaint(bool enable, bool statusData = false);

    bool lastStatus;

    QString nameImage;
    const QString format = ".png";
    QLabel* pLabel;
    types type;

    const QString strON = "ON";
    const QString strOFF = "OFF";

    int timeDataOut = 1000; // msec
    QTimer* timerToOFF;
    QTimer* timerToON;

};

#endif // VECTOR_H
