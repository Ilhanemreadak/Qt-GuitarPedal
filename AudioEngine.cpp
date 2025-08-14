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

    const QAudioDevice inDev = QMediaDevices::defaultAudioInput();
    const QAudioDevice outDev = QMediaDevices::defaultAudioOutput();

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
 * @brief Girişten gelen ses verisini işleyip çıkışa yazar.
 *
 * readyRead() sinyali ile tetiklenir:
 * 1. Tüm ses verisi m_buffer'a okunur.
 * 2. Format 16-bit PCM ise her örnek m_gain ile çarpılır.
 * 3. Taşma durumunda örnekler sınırlandırılır.
 * 4. İşlenen veri çıkış cihazına yazılır.
 */
void AudioEngine::onReadyRead(){
    if(!m_input || !m_output)
        return;

    m_buffer = m_input->readAll();

    if(!m_buffer.isEmpty() && m_format.sampleFormat() == QAudioFormat::Int16){
        qint16* samples = reinterpret_cast<qint16*>(m_buffer.data());
        int sampleCount = m_buffer.size() / sizeof(qint16);

        if(m_gain != 1.0f){
            for(int i = 0; i < sampleCount; ++i){
                float s = samples[i] * m_gain;
                if(s >  32767.0f) s =  32767.0f;
                if(s < -32768.0f) s = -32768.0f;

                samples[i] = static_cast<qint16>(s);
            }
        }
    }

    if(!m_buffer.isEmpty()){
        m_output->write(m_buffer);
    }
}
