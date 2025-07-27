#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "components.h"
#include "render-strategies.h"  // RenderStrategy'nin tam tanımı için

// Forward declaration to avoid including SDL headers
typedef struct SDL_Renderer SDL_Renderer;

// Mock RenderStrategy that matches the real interface
class MockRenderStrategy : public RenderStrategy {
public:
    MOCK_METHOD(void, Render, (SDL_Renderer* renderer, const Transform& transform), (override));
};

// Mock Renderer
class MockRenderer {
public:
    MOCK_METHOD(SDL_Renderer*, GetSDLRenderer, (), ());
};

// Mock GraphicalObject
class MockGraphicalObject {
public:
    std::unique_ptr<Transform> mockTransform;
    
    template<typename T>
    T* GetComponent() {
        if constexpr (std::is_same_v<T, Transform>) {
            return static_cast<T*>(mockTransform.get());
        }
        return nullptr;
    }
};

class RenderComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockRenderer = std::make_unique<MockRenderer>();
        mockGraphicalObject = std::make_unique<MockGraphicalObject>();
        mockGraphicalObject->mockTransform = std::make_unique<Transform>(10.0f, 20.0f);
        
        // Create a fake SDL_Renderer pointer for testing
        fakeSDLRenderer = reinterpret_cast<SDL_Renderer*>(0x12345678);
    }

    std::unique_ptr<MockRenderer> mockRenderer;
    std::unique_ptr<MockGraphicalObject> mockGraphicalObject;
    SDL_Renderer* fakeSDLRenderer;
    
    // Helper to create mock strategy and cast to base type
    std::unique_ptr<RenderStrategy> CreateMockStrategy() {
        return std::unique_ptr<RenderStrategy>(new MockRenderStrategy());
    }
};

// Constructor Tests
TEST_F(RenderComponentTest, ConstructorShouldAcceptStrategy) {
    auto strategy = CreateMockStrategy();
    
    EXPECT_NO_THROW({
        RenderComponent component(std::move(strategy));
    });
}

TEST_F(RenderComponentTest, ConstructorShouldAcceptNullStrategy) {
    EXPECT_NO_THROW({
        RenderComponent component(nullptr);
    });
}

// SetStrategy Tests
TEST_F(RenderComponentTest, SetStrategyShouldReplaceStrategy) {
    auto initialStrategy = CreateMockStrategy();
    RenderComponent component(std::move(initialStrategy));
    
    auto newStrategy = CreateMockStrategy();
    
    EXPECT_NO_THROW({
        component.SetStrategy(std::move(newStrategy));
    });
}

TEST_F(RenderComponentTest, SetStrategyShouldAcceptNull) {
    auto strategy = CreateMockStrategy();
    RenderComponent component(std::move(strategy));
    
    EXPECT_NO_THROW({
        component.SetStrategy(nullptr);
    });
}

// Render Tests  
TEST_F(RenderComponentTest, RenderShouldCallStrategyWhenAllConditionsMet) {
    // Create mock and get pointer before moving
    auto mockStrategyPtr = std::make_unique<MockRenderStrategy>();
    auto* rawMockPtr = mockStrategyPtr.get();
    
    // Move to base class pointer
    std::unique_ptr<RenderStrategy> strategy(mockStrategyPtr.release());
    
    RenderComponent component(std::move(strategy));
    component.mOwner = reinterpret_cast<GraphicalObject*>(mockGraphicalObject.get());
    
    EXPECT_CALL(*mockRenderer, GetSDLRenderer())
        .WillOnce(::testing::Return(fakeSDLRenderer));
    EXPECT_CALL(*rawMockPtr, Render(fakeSDLRenderer, ::testing::_))
        .Times(1);
    
    component.Render(reinterpret_cast<Renderer&>(*mockRenderer));
}

TEST_F(RenderComponentTest, RenderShouldNotCallStrategyWhenStrategyIsNull) {
    RenderComponent component(nullptr);
    component.mOwner = reinterpret_cast<GraphicalObject*>(mockGraphicalObject.get());
    
    EXPECT_NO_THROW({
        component.Render(reinterpret_cast<Renderer&>(*mockRenderer));
    });
}

TEST_F(RenderComponentTest, RenderShouldNotCallStrategyWhenOwnerIsNull) {
    auto strategy = CreateMockStrategy();
    RenderComponent component(std::move(strategy));
    component.mOwner = nullptr;
    
    EXPECT_NO_THROW({
        component.Render(reinterpret_cast<Renderer&>(*mockRenderer));
    });
}

TEST_F(RenderComponentTest, RenderShouldNotCallStrategyWhenTransformIsNull) {
    auto strategy = CreateMockStrategy();
    RenderComponent component(std::move(strategy));
    component.mOwner = reinterpret_cast<GraphicalObject*>(mockGraphicalObject.get());
    
    mockGraphicalObject->mockTransform.reset(); // Remove transform
    
    EXPECT_NO_THROW({
        component.Render(reinterpret_cast<Renderer&>(*mockRenderer));
    });
}

// Integration Test
TEST_F(RenderComponentTest, MultipleRenderCallsShouldWork) {
    // Create mock and get pointer before moving
    auto mockStrategyPtr = std::make_unique<MockRenderStrategy>();
    auto* rawMockPtr = mockStrategyPtr.get();
    
    // Move to base class pointer
    std::unique_ptr<RenderStrategy> strategy(mockStrategyPtr.release());
    
    RenderComponent component(std::move(strategy));
    component.mOwner = reinterpret_cast<GraphicalObject*>(mockGraphicalObject.get());
    
    EXPECT_CALL(*mockRenderer, GetSDLRenderer())
        .Times(2)
        .WillRepeatedly(::testing::Return(fakeSDLRenderer));
    EXPECT_CALL(*rawMockPtr, Render(fakeSDLRenderer, ::testing::_))
        .Times(2);
    
    component.Render(reinterpret_cast<Renderer&>(*mockRenderer));
    component.Render(reinterpret_cast<Renderer&>(*mockRenderer));
}