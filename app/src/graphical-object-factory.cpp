#include "graphical-object-factory.h"
#include "render-strategies.h"

void GraphicalObject::Update(float deltaTime) {
    for (auto& component : mComponents) {
        component->Update(deltaTime);
    }
}

void GraphicalObject::Render(Renderer& renderer) {
    for (auto& component : mComponents) {
        component->Render(renderer);
    }
}

std::unique_ptr<GraphicalObject> GraphicalObjectFactory::CreateRectangle(float x, float y) {
    auto rectangleObj = std::make_unique<GraphicalObject>();
    rectangleObj->AddComponent<Transform>(x, y);
    rectangleObj->AddComponent<Velocity>(0.0f, 0.0f);
    
    auto renderStrategy = std::make_unique<RectangleRenderer>(
        SDL_Color{0, 255, 0, 255}, 50, 50);
    rectangleObj->AddComponent<RenderComponent>(std::move(renderStrategy));
    
    return rectangleObj;
}

std::unique_ptr<GraphicalObject> GraphicalObjectFactory::CreateCircle(float x, float y) {
    auto circleObj = std::make_unique<GraphicalObject>();
    circleObj->AddComponent<Transform>(x, y);
    circleObj->AddComponent<Velocity>(100.0f, 50.0f);
    
    auto renderStrategy = std::make_unique<CircleRenderer>(
        SDL_Color{255, 0, 0, 255}, 25);
    circleObj->AddComponent<RenderComponent>(std::move(renderStrategy));
    
    return circleObj;
}

std::unique_ptr<GraphicalObject> GraphicalObjectFactory::CreateTriangle(float x, float y){
    auto triangleObj = std::make_unique<GraphicalObject>();
    triangleObj->AddComponent<Transform>(x, y);
    triangleObj->AddComponent<Velocity>(-80.0f, 120.0f);
    auto renderStrategy = std::make_unique<TriangleRenderer>(
            SDL_FColor{1.0f, 0, 1.0f, 1.0f}, 90.0f);
      
    triangleObj->AddComponent<RenderComponent>(std::move(renderStrategy));
    
    return triangleObj;
}