add_library(libdelaybox OBJECT
        delaybox.cc
        delaybox.h
        )
target_include_directories(libdelaybox PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<TARGET_PROPERTY:libcommon,INTERFACE_INCLUDE_DIRECTORIES>
        )
