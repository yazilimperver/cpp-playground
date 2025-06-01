/** 
 * @file render-strategies.h
 * @author yazilimperver (yazilimperver@gmail.com)
 * @brief 
 * @date 2025-05-31
 */
#include "sdl-resource.h"

#pragma once

class Transform;

/** 
 * @brief RenderStrategy sınıfı, farklı render stratejilerini temsil eden soyut bir sınıftır.
 * @ref   https://refactoring.guru/design-patterns/strategy/cpp/example
 */
class RenderStrategy {
public:
    virtual ~RenderStrategy() = default;
    virtual void Render(SDL_Renderer* renderer, const Transform& transform) = 0;
};

/** 
 * @brief RectangleRenderer sınıfı, dikdörtgenleri çizmek için kullanılan bir render stratejisidir.
 */
class RectangleRenderer : public RenderStrategy {
private:
    SDL_Color mColor;
    int32_t mWidth, mHeight;
public:
    RectangleRenderer(SDL_Color color, int32_t width, int32_t height);    
    void Render(SDL_Renderer* renderer, const Transform& transform) override;
};

/** 
 * @brief CircleRenderer sınıfı, daireleri çizmek için kullanılan bir render stratejisidir.
 */
class CircleRenderer : public RenderStrategy {
private:
    SDL_Color mColor;
    int32_t mRadius;
public:
    CircleRenderer(SDL_Color color, int32_t radius);    
    void Render(SDL_Renderer* renderer, const Transform& transform) override;
};

/** 
 * @brief TriangleRenderer sınıfı, üçgenleri çizmek için kullanılan bir render stratejisidir.
 */
class TriangleRenderer : public RenderStrategy {
private:
    SDL_FColor mColor;
    float mEdgeLength;

public:
    TriangleRenderer(SDL_FColor color, float size);    
    void Render(SDL_Renderer* renderer, const Transform& transform) override;
};