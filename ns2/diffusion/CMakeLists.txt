add_library(libdiffusion OBJECT
        diff_header.h
        diff_prob.h
        diff_prob.cc
        diff_rate.h
        diff_rate.cc
        diff_sink.h
        diff_sink.cc
        diffusion.h
        diffusion.cc
        flooding.h
        flooding.cc
        hash_table.h
        hash_table.cc
        iflist.h
        iflist.cc
        omni_mcast.h
        omni_mcast.cc
        routing_table.h
        routing_table.cc
        )
target_include_directories(libdiffusion PUBLIC $<TARGET_PROPERTY:libcommon,INTERFACE_INCLUDE_DIRECTORIES>)