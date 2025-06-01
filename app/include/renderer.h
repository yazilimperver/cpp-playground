/** 
 * @file renderer.h
 * @author yazilimperver (yazilimperver@gmail.com)
 * @brief 
 * @date 2025-05-31
 */
#pragma once

#include <memory>

#include "sdl-resource.h"

/** 
 * @brief Renderer sınıfı, SDL_Renderer'ı singleton design pattern'i kullanarak yöneten sınıftır.
 */
class Renderer {
private:
    static std::unique_ptr<Renderer> mInstance;
    SDLRenderer mRenderer;
    
    explicit Renderer(SDL_Renderer* renderer);
public:
    static Renderer& Instance();    
    static bool Initialize(SDL_Renderer* renderer);    
    static void Shutdown();    
    SDL_Renderer* SDLRenderer() const;    
    void Clear(SDL_Color color = {0, 0, 0, 255});    
    void Present();
};