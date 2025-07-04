set(TARGET_NAME sdl3-example-app)

add_executable(${TARGET_NAME} "")

set_target_properties(${TARGET_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_sources(${TARGET_NAME} PRIVATE
    src/main.cpp
    src/components.cpp
    src/event-system.cpp
    src/render-strategies.cpp
    src/renderer.cpp
    src/sdl-application.cpp
    src/graphical-object-factory.cpp
)

target_include_directories(${TARGET_NAME}
  PUBLIC  include
  PRIVATE src
)

target_link_libraries(${TARGET_NAME}
    PRIVATE SDL3::SDL3
)

# Windows için gerekli DLL'leri yuklemek icin ekledigimiz komuttur
if(WIN32)
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:${TARGET_NAME}>
        VERBATIM
    )
endif()