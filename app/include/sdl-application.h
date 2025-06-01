/** 
 * @file sdl-application.h
 * @author yazilimperver (yazilimperver@gmail.com)
 * @brief 
 * @date 2025-05-31
 */
#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <SDL3/SDL.h>

#include "sdl-resource.h"
#include "event-system.h"
#include "graphical-object-factory.h"

class Sdl3Application : public EventObserver {
private:
    bool mRunning = true;    
    SDLWindow mWindow;
    EventSubject mEventSubject;
    std::vector<std::unique_ptr<GraphicalObject>> mGraphicalObjects;
    std::chrono::high_resolution_clock::time_point mLastTime;

public:
    Sdl3Application();

    bool Initialize();    
    void Run();    
    void Shutdown();    
    void OnEvent(const SDL_Event& event) override;

private:
    void HandleEvents();    
    void HandleKeyDown(const SDL_KeyboardEvent& key);    
    void Update();
    void Render();
};