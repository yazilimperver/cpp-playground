#include <cmath>
#include <vector>

#include "render-strategies.h"
#include "components.h"

RectangleRenderer::RectangleRenderer(SDL_Color color, int32_t width, int32_t height) 
        : mColor(color), mWidth(width), mHeight(height) {
}
    
void RectangleRenderer::Render(SDL_Renderer* renderer, const Transform& transform) {
    SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
    SDL_FRect rect = {
        transform.mX - (mWidth * transform.mScaleX) / 2.0f, 
        transform.mY - (mHeight * transform.mScaleY) / 2.0f, 
        mWidth * transform.mScaleX, 
        mHeight * transform.mScaleY
    };
    SDL_RenderFillRect(renderer, &rect);
}

CircleRenderer::CircleRenderer(SDL_Color color, int32_t radius) 
        : mColor(color), mRadius(radius) {
}
    
void CircleRenderer::Render(SDL_Renderer* renderer, const Transform& transform) {
    SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
    
    // Simple circle drawing algorithm
    int32_t scaledRadius = static_cast<int32_t>(mRadius * transform.mScaleX);
    int32_t centerX = static_cast<int32_t>(transform.mX);
    int32_t centerY = static_cast<int32_t>(transform.mY);
    
    for (int32_t y = -scaledRadius; y <= scaledRadius; y++) {
        for (int32_t x = -scaledRadius; x <= scaledRadius; x++) {
            if (x*x + y*y <= scaledRadius*scaledRadius) {
                SDL_RenderPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

TriangleRenderer::TriangleRenderer(SDL_FColor color, float size) 
        : mColor(color), mEdgeLength(size) {
}
    
void TriangleRenderer::Render(SDL_Renderer* renderer, const Transform& transform) {
    SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
    
    float scaledSize = mEdgeLength * transform.mScaleX;
    float centerX = transform.mX;
    float centerY = transform.mY;
    float rotation = transform.mRotation;
    
    // Create vertices for equilateral triangle
    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;
    
    // Triangle vertices (pointing upward by default)
    float height = scaledSize * 0.866f; // sqrt(3)/2 for equilateral triangle
    float halfBase = scaledSize * 0.5f;
    
    // Top vertex
    float topX = centerX;
    float topY = centerY - height * 0.667f; // 2/3 of height from center
    
    // Bottom left vertex
    float leftX = centerX - halfBase;
    float leftY = centerY + height * 0.333f; // 1/3 of height from center
    
    // Bottom right vertex
    float rightX = centerX + halfBase;
    float rightY = centerY + height * 0.333f;
    
    // Apply rotation if needed
    if (rotation != 0.0f) {
        float cosR = cosf(rotation);
        float sinR = sinf(rotation);
        
        // Rotate top vertex
        float tempX = topX - centerX;
        float tempY = topY - centerY;
        topX = centerX + tempX * cosR - tempY * sinR;
        topY = centerY + tempX * sinR + tempY * cosR;
        
        // Rotate left vertex
        tempX = leftX - centerX;
        tempY = leftY - centerY;
        leftX = centerX + tempX * cosR - tempY * sinR;
        leftY = centerY + tempX * sinR + tempY * cosR;
        
        // Rotate right vertex
        tempX = rightX - centerX;
        tempY = rightY - centerY;
        rightX = centerX + tempX * cosR - tempY * sinR;
        rightY = centerY + tempX * sinR + tempY * cosR;
    }
    
    // Create vertices    
    vertices.push_back(SDL_Vertex{
        SDL_FPoint{topX, topY},
        mColor,
        SDL_FPoint{0, 0}
    });
    
    vertices.push_back({
        {leftX, leftY},
        mColor,
        {0, 0}
    });
    
    vertices.push_back({
        {rightX, rightY},
        mColor,
        {0, 0}
    });
    
    // Triangle indices
    indices = {0, 1, 2};
    
    // Render the triangle
    SDL_RenderGeometry(renderer, nullptr, 
                        vertices.data(), static_cast<int>(vertices.size()),
                        indices.data(), static_cast<int>(indices.size()));
}