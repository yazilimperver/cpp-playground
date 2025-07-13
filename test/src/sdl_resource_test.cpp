#include <gtest/gtest.h>
#include <sdl-resource.h>

// Sahte kaynak ve silici (deleter) fonksiyon
struct MockResource {
    int value;
};

static int delete_call_count = 0;
void MockDeleter(MockResource* resource) {
    if (resource) {
        ++delete_call_count;
        delete resource;
    }
}

using Resource = SDLResource<MockResource, MockDeleter>;

class SDLResourceTest : public ::testing::Test {
protected:
    void SetUp() override {
        delete_call_count = 0;
    }
};

TEST_F(SDLResourceTest, ConstructorShouldStoreGivenResource) {
    auto* raw = new MockResource{42};
    Resource res(raw);
    EXPECT_EQ(res.Get(), raw);
    EXPECT_TRUE(res.IsValid());
}

TEST_F(SDLResourceTest, DestructorShouldCallDeleterWhenResourceIsValid) {
    {
        auto* raw = new MockResource{42};
        Resource res(raw);
    }
    EXPECT_EQ(delete_call_count, 1);
}

TEST_F(SDLResourceTest, DestructorShouldNotCallDeleterWhenResourceIsNull) {
    {
        Resource res(nullptr);
    }
    EXPECT_EQ(delete_call_count, 0);
}

TEST_F(SDLResourceTest, MoveConstructorShouldTransferOwnership) {
    auto* raw = new MockResource{42};
    Resource res1(raw);
    Resource res2(std::move(res1));

    EXPECT_EQ(res2.Get(), raw);
    EXPECT_FALSE(res1.IsValid());
    EXPECT_TRUE(res2.IsValid());
}

TEST_F(SDLResourceTest, MoveAssignmentShouldTransferOwnershipAndCallDeleterOnOldResource) {
    auto* raw1 = new MockResource{1};
    auto* raw2 = new MockResource{2};

    Resource res1(raw1);
    Resource res2(raw2);

    res2 = std::move(res1);

    EXPECT_EQ(res2.Get(), raw1);
    EXPECT_FALSE(res1.IsValid());
    EXPECT_TRUE(res2.IsValid());
    EXPECT_EQ(delete_call_count, 1); // raw2 silinmiş olmalı
}

TEST_F(SDLResourceTest, MoveAssignmentShouldHandleSelfAssignmentGracefully) {
    auto* raw = new MockResource{42};
    Resource res(raw);
    res = std::move(res); // kendi kendine move

    EXPECT_EQ(res.Get(), raw); // kaynak kaybolmamalı
    EXPECT_TRUE(res.IsValid());
    EXPECT_EQ(delete_call_count, 0);
}

TEST_F(SDLResourceTest, ReleaseShouldReturnAndClearInternalPointer) {
    auto* raw = new MockResource{123};
    Resource res(raw);

    auto* released = res.Release();

    EXPECT_EQ(released, raw);
    EXPECT_FALSE(res.IsValid());

    // Manuel silmek gerekir çünkü artık RAII dışı
    MockDeleter(released);
    EXPECT_EQ(delete_call_count, 1);
}

TEST_F(SDLResourceTest, BoolConversionShouldReflectValidity) {
    Resource valid(new MockResource{});
    Resource invalid(nullptr);

    EXPECT_TRUE(valid);
    EXPECT_FALSE(invalid);
}
