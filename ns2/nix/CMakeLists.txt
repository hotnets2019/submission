add_library(libnix OBJECT
        classifier-nix.h
        classifier-nix.cc
        hdr_nv.h
        hdr_nv.cc
        nixnode.h
        nixnode.cc
        nixroute.cc
        nixvec.h
        nixvec.cc
        )
target_include_directories(libnix PUBLIC
        $<TARGET_PROPERTY:libcommon,INTERFACE_INCLUDE_DIRECTORIES>)
