/** 
 * @file sdl-resource.h
 * @author yazilimperver (yazilimperver@gmail.com)
 * @brief 
 * @date 2025-05-31
 */
#pragma once

#include <SDL3/SDL.h>

/** 
 * @brief SDL API'si tarafından sunulan bir takım kaynaklar için kaynak yönetimi sunan sınıftır.alignas
 *        Sağlanan custom deleter ile ilgili kaynak silinir
 *        Bu sayede kaynaklar, nesne kapsamından çıktığında düzgün bir şekilde serbest bırakılır.
 *        Bu, SDL kaynakları için basit bir RAII sarmalayıcıdır, özel bir deleter işlevi kullanır.
 */
template<typename T, void(*Deleter)(T*)>
class SDLResource {
private:
    T* mResource;

public:
    explicit SDLResource(T* resource = nullptr) : mResource(resource) {}
    
    ~SDLResource() {
        if (mResource) {
            Deleter(mResource);
        }
    }
    
    SDLResource(SDLResource&& other) noexcept : mResource(other.mResource) {
        other.mResource = nullptr;
    }
    
    SDLResource& operator=(SDLResource&& other) noexcept {
        if (this != &other) {
            if (mResource) {
                Deleter(mResource);
            }
            mResource = other.mResource;
            other.mResource = nullptr;
        }
        return *this;
    }
    
    SDLResource(const SDLResource&) = delete;
    SDLResource& operator=(const SDLResource&) = delete;
    
    T* Get() const { return mResource; }
    T* Release() {
        T* temp = mResource;
        mResource = nullptr;
        return temp;
    }
    
    bool IsValid() const { return mResource != nullptr; }
    operator bool() const { return IsValid(); }
};

// Bilindik SDL kaynakları için tanımlamalar
using SDLWindow = SDLResource<SDL_Window, SDL_DestroyWindow>;
using SDLRenderer = SDLResource<SDL_Renderer, SDL_DestroyRenderer>;
using SDLTexture = SDLResource<SDL_Texture, SDL_DestroyTexture>;