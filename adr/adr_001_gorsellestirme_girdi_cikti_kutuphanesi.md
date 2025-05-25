# ADR-001: Görselleştirme ve Girdi/Çıktı Kütüphanesinin Seçilmesi

- Durum: Kabul Edildi
- Tarih: 2025-05-19

## Bağlam

Yazılarımla paralel bir şekilde girdi/çıktı ihtiyacı da olacak 2B/3B, gerçek zamanlı C++ uygulamalarının geliştirilmesi planlanmaktadır.
Bu uygulamalar, muhtemelen aşağıdaki bileşenleri içerecek:

- Gerçek zamanlı 2B/3B görselleştirme,
- Klavye, fare ve oyun kumandası gibi G/Ç'larından girdi alınması,
- Ses/müzik çalma,
- Çoklu platform desteği (Linux ve Windows).

Bu gereksinimleri karşılayacak bir kütüphane seçeceğiz.

## Karar

Medya, grafik, pencere ve G/Ç yönetimi gibi işlevler için `SDL3` (Simple DirectMedia Layer 3) kütüphanesi kullanılacaktır.

## Gerekçeler

- **Çapraz Platform Uyumluluk:** SDL; Windows, Linux ve android üzerinde çalışıyor. Platforma özel kod ihtiyacı oldukça az.
- **Modüler Mimari:** SDL'de yalnızca ihtiyaç duyulan bileşenlerle kullanılabilir (örneğin sadece giriş veya ses).
- **Topluluk ve Dokümantasyon:** SDL yaygın olarak kullanılır, açık kaynaklıdır ve zengin bir dökümantasyona sahiptir.
- **Performans:** Düşük seviyeli donanım erişimi sağlar, oyun ve gömülü sistemler için uygundur.
- **Diğer Araçlarla Uyumluluk:** OpenGL, Vulkan gibi grafik API'leri ile entegre çalışabilir.
- **Tecrübe:** uEngine4 ve daha önceki uygulamalarda her ne kadar SDL2 kullanılsa da, SDL'e yönelik tecrübe fazla.
- **CMake Desteği:** CMake üzerinden kolayca entegre edilebilir.

## Alternatifler

- **SFML (Simple and Fast Multimedia Library)**
  - Artılar: Daha modern C++ API, ses ve grafik yönünden entegre bir yapı.
  - Eksiler: Platform desteği SDL kadar geniş değil, bazı özel donanım erişimleri sınırlı. Mobil tarafı çok kuvvetli değil.

- **GLFW**
  - Artılar: Hafif ve sade, özellikle OpenGL uygulamaları için. Basit demo vs için kullanılabilir. 
  - Eksiler: Sadece pencere ve giriş yönetimi sunar; ses ve joystick gibi özellikler için ek kütüphane gerekir.

- **Platforma Özel API’ler (WinAPI, X11 vs.)**
  - Artılar: Doğrudan kontrol sağlar.
  - Eksiler: Taşınabilirlik yok, kod karmaşıklığı artar.

## Sonuçlar

- SDL3 projenin bağımlılıklarına eklenecek (tercihen CMake FetchContent veya vcpkg ile).
- Görselleştirme ve G/Ç'lar SDL ile yönetilecek.
- İleride ihtiyaç duyulursa, SDL üzerinden OpenGL/Vulkan entegrasyonu yapılabilecek.
- Proje dokümantasyonuna SDL kullanımı ve yapılandırması eklenecek.