set(TARGET_NAME sdl3-example-app)

set(TARGET_UNIT_TEST_LIB ${TARGET_NAME}-lib)
add_library(${TARGET_UNIT_TEST_LIB} "")

set_target_properties(${TARGET_UNIT_TEST_LIB}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(${TARGET_UNIT_TEST_LIB}
  PUBLIC  include
  PRIVATE src
)

target_sources(${TARGET_UNIT_TEST_LIB} PRIVATE
    src/components.cpp
    src/event-system.cpp
    src/render-strategies.cpp
    src/renderer.cpp
    src/sdl-application.cpp
    src/graphical-object-factory.cpp
)

add_executable(${TARGET_NAME} src/main.cpp)
target_include_directories(${TARGET_NAME}
  PUBLIC  include
  PRIVATE src
)

target_link_libraries(${TARGET_NAME}
    PRIVATE SDL3::SDL3 ${TARGET_UNIT_TEST_LIB}
)

set_target_properties(${TARGET_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

# Windows için gerekli DLL'leri yuklemek icin ekledigimiz komuttur
if(WIN32)
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:${TARGET_NAME}>
        VERBATIM
    )
endif()