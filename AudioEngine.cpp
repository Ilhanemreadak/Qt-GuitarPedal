#include <QObject>
#include <AudioEngine.h>
#include <QAudioSink>
#include <QMediaDevices>

/**
 * @brief AudioEngine nesnesini başlatır ve varsayılan giriş formatını ayarlar.
 * @param parent Üst QObject, yoksa nullptr.
 */
AudioEngine::AudioEngine(QObject* parent) : QObject(parent)
{
    const QAudioDevice inDev = QMediaDevices::defaultAudioInput();
    m_format = inDev.preferredFormat();
}

/**
 * @brief Ses giriş ve çıkış cihazlarını başlatır.
 *
 * Varsayılan giriş ve çıkış cihazlarını kullanarak
 * QAudioSource ve QAudioSink nesnelerini oluşturur,
 * verileri okumaya ve yazmaya başlar.
 *
 * @return true Başarıyla başlatıldıysa.
 * @return false Cihazlar başlatılamazsa.
 */
bool AudioEngine::start()
{
    if(isRunning())
        return true;

    const QAudioDevice inDev = m_selectedInput.isNull() ? QMediaDevices::defaultAudioInput() : m_selectedInput;
    const QAudioDevice outDev = m_selectedOutput.isNull() ? QMediaDevices::defaultAudioOutput() : m_selectedOutput;

    m_format = inDev.preferredFormat();

    m_source.reset(new QAudioSource(inDev, m_format, this));
    m_sink.reset(new QAudioSink(outDev, m_format, this));

    m_input = m_source->start();
    m_output = m_sink->start();

    if(!m_input || !m_output){
        stop();
        return false;
    }

    connect(m_input, &QIODevice::readyRead, this, &AudioEngine::onReadyRead);

    return true;
}

/**
 * @brief Ses giriş ve çıkış cihazlarını durdurur.
 *
 * Tüm bağlantıları keser, cihazları kapatır
 * ve bellekten temizler.
 */
void AudioEngine::stop(){
    if(m_input)
        disconnect(m_input, nullptr, nullptr, nullptr);

    if(m_source) m_source->stop();
    if (m_sink) m_sink->stop();

    m_input = nullptr;
    m_output = nullptr;

    m_source.reset();
    m_sink.reset();
}

/**
 * @brief Girişten gelen ses verisini gain ve delay efektleriyle işleyip çıkışa yazar.
 *
 * readyRead() sinyali ile tetiklenir:
 * 1. Tüm ses verisi m_buffer'a okunur.
 * 2. Format 16-bit PCM ise:
 *    - Delay etkinse, gecikmiş örnekler mevcut örneğe eklenir ve feedback uygulanır.
 *    - Gain değeri uygulanır.
 * 3. Taşma durumunda örnekler sınırlandırılır.
 * 4. İşlenen veri çıkış cihazına yazılır.
 *
 * @note Delay süresi ve feedback değeri setDelay() ile ayarlanır.
 */

void AudioEngine::onReadyRead(){
    if(!m_input || !m_output)
        return;

    m_buffer = m_input->readAll();

    if(!m_buffer.isEmpty() && m_format.sampleFormat() == QAudioFormat::Int16){
        qint16* samples = reinterpret_cast<qint16*>(m_buffer.data());
        int sampleCount = m_buffer.size() / sizeof(qint16);
        qint16* delayData = reinterpret_cast<qint16*>(m_delayBuffer.data());

        for(int i = 0; i < sampleCount; ++i){
            float inSample = samples[i];
            float delayedSample = delayData[m_delayPos];

            if (m_effectType == 1) {
                // Delay
                delayData[m_delayPos] = static_cast<qint16>(inSample + delayedSample * m_feedback);
            }
            else if (m_effectType == 2) {
                // Chorus → modülasyon ekleme
                float lfo = sinf(2.0f * M_PI * (m_delayPos % m_delaySamples) / m_delaySamples) * 0.5f;
                delayData[m_delayPos] = static_cast<qint16>((inSample + delayedSample) * (1.0f + lfo));
            }

            float outSample = inSample + delayedSample;
            outSample *= m_gain;

            if(outSample >  32767.0f) outSample =  32767.0f;
            if(outSample < -32768.0f) outSample = -32768.0f;

            samples[i] = static_cast<qint16>(outSample);

            m_delayPos++;
            if(m_delayPos >= m_delaySamples) m_delayPos = 0;
        }
    }


    // Çıkışa yaz
    if(!m_buffer.isEmpty()) {
        m_output->write(m_buffer);
    }

}

void AudioEngine::setInputDevice(const QAudioDevice& dev){
    m_selectedInput = dev;
    if(isRunning()) { stop(); start();}
}

void AudioEngine::setOutputDevice(const QAudioDevice& dev) {
    m_selectedOutput = dev;
    if (isRunning()) { stop(); start(); }
}
