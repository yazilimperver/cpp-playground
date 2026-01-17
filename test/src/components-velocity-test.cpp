#include <gtest/gtest.h>
#include <memory>
#include "components.h"

// Test fixture for Velocity tests
class VelocityTest : public ::testing::Test {
protected:
    // Minimal setup since we don't need mocks for this simple class
};

// Constructor Tests
TEST_F(VelocityTest, DefaultConstructorShouldInitializeWithZeroVelocity) {
    // Act
    Velocity velocity;
    
    // Assert
    EXPECT_FLOAT_EQ(velocity.mVx, 0.0f);
    EXPECT_FLOAT_EQ(velocity.mVy, 0.0f);
}

TEST_F(VelocityTest, ParameterizedConstructorShouldSetVelocityCorrectly) {
    // Arrange
    float expectedVx = 10.5f;
    float expectedVy = -20.3f;
    
    // Act
    Velocity velocity(expectedVx, expectedVy);
    
    // Assert
    EXPECT_FLOAT_EQ(velocity.mVx, expectedVx);
    EXPECT_FLOAT_EQ(velocity.mVy, expectedVy);
}

TEST_F(VelocityTest, ConstructorShouldAcceptZeroValues) {
    // Act
    Velocity velocity(0.0f, 0.0f);
    
    // Assert
    EXPECT_FLOAT_EQ(velocity.mVx, 0.0f);
    EXPECT_FLOAT_EQ(velocity.mVy, 0.0f);
}

// Member Variable Tests
TEST_F(VelocityTest, VelocityComponentsShouldBeModifiable) {
    // Arrange
    Velocity velocity;
    float newVx = 42.0f;
    float newVy = -37.5f;
    
    // Act
    velocity.mVx = newVx;
    velocity.mVy = newVy;
    
    // Assert
    EXPECT_FLOAT_EQ(velocity.mVx, newVx);
    EXPECT_FLOAT_EQ(velocity.mVy, newVy);
}

// Component Inheritance Tests
TEST_F(VelocityTest, ShouldInheritFromComponent) {
    // Arrange
    Velocity velocity;
    
    // Act
    Component* component = &velocity;
    
    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_EQ(component->mOwner, nullptr);
}

TEST_F(VelocityTest, ShouldBeAssignableToComponentPointer) {
    // Arrange
    auto velocity = std::make_unique<Velocity>(5.0f, 10.0f);
    
    // Act
    Component* component = velocity.get();
    
    // Assert
    EXPECT_NE(component, nullptr);
}

// Edge Cases
TEST_F(VelocityTest, ShouldHandleNegativeValues) {
    // Act
    Velocity velocity(-15.0f, -25.0f);
    
    // Assert
    EXPECT_FLOAT_EQ(velocity.mVx, -15.0f);
    EXPECT_FLOAT_EQ(velocity.mVy, -25.0f);
}

TEST_F(VelocityTest, ShouldHandleVeryLargeValues) {
    // Act
    Velocity velocity(999999.0f, -888888.0f);
    
    // Assert
    EXPECT_FLOAT_EQ(velocity.mVx, 999999.0f);
    EXPECT_FLOAT_EQ(velocity.mVy, -888888.0f);
}

// Copy Tests
TEST_F(VelocityTest, CopyConstructorShouldCopyAllMembers) {
    // Arrange
    Velocity original(123.45f, -67.89f);
    
    // Act
    Velocity copy(original);
    
    // Assert
    EXPECT_FLOAT_EQ(copy.mVx, original.mVx);
    EXPECT_FLOAT_EQ(copy.mVy, original.mVy);
}

TEST_F(VelocityTest, AssignmentOperatorShouldCopyAllMembers) {
    // Arrange
    Velocity original(99.9f, 11.1f);
    Velocity assigned;
    
    // Act
    assigned = original;
    
    // Assert
    EXPECT_FLOAT_EQ(assigned.mVx, original.mVx);
    EXPECT_FLOAT_EQ(assigned.mVy, original.mVy);
}

// Multiple Instance Test
TEST_F(VelocityTest, MultipleInstancesShouldBeIndependent) {
    // Arrange & Act
    Velocity velocity1(10.0f, 20.0f);
    Velocity velocity2(30.0f, 40.0f);
    
    // Assert
    EXPECT_FLOAT_EQ(velocity1.mVx, 10.0f);
    EXPECT_FLOAT_EQ(velocity1.mVy, 20.0f);
    EXPECT_FLOAT_EQ(velocity2.mVx, 30.0f);
    EXPECT_FLOAT_EQ(velocity2.mVy, 40.0f);
}

// Destructor Test
TEST_F(VelocityTest, DestructorShouldNotThrow) {
    // Act & Assert
    EXPECT_NO_THROW({
        auto velocity = std::make_unique<Velocity>(100.0f, 200.0f);
    });
}