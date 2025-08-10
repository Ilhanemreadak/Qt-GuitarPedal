#ifndef PEDALCONTROLLER_H
#define PEDALCONTROLLER_H

#include <QObject>
#include <QDebug>

class PedalController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool effectActive READ effectActive NOTIFY effectActiveChanged FINAL)
public:
    explicit PedalController(QObject *parent = nullptr) : QObject(parent){}

    Q_INVOKABLE void toggleEffect(bool on) {
        if(m_effectActive == on) return;
        m_effectActive = on;
        emit effectActiveChanged();
        qDebug() << "Efekt" << (on ? "Efekt açıldı" : "Efekt kapandı");
    }

    Q_INVOKABLE void changeMode(bool active) {
        m_modeName = active ? "Delay" : "Chorus";
        emit modeNameChanged();
        qDebug() << "Mode değişti: " << m_modeName;
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

    // QML'in okuyacağı getter
    bool effectActive() const { return m_effectActive; }
    QString modeName() const {return m_modeName; }

signals:
    void effectActiveChanged();
    void modeNameChanged();

private:
    bool m_effectActive = false;               // LED’in bağlı olacağı durum
    QString m_modeName = "Chorus";
};

#endif // PEDALCONTROLLER_H
