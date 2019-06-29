add_library(libtrace OBJECT
        basetrace.cc
        basetrace.h
        cmu-trace.cc
        cmu-trace.h
        trace-ip.cc
        trace.cc
        trace.h
        traffictrace.cc
        )
target_include_directories(libtrace PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<TARGET_PROPERTY:libcommon,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:libtcp,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:libsctp,INTERFACE_INCLUDE_DIRECTORIES>
        )
