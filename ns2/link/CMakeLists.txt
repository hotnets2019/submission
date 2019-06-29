add_library(liblink OBJECT
        delay.cc
        delay.h
        dynalink.cc
        hackloss.cc
        )
target_include_directories(liblink PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<TARGET_PROPERTY:libcommon,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:libmcast,INTERFACE_INCLUDE_DIRECTORIES>
        )
target_link_libraries(liblink libcommon libmcast)