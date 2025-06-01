#include "sdl-application.h"

Sdl3Application::Sdl3Application() 
    : mLastTime(std::chrono::high_resolution_clock::now()) { 
}

bool Sdl3Application::Initialize() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    mWindow = SDLWindow(SDL_CreateWindow(
        "SDL3 OOP Example",
        800, 600,
        SDL_WINDOW_RESIZABLE
    ));
    
    if (!mWindow) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(mWindow.Get(), nullptr);

    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    Renderer::Initialize(renderer);

    mEventSubject.AddObserver(this);
    
    mGraphicalObjects.push_back(GraphicalObjectFactory::CreateRectangle(400, 300));
    mGraphicalObjects.push_back(GraphicalObjectFactory::CreateCircle(100, 100));
    mGraphicalObjects.push_back(GraphicalObjectFactory::CreateTriangle(300, 50));   
    
    return true;
}

void Sdl3Application::Run() {
    while (mRunning) {
        HandleEvents();
        Update();
        Render();
    }
}

void Sdl3Application::Shutdown() {
    Renderer::Shutdown();
    SDL_Quit();
}

void Sdl3Application::OnEvent(const SDL_Event& event) {
    if (event.type == SDL_EVENT_QUIT) {
        mRunning = false;
    }
    
    if (event.type == SDL_EVENT_KEY_DOWN) {
        HandleKeyDown(event.key);
    }
}

void Sdl3Application::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        mEventSubject.NotifyObservers(event);
    }
}

void Sdl3Application::HandleKeyDown(const SDL_KeyboardEvent& key) {
    auto rectangleObj = mGraphicalObjects[0].get(); // First object is rectangleObj
    auto* velocity = rectangleObj->GetComponent<Velocity>();
    
    if (!velocity) 
        return;
    
    const float speed = 100.0f;
    printf("Key pressed: %s\n", SDL_GetScancodeName(key.scancode));
    
    velocity->mVy = 0.0f; 
    velocity->mVx = 0.0f; 
    
    switch (key.scancode) {
        case SDL_SCANCODE_W: 
            velocity->mVy = -speed; 
            break;

        case SDL_SCANCODE_S: 
            velocity->mVy = speed; 
            break;

        case SDL_SCANCODE_A: 
            velocity->mVx = -speed; 
            break;

        case SDL_SCANCODE_D: 
            velocity->mVx = speed; 
            break;

        case SDL_SCANCODE_ESCAPE: 
            mRunning = false; 
            break;
    }
}

void Sdl3Application::Update() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - mLastTime).count();
    mLastTime = currentTime;
    
    // Update all game objects
    for (auto& obj : mGraphicalObjects) {
        obj->Update(deltaTime);
        
        // Simple movement system
        auto* transform = obj->GetComponent<Transform>();
        auto* velocity = obj->GetComponent<Velocity>();
        
        if (transform && velocity) {
            transform->mX += velocity->mVx * deltaTime;
            transform->mY += velocity->mVy * deltaTime;
            
            // Simple boundary wrapping
            if (transform->mX < 0) transform->mX = 800;
            if (transform->mX > 800) transform->mX = 0;
            if (transform->mY < 0) transform->mY = 600;
            if (transform->mY > 600) transform->mY = 0;
        }
    }
}

void Sdl3Application::Render() {
    auto& renderer = Renderer::Instance();
    renderer.Clear({30, 30, 30, 255}); // Dark gray background
    
    for (auto& obj : mGraphicalObjects) {
        obj->Render(renderer); // Strategy pattern çalışıyor!
    }
    
    renderer.Present();
}