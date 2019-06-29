add_library(delay_transport SHARED
        DelayTransportImpl.h
        DelayTransportImpl.cpp
        DelayTransport.cpp
        Analyzer.cpp
        Analyzer.h
        ../include/delay_transport/DropInfo.h
        OutboundMessage.cpp
        OutboundMessage.h
        Error.cpp
        ../include/delay_transport/Error.h
        ../include/delay_transport/Environment.h
        ExpirationListener.h
        InboundMessage.h
        InboundMessage.cpp)
target_include_directories(delay_transport PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(delay_transport PUBLIC ${CMAKE_SOURCE_DIR}/include)

