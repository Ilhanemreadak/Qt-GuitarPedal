#ifndef PEDALCONTROLLER_H
#define PEDALCONTROLLER_H

#include <QObject>
#include <QDebug>

class PedalController : public QObject
{
    Q_OBJECT
public:
    explicit PedalController(QObject *parent = nullptr) : QObject(parent){}

    Q_INVOKABLE void toggleEffect(bool active) {
        qDebug() << "Efekt" << (active ? "açıldı 🎸" : "kapandı 🔇");
    }

    Q_INVOKABLE void changeMode(bool active) {
        if (active)
            qDebug() << "Mod: Delay";
        else
            qDebug() << "Mod: Chorus";
    }

    Q_INVOKABLE void setTime(int value) {
        qDebug() << "Time değeri:" << value;
    }

    Q_INVOKABLE void setFeedback(int value) {
        qDebug() << "Feedback değeri:" << value;
    }

    Q_INVOKABLE void setLevel(int value) {
        qDebug() << "Level değeri:" << value;
    }

};

#endif // PEDALCONTROLLER_H
