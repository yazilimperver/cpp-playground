#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "components.h"

// Mock classes for dependencies
class MockRenderer {
public:
    virtual ~MockRenderer() = default;
};

class MockGraphicalObject {
public:
    virtual ~MockGraphicalObject() = default;
};

// Test fixture for Transform tests
class TransformTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockRenderer = std::make_unique<MockRenderer>();
        mockGraphicalObject = std::make_unique<MockGraphicalObject>();
    }

    std::unique_ptr<MockRenderer> mockRenderer;
    std::unique_ptr<MockGraphicalObject> mockGraphicalObject;
};

// Constructor Tests
TEST_F(TransformTest, DefaultConstructorShouldInitializeWithZeroPosition) {
    // Arrange & Act
    Transform transform;
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mX, 0.0f);
    EXPECT_FLOAT_EQ(transform.mY, 0.0f);
    EXPECT_FLOAT_EQ(transform.mRotation, 0.0f);
    EXPECT_FLOAT_EQ(transform.mScaleX, 1.0f);
    EXPECT_FLOAT_EQ(transform.mScaleY, 1.0f);
}

TEST_F(TransformTest, ParameterizedConstructorShouldSetPositionCorrectly) {
    // Arrange
    float expectedX = 10.5f;
    float expectedY = 20.7f;
    
    // Act
    Transform transform(expectedX, expectedY);
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mX, expectedX);
    EXPECT_FLOAT_EQ(transform.mY, expectedY);
    EXPECT_FLOAT_EQ(transform.mRotation, 0.0f);
    EXPECT_FLOAT_EQ(transform.mScaleX, 1.0f);
    EXPECT_FLOAT_EQ(transform.mScaleY, 1.0f);
}

TEST_F(TransformTest, ConstructorShouldAcceptNegativeValues) {
    // Arrange
    float expectedX = -15.3f;
    float expectedY = -25.8f;
    
    // Act
    Transform transform(expectedX, expectedY);
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mX, expectedX);
    EXPECT_FLOAT_EQ(transform.mY, expectedY);
}

TEST_F(TransformTest, ConstructorShouldAcceptZeroValues) {
    // Arrange & Act
    Transform transform(0.0f, 0.0f);
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mX, 0.0f);
    EXPECT_FLOAT_EQ(transform.mY, 0.0f);
}

TEST_F(TransformTest, ConstructorShouldAcceptVeryLargeValues) {
    // Arrange
    float expectedX = 999999.99f;
    float expectedY = 888888.88f;
    
    // Act
    Transform transform(expectedX, expectedY);
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mX, expectedX);
    EXPECT_FLOAT_EQ(transform.mY, expectedY);
}

// Member Variable Modification Tests
TEST_F(TransformTest, PositionShouldBeModifiableAfterConstruction) {
    // Arrange
    Transform transform;
    float newX = 42.5f;
    float newY = 37.2f;
    
    // Act
    transform.mX = newX;
    transform.mY = newY;
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mX, newX);
    EXPECT_FLOAT_EQ(transform.mY, newY);
}

TEST_F(TransformTest, RotationShouldBeModifiableAfterConstruction) {
    // Arrange
    Transform transform;
    float newRotation = 90.0f;
    
    // Act
    transform.mRotation = newRotation;
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mRotation, newRotation);
}

TEST_F(TransformTest, ScaleShouldBeModifiableAfterConstruction) {
    // Arrange
    Transform transform;
    float newScaleX = 2.5f;
    float newScaleY = 0.5f;
    
    // Act
    transform.mScaleX = newScaleX;
    transform.mScaleY = newScaleY;
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mScaleX, newScaleX);
    EXPECT_FLOAT_EQ(transform.mScaleY, newScaleY);
}

TEST_F(TransformTest, AllMembersShouldBeModifiableIndependently) {
    // Arrange
    Transform transform(1.0f, 2.0f);
    
    // Act
    transform.mX = 10.0f;
    transform.mY = 20.0f;
    transform.mRotation = 45.0f;
    transform.mScaleX = 3.0f;
    transform.mScaleY = 0.8f;
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mX, 10.0f);
    EXPECT_FLOAT_EQ(transform.mY, 20.0f);
    EXPECT_FLOAT_EQ(transform.mRotation, 45.0f);
    EXPECT_FLOAT_EQ(transform.mScaleX, 3.0f);
    EXPECT_FLOAT_EQ(transform.mScaleY, 0.8f);
}

// Component Inheritance Tests
TEST_F(TransformTest, ShouldInheritFromComponent) {
    // Arrange
    Transform transform;
    
    // Act & Assert
    Component* component = &transform;
    EXPECT_NE(component, nullptr);
    EXPECT_EQ(component->mOwner, nullptr);
}

TEST_F(TransformTest, ShouldBeAssignableToComponentPointer) {
    // Arrange
    auto transform = std::make_unique<Transform>(5.0f, 10.0f);
    
    // Act
    Component* component = transform.get();
    
    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_EQ(component->mOwner, nullptr);
}

TEST_F(TransformTest, ShouldInheritOwnerFromComponent) {
    // Arrange
    Transform transform;
    auto* graphicalObject = reinterpret_cast<GraphicalObject*>(mockGraphicalObject.get());
    
    // Act
    transform.mOwner = graphicalObject;
    
    // Assert
    EXPECT_EQ(transform.mOwner, graphicalObject);
}

TEST_F(TransformTest, ShouldInheritVirtualMethodsFromComponent) {
    // Arrange
    Transform transform;
    
    // Act & Assert - Should not throw
    EXPECT_NO_THROW({
        transform.Update(16.67f);
        transform.Render(reinterpret_cast<Renderer&>(*mockRenderer));
    });
}

// Edge Cases and Boundary Tests
TEST_F(TransformTest, ShouldHandleVerySmallValues) {
    // Arrange
    float verySmall = 0.000001f;
    
    // Act
    Transform transform(verySmall, verySmall);
    transform.mRotation = verySmall;
    transform.mScaleX = verySmall;
    transform.mScaleY = verySmall;
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mX, verySmall);
    EXPECT_FLOAT_EQ(transform.mY, verySmall);
    EXPECT_FLOAT_EQ(transform.mRotation, verySmall);
    EXPECT_FLOAT_EQ(transform.mScaleX, verySmall);
    EXPECT_FLOAT_EQ(transform.mScaleY, verySmall);
}

TEST_F(TransformTest, ShouldHandleNegativeRotation) {
    // Arrange
    Transform transform;
    float negativeRotation = -180.0f;
    
    // Act
    transform.mRotation = negativeRotation;
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mRotation, negativeRotation);
}

TEST_F(TransformTest, ShouldHandleNegativeScale) {
    // Arrange
    Transform transform;
    float negativeScaleX = -2.0f;
    float negativeScaleY = -1.5f;
    
    // Act
    transform.mScaleX = negativeScaleX;
    transform.mScaleY = negativeScaleY;
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mScaleX, negativeScaleX);
    EXPECT_FLOAT_EQ(transform.mScaleY, negativeScaleY);
}

TEST_F(TransformTest, ShouldHandleZeroScale) {
    // Arrange
    Transform transform;
    
    // Act
    transform.mScaleX = 0.0f;
    transform.mScaleY = 0.0f;
    
    // Assert
    EXPECT_FLOAT_EQ(transform.mScaleX, 0.0f);
    EXPECT_FLOAT_EQ(transform.mScaleY, 0.0f);
}

// Copy and Assignment Tests
TEST_F(TransformTest, CopyConstructorShouldCopyAllMembers) {
    // Arrange
    Transform original(15.0f, 25.0f);
    original.mRotation = 30.0f;
    original.mScaleX = 2.0f;
    original.mScaleY = 1.5f;
    
    // Act
    Transform copy(original);
    
    // Assert
    EXPECT_FLOAT_EQ(copy.mX, original.mX);
    EXPECT_FLOAT_EQ(copy.mY, original.mY);
    EXPECT_FLOAT_EQ(copy.mRotation, original.mRotation);
    EXPECT_FLOAT_EQ(copy.mScaleX, original.mScaleX);
    EXPECT_FLOAT_EQ(copy.mScaleY, original.mScaleY);
}

TEST_F(TransformTest, AssignmentOperatorShouldCopyAllMembers) {
    // Arrange
    Transform original(20.0f, 30.0f);
    original.mRotation = 45.0f;
    original.mScaleX = 3.0f;
    original.mScaleY = 0.5f;
    
    Transform assigned;
    
    // Act
    assigned = original;
    
    // Assert
    EXPECT_FLOAT_EQ(assigned.mX, original.mX);
    EXPECT_FLOAT_EQ(assigned.mY, original.mY);
    EXPECT_FLOAT_EQ(assigned.mRotation, original.mRotation);
    EXPECT_FLOAT_EQ(assigned.mScaleX, original.mScaleX);
    EXPECT_FLOAT_EQ(assigned.mScaleY, original.mScaleY);
}

// Multiple Instance Tests
TEST_F(TransformTest, MultipleInstancesShouldBeIndependent) {
    // Arrange
    Transform transform1(10.0f, 20.0f);
    Transform transform2(30.0f, 40.0f);
    
    // Act
    transform1.mRotation = 90.0f;
    transform2.mRotation = 180.0f;
    
    // Assert
    EXPECT_FLOAT_EQ(transform1.mX, 10.0f);
    EXPECT_FLOAT_EQ(transform1.mY, 20.0f);
    EXPECT_FLOAT_EQ(transform1.mRotation, 90.0f);
    
    EXPECT_FLOAT_EQ(transform2.mX, 30.0f);
    EXPECT_FLOAT_EQ(transform2.mY, 40.0f);
    EXPECT_FLOAT_EQ(transform2.mRotation, 180.0f);
}

// Destructor Test
TEST_F(TransformTest, DestructorShouldNotThrowException) {
    // Arrange & Act & Assert
    EXPECT_NO_THROW({
        auto transform = std::make_unique<Transform>(100.0f, 200.0f);
        // Destructor called automatically when unique_ptr goes out of scope
    });
}

// Polymorphic Destruction Test
TEST_F(TransformTest, PolymorphicDestructionShouldWorkCorrectly) {
    // Arrange
    bool destructorCalled = false;
    
    class TestTransform : public Transform {
    public:
        bool* flag;
        TestTransform(bool* f, float x, float y) : Transform(x, y), flag(f) {}
        ~TestTransform() override { *flag = true; }
    };
    
    // Act
    Component* component = new TestTransform(&destructorCalled, 5.0f, 10.0f);
    delete component;
    
    // Assert
    EXPECT_TRUE(destructorCalled);
}