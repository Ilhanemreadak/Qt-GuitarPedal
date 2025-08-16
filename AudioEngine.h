#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QAudioFormat>
#include <QAudioSource>
#include <QAudioSink>
#include <QMediaDevices>
#include <QIODevice>
#include <QByteArray>
#include <QScopedPointer>

class AudioEngine : public QObject
{
    Q_OBJECT

public:
    explicit AudioEngine(QObject* parent = nullptr);

    bool start();
    void stop();
    bool isRunning() const { return m_input && m_output; }

    void setGain(float g) { m_gain = g; }
    float gain() const {return m_gain;}

    void setDelay(int ms, float feedback) {
        m_feedback = feedback;
        m_delaySamples = (m_format.sampleRate() * ms) / 1000;
        m_delayBuffer.resize(m_delaySamples * sizeof(qint16));
        m_delayBuffer.fill(0);
        m_delayPos = 0;
    }

    void enableDelay(int ms, float feedback) {
        setDelay(ms, feedback);
        m_effectType = 1; // Delay
    }

    void enableChorus() {
        // Chorus: kısa delay + modülasyon
        setDelay(25, 0.4f);
        m_effectType = 2; // Chorus
    }


private slots:
    void onReadyRead();

private:
    QAudioFormat m_format;
    QScopedPointer<QAudioSource> m_source;
    QScopedPointer<QAudioSink> m_sink;

    QIODevice* m_input = nullptr;
    QIODevice* m_output = nullptr;

    int m_effectType = 0;

    float  m_gain = 1.0f;
    QByteArray m_buffer;

    QByteArray m_delayBuffer;
    int m_delayPos = 0;
    int m_delaySamples = 0;
    float m_feedback = 0.3f;
};

#endif // AUDIOENGINE_H
