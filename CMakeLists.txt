# Detaylar icin yazilimperver.com 
cmake_minimum_required (VERSION 3.23)

# https://cmake.org/cmake/help/latest/policy/CMP0135.html ilgili policy aktiflestirmek icin
cmake_policy(SET CMP0135 NEW)

# cmake konfigurasyon dizini
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Ortak cmake betiklerini alalim
include(StandardOptions) # Standard ayarlar
include(InsourceGuard) # Source icerisinde buildi onleme

# Proje ismi, surum ve dil ayarlari
project(
    sdl-example
    VERSION 1.0.0
    LANGUAGES C CXX)

# OS Spesifik isler, proje tanimindan sonra olmali
include(TargetOSSetup)
include(CompilerOptions)

# CCache'e yonelik ekleme
find_program(ccache_exe ccache) 

# SDL3'u artık bu şekilde CMake betiklerimize dahil edebiliyor oalcağız
find_package(SDL3 QUIET)
include(Sdl3Finder)

if(ccache_exe )
    message("CCACHE is found")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)

    # CCache icin windows debug ve RelWithDebugInfo ayarlar
    if (MSVC)
        foreach(config DEBUG RELWITHDEBINFO)
            foreach(lang C CXX)
                set(flags_var "CMAKE_${lang}_FLAGS_${config}")
                string(REPLACE "/Zi" "/Z7" ${flags_var} "${${flags_var}}")
                set(${flags_var} "${${flags_var}}")
            endforeach()
        endforeach()
    endif()
else(ccache_exe )
    message("CCACHE is NOT found")
endif(ccache_exe )

# compile_commands.json dosyasi olusturma kabiliyeti eklenir
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Alt projeler
# add_subdirectory(lib)
add_subdirectory(app)