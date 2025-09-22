// Copyright (c) OpenAI. All rights reserved.

#ifndef AIRLIB_HEADER_ONLY
#ifndef AIRLIB_NO_RPC

#include "vehicles/fixedwing/api/FixedWingRpcLibClient.hpp"
#include "vehicles/fixedwing/api/FixedWingRpcLibAdaptors.hpp"

#include "common/Common.hpp"
#include "common/ClockFactory.hpp"
#include <thread>
STRICT_MODE_OFF

#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif

#ifdef nil
#undef nil
#endif

#include "common/common_utils/WindowsApisCommonPre.hpp"
#undef FLOAT
#undef check
#include "rpc/client.h"
#ifndef check
#define check(expr) (static_cast<void>((expr)))
#endif
#include "common/common_utils/WindowsApisCommonPost.hpp"

STRICT_MODE_ON
#ifdef _MSC_VER
__pragma(warning(disable : 4239))
#endif

namespace msr { namespace airlib {

typedef msr::airlib_rpclib::FixedWingRpcLibAdaptors FixedWingRpcLibAdaptors;

FixedWingRpcLibClient::FixedWingRpcLibClient(const string& ip_address, uint16_t port, float timeout_sec)
    : RpcLibClientBase(ip_address, port, timeout_sec)
{
}

FixedWingRpcLibClient::~FixedWingRpcLibClient() {}

void FixedWingRpcLibClient::setFixedWingControls(const FixedWingApiBase::Controls& controls, const std::string& vehicle_name)
{
    static_cast<rpc::client*>(getClient())->call("setFixedWingControls", FixedWingRpcLibAdaptors::Controls(controls), vehicle_name);
}

FixedWingApiBase::Controls FixedWingRpcLibClient::getFixedWingControls(const std::string& vehicle_name)
{
    return static_cast<rpc::client*>(getClient())->call("getFixedWingControls", vehicle_name).as<FixedWingRpcLibAdaptors::Controls>().to();
}

FixedWingApiBase::State FixedWingRpcLibClient::getFixedWingState(const std::string& vehicle_name)
{
    return static_cast<rpc::client*>(getClient())->call("getFixedWingState", vehicle_name).as<FixedWingRpcLibAdaptors::State>().to();
}

}} // namespace

#endif
#endif

