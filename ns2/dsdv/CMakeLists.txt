add_library(libdsdv OBJECT
            dsdv.cc
            dsdv.h
            rtable.cc
            rtable.h
        )
target_include_directories(libdsdv PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<TARGET_PROPERTY:libcommon,INTERFACE_INCLUDE_DIRECTORIES>
        )
