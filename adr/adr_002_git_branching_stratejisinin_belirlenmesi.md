# ADR-002: Git Branching ve Merge Stratejisinin Belirlenmesi

- Durum: Kabul Edildi
- Tarih: 2026-01-17

## Bağlam

C++ ve git kullanılarak geliştirilecek olan projeler için kullanılabilecek ve C++/git'e yeni başlayanlara yardımcı olacak bir yaklaşımın belirlenmesi hedeflenmektedir. Bu projeler:

- GitHub ve GitLab üzerinde barındırılacaktır. Ağırlıklı olarak gitlab olacaktır,
- Tek geliştiricili başlayıp zamanla ekipli geliştirmeye evrilebilecektir,
- CI/CD süreçleri (build, test, release) ile entegre çalışacaktır,
- Kararlı sürümler ile aktif geliştirme çalışmalarının net şekilde ayrılmasını gerektirmektedir.

Bu bağlamda; kod kalitesini koruyan, sürüm yönetimini sadeleştiren ve C++ ekosistemindeki (CMake, Conan, CI pipeline’ları) pratiklerle uyumlu bir Git branching stratejisi seçilecektir.

## Karar

Projelerde **hafifletilmiş GitFlow benzeri**, aşağıdaki ana branch'leri ve branch kurallarını içeren bir branching stratejisi kullanılacaktır:

- `main`
- `feature/*`
- `bugfix/*`
- `release/*`

Gitlab ve github üzerinde bu isimlendirmeleri de kontrol ederek dikte edilebilecek yöntemler incelenecektir.

## Karar: Merge Stratejisinin Belirlenmesi

Projede **varsayılan merge stratejisi olarak *Squash Merge* kullanılacaktır**.

Bu karar; commit geçmişinin sade, okunabilir ve anlamlı kalmasını sağlamak amacıyla alınmıştır. Her feature veya bugfix çalışması, hedef branche **tek ve atomik bir commit** olarak girecektir.

---

## Merge Stratejisi

### Squash Merge (Seçilen)

* Bir Pull / Merge Request içindeki tüm commit’ler **tek commit** haline getirilir
* Commit mesajı, PR/MR başlığı ve açıklamasından türetilir
* Hedef branch'te:

  * Daha temiz bir geçmiş
  * Daha kolay `git bisect` ve rollback
  * Sürüm notu (changelog) üretimi için net commit’ler elde edilir

**Kurallar:**

* `main` branchine yapılan tüm merge’ler squash olmak zorundadır
* Squash commit mesajı aşağıdaki formatı izler:

  * `<type>: <kısa açıklama>`
  * Örnek: `feature: telemetry pipeline integration`

### Rebase + Merge (İstisnai)

* Sadece çok küçük ve tek commit’lik değişikliklerde
* Lead / maintainer onayı ile

### Merge Commit (Kullanılmayacak)

* Varsayılan olarak **devre dışı** bırakılır
* Commit geçmişinde gürültü oluşturduğu için tercih edilmez

---

## Branch Tanımları ve Kurallar

### `main`

- Daima **production-ready** durumdadır. Ne demek bu? Yani bu branch'teki uygulama her zaman çalışır, hazır ve nazır durumda olmalıdır.
- Her merge işlemi **tag** (ör. `v1.2.0`) ile işaretlenebilir.
- CI üzerinde tam build + test çalıştırılır.
- `feature/*` ve `bugfix/*` branch'leri buraya merge edilir (PR ve MR ile).

### `feature/*`

- Yeni özellik geliştirmeleri için kullanılır.
- `main` branch'inden ya da diğer branch'lerden türetilir.
- İsimlendirme: `feature/<kisa-aciklama>`
  - Örnek: `feature/telemetry-integration`
- Tamamlandığında **Pull/Merge Request** ile `main` branchine alınır.

### `bugfix/*`

- Geliştirme sürecinde tespit edilen hataların düzeltilmesi için kullanılır.
- `main` branch'inden ya da diğer branch'lerden türetilir.
- İsimlendirme: `bugfix/<kisa-aciklama>`
  - Örnek: `bugfix/nullptr-crash-render-loop`
- Düzeltme sonrası `main` branchine merge edilir.

### `release/*`

- Sürüm hazırlık branchidir.
- `main` branchinden türetilir.
- İsimlendirme: `release/<major.minor.patch>`
  - Örnek: `release/1.3.0`
- Bu branchde yalnızca:
  - Versiyon numarası güncellemesi,
  - Dokümantasyon düzenlemeleri,
  - Kritik bugfix’ler yapılır.

## Gerekçeler

- **C++ Projelerine Uygunluk:** Uzun süren feature geliştirmeleri ve stabilizasyon ihtiyacı için idealdir.
- **Release Disiplini:** Sürüm öncesi kod dondurma (code freeze) sürecini doğal olarak destekler.
- **CI/CD Uyumlu:** GitHub Actions ve GitLab CI ile kolay entegre edilir.
- **Okunabilirlik:** Branch isimleri niyetini açıkça ifade eder.
- **Ölçeklenebilirlik:** Tek geliştiriciden ekipli geliştirmeye sorunsuz evrilir.
- **Araç Desteği:** SemVer, changelog ve otomatik versiyonlama süreçleriyle uyumludur.

## Alternatifler

### Merge Politikaları

- **Squash merge** (tercih edilen):
  - Temiz commit geçmişi
  - Tek feature = tek commit
- Alternatif olarak **Rebase + merge**
- `main` branchinde **merge commit** tercih edilmez

### Trunk-Based Development

- Artılar:
  - Basit branch yapısı,
  - Hızlı entegrasyon.
- Eksiler:
  - Uzun süren feature’lar için zorlayıcı,
  - C++ projelerinde stabilite riski.

### Klasik GitFlow

- Artılar:
  - Net roller ve süreçler.
- Eksiler:
  - Fazla karmaşık,
  - Küçük ve orta ölçekli projelerde aşırı ağır.

### Sadece `main` + `feature/*`

- Artılar:
  - Minimal yapı.
- Eksiler:
  - Release yönetimi ve stabilizasyon zorlaşır.

## GitHub / GitLab Branch Protection ve Merge Kuralları

Aşağıdaki kurallar hem GitHub hem de GitLab üzerinde uygulanacak şekilde tasarlanmıştır. Amaç; `main` branchinin stabilitesini korumak ve kontrolsüz değişiklikleri engellemektir.

### `main` Branch İçin Kurallar

- **Direct push yasak**
- Sadece **Pull / Merge Request** ile değişiklik yapılabilir
- Minimum **1–2 code review** zorunlu
- Tüm CI adımları (build + test) **başarılı olmak zorunda**
- Merge sonrası **otomatik tag** veya manuel versiyon etiketi zorunlu
- Force push **kapalı**
- Branch silme **kapalı**

### `feature/*` ve `bugfix/*` Branchleri İçin Kurallar

- Branch isimlendirme standardı zorunlu
- Direct push serbest
- `main` branchine PR/MR açılması zorunlu
- CI build çalışmalı (test opsiyonel veya kısmi)
- Merge sonrası branch **otomatik silinir**

### Ek CI/CD Tavsiyeleri

- `main` → full build + unit + integration test
- `feature/*` → build + unit test
- Release tag’leri üzerinden:
  - Binary / package üretimi
  - Conan / vcpkg publish adımı

## Sonuçlar

- Branch protection kuralları GitHub ve GitLab üzerinde standart hale getirilecektir.
- Merge kuralları CI/CD başarısına bağlı olacaktır.
- `main` branchleri her zaman deploy edilebilir veya stabilize edilebilir durumda tutulacaktır.
- Bu kurallar proje büyüdükçe sıkılaştırılabilecek şekilde tasarlanmıştır.