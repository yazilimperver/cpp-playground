#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <stdexcept>

// SDL3 types için forward declarations ve mock types
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Color {
    uint8_t r, g, b, a;
} SDL_Color;

// SDL API Wrapper int32_terface
class ISDLWrapper {
public:
    virtual ~ISDLWrapper() = default;
    virtual int32_t SetRenderDrawColor(SDL_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a) = 0;
    virtual int32_t RenderClear(SDL_Renderer* renderer) = 0;
    virtual void RenderPresent(SDL_Renderer* renderer) = 0;
    virtual int32_t GetRenderDrawColor(SDL_Renderer* renderer, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) = 0;
};

// Mock SDL Wrapper
class MockSDLWrapper : public ISDLWrapper {
public:
    MOCK_METHOD(int32_t, SetRenderDrawColor, (SDL_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a), (override));
    MOCK_METHOD(int32_t, RenderClear, (SDL_Renderer* renderer), (override));
    MOCK_METHOD(void, RenderPresent, (SDL_Renderer* renderer), (override));
    MOCK_METHOD(int32_t, GetRenderDrawColor, (SDL_Renderer* renderer, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a), (override));
};

// Renderer sınıfını mocklanabilir hale getirmek için modifiye edilmesi gereken hali
// (Gerçek implementasyonunuzda ISDLWrapper dependency injection eklemeniz gerekecek)
class TestableRenderer {
private:
    static std::unique_ptr<TestableRenderer> mInstance;
    static std::unique_ptr<ISDLWrapper> mSDLWrapper;
    SDL_Renderer* mRenderer;
    
    explicit TestableRenderer(SDL_Renderer* renderer) : mRenderer(renderer) {}
    
public:
    static TestableRenderer& Instance() {
        if (!mInstance) {
            throw std::runtime_error("Renderer not initialized");
        }
        return *mInstance;
    }
    
    static bool Initialize(SDL_Renderer* renderer, std::unique_ptr<ISDLWrapper> sdlWrapper = nullptr) {
        if (mInstance || !renderer) {
            return false;
        }
        
        mInstance = std::unique_ptr<TestableRenderer>(new TestableRenderer(renderer));
        if (sdlWrapper) {
            mSDLWrapper = std::move(sdlWrapper);
        }
        return true;
    }
    
    static void Shutdown() {
        mInstance.reset();
        mSDLWrapper.reset();
    }
    
    static void SetSDLWrapper(std::unique_ptr<ISDLWrapper> wrapper) {
        mSDLWrapper = std::move(wrapper);
    }
    
    SDL_Renderer* GetSDLRenderer() const {
        return mRenderer;
    }
    
    void Clear(SDL_Color color = {0, 0, 0, 255}) {
        if (mSDLWrapper) {
            mSDLWrapper->SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
            mSDLWrapper->RenderClear(mRenderer);
        }
    }
    
    void Present() {
        if (mSDLWrapper) {
            mSDLWrapper->RenderPresent(mRenderer);
        }
    }
};

// Static member definitions
std::unique_ptr<TestableRenderer> TestableRenderer::mInstance = nullptr;
std::unique_ptr<ISDLWrapper> TestableRenderer::mSDLWrapper = nullptr;

class MockedRendererTest : public ::testing::Test {
protected:
    SDL_Renderer* mockRenderer;
    std::unique_ptr<MockSDLWrapper> mockSDLWrapper;
    MockSDLWrapper* mockSDLWrapperPtr; // Raw point32_ter for setting expectations
    
    void SetUp() override {
        // Mock renderer
        mockRenderer = reinterpret_cast<SDL_Renderer*>(0x12345678);
        
        // Mock SDL wrapper oluştur
        mockSDLWrapper = std::make_unique<MockSDLWrapper>();
        mockSDLWrapperPtr = mockSDLWrapper.get();
    }
    
    void TearDown() override {
        TestableRenderer::Shutdown();
    }
    
    void InitializeRendererWithMock() {
        ASSERT_TRUE(TestableRenderer::Initialize(mockRenderer, std::move(mockSDLWrapper)));
    }
};

// Singleton Pattern Testleri
TEST_F(MockedRendererTest, InitializeShouldReturnTrueWithValidRenderer) {
    bool result = TestableRenderer::Initialize(mockRenderer, std::move(mockSDLWrapper));
    EXPECT_TRUE(result);
}

TEST_F(MockedRendererTest, InitializeShouldReturnFalseWithNullRenderer) {
    bool result = TestableRenderer::Initialize(nullptr, std::move(mockSDLWrapper));
    EXPECT_FALSE(result);
}

TEST_F(MockedRendererTest, InitializeShouldReturnFalseWhenCalledTwice) {
    EXPECT_TRUE(TestableRenderer::Initialize(mockRenderer, std::move(mockSDLWrapper)));
    
    // İkinci kez initialize etmeye çalış
    auto anotherMockWrapper = std::make_unique<MockSDLWrapper>();
    SDL_Renderer* anotherRenderer = reinterpret_cast<SDL_Renderer*>(0x87654321);
    
    bool result = TestableRenderer::Initialize(anotherRenderer, std::move(anotherMockWrapper));
    EXPECT_FALSE(result);
}

TEST_F(MockedRendererTest, InstanceShouldReturnSameObjectAfterInitialization) {
    InitializeRendererWithMock();
    
    TestableRenderer& instance1 = TestableRenderer::Instance();
    TestableRenderer& instance2 = TestableRenderer::Instance();
    
    EXPECT_EQ(&instance1, &instance2);
}

TEST_F(MockedRendererTest, InstanceShouldThrowExceptionWhenNotInitialized) {
    EXPECT_THROW(TestableRenderer::Instance(), std::runtime_error);
}

// SDL Renderer Erişim Testleri
TEST_F(MockedRendererTest, GetSDLRendererShouldReturnCorrectRenderer) {
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    SDL_Renderer* returnedRenderer = renderer.GetSDLRenderer();
    
    EXPECT_EQ(returnedRenderer, mockRenderer);
}

TEST_F(MockedRendererTest, GetSDLRendererShouldReturnNonNullPoint32_ter) {
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    SDL_Renderer* returnedRenderer = renderer.GetSDLRenderer();
    
    EXPECT_NE(returnedRenderer, nullptr);
}

// Clear Fonksiyonu Testleri
TEST_F(MockedRendererTest, ClearShouldCallSDLWithDefaultBlackColor) {
    // Mock expectations
    EXPECT_CALL(*mockSDLWrapperPtr, SetRenderDrawColor(mockRenderer, 0, 0, 0, 255))
        .Times(1)
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockSDLWrapperPtr, RenderClear(mockRenderer))
        .Times(1)
        .WillOnce(::testing::Return(0));
    
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    renderer.Clear(); // Varsayılan siyah renk
}

TEST_F(MockedRendererTest, ClearShouldCallSDLWithCustomColor) {
    SDL_Color customColor = {255, 128, 64, 200};
    
    // Mock expectations
    EXPECT_CALL(*mockSDLWrapperPtr, SetRenderDrawColor(mockRenderer, 255, 128, 64, 200))
        .Times(1)
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockSDLWrapperPtr, RenderClear(mockRenderer))
        .Times(1)
        .WillOnce(::testing::Return(0));
    
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    renderer.Clear(customColor);
}

TEST_F(MockedRendererTest, ClearShouldHandleSDLSetColorFailure) {
    // Mock SDL fonksiyonunun hata döndürmesi
    EXPECT_CALL(*mockSDLWrapperPtr, SetRenderDrawColor(mockRenderer, 0, 0, 0, 255))
        .Times(1)
        .WillOnce(::testing::Return(-1)); // SDL error
    EXPECT_CALL(*mockSDLWrapperPtr, RenderClear(mockRenderer))
        .Times(1)
        .WillOnce(::testing::Return(0));
    
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    
    // Hata durumunda da exception fırlatmamalı
    EXPECT_NO_THROW(renderer.Clear());
}

TEST_F(MockedRendererTest, ClearShouldWorkWithMultipleDifferentColors) {
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    
    // Farklı renkler için expectations
    std::vector<SDL_Color> testColors = {
        {255, 0, 0, 255},     // Kırmızı
        {0, 255, 0, 255},     // Yeşil
        {0, 0, 255, 255},     // Mavi
        {128, 128, 128, 128}  // Gri yarı şeffaf
    };
    
    for (const auto& color : testColors) {
        EXPECT_CALL(*mockSDLWrapperPtr, SetRenderDrawColor(mockRenderer, color.r, color.g, color.b, color.a))
            .Times(1)
            .WillOnce(::testing::Return(0));
        EXPECT_CALL(*mockSDLWrapperPtr, RenderClear(mockRenderer))
            .Times(1)
            .WillOnce(::testing::Return(0));
        
        renderer.Clear(color);
    }
}

// Present Fonksiyonu Testleri
TEST_F(MockedRendererTest, PresentShouldCallSDLRenderPresent) {
    EXPECT_CALL(*mockSDLWrapperPtr, RenderPresent(mockRenderer))
        .Times(1);
    
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    renderer.Present();
}

TEST_F(MockedRendererTest, PresentShouldWorkAfterClearOperation) {
    // Clear için expectations
    EXPECT_CALL(*mockSDLWrapperPtr, SetRenderDrawColor(mockRenderer, 100, 150, 200, 255))
        .Times(1)
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockSDLWrapperPtr, RenderClear(mockRenderer))
        .Times(1)
        .WillOnce(::testing::Return(0));
    
    // Present için expectations
    EXPECT_CALL(*mockSDLWrapperPtr, RenderPresent(mockRenderer))
        .Times(1);
    
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    renderer.Clear({100, 150, 200, 255});
    renderer.Present();
}

// Shutdown Testleri
TEST_F(MockedRendererTest, ShutdownShouldAllowReinitialization) {
    // İlk initialize
    InitializeRendererWithMock();
    
    TestableRenderer& instance1 = TestableRenderer::Instance();
    EXPECT_EQ(instance1.GetSDLRenderer(), mockRenderer);
    
    // Shutdown
    TestableRenderer::Shutdown();
    
    // Tekrar initialize edilebilmeli
    auto newMockWrapper = std::make_unique<MockSDLWrapper>();
    SDL_Renderer* newMockRenderer = reinterpret_cast<SDL_Renderer*>(0x11111111);
    
    EXPECT_TRUE(TestableRenderer::Initialize(newMockRenderer, std::move(newMockWrapper)));
    
    TestableRenderer& instance2 = TestableRenderer::Instance();
    EXPECT_EQ(instance2.GetSDLRenderer(), newMockRenderer);
}

TEST_F(MockedRendererTest, ShutdownShouldMakeInstanceThrowException) {
    InitializeRendererWithMock();
    
    // Instance erişilebilir olmalı
    EXPECT_NO_THROW(TestableRenderer::Instance());
    
    // Shutdown sonrası instance erişimi exception fırlatmalı
    TestableRenderer::Shutdown();
    EXPECT_THROW(TestableRenderer::Instance(), std::runtime_error);
}

TEST_F(MockedRendererTest, ShutdownShouldBeIdempotent) {
    InitializeRendererWithMock();
    
    // Birden fazla shutdown çağrısı sorun çıkarmamalı
    EXPECT_NO_THROW(TestableRenderer::Shutdown());
    EXPECT_NO_THROW(TestableRenderer::Shutdown());
    EXPECT_NO_THROW(TestableRenderer::Shutdown());
}

// Mock int32_teraction Testleri
TEST_F(MockedRendererTest, ClearShouldNotCallSDLWhenWrapperIsNull) {
    // Wrapper olmadan initialize et
    ASSERT_TRUE(TestableRenderer::Initialize(mockRenderer, nullptr));
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    
    // SDL fonksiyonları çağrılmamalı çünkü wrapper null
    EXPECT_NO_THROW(renderer.Clear());
    EXPECT_NO_THROW(renderer.Present());
}

TEST_F(MockedRendererTest, MultipleOperationsShouldCallSDLInCorrectOrder) {
    // Sıralı expectations
    ::testing::InSequence seq;
    
    // İlk clear
    EXPECT_CALL(*mockSDLWrapperPtr, SetRenderDrawColor(mockRenderer, 255, 0, 0, 255))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockSDLWrapperPtr, RenderClear(mockRenderer))
        .WillOnce(::testing::Return(0));
    
    // İlk present
    EXPECT_CALL(*mockSDLWrapperPtr, RenderPresent(mockRenderer));
    
    // İkinci clear
    EXPECT_CALL(*mockSDLWrapperPtr, SetRenderDrawColor(mockRenderer, 0, 255, 0, 255))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockSDLWrapperPtr, RenderClear(mockRenderer))
        .WillOnce(::testing::Return(0));
    
    // İkinci present
    EXPECT_CALL(*mockSDLWrapperPtr, RenderPresent(mockRenderer));
    
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    
    renderer.Clear({255, 0, 0, 255}); // Kırmızı
    renderer.Present();
    renderer.Clear({0, 255, 0, 255}); // Yeşil
    renderer.Present();
}

TEST_F(MockedRendererTest, ClearShouldCallSDLFunctionsExactlyOnce) {
    // Strict mock - tam olarak bir kez çağrılmalı
    EXPECT_CALL(*mockSDLWrapperPtr, SetRenderDrawColor(mockRenderer, 50, 100, 150, 200))
        .Times(::testing::Exactly(1))
        .WillOnce(::testing::Return(0));
    EXPECT_CALL(*mockSDLWrapperPtr, RenderClear(mockRenderer))
        .Times(::testing::Exactly(1))
        .WillOnce(::testing::Return(0));
    
    InitializeRendererWithMock();
    
    TestableRenderer& renderer = TestableRenderer::Instance();
    renderer.Clear({50, 100, 150, 200});
    
    // Mock expectations otomatik olarak verify edilecek
}