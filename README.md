# Guitar Pedal - En

A simple guitar pedal simulation built with Qt/QML.
Real-time audio processing: Gain (Level) and Delay (Time + Feedback) effects are supported.
The foot pedal can toggle the effect on/off, and knobs allow parameter adjustments.

---

## Features

* Gain (Level)
* Delay (Time)
* Delay Feedback
* Mode switch (Delay ↔ Chorus)
* Foot Pedal ON/OFF
* Tab interface (Pedal / Settings)

---

## Usage

1. Start the application.
2. On the Pedal tab → Toggle effect on/off with the foot pedal.
3. Turn TIME, FEEDBACK, LEVEL knobs to adjust sound.
4. On the Settings tab → Device input/output selection placeholder (to be completed).

---

## Requirements

* Qt 6.8+ (Quick, Core, Multimedia)
* CMake 3.16+

```cmake
find_package(Qt6 REQUIRED COMPONENTS Quick Core Multimedia)
```

---

## Status

* Working prototype
* Device input/output selection and additional effects can be added


# Guitar Pedal - Tr

Qt/QML ile yapılmış basit bir gitar pedalı simülasyonu.
Gerçek zamanlı ses işleme: Gain (Level) ve Delay (Time + Feedback) efektleri desteklenir.
Foot pedal ile aç/kapat yapılabilir, knoblar ile parametreler ayarlanabilir.

---

## Özellikler / Features

* Gain (Level)
* Delay (Time)
* Delay Feedback
* Mode tuşu (Delay ↔ Chorus)
* Foot Pedal ON/OFF
* Tab arayüzü (Pedal / Ayarlar)

---

## Kullanım / Usage

1. Uygulamayı başlatın.
2. Pedal sekmesi → Foot pedal ile efekti aç/kapatın.
3. TIME, FEEDBACK, LEVEL knoblarını çevirerek sesi değiştirin.
4. Ayarlar sekmesi → Giriş/çıkış cihazlarını seçmek için hazır (tamamlanabilir).

---

## Gereksinimler / Requirements

* Qt 6.8+ (Quick, Core, Multimedia)
* CMake 3.16+

```cmake
find_package(Qt6 REQUIRED COMPONENTS Quick Core Multimedia)
```

---

## Durum / Status

* Çalışır prototip
* Giriş/çıkış cihaz seçimi ve ek efektler eklenebilir
