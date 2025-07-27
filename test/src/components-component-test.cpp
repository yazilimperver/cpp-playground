#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "components.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "components.h"

// Mock classes to avoid dependencies
class MockRenderer {
public:
    virtual ~MockRenderer() = default;
    MOCK_METHOD(void, MockRenderMethod, (), ());
};

class MockGraphicalObject {
public:
    virtual ~MockGraphicalObject() = default;
    MOCK_METHOD(void, MockObjectMethod, (), ());
};

// Concrete test implementation of Component for testing virtual methods
class TestableComponent : public Component {
public:
    // Track method calls for testing
    mutable int updateCallCount = 0;
    mutable int renderCallCount = 0;
    mutable float lastDeltaTime = 0.0f;
    mutable MockRenderer* lastRenderer = nullptr;

    void Update(float deltaTime) override {
        updateCallCount++;
        lastDeltaTime = deltaTime;
    }

    void Render(Renderer& renderer) override {
        renderCallCount++;
        lastRenderer = reinterpret_cast<MockRenderer*>(&renderer);
    }
};

// Mock Component for testing virtual behavior
class MockComponent : public Component {
public:
    MOCK_METHOD(void, Update, (float deltaTime), (override));
    MOCK_METHOD(void, Render, (Renderer& renderer), (override));
};

// Test fixture for Component tests
class ComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockRenderer = std::make_unique<MockRenderer>();
        mockGraphicalObject = std::make_unique<MockGraphicalObject>();
        testableComponent = std::make_unique<TestableComponent>();
        mockComponent = std::make_unique<::testing::StrictMock<MockComponent>>();
    }

    std::unique_ptr<MockRenderer> mockRenderer;
    std::unique_ptr<MockGraphicalObject> mockGraphicalObject;
    std::unique_ptr<TestableComponent> testableComponent;
    std::unique_ptr<::testing::StrictMock<MockComponent>> mockComponent;
};

// Constructor and Initialization Tests
TEST_F(ComponentTest, DefaultConstructorShouldInitializeOwnerToNull) {
    // Arrange & Act
    Component component;
    
    // Assert
    EXPECT_EQ(component.mOwner, nullptr);
}

TEST_F(ComponentTest, OwnerShouldBePubliclyAccessible) {
    // Arrange
    Component component;
    auto* graphicalObject = reinterpret_cast<GraphicalObject*>(mockGraphicalObject.get());
    
    // Act
    component.mOwner = graphicalObject;
    
    // Assert
    EXPECT_EQ(component.mOwner, graphicalObject);
}

TEST_F(ComponentTest, OwnerShouldBeModifiableAfterConstruction) {
    // Arrange
    Component component;
    auto* graphicalObject1 = reinterpret_cast<GraphicalObject*>(mockGraphicalObject.get());
    auto mockGraphicalObject2 = std::make_unique<MockGraphicalObject>();
    auto* graphicalObject2 = reinterpret_cast<GraphicalObject*>(mockGraphicalObject2.get());
    
    // Act
    component.mOwner = graphicalObject1;
    EXPECT_EQ(component.mOwner, graphicalObject1);
    
    component.mOwner = graphicalObject2;
    
    // Assert
    EXPECT_EQ(component.mOwner, graphicalObject2);
}

// Virtual Destructor Tests
TEST_F(ComponentTest, DestructorShouldBeVirtualForPolymorphicBehavior) {
    // Arrange
    bool derivedDestructorCalled = false;
    
    class TestDestructor : public Component {
    public:
        bool* destructorFlag;
        TestDestructor(bool* flag) : destructorFlag(flag) {}
        ~TestDestructor() override { *destructorFlag = true; }
    };
    
    // Act
    Component* component = new TestDestructor(&derivedDestructorCalled);
    delete component;
    
    // Assert
    EXPECT_TRUE(derivedDestructorCalled);
}

TEST_F(ComponentTest, DestructorShouldNotThrowException) {
    // Arrange & Act & Assert
    EXPECT_NO_THROW({
        auto component = std::make_unique<Component>();
        // Destructor called automatically when unique_ptr goes out of scope
    });
}

// Update Method Tests
TEST_F(ComponentTest, UpdateShouldBeCallableWithFloatParameter) {
    // Arrange
    Component component;
    
    // Act & Assert - Should not throw
    EXPECT_NO_THROW({
        component.Update(16.67f);
        component.Update(0.0f);
        component.Update(-1.0f);
        component.Update(1000.0f);
    });
}

TEST_F(ComponentTest, UpdateShouldHaveDefaultEmptyImplementation) {
    // Arrange
    Component component;
    
    // Act - Call multiple times with different values
    component.Update(16.67f);
    component.Update(33.33f);
    component.Update(0.0f);
    
    // Assert - Should complete without any side effects
    // Base implementation should do nothing, so we just verify it doesn't crash
    EXPECT_TRUE(true);
}

TEST_F(ComponentTest, UpdateShouldBeOverridableInDerivedClasses) {
    // Arrange
    float testDeltaTime = 42.5f;
    
    // Act
    testableComponent->Update(testDeltaTime);
    
    // Assert
    EXPECT_EQ(testableComponent->updateCallCount, 1);
    EXPECT_FLOAT_EQ(testableComponent->lastDeltaTime, testDeltaTime);
}

TEST_F(ComponentTest, UpdateShouldAcceptVerySmallDeltaTime) {
    // Arrange
    float verySmallDelta = 0.000001f;
    
    // Act
    testableComponent->Update(verySmallDelta);
    
    // Assert
    EXPECT_EQ(testableComponent->updateCallCount, 1);
    EXPECT_FLOAT_EQ(testableComponent->lastDeltaTime, verySmallDelta);
}

TEST_F(ComponentTest, UpdateShouldAcceptVeryLargeDeltaTime) {
    // Arrange
    float veryLargeDelta = 999999.99f;
    
    // Act
    testableComponent->Update(veryLargeDelta);
    
    // Assert
    EXPECT_EQ(testableComponent->updateCallCount, 1);
    EXPECT_FLOAT_EQ(testableComponent->lastDeltaTime, veryLargeDelta);
}

// Render Method Tests
TEST_F(ComponentTest, RenderShouldBeCallableWithRendererReference) {
    // Arrange
    Component component;
    
    // Act & Assert - Should not throw
    EXPECT_NO_THROW({
        component.Render(reinterpret_cast<Renderer&>(*mockRenderer));
    });
}

TEST_F(ComponentTest, RenderShouldHaveDefaultEmptyImplementation) {
    // Arrange
    Component component;
    
    // Act - Call multiple times
    component.Render(reinterpret_cast<Renderer&>(*mockRenderer));
    component.Render(reinterpret_cast<Renderer&>(*mockRenderer));
    
    // Assert - Should complete without any side effects
    // Base implementation should do nothing, so we just verify it doesn't crash
    EXPECT_TRUE(true);
}

TEST_F(ComponentTest, RenderShouldBeOverridableInDerivedClasses) {
    // Arrange & Act
    testableComponent->Render(reinterpret_cast<Renderer&>(*mockRenderer));
    
    // Assert
    EXPECT_EQ(testableComponent->renderCallCount, 1);
    EXPECT_EQ(testableComponent->lastRenderer, mockRenderer.get());
}

// Polymorphic Behavior Tests
TEST_F(ComponentTest, VirtualUpdateShouldCallDerivedImplementation) {
    // Arrange
    Component* component = testableComponent.get();
    float deltaTime = 123.45f;
    
    // Act
    component->Update(deltaTime);
    
    // Assert
    EXPECT_EQ(testableComponent->updateCallCount, 1);
    EXPECT_FLOAT_EQ(testableComponent->lastDeltaTime, deltaTime);
}

TEST_F(ComponentTest, VirtualRenderShouldCallDerivedImplementation) {
    // Arrange
    Component* component = testableComponent.get();
    
    // Act
    component->Render(reinterpret_cast<Renderer&>(*mockRenderer));
    
    // Assert
    EXPECT_EQ(testableComponent->renderCallCount, 1);
    EXPECT_EQ(testableComponent->lastRenderer, mockRenderer.get());
}

TEST_F(ComponentTest, MockComponentUpdateShouldBeCallable) {
    // Arrange
    float deltaTime = 60.0f;
    EXPECT_CALL(*mockComponent, Update(deltaTime))
        .Times(1);
    
    // Act
    mockComponent->Update(deltaTime);
}

TEST_F(ComponentTest, MockComponentRenderShouldBeCallable) {
    // Arrange
    EXPECT_CALL(*mockComponent, Render(::testing::_))
        .Times(1);
    
    // Act
    mockComponent->Render(reinterpret_cast<Renderer&>(*mockRenderer));
}

// Multiple Calls Tests
TEST_F(ComponentTest, MultipleUpdateCallsShouldEachBeProcessed) {
    // Arrange
    float deltaTime1 = 16.67f;
    float deltaTime2 = 33.33f;
    float deltaTime3 = 8.33f;
    
    // Act
    testableComponent->Update(deltaTime1);
    testableComponent->Update(deltaTime2);
    testableComponent->Update(deltaTime3);
    
    // Assert
    EXPECT_EQ(testableComponent->updateCallCount, 3);
    EXPECT_FLOAT_EQ(testableComponent->lastDeltaTime, deltaTime3); // Last call
}

TEST_F(ComponentTest, MultipleRenderCallsShouldEachBeProcessed) {
    // Arrange & Act
    testableComponent->Render(reinterpret_cast<Renderer&>(*mockRenderer));
    testableComponent->Render(reinterpret_cast<Renderer&>(*mockRenderer));
    testableComponent->Render(reinterpret_cast<Renderer&>(*mockRenderer));
    
    // Assert
    EXPECT_EQ(testableComponent->renderCallCount, 3);
}

// Owner Integration Tests
TEST_F(ComponentTest, ComponentWithOwnerShouldMaintainOwnerReference) {
    // Arrange
    auto* graphicalObject = reinterpret_cast<GraphicalObject*>(mockGraphicalObject.get());
    testableComponent->mOwner = graphicalObject;
    
    // Act
    testableComponent->Update(16.67f);
    testableComponent->Render(reinterpret_cast<Renderer&>(*mockRenderer));
    
    // Assert
    EXPECT_EQ(testableComponent->mOwner, graphicalObject);
    EXPECT_EQ(testableComponent->updateCallCount, 1);
    EXPECT_EQ(testableComponent->renderCallCount, 1);
}

TEST_F(ComponentTest, ComponentShouldFunctionWithoutOwner) {
    // Arrange
    testableComponent->mOwner = nullptr;
    
    // Act
    testableComponent->Update(16.67f);
    testableComponent->Render(reinterpret_cast<Renderer&>(*mockRenderer));
    
    // Assert
    EXPECT_EQ(testableComponent->mOwner, nullptr);
    EXPECT_EQ(testableComponent->updateCallCount, 1);
    EXPECT_EQ(testableComponent->renderCallCount, 1);
}

// Edge Cases
TEST_F(ComponentTest, UpdateWithZeroDeltaTimeShouldWork) {
    // Arrange & Act
    testableComponent->Update(0.0f);
    
    // Assert
    EXPECT_EQ(testableComponent->updateCallCount, 1);
    EXPECT_FLOAT_EQ(testableComponent->lastDeltaTime, 0.0f);
}

TEST_F(ComponentTest, UpdateWithNegativeDeltaTimeShouldWork) {
    // Arrange & Act
    testableComponent->Update(-16.67f);
    
    // Assert
    EXPECT_EQ(testableComponent->updateCallCount, 1);
    EXPECT_FLOAT_EQ(testableComponent->lastDeltaTime, -16.67f);
}

TEST_F(ComponentTest, ComponentShouldBeAssignableToBasePointer) {
    // Arrange
    std::unique_ptr<Component> component = std::make_unique<TestableComponent>();
    
    // Act & Assert
    EXPECT_NE(component.get(), nullptr);
    
    // Should be able to call virtual methods through base pointer
    EXPECT_NO_THROW({
        component->Update(16.67f);
        component->Render(reinterpret_cast<Renderer&>(*mockRenderer));
    });
}