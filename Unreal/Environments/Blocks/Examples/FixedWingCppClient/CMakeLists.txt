cmake_minimum_required(VERSION 3.16)
project(FixedWingCppClient CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure AirLib root (override with -DAIRSIM_ROOT=...)
set(AIRSIM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../Plugins/AirSim/Source/AirLib" CACHE PATH "Path to AirLib root")

message(STATUS "AIRSIM_ROOT = ${AIRSIM_ROOT}")

add_executable(FixedWingCppClient main.cpp)

target_compile_definitions(FixedWingCppClient PRIVATE RPCLIB_MSGPACK=clmdep_msgpack)

target_include_directories(FixedWingCppClient PRIVATE
    "${AIRSIM_ROOT}/include"
    "${AIRSIM_ROOT}/deps/eigen3"            # for Eigen/Dense
    "${AIRSIM_ROOT}/deps/rpclib/include"    # rpclib headers used by AirLib RPC
)

if (WIN32)
    # Pick Debug/Release lib dirs based on build config
    set(LIBCFG $<IF:$<CONFIG:Debug>,Debug,Release>)
    target_link_directories(FixedWingCppClient PRIVATE
        "${AIRSIM_ROOT}/lib/x64/${LIBCFG}"
        "${AIRSIM_ROOT}/deps/rpclib/lib/x64/${LIBCFG}"
    )
    target_link_libraries(FixedWingCppClient PRIVATE AirLib rpc ws2_32)
else()
    # Linux/macOS: statically link if available
    target_link_directories(FixedWingCppClient PRIVATE
        "${AIRSIM_ROOT}/lib"
        "${AIRSIM_ROOT}/deps/rpclib/lib"
    )
    target_link_libraries(FixedWingCppClient PRIVATE AirLib rpc pthread)
endif()
