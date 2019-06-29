add_library(libtmix OBJECT
            tmix.cc
            tmix.h
            tmix_delaybox.cc
            tmix_delaybox.h
        )
target_include_directories(libtmix PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<TARGET_PROPERTY:libcommon,INTERFACE_INCLUDE_DIRECTORIES>
        )
