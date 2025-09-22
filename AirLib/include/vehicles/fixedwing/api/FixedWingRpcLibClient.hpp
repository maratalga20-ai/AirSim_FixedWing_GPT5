// Copyright (c) OpenAI. All rights reserved.

#ifndef air_FixedWingRpcLibClient_hpp
#define air_FixedWingRpcLibClient_hpp

#include "common/Common.hpp"
#include "api/RpcLibClientBase.hpp"
#include "vehicles/fixedwing/api/FixedWingApiBase.hpp"
#include "vehicles/fixedwing/api/FixedWingRpcLibAdaptors.hpp"
#include "rpc/client.h"

namespace msr { namespace airlib {

class FixedWingRpcLibClient : public RpcLibClientBase {
public:
    FixedWingRpcLibClient(const string& ip_address = "localhost", uint16_t port = RpcLibPort, float timeout_sec = 60);
    virtual ~FixedWingRpcLibClient();

    void setFixedWingControls(const FixedWingApiBase::Controls& controls, const std::string& vehicle_name = "");
    FixedWingApiBase::Controls getFixedWingControls(const std::string& vehicle_name = "");
    FixedWingApiBase::State getFixedWingState(const std::string& vehicle_name = "");

    // Convenience wrappers
    void setFlap(float value, const std::string& vehicle_name = "")
    {
        static_cast<rpc::client*>(getClient())->call("setFlap", value, vehicle_name);
    }
    void setGearDown(bool down, const std::string& vehicle_name = "")
    {
        static_cast<rpc::client*>(getClient())->call("setGearDown", down, vehicle_name);
    }
    void setBrakes(float left, float right, float center, float parking, const std::string& vehicle_name = "")
    {
        static_cast<rpc::client*>(getClient())->call("setBrakes", left, right, center, parking, vehicle_name);
    }
    void setSpeedBrake(float value, const std::string& vehicle_name = "")
    {
        static_cast<rpc::client*>(getClient())->call("setSpeedBrake", value, vehicle_name);
    }
    void setSpoiler(float value, const std::string& vehicle_name = "")
    {
        static_cast<rpc::client*>(getClient())->call("setSpoiler", value, vehicle_name);
    }
    void setTrims(float roll, float pitch, float yaw, const std::string& vehicle_name = "")
    {
        static_cast<rpc::client*>(getClient())->call("setTrims", roll, pitch, yaw, vehicle_name);
    }
};

// Header-only inline impl
#ifdef AIRLIB_HEADER_ONLY

inline FixedWingRpcLibClient::FixedWingRpcLibClient(const string& ip_address, uint16_t port, float timeout_sec)
    : RpcLibClientBase(ip_address, port, timeout_sec)
{
}

inline FixedWingRpcLibClient::~FixedWingRpcLibClient() {}

inline void FixedWingRpcLibClient::setFixedWingControls(const FixedWingApiBase::Controls& controls, const std::string& vehicle_name)
{
    static_cast<rpc::client*>(getClient())->call("setFixedWingControls", msr::airlib_rpclib::FixedWingRpcLibAdaptors::Controls(controls), vehicle_name);
}

inline FixedWingApiBase::Controls FixedWingRpcLibClient::getFixedWingControls(const std::string& vehicle_name)
{
    return static_cast<rpc::client*>(getClient())->call("getFixedWingControls", vehicle_name).as<msr::airlib_rpclib::FixedWingRpcLibAdaptors::Controls>().to();
}

inline FixedWingApiBase::State FixedWingRpcLibClient::getFixedWingState(const std::string& vehicle_name)
{
    return static_cast<rpc::client*>(getClient())->call("getFixedWingState", vehicle_name).as<msr::airlib_rpclib::FixedWingRpcLibAdaptors::State>().to();
}

#endif // AIRLIB_HEADER_ONLY

}} // namespace

#endif
