cmake_minimum_required(VERSION 3.16)
project(FixedWingMinimalCpp CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Налаштуйте шлях до AirLib за потреби через -DAIRSIM_ROOT=...
set(AIRSIM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../Plugins/AirSim/Source/AirLib" CACHE PATH "Path to AirLib root")
message(STATUS "AIRSIM_ROOT = ${AIRSIM_ROOT}")

add_executable(FixedWingMinimal main.cpp)

target_compile_definitions(FixedWingMinimal PRIVATE RPCLIB_MSGPACK=clmdep_msgpack)

target_include_directories(FixedWingMinimal PRIVATE
    "${AIRSIM_ROOT}/include"
    "${AIRSIM_ROOT}/deps/eigen3"
    "${AIRSIM_ROOT}/deps/rpclib/include"
)

if (WIN32)
    set(LIBCFG $<IF:$<CONFIG:Debug>,Debug,Release>)
    target_link_directories(FixedWingMinimal PRIVATE
        "${AIRSIM_ROOT}/lib/x64/${LIBCFG}"
        "${AIRSIM_ROOT}/deps/rpclib/lib/x64/${LIBCFG}"
    )
    target_link_libraries(FixedWingMinimal PRIVATE AirLib rpc ws2_32)
else()
    target_link_directories(FixedWingMinimal PRIVATE
        "${AIRSIM_ROOT}/lib"
        "${AIRSIM_ROOT}/deps/rpclib/lib"
    )
    target_link_libraries(FixedWingMinimal PRIVATE AirLib rpc pthread)
endif()

