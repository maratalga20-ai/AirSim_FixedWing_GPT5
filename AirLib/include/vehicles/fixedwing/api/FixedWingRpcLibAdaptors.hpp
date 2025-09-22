// Copyright (c) OpenAI. All rights reserved.

#ifndef air_FixedWingRpcLibAdaptors_hpp
#define air_FixedWingRpcLibAdaptors_hpp

#include "common/Common.hpp"
#include "api/RpcLibAdaptorsBase.hpp"
#include "vehicles/fixedwing/api/FixedWingApiBase.hpp"


namespace msr { namespace airlib_rpclib {

class FixedWingRpcLibAdaptors : public RpcLibAdaptorsBase {
public:
    struct Controls {
        float aileron = 0.f;
        float elevator = 0.f;
        float rudder = 0.f;
        float throttle = 0.f;
        float flap = 0.f;
        float speed_brake = 0.f;
        float spoiler = 0.f;
        float left_brake = 0.f;
        float right_brake = 0.f;
        float center_brake = 0.f;
        float parking_brake = 0.f;
        float gear_down = 1.f;

        MSGPACK_DEFINE_MAP(aileron, elevator, rudder, throttle, flap, speed_brake, spoiler, left_brake, right_brake, center_brake, parking_brake, gear_down);

        Controls() {}
        Controls(const msr::airlib::FixedWingApiBase::Controls& s)
        {
            aileron = s.aileron; elevator = s.elevator; rudder = s.rudder; throttle = s.throttle;
            flap = s.flap; speed_brake = s.speed_brake; spoiler = s.spoiler;
            left_brake = s.left_brake; right_brake = s.right_brake; center_brake = s.center_brake; parking_brake = s.parking_brake; gear_down = s.gear_down;
        }
        msr::airlib::FixedWingApiBase::Controls to() const
        {
            msr::airlib::FixedWingApiBase::Controls d;
            d.aileron = aileron; d.elevator = elevator; d.rudder = rudder; d.throttle = throttle;
            d.flap = flap; d.speed_brake = speed_brake; d.spoiler = spoiler;
            d.left_brake = left_brake; d.right_brake = right_brake; d.center_brake = center_brake; d.parking_brake = parking_brake; d.gear_down = gear_down;
            return d;
        }
    };

    struct State {
        KinematicsState kinematics_estimated;
        GeoPoint gps_location;
        uint64_t timestamp = 0;

        float flap = 0.f;
        float speed_brake = 0.f;
        float spoiler = 0.f;
        float left_brake = 0.f;
        float right_brake = 0.f;
        float center_brake = 0.f;
        float parking_brake = 0.f;
        float gear_down = 1.f;
        float roll_trim = 0.f;
        float pitch_trim = 0.f;
        float yaw_trim = 0.f;

        float elevator_pos_deg = 0.f;
        float aileron_left_pos_deg = 0.f;
        float aileron_right_pos_deg = 0.f;
        float rudder_pos_deg = 0.f;

        float cas_kts = 0.f;
        float gs_kts = 0.f;
        float total_velocity_kts = 0.f;
        float alt_asl_ft = 0.f;
        float alt_agl_ft = 0.f;
        float alt_rate_ftps = 0.f;
        float stall_warning = 0.f;

        MSGPACK_DEFINE_MAP(kinematics_estimated, gps_location, timestamp,
            flap, speed_brake, spoiler, left_brake, right_brake, center_brake, parking_brake, gear_down,
            roll_trim, pitch_trim, yaw_trim,
            elevator_pos_deg, aileron_left_pos_deg, aileron_right_pos_deg, rudder_pos_deg,
            cas_kts, gs_kts, total_velocity_kts, alt_asl_ft, alt_agl_ft, alt_rate_ftps, stall_warning);

        State() {}
        State(const msr::airlib::FixedWingApiBase::State& s)
        {
            kinematics_estimated = s.kinematics_estimated; gps_location = s.gps_location; timestamp = s.timestamp;
            flap = s.flap; speed_brake = s.speed_brake; spoiler = s.spoiler; left_brake = s.left_brake; right_brake = s.right_brake; center_brake = s.center_brake; parking_brake = s.parking_brake; gear_down = s.gear_down;
            roll_trim = s.roll_trim; pitch_trim = s.pitch_trim; yaw_trim = s.yaw_trim;
            elevator_pos_deg = s.elevator_pos_deg; aileron_left_pos_deg = s.aileron_left_pos_deg; aileron_right_pos_deg = s.aileron_right_pos_deg; rudder_pos_deg = s.rudder_pos_deg;
            cas_kts = s.cas_kts; gs_kts = s.gs_kts; total_velocity_kts = s.total_velocity_kts; alt_asl_ft = s.alt_asl_ft; alt_agl_ft = s.alt_agl_ft; alt_rate_ftps = s.alt_rate_ftps; stall_warning = s.stall_warning;
        }
        msr::airlib::FixedWingApiBase::State to() const
        {
            msr::airlib::FixedWingApiBase::State d;
            d.kinematics_estimated = kinematics_estimated.to(); d.gps_location = gps_location.to(); d.timestamp = timestamp;
            d.flap = flap; d.speed_brake = speed_brake; d.spoiler = spoiler; d.left_brake = left_brake; d.right_brake = right_brake; d.center_brake = center_brake; d.parking_brake = parking_brake; d.gear_down = gear_down;
            d.roll_trim = roll_trim; d.pitch_trim = pitch_trim; d.yaw_trim = yaw_trim;
            d.elevator_pos_deg = elevator_pos_deg; d.aileron_left_pos_deg = aileron_left_pos_deg; d.aileron_right_pos_deg = aileron_right_pos_deg; d.rudder_pos_deg = rudder_pos_deg;
            d.cas_kts = cas_kts; d.gs_kts = gs_kts; d.total_velocity_kts = total_velocity_kts; d.alt_asl_ft = alt_asl_ft; d.alt_agl_ft = alt_agl_ft; d.alt_rate_ftps = alt_rate_ftps; d.stall_warning = stall_warning;
            return d;
        }
    };
};

}} // namespace

#endif
