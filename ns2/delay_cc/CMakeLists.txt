add_library(libdelay_cc OBJECT
        DelayTransport.h
        DelayTransport.cpp
        hdr_delay.h
        hdr_delay.cpp
        PointerPeer.cpp
        PointerPeer.h
        TimerAdapter.cpp
        TimerAdapter.h
        PriorityQueue.cpp
        PriorityQueue.h DelayAssigner.h FileDelayAssigner.cpp FileDelayAssigner.h PIASDelayAssigner.cpp PIASDelayAssigner.h)
set_property(TARGET libdelay_cc PROPERTY CXX_STANDARD 17)
set_property(TARGET libdelay_cc PROPERTY CXX_STANDARD_REQUIRED TRUE)
target_link_libraries(libdelay_cc PUBLIC libcommon delay_transport)
