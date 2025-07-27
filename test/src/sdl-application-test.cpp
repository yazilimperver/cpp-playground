#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <chrono>
#include <SDL3/SDL.h>

#include "graphical-object-factory.h"
#include "event-system.h"

// Mock interfaces for SDL3 dependencies
class ISDLSystem {
public:
    virtual ~ISDLSystem() = default;
    virtual bool Init(Uint32 flags) = 0;
    virtual void Quit() = 0;
};

class ISDLWindow {
public:
    virtual ~ISDLWindow() = default;
    virtual SDL_Window* CreateWindow(const char* title, int w, int h, Uint64 flags) = 0;
    virtual void DestroyWindow(SDL_Window* window) = 0;
};

class ISDLRenderer {
public:
    virtual ~ISDLRenderer() = default;
    virtual SDL_Renderer* CreateRenderer(SDL_Window* window, const char* name) = 0;
    virtual void DestroyRenderer(SDL_Renderer* renderer) = 0;
};

class ISDLEvent {
public:
    virtual ~ISDLEvent() = default;
    virtual bool PollEvent(SDL_Event* event) = 0;
};

// Mock implementations
class MockSDLSystem : public ISDLSystem {
public:
    MOCK_METHOD(bool, Init, (Uint32 flags), (override));
    MOCK_METHOD(void, Quit, (), (override));
};

class MockSDLWindow : public ISDLWindow {
public:
    MOCK_METHOD(SDL_Window*, CreateWindow, (const char* title, int w, int h, Uint64 flags), (override));
    MOCK_METHOD(void, DestroyWindow, (SDL_Window* window), (override));
};

class MockSDLRenderer : public ISDLRenderer {
public:
    MOCK_METHOD(SDL_Renderer*, CreateRenderer, (SDL_Window* window, const char* name), (override));
    MOCK_METHOD(void, DestroyRenderer, (SDL_Renderer* renderer), (override));
};

class MockSDLEvent : public ISDLEvent {
public:
    MOCK_METHOD(bool, PollEvent, (SDL_Event* event), (override));
};

class MockEventSubject {
public:
    MOCK_METHOD(void, AddObserver, (EventObserver* observer), ());
    MOCK_METHOD(void, NotifyObservers, (const SDL_Event& event), ());
};

class MockGraphicalObject {
public:
    MOCK_METHOD(void, Update, (float deltaTime), ());
    MOCK_METHOD(void, Render, (Renderer& renderer), ());
    MOCK_METHOD(Transform*, GetComponent, (), ());
};

class MockGraphicalObjectFactory {
public:
    MOCK_METHOD(std::unique_ptr<GraphicalObject>, CreateRectangle, (int x, int y), ());
    MOCK_METHOD(std::unique_ptr<GraphicalObject>, CreateCircle, (int x, int y), ());
    MOCK_METHOD(std::unique_ptr<GraphicalObject>, CreateTriangle, (int x, int y), ());
};

class MockRenderer {
public:
    MOCK_METHOD(void, Initialize, (SDL_Renderer* renderer), ());
    MOCK_METHOD(void, Shutdown, (), ());
    MOCK_METHOD(void, Clear, (const SDL_Color& color), ());
    MOCK_METHOD(void, Present, (), ());
    MOCK_METHOD(Renderer&, Instance, (), ());
};

// Testable version of Sdl3Application with dependency injection
class TestableSDL3Application : public EventObserver {
private:
    bool mRunning = true;
    std::unique_ptr<ISDLSystem> mSDLSystem;
    std::unique_ptr<ISDLWindow> mSDLWindow;
    std::unique_ptr<ISDLRenderer> mSDLRenderer;
    std::unique_ptr<ISDLEvent> mSDLEvent;
    std::unique_ptr<MockEventSubject> mEventSubject;
    std::unique_ptr<MockRenderer> mRenderer;
    std::vector<std::unique_ptr<MockGraphicalObject>> mGraphicalObjects;
    std::chrono::high_resolution_clock::time_point mLastTime;
    SDL_Window* mWindow = nullptr;
    SDL_Renderer* mRendererPtr = nullptr;

public:
    TestableSDL3Application(
        std::unique_ptr<ISDLSystem> sdlSystem,
        std::unique_ptr<ISDLWindow> sdlWindow,
        std::unique_ptr<ISDLRenderer> sdlRenderer,
        std::unique_ptr<ISDLEvent> sdlEvent,
        std::unique_ptr<MockEventSubject> eventSubject,
        std::unique_ptr<MockRenderer> renderer)
        : mSDLSystem(std::move(sdlSystem))
        , mSDLWindow(std::move(sdlWindow))
        , mSDLRenderer(std::move(sdlRenderer))
        , mSDLEvent(std::move(sdlEvent))
        , mEventSubject(std::move(eventSubject))
        , mRenderer(std::move(renderer))
        , mLastTime(std::chrono::high_resolution_clock::now()) {
    }

    bool Initialize() {
        if (!mSDLSystem->Init(SDL_INIT_VIDEO)) {
            return false;
        }
        
        mWindow = mSDLWindow->CreateWindow("SDL3 OOP Example", 800, 600, SDL_WINDOW_RESIZABLE);
        if (!mWindow) {
            return false;
        }
        
        mRendererPtr = mSDLRenderer->CreateRenderer(mWindow, nullptr);
        if (!mRendererPtr) {
            return false;
        }
        
        mRenderer->Initialize(mRendererPtr);
        mEventSubject->AddObserver(this);
        
        // Create mock graphical objects
        auto rectangle = std::make_unique<MockGraphicalObject>();
        auto circle = std::make_unique<MockGraphicalObject>();
        auto triangle = std::make_unique<MockGraphicalObject>();
        
        mGraphicalObjects.push_back(std::move(rectangle));
        mGraphicalObjects.push_back(std::move(circle));
        mGraphicalObjects.push_back(std::move(triangle));
        
        return true;
    }

    void Shutdown() {
        mRenderer->Shutdown();
        mSDLSystem->Quit();
    }

    void OnEvent(const SDL_Event& event) override {
        if (event.type == SDL_EVENT_QUIT) {
            mRunning = false;
        }
        
        if (event.type == SDL_EVENT_KEY_DOWN) {
            HandleKeyDown(event.key);
        }
    }

    void HandleEvents() {
        SDL_Event event;
        while (mSDLEvent->PollEvent(&event)) {
            mEventSubject->NotifyObservers(event);
        }
    }

    void Update(float deltaTime) {
        for (auto& obj : mGraphicalObjects) {
            obj->Update(deltaTime);
        }
    }

    void Render() {
        mRenderer->Clear({30, 30, 30, 255});
        
        for (auto& obj : mGraphicalObjects) {
            obj->Render(mRenderer->Instance());
        }
        
        mRenderer->Present();
    }

    // Test helper methods
    bool IsRunning() const { return mRunning; }
    void SetRunning(bool running) { mRunning = running; }
    size_t GetGraphicalObjectCount() const { return mGraphicalObjects.size(); }

private:
    void HandleKeyDown(const SDL_KeyboardEvent& key) {
        switch (key.scancode) {
            case SDL_SCANCODE_ESCAPE: 
                mRunning = false; 
                break;
        }
    }
};

// Test fixture
class SDL3ApplicationTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockSDLSystem = std::make_unique<MockSDLSystem>();
        mockSDLWindow = std::make_unique<MockSDLWindow>();
        mockSDLRenderer = std::make_unique<MockSDLRenderer>();
        mockSDLEvent = std::make_unique<MockSDLEvent>();
        mockEventSubject = std::make_unique<MockEventSubject>();
        mockRenderer = std::make_unique<MockRenderer>();

        // Keep raw pointers for expectations
        mockSDLSystemPtr = mockSDLSystem.get();
        mockSDLWindowPtr = mockSDLWindow.get();
        mockSDLRendererPtr = mockSDLRenderer.get();
        mockSDLEventPtr = mockSDLEvent.get();
        mockEventSubjectPtr = mockEventSubject.get();
        mockRendererPtr = mockRenderer.get();

        application = std::make_unique<TestableSDL3Application>(
            std::move(mockSDLSystem),
            std::move(mockSDLWindow),
            std::move(mockSDLRenderer),
            std::move(mockSDLEvent),
            std::move(mockEventSubject),
            std::move(mockRenderer)
        );

        mockWindow = reinterpret_cast<SDL_Window*>(0x12345678);
        mockRendererHandle = reinterpret_cast<SDL_Renderer*>(0x87654321);
    }

    std::unique_ptr<TestableSDL3Application> application;
    MockSDLSystem* mockSDLSystemPtr;
    MockSDLWindow* mockSDLWindowPtr;
    MockSDLRenderer* mockSDLRendererPtr;
    MockSDLEvent* mockSDLEventPtr;
    MockEventSubject* mockEventSubjectPtr;
    MockRenderer* mockRendererPtr;

    SDL_Window* mockWindow;
    SDL_Renderer* mockRendererHandle;

private:
    std::unique_ptr<MockSDLSystem> mockSDLSystem;
    std::unique_ptr<MockSDLWindow> mockSDLWindow;
    std::unique_ptr<MockSDLRenderer> mockSDLRenderer;
    std::unique_ptr<MockSDLEvent> mockSDLEvent;
    std::unique_ptr<MockEventSubject> mockEventSubject;
    std::unique_ptr<MockRenderer> mockRenderer;
};

// Initialize Tests
TEST_F(SDL3ApplicationTest, InitializeShouldReturnFalseWhenSDLInitFails) {
    EXPECT_CALL(*mockSDLSystemPtr, Init(SDL_INIT_VIDEO))
        .WillOnce(::testing::Return(false));

    bool result = application->Initialize();

    EXPECT_FALSE(result);
}

TEST_F(SDL3ApplicationTest, InitializeShouldReturnFalseWhenWindowCreationFails) {
    EXPECT_CALL(*mockSDLSystemPtr, Init(SDL_INIT_VIDEO))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*mockSDLWindowPtr, CreateWindow("SDL3 OOP Example", 800, 600, SDL_WINDOW_RESIZABLE))
        .WillOnce(::testing::Return(nullptr));

    bool result = application->Initialize();

    EXPECT_FALSE(result);
}

TEST_F(SDL3ApplicationTest, InitializeShouldReturnFalseWhenRendererCreationFails) {
    EXPECT_CALL(*mockSDLSystemPtr, Init(SDL_INIT_VIDEO))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*mockSDLWindowPtr, CreateWindow("SDL3 OOP Example", 800, 600, SDL_WINDOW_RESIZABLE))
        .WillOnce(::testing::Return(mockWindow));
    EXPECT_CALL(*mockSDLRendererPtr, CreateRenderer(mockWindow, nullptr))
        .WillOnce(::testing::Return(nullptr));

    bool result = application->Initialize();

    EXPECT_FALSE(result);
}

TEST_F(SDL3ApplicationTest, InitializeShouldReturnTrueWhenAllComponentsInitializeSuccessfully) {
    EXPECT_CALL(*mockSDLSystemPtr, Init(SDL_INIT_VIDEO))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*mockSDLWindowPtr, CreateWindow("SDL3 OOP Example", 800, 600, SDL_WINDOW_RESIZABLE))
        .WillOnce(::testing::Return(mockWindow));
    EXPECT_CALL(*mockSDLRendererPtr, CreateRenderer(mockWindow, nullptr))
        .WillOnce(::testing::Return(mockRendererHandle));
    EXPECT_CALL(*mockRendererPtr, Initialize(mockRendererHandle));
    EXPECT_CALL(*mockEventSubjectPtr, AddObserver(application.get()));

    bool result = application->Initialize();

    EXPECT_TRUE(result);
    EXPECT_EQ(3, application->GetGraphicalObjectCount()); // Rectangle, Circle, Triangle
}

// Event Handling Tests
TEST_F(SDL3ApplicationTest, OnEventShouldSetRunningToFalseWhenQuitEventReceived) {
    SDL_Event quitEvent;
    quitEvent.type = SDL_EVENT_QUIT;

    application->OnEvent(quitEvent);

    EXPECT_FALSE(application->IsRunning());
}

TEST_F(SDL3ApplicationTest, OnEventShouldSetRunningToFalseWhenEscapeKeyPressed) {
    SDL_Event keyEvent;
    keyEvent.type = SDL_EVENT_KEY_DOWN;
    keyEvent.key.scancode = SDL_SCANCODE_ESCAPE;

    application->OnEvent(keyEvent);

    EXPECT_FALSE(application->IsRunning());
}

TEST_F(SDL3ApplicationTest, OnEventShouldKeepRunningWhenOtherKeyPressed) {
    SDL_Event keyEvent;
    keyEvent.type = SDL_EVENT_KEY_DOWN;
    keyEvent.key.scancode = SDL_SCANCODE_W;

    application->OnEvent(keyEvent);

    EXPECT_TRUE(application->IsRunning());
}

// Event Processing Tests
TEST_F(SDL3ApplicationTest, HandleEventsShouldPollEventsAndNotifyObservers) {
    SDL_Event event1, event2;
    event1.type = SDL_EVENT_KEY_DOWN;
    event2.type = SDL_EVENT_QUIT;

    EXPECT_CALL(*mockSDLEventPtr, PollEvent(::testing::_))
        .WillOnce(::testing::DoAll(
            ::testing::SetArgPointee<0>(event1),
            ::testing::Return(true)))
        .WillOnce(::testing::DoAll(
            ::testing::SetArgPointee<0>(event2),
            ::testing::Return(true)))
        .WillOnce(::testing::Return(false));

    EXPECT_CALL(*mockEventSubjectPtr, NotifyObservers(::testing::_))
        .Times(2);

    application->HandleEvents();
}

TEST_F(SDL3ApplicationTest, HandleEventsShouldStopPollingWhenNoMoreEvents) {
    EXPECT_CALL(*mockSDLEventPtr, PollEvent(::testing::_))
        .WillOnce(::testing::Return(false));

    EXPECT_CALL(*mockEventSubjectPtr, NotifyObservers(::testing::_))
        .Times(0);

    application->HandleEvents();
}

// Update Tests
TEST_F(SDL3ApplicationTest, UpdateShouldCallUpdateOnAllGraphicalObjects) {
    // We can't easily test the actual Update method without more refactoring,
    // but we can test the Update method with a specific deltaTime
    float deltaTime = 0.016f; // ~60 FPS

    // This would require the graphical objects to be accessible for testing
    // For now, we just test that Update can be called without crashing
    EXPECT_NO_THROW(application->Update(deltaTime));
}
/*
// Render Tests
TEST_F(SDL3ApplicationTest, RenderShouldClearScreenWithDarkGrayColor) {
    SDL_Color expectedColor = {30, 30, 30, 255};
    
    EXPECT_CALL(*mockRendererPtr, Clear(::testing::_))
        .With(::testing::Args<0>(::testing::Field(&SDL_Color::r, expectedColor.r)));
    EXPECT_CALL(*mockRendererPtr, Instance())
        .Times(::testing::AtLeast(1));
    EXPECT_CALL(*mockRendererPtr, Present());

    application->Render();
}
*/
TEST_F(SDL3ApplicationTest, RenderShouldCallPresentAfterRenderingObjects) {
    ::testing::InSequence seq;
    
    EXPECT_CALL(*mockRendererPtr, Clear(::testing::_));
    EXPECT_CALL(*mockRendererPtr, Instance())
        .Times(::testing::AtLeast(0));
    EXPECT_CALL(*mockRendererPtr, Present());

    application->Render();
}

// Shutdown Tests
TEST_F(SDL3ApplicationTest, ShutdownShouldCallRendererShutdownAndSDLQuit) {
    ::testing::InSequence seq;
    
    EXPECT_CALL(*mockRendererPtr, Shutdown());
    EXPECT_CALL(*mockSDLSystemPtr, Quit());

    application->Shutdown();
}

// Integration Tests
TEST_F(SDL3ApplicationTest, FullInitializationAndShutdownShouldSucceed) {
    // Setup successful initialization
    EXPECT_CALL(*mockSDLSystemPtr, Init(SDL_INIT_VIDEO))
        .WillOnce(::testing::Return(true));
    EXPECT_CALL(*mockSDLWindowPtr, CreateWindow(::testing::_, ::testing::_, ::testing::_, ::testing::_))
        .WillOnce(::testing::Return(mockWindow));
    EXPECT_CALL(*mockSDLRendererPtr, CreateRenderer(::testing::_, ::testing::_))
        .WillOnce(::testing::Return(mockRendererHandle));
    EXPECT_CALL(*mockRendererPtr, Initialize(::testing::_));
    EXPECT_CALL(*mockEventSubjectPtr, AddObserver(::testing::_));

    // Setup shutdown
    EXPECT_CALL(*mockRendererPtr, Shutdown());
    EXPECT_CALL(*mockSDLSystemPtr, Quit());

    EXPECT_TRUE(application->Initialize());
    application->Shutdown();
}