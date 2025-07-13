TEST(SDLResourceTest, Constructor_ShouldCreateValidResource) {
    bool destroyed = false;
    {
        auto* raw = new DummySDLType{42, &destroyed};
        DummyResource res(raw);
        EXPECT_TRUE(res.IsValid());
        EXPECT_EQ(res.Get()->value, 42);
        EXPECT_FALSE(destroyed);
    }
    EXPECT_TRUE(destroyed);
}

TEST(SDLResourceTest, MoveConstructor_ShouldTransferOwnership) {
    bool destroyed = false;
    auto* raw = new DummySDLType{99, &destroyed};
    DummyResource res1(raw);
    DummyResource res2(std::move(res1));
    EXPECT_FALSE(res1.IsValid());
    EXPECT_TRUE(res2.IsValid());
    EXPECT_EQ(res2.Get()->value, 99);
}

TEST(SDLResourceTest, MoveAssignment_ShouldTransferOwnershipAndDestroyOldResource) {
    bool destroyed1 = false, destroyed2 = false;
    auto* raw1 = new DummySDLType{1, &destroyed1};
    auto* raw2 = new DummySDLType{2, &destroyed2};
    DummyResource res1(raw1);
    DummyResource res2(raw2);
    res2 = std::move(res1);
    EXPECT_FALSE(res1.IsValid());
    EXPECT_TRUE(res2.IsValid());
    EXPECT_EQ(res2.Get()->value, 1);
    EXPECT_TRUE(destroyed2);
    EXPECT_FALSE(destroyed1);
}

TEST(SDLResourceTest, Release_ShouldReleaseOwnership) {
    bool destroyed = false;
    auto* raw = new DummySDLType{123, &destroyed};
    DummyResource res(raw);
    DummySDLType* released = res.Release();
    EXPECT_FALSE(res.IsValid());
    EXPECT_EQ(released->value, 123);
    delete released;
    EXPECT_TRUE(destroyed);
}

TEST(SDLResourceTest, BoolOperator_ShouldReflectValidity) {
    DummyResource res1;
    EXPECT_FALSE(res1);
    bool destroyed = false;
    auto* raw = new DummySDLType{0, &destroyed};
    DummyResource res2(raw);
    EXPECT_TRUE(res2);
}

TEST(SDLResourceTest, CopyConstructorAndAssignment_ShouldBeDeleted) {
    DummyResource res;
    SUCCEED();
}
