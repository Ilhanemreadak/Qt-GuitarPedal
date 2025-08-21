#ifndef PEDALCONTROLLER_H
#define PEDALCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <AudioEngine.h>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QStringList>

class PedalController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList inputDevices READ inputDevices NOTIFY devicesChanged FINAL)
    Q_PROPERTY(QStringList outputDevices READ outputDevices NOTIFY devicesChanged  FINAL)

    Q_PROPERTY(bool effectActive READ effectActive NOTIFY effectActiveChanged FINAL)

    Q_PROPERTY(int time READ time WRITE setTime NOTIFY timeChanged FINAL)
    Q_PROPERTY(int feedback READ feedback WRITE setFeedback NOTIFY feedbackChanged FINAL)
    Q_PROPERTY(int level READ level WRITE setLevel NOTIFY levelChanged FINAL)



public:
    explicit PedalController(QObject *parent = nullptr) : QObject(parent){}

    QStringList inputDevices() const {
        QStringList list;
        const auto inputs = QMediaDevices::audioInputs();
        for (const auto &d : inputs) list << d.description();

        return list;
    }

    QStringList outputDevices() const {
        QStringList list;
        const auto outputs = QMediaDevices::audioOutputs();
        for (const auto &d : outputs) list << d.description();

        return list;
    }

    Q_INVOKABLE void setInputDevice(int index){
        const auto inputs = QMediaDevices::audioInputs();

        if(index >= 0 && index < inputs.size()){
            m_audio.setInputDevice(inputs[index]);
            qDebug() << "Giriş cihazı değişti:" << inputs[index].description();
        }
    }

    Q_INVOKABLE void setOutputDevice(int index) {
        const auto outputs = QMediaDevices::audioOutputs();
        if (index >= 0 && index < outputs.size()) {
            m_audio.setOutputDevice(outputs[index]);
            qDebug() << "Çıkış cihazı değişti:" << outputs[index].description();
        }
    }

    Q_INVOKABLE void toggleEffect(bool on) {
        if(m_effectActive == on) return;
        m_effectActive = on;
        emit effectActiveChanged();

        if (on) {
            if (m_audio.start()) {
                qDebug() << "Efekt açıldı, AudioEngine başlatıldı";
            } else {
                qDebug() << "Efekt açıldı ama AudioEngine başlatılamadı!";
            }
        } else {
            m_audio.stop();
            qDebug() << "Efekt kapandı, AudioEngine durdu";
        }
    }

    Q_INVOKABLE void changeMode(bool active) {
        m_modeName = active ? "Delay" : "Chorus";
        emit modeNameChanged();

        if (active) {
            m_audio.enableDelay(m_time, m_feedback / 100.0f);
        } else {
            m_audio.enableChorus();
        }

        qDebug() << "Mode değişti: " << m_modeName;
    }


    Q_INVOKABLE void setTime(int value) {
        if(m_time == value) return;
        m_time = value;
        emit timeChanged();

        // AudioEngine delay ayarı
        m_audio.setDelay(m_time, m_feedback / 100.0f);
        qDebug() << "Time değeri:" << value;
    }

    Q_INVOKABLE void setFeedback(int value) {
        if(m_feedback == value) return;
        m_feedback = value;
        emit feedbackChanged();

        // AudioEngine delay ayarı
        m_audio.setDelay(m_time, m_feedback / 100.0f);
        qDebug() << "Feedback değeri:" << value;
    }

    Q_INVOKABLE void setLevel(int value) {
        if(m_level == value) return;
        m_level = value;
        emit levelChanged();

        float g = m_level / 50.0f;

        if(g < 0.0f) g=0.0f;

        m_audio.setGain(g);

        qDebug() << "Level:" << m_level << "Gain:" << g;
    }

    // QML'in okuyacağı getter

    bool effectActive() const { return m_effectActive; }
    QString modeName() const {return m_modeName; }
    int time() const {return m_time;}
    int feedback() const {return m_feedback; }
    int level() const {return m_level; }
signals:
    void devicesChanged();
    void effectActiveChanged();
    void modeNameChanged();
    void timeChanged();
    void feedbackChanged();
    void levelChanged();

private:
    int m_time = 0;
    int m_feedback = 0;
    int m_level = 0;
    bool m_effectActive = false;               // LED’in bağlı olacağı durum
    QString m_modeName = "Chorus";

    AudioEngine m_audio;
};

#endif // PEDALCONTROLLER_H
