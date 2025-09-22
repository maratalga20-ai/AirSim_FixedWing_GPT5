// Copyright (c) OpenAI. All rights reserved.

#ifndef AIRLIB_HEADER_ONLY
#ifndef AIRLIB_NO_RPC

#include "vehicles/fixedwing/api/FixedWingRpcLibServer.hpp"
#include "vehicles/fixedwing/api/FixedWingRpcLibAdaptors.hpp"

#include "common/Common.hpp"
STRICT_MODE_OFF

#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif

#include "common/common_utils/WindowsApisCommonPre.hpp"
#undef FLOAT
#undef check
#include "rpc/server.h"
#ifndef check
#define check(expr) (static_cast<void>((expr)))
#endif
#include "common/common_utils/WindowsApisCommonPost.hpp"

STRICT_MODE_ON

namespace msr { namespace airlib {

typedef msr::airlib_rpclib::FixedWingRpcLibAdaptors FixedWingRpcLibAdaptors;

FixedWingRpcLibServer::FixedWingRpcLibServer(ApiProvider* api_provider, string server_address, uint16_t port)
    : RpcLibServerBase(api_provider, server_address, port)
{
    auto* srv = static_cast<rpc::server*>(getServer());

    srv->bind("getFixedWingState", [&](const std::string& vehicle_name) -> FixedWingRpcLibAdaptors::State {
        return FixedWingRpcLibAdaptors::State(getVehicleApi(vehicle_name)->getState());
    });
    srv->bind("setFixedWingControls", [&](const FixedWingRpcLibAdaptors::Controls& controls, const std::string& vehicle_name) -> void {
        getVehicleApi(vehicle_name)->setControls(controls.to());
    });
    srv->bind("getFixedWingControls", [&](const std::string& vehicle_name) -> FixedWingRpcLibAdaptors::Controls {
        return FixedWingRpcLibAdaptors::Controls(getVehicleApi(vehicle_name)->getControls());
    });

    // Convenience control endpoints
    srv->bind("setFlap", [&](float value, const std::string& vehicle_name) -> void {
        auto c = getVehicleApi(vehicle_name)->getControls(); c.flap = value; getVehicleApi(vehicle_name)->setControls(c);
    });
    srv->bind("setGearDown", [&](bool down, const std::string& vehicle_name) -> void {
        auto c = getVehicleApi(vehicle_name)->getControls(); c.gear_down = down ? 1.f : 0.f; getVehicleApi(vehicle_name)->setControls(c);
    });
    srv->bind("setBrakes", [&](float left, float right, float center, float parking, const std::string& vehicle_name) -> void {
        auto c = getVehicleApi(vehicle_name)->getControls(); c.left_brake = left; c.right_brake = right; c.center_brake = center; c.parking_brake = parking; getVehicleApi(vehicle_name)->setControls(c);
    });
    srv->bind("setSpeedBrake", [&](float value, const std::string& vehicle_name) -> void {
        auto c = getVehicleApi(vehicle_name)->getControls(); c.speed_brake = value; getVehicleApi(vehicle_name)->setControls(c);
    });
    srv->bind("setSpoiler", [&](float value, const std::string& vehicle_name) -> void {
        auto c = getVehicleApi(vehicle_name)->getControls(); c.spoiler = value; getVehicleApi(vehicle_name)->setControls(c);
    });
    srv->bind("setTrims", [&](float roll, float pitch, float yaw, const std::string& vehicle_name) -> void {
        auto c = getVehicleApi(vehicle_name)->getControls(); c.roll_trim = roll; c.pitch_trim = pitch; c.yaw_trim = yaw; getVehicleApi(vehicle_name)->setControls(c);
    });
}

FixedWingRpcLibServer::~FixedWingRpcLibServer() {}

}} // namespace

#endif
#endif
