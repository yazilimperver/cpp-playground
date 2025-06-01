#include "renderer.h"

#include <SDL3/SDL.h>
#include <stdexcept>

std::unique_ptr<Renderer> Renderer::mInstance = nullptr;

Renderer::Renderer(SDL_Renderer* renderer) 
    : mRenderer(renderer) {}

Renderer& Renderer::Instance() {
    if (!mInstance) {
        throw std::runtime_error("Renderer not Initialized");
    }
    return *mInstance;
}

bool Renderer::Initialize(SDL_Renderer* renderer) {
    if (!mInstance) {
        mInstance = std::unique_ptr<Renderer>(new Renderer(renderer));
        return true;
    }
    return false;
}

void Renderer::Shutdown() {
    mInstance.reset();
}

SDL_Renderer* Renderer::SDLRenderer() const { 
    return mRenderer.Get(); 
}

void Renderer::Clear(SDL_Color color) {
    SDL_SetRenderDrawColor(mRenderer.Get(), color.r, color.g, color.b, color.a);
    SDL_RenderClear(mRenderer.Get());
}

void Renderer::Present() {
    SDL_RenderPresent(mRenderer.Get());
}
