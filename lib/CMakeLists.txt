set(TARGET_NAME sdl3-lib)

include(GenerateExportHeader)

add_library(${TARGET_NAME} STATIC "")


set_target_properties( ${TARGET_NAME}
    PROPERTIES
        OUTPUT_NAME ${TARGET_NAME}_lib
        EXPORT_NAME ${TARGET_NAME}_lib
        CXX_EXTENSIONS OFF)

set_target_properties(${TARGET_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

# IDE'lere kolaylik olmasi acisindan bu sekilde eklemekte fayda var
target_sources(${TARGET_NAME}
    PRIVATE
        src/sdl_lib.cpp
    PUBLIC
        include/sdl_control.h
        include/sdl_lib.h
        include/sdl_resource.h
)

target_include_directories(${TARGET_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/exports>
    include
  PRIVATE 
    src
)

target_link_libraries(${TARGET_NAME} PRIVATE SDL3::SDL3)