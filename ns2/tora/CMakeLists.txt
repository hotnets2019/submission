add_library(libtora OBJECT
            tora.cc
            tora.h
            tora_api.cc
            tora_dest.cc
            tora_dest.h
            tora_io.cc
            tora_logs.cc
            tora_neighbor.cc
            tora_neighbor.h
            tora_packet.h
        )
target_include_directories(libtora PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        $<TARGET_PROPERTY:libcommon,INTERFACE_INCLUDE_DIRECTORIES>
        )
