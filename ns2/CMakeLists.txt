cmake_minimum_required(VERSION 3.13)

project(ns2 LANGUAGES C CXX)

add_compile_definitions(USE_INTERP_RESULT USE_INTERP_ERRORLINE)
add_compile_definitions(HAVE_ARPA_INET_H HAVE_STL)
add_compile_definitions("RANDOM_RETURN_TYPE=long int")

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_SUFFIX)
include("../delay_transport/cmake-build-${BUILD_TYPE_SUFFIX}/delay-transport-exports.cmake")

include(CheckTypeSize)
include(CheckCXXSymbolExists)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

check_type_size(long SIZEOF_LONG)
check_cxx_symbol_exists(strtoll cstdlib HAVE_STRTOLL)
check_cxx_symbol_exists(snprintf cstdio HAVE_SNPRINTF)

if(DEFINED HAVE_STRTOLL)
    add_compile_definitions(HAVE_STRTOLL)
endif()
if(DEFINED HAVE_SNPRINTF)
    add_compile_definitions(HAVE_SNPRINTF)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_options(-ggdb -O0)
endif()

set(CMAKE_CXX_STANDARD 11)


find_package(OTCL REQUIRED)
find_package(TCLCL REQUIRED)
find_package(TCL REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR} 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${TCLCL_INCLUDE_DIRS}
    ${TCL_INCLUDE_PATH}
    ${OTCL_INCLUDE_DIRS})

add_subdirectory(adc)
add_subdirectory(aodv)
add_subdirectory(aomdv)
add_subdirectory(apps)
add_subdirectory(asim)
add_subdirectory(classifier)
add_subdirectory(common)
add_subdirectory(delaybox)
add_subdirectory(delay_cc)
add_subdirectory(diffserv)
add_subdirectory(diffusion)
add_subdirectory(dsdv)
add_subdirectory(dsr)
add_subdirectory(empweb)
add_subdirectory(gaf)
add_subdirectory(imep)
add_subdirectory(link)
add_subdirectory(linkstate)
add_subdirectory(mac)
add_subdirectory(mcast)
add_subdirectory(mobile)
add_subdirectory(mpls)
add_subdirectory(nix)
add_subdirectory(packmime)
add_subdirectory(pgm)
add_subdirectory(plm)
add_subdirectory(pushback)
add_subdirectory(qs)
add_subdirectory(queue)
add_subdirectory(rap)
add_subdirectory(realaudio)
add_subdirectory(routing)
add_subdirectory(routealgo)
add_subdirectory(satellite)
add_subdirectory(sctp)
add_subdirectory(tcp)
add_subdirectory(tmix)
add_subdirectory(tools)
add_subdirectory(tora)
add_subdirectory(trace)
add_subdirectory(webcache)
add_subdirectory(wpan)
add_subdirectory(xcp)

set(NS_TCL_LIB_STL
        ${CMAKE_CURRENT_SOURCE_DIR}/tcl/lib/ns-diffusion.tcl
        ${CMAKE_CURRENT_SOURCE_DIR}/tcl/delaybox/delaybox.tcl
        ${CMAKE_CURRENT_SOURCE_DIR}/tcl/packmime/packmime.tcl
        ${CMAKE_CURRENT_SOURCE_DIR}/tcl/tmix/tmix.tcl
        ${CMAKE_CURRENT_SOURCE_DIR}/tcl/tmix/tmix_delaybox.tcl
        )
STRING(REPLACE ";" "	" NS_TCL_LIB_STL "${NS_TCL_LIB_STL}")


add_custom_command(
        OUTPUT version.c ns_tcl.cc ptypes.cc
        COMMAND ${TCL_TCLSH} ${CMAKE_CURRENT_SOURCE_DIR}/bin/string2c.tcl version_string < ${CMAKE_CURRENT_SOURCE_DIR}/VERSION > version.c
        COMMAND ./ptypes2tcl > ptypes.cc
        COMMAND ${TCL_TCLSH} ${CMAKE_CURRENT_SOURCE_DIR}/bin/tcl-expand.tcl ${CMAKE_CURRENT_SOURCE_DIR}/tcl/lib/ns-lib.tcl ${NS_TCL_LIB_STL} | ${TCLCL2CXX} et_ns_lib > ns_tcl.cc
        DEPENDS
            ptypes2tcl
            ${NS_TCL_LIB_STL}
            ${CMAKE_SOURCE_DIR}/tcl/lib/ns-default.tcl
            ${CMAKE_SOURCE_DIR}/tcl/lib/ns-lib.tcl
            ${CMAKE_SOURCE_DIR}/tcl/lib/ns-route.tcl
        )

add_executable(ptypes2tcl common/ptypes2tcl.cc)
target_include_directories(ptypes2tcl PRIVATE
        $<TARGET_PROPERTY:libcommon,INTERFACE_INCLUDE_DIRECTORIES>
        )

add_executable(ns
        common/main-monolithic.cc
        common/tclAppInit.cc
        version.c
        ns_tcl.cc
        ptypes.cc)
message("Tcl Library: ${TCL_LIBRARY}")
target_link_libraries(ns PRIVATE
        libadc
        libaodv
        libaomdv
        libapps
        libasim
        libclassifier
        libcommon
        libdelay_cc
        libdelaybox
        libdiffserv
        libdsdv
        libdsr
        libempweb
        libimep
        libgaf
        liblinkstate
        libnix
        libtools
        libmobile
        libpushback
        librap
        librealaudio
        libroutealgo
        librouting
        libsatellite
        libtmix
        liblink
        libmac
        libmcast
        libmpls
        libdiffusion
        libqueue
        libpackmime
        libpgm
        libplm
        libsctp
        libtcp
        libtora
        libtools
        libtrace
        libqs
        libwebcache
        libwpan
        libxcp
        ${TCL_LIBRARY} ${TCLCL_LIBRARIES} ${OTCL_LIBRARIES})
target_compile_options(ns PRIVATE)
