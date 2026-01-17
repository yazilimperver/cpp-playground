set(TEST_TARGET_NAME sdl3-example-app-tests)

# https://google.github.io/googletest/quickstart-cmake.html'de detaylar mevcut
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

# For Windows: Prevent overriding the parent project's compiler/linker settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Testleri aktiflestirelim
enable_testing()

# Test projesi
add_executable(${TEST_TARGET_NAME} 
    src/sdl-resource-test.cpp
    src/sdl-renderer-test.cpp
    src/sdl-application-test.cpp
    src/components-component-test.cpp
    src/components-transform-test.cpp
    src/components-velocity-test.cpp
    src/event-system-test.cpp
)

# GoogleTest icin en az C++14
target_compile_features(${TEST_TARGET_NAME} PUBLIC cxx_std_17)

# Kapsama analizi gerekli flagler
target_compile_options(${TEST_TARGET_NAME} PRIVATE --coverage)
target_link_options(${TEST_TARGET_NAME} PRIVATE --coverage)

target_link_libraries(${TEST_TARGET_NAME} 
    GTest::gtest_main 
    GTest::gmock
    sdl3-example-app-lib
    SDL3::SDL3 
)

include(GoogleTest)
# Testlerin otomatik olarak bulunması için bu kullanılabilir
gtest_discover_tests(${TEST_TARGET_NAME})

# Ayrı ayrı testleri tanımlamak için
# add_test(NAME SDLResourceTest.ConstructorShouldStoreGivenResource
#          COMMAND ${TEST_TARGET_NAME}
#          --gtest_filter=SDLResourceTest.ConstructorShouldStoreGivenResource)

# add_test(NAME SDLResourceTest.DestructorShouldCallDeleterWhenResourceIsValid
#          COMMAND ${TEST_TARGET_NAME}
#          --gtest_filter=SDLResourceTest.DestructorShouldCallDeleterWhenResourceIsValid)

# add_test(NAME SDLResourceTest.MoveAssignmentShouldTransferOwnershipAndCallDeleterOnOldResource
#          COMMAND ${TEST_TARGET_NAME}
#          --gtest_filter=SDLResourceTest.MoveAssignmentShouldTransferOwnershipAndCallDeleterOnOldResource)

# Etiketleme
# set_tests_properties(SDLResourceTest.ConstructorShouldStoreGivenResource PROPERTIES LABELS "sdl_resource;creation")
# set_tests_properties(SDLResourceTest.DestructorShouldCallDeleterWhenResourceIsValid PROPERTIES LABELS "sdl_resource;destruction")
# set_tests_properties(SDLResourceTest.MoveAssignmentShouldTransferOwnershipAndCallDeleterOnOldResource PROPERTIES LABELS "sdl_resource;move")