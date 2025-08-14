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

private slots:
    void onReadyRead();

private:
    QAudioFormat m_format;
    QScopedPointer<QAudioSource> m_source;
    QScopedPointer<QAudioSink> m_sink;
    QIODevice* m_input = nullptr;
    QIODevice* m_output = nullptr;
    float  m_gain = 1.0f;
    QByteArray m_buffer;

};

#endif // AUDIOENGINE_H
