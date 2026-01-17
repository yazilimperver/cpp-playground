#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <algorithm>

// Mock SDL_Event yapısı - SDL3'e bağımlılığı kaldırmak için
struct MockSDL_Event {
    uint32_t type;
    uint64_t timestamp;
    // Diğer gerekli alanlar eklenebilir
    
    MockSDL_Event(uint32_t eventType = 0, uint64_t ts = 0) 
        : type(eventType), timestamp(ts) {}
};

// SDL_Event yerine MockSDL_Event kullanmak için typedef
using SDL_Event = MockSDL_Event;

// Test edilecek sınıflar (header dosyasından kopyalandı)
class EventObserver {
public:
    virtual ~EventObserver() = default;
    virtual void OnEvent(const SDL_Event& event) = 0;
};

class EventSubject {
private:
    std::vector<EventObserver*> mObservers;

public:
    void AddObserver(EventObserver* observer) {
        if (observer && std::find(mObservers.begin(), mObservers.end(), observer) == mObservers.end()) {
            mObservers.push_back(observer);
        }
    }
    
    void RemoveObserver(EventObserver* observer) {
        auto it = std::find(mObservers.begin(), mObservers.end(), observer);
        if (it != mObservers.end()) {
            mObservers.erase(it);
        }
    }
    
    void NotifyObservers(const SDL_Event& event) {
        for (auto* observer : mObservers) {
            if (observer) {
                observer->OnEvent(event);
            }
        }
    }
    
    // Test için ek helper metodlar
    size_t GetObserverCount() const { return mObservers.size(); }
};

// Mock Observer sınıfı
class MockEventObserver : public EventObserver {
public:
    MOCK_METHOD(void, OnEvent, (const SDL_Event& event), (override));
};

// Test Fixture
class EventSubjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        subject = std::make_unique<EventSubject>();
        mockObserver1 = std::make_unique<MockEventObserver>();
        mockObserver2 = std::make_unique<MockEventObserver>();
        mockObserver3 = std::make_unique<MockEventObserver>();
    }

    void TearDown() override {
        subject.reset();
        mockObserver1.reset();
        mockObserver2.reset();
        mockObserver3.reset();
    }

    std::unique_ptr<EventSubject> subject;
    std::unique_ptr<MockEventObserver> mockObserver1;
    std::unique_ptr<MockEventObserver> mockObserver2;
    std::unique_ptr<MockEventObserver> mockObserver3;
};

// AddObserver testleri
TEST_F(EventSubjectTest, AddObserverShouldAddSingleObserverSuccessfully) {
    subject->AddObserver(mockObserver1.get());
    
    EXPECT_EQ(subject->GetObserverCount(), 1);
}

TEST_F(EventSubjectTest, AddObserverShouldAddMultipleObserversSuccessfully) {
    subject->AddObserver(mockObserver1.get());
    subject->AddObserver(mockObserver2.get());
    subject->AddObserver(mockObserver3.get());
    
    EXPECT_EQ(subject->GetObserverCount(), 3);
}

TEST_F(EventSubjectTest, AddObserverShouldIgnoreNullPointer) {
    subject->AddObserver(nullptr);
    
    EXPECT_EQ(subject->GetObserverCount(), 0);
}

TEST_F(EventSubjectTest, AddObserverShouldPreventDuplicateObservers) {
    subject->AddObserver(mockObserver1.get());
    subject->AddObserver(mockObserver1.get()); // Aynı observer'ı tekrar ekleme
    
    EXPECT_EQ(subject->GetObserverCount(), 1);
}

// RemoveObserver testleri
TEST_F(EventSubjectTest, RemoveObserverShouldRemoveExistingObserverSuccessfully) {
    subject->AddObserver(mockObserver1.get());
    subject->AddObserver(mockObserver2.get());
    
    subject->RemoveObserver(mockObserver1.get());
    
    EXPECT_EQ(subject->GetObserverCount(), 1);
}

TEST_F(EventSubjectTest, RemoveObserverShouldIgnoreNonExistentObserver) {
    subject->AddObserver(mockObserver1.get());
    
    subject->RemoveObserver(mockObserver2.get()); // Eklenmemiş observer'ı silme
    
    EXPECT_EQ(subject->GetObserverCount(), 1);
}

TEST_F(EventSubjectTest, RemoveObserverShouldIgnoreNullPointer) {
    subject->AddObserver(mockObserver1.get());
    
    subject->RemoveObserver(nullptr);
    
    EXPECT_EQ(subject->GetObserverCount(), 1);
}

TEST_F(EventSubjectTest, RemoveObserverShouldHandleEmptyObserverList) {
    subject->RemoveObserver(mockObserver1.get());
    
    EXPECT_EQ(subject->GetObserverCount(), 0);
}

// NotifyObservers testleri
TEST_F(EventSubjectTest, NotifyObserversShouldCallOnEventForSingleObserver) {
    SDL_Event testEvent(100, 12345);
    subject->AddObserver(mockObserver1.get());
    
    EXPECT_CALL(*mockObserver1, OnEvent(::testing::_))
        .Times(1);
    
    subject->NotifyObservers(testEvent);
}

TEST_F(EventSubjectTest, NotifyObserversShouldCallOnEventForAllObservers) {
    SDL_Event testEvent(200, 67890);
    subject->AddObserver(mockObserver1.get());
    subject->AddObserver(mockObserver2.get());
    subject->AddObserver(mockObserver3.get());
    EXPECT_CALL(*mockObserver1, OnEvent(::testing::_)).Times(1);
    EXPECT_CALL(*mockObserver2, OnEvent(::testing::_)).Times(1);
    EXPECT_CALL(*mockObserver3, OnEvent(::testing::_)).Times(1);
    
    subject->NotifyObservers(testEvent);
}

TEST_F(EventSubjectTest, NotifyObserversShouldHandleEmptyObserverList) {
    SDL_Event testEvent(400, 22222);
    
    // Hiçbir observer eklenmediğinde crash olmamalı
    EXPECT_NO_THROW(subject->NotifyObservers(testEvent));
}

TEST_F(EventSubjectTest, NotifyObserversShouldNotCallRemovedObserver) {
    SDL_Event testEvent(500, 33333);
    subject->AddObserver(mockObserver1.get());
    subject->AddObserver(mockObserver2.get());
    
    // mockObserver1'i kaldır
    subject->RemoveObserver(mockObserver1.get());
    
    // Sadece mockObserver2 çağrılmalı
    EXPECT_CALL(*mockObserver1, OnEvent(::testing::_)).Times(0);
    EXPECT_CALL(*mockObserver2, OnEvent(::testing::_)).Times(1);
    
    subject->NotifyObservers(testEvent);
}

// Entegrasyon testleri
TEST_F(EventSubjectTest, CompleteWorkflowShouldWorkCorrectly) {
    SDL_Event event1(600, 44444);
    SDL_Event event2(700, 55555);
    
    // Observer'ları ekle
    subject->AddObserver(mockObserver1.get());
    subject->AddObserver(mockObserver2.get());
    
    // İlk event'i gönder
    EXPECT_CALL(*mockObserver1, OnEvent(::testing::_)).Times(1);
    EXPECT_CALL(*mockObserver2, OnEvent(::testing::_)).Times(1);
    subject->NotifyObservers(event1);
    
    // Bir observer'ı kaldır
    subject->RemoveObserver(mockObserver1.get());
    
    // İkinci event'i gönder - sadece mockObserver2 çağrılmalı
    EXPECT_CALL(*mockObserver1, OnEvent(::testing::_)).Times(0);
    EXPECT_CALL(*mockObserver2, OnEvent(::testing::_)).Times(1);
    subject->NotifyObservers(event2);
    
    EXPECT_EQ(subject->GetObserverCount(), 1);
}

// Performans/Stress testi
TEST_F(EventSubjectTest, ShouldHandleLargeNumberOfObservers) {
    const int OBSERVER_COUNT = 1000;
    std::vector<std::unique_ptr<MockEventObserver>> observers;
    
    // Çok sayıda observer oluştur ve ekle
    for (int i = 0; i < OBSERVER_COUNT; ++i) {
        auto observer = std::make_unique<MockEventObserver>();
        EXPECT_CALL(*observer, OnEvent(::testing::_)).Times(1);
        subject->AddObserver(observer.get());
        observers.push_back(std::move(observer));
    }
    
    EXPECT_EQ(subject->GetObserverCount(), OBSERVER_COUNT);
    
    SDL_Event testEvent(800, 66666);
    subject->NotifyObservers(testEvent);
}