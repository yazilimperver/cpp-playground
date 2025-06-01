if(NOT SDL3_FOUND)
    # Try pkg-config as fallback
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SDL3 QUIET sdl3)
    endif()
    
    # Paketleri bulamazsak aşağıdaki dizinlere bir göz atalım
    if(NOT SDL3_FOUND)
        if(WIN32)
            set(SDL3_SEARCH_PATHS "C:/SDL3" "C:/SDL3-devel" "$ENV{SDL3DIR}")
        else()
            set(SDL3_SEARCH_PATHS "/usr/local" "/usr" "$ENV{SDL3DIR}")
        endif()
        
        find_path(SDL3_INCLUDE_DIR NAMES SDL3/SDL.h PATHS ${SDL3_SEARCH_PATHS} PATH_SUFFIXES include)
        find_library(SDL3_LIBRARY NAMES SDL3 libSDL3 PATHS ${SDL3_SEARCH_PATHS} PATH_SUFFIXES lib lib64)
        
        if(SDL3_INCLUDE_DIR AND SDL3_LIBRARY)
            add_library(SDL3::SDL3 UNKNOWN IMPORTED)
            set_target_properties(SDL3::SDL3 PROPERTIES
                IMPORTED_LOCATION "${SDL3_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIR}"
            )
            set(SDL3_FOUND TRUE)
        endif()
    endif()
endif()

if(NOT SDL3_FOUND)
    message(FATAL_ERROR "SDL3 not found! Please install SDL3 or set SDL3DIR environment variable.")
endif()