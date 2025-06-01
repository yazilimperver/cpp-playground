#include "components.h"
#include "render-strategies.h"
#include "graphical-object-factory.h"

RenderComponent::RenderComponent(std::unique_ptr<RenderStrategy> strategy) 
    : mStrategy(std::move(strategy)) {
        
    }

void RenderComponent::SetStrategy(std::unique_ptr<RenderStrategy> strategy) {
    mStrategy = std::move(strategy);
}

void RenderComponent::Render(Renderer& renderer) {
    if (mStrategy && mOwner) {
        auto* transform = mOwner->GetComponent<Transform>();

        if (transform)
            mStrategy->Render(renderer.GetSDLRenderer(), *transform);
    }
}