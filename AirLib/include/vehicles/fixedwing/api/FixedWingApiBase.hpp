// Copyright (c) OpenAI. All rights reserved.

#ifndef air_FixedWingApiBase_hpp
#define air_FixedWingApiBase_hpp

#include "api/VehicleApiBase.hpp"
#include "sensors/SensorCollection.hpp"
#include "sensors/SensorFactory.hpp"

namespace msr { namespace airlib {

class FixedWingApiBase : public VehicleApiBase {
public:
    struct Controls {
        float aileron = 0.f;       // [-1..1]
        float elevator = 0.f;      // [-1..1]
        float rudder = 0.f;        // [-1..1]
        float throttle = 0.f;      // [0..1]
        float flap = 0.f;          // [0..1]
        float speed_brake = 0.f;   // [0..1]
        float spoiler = 0.f;       // [0..1]
        float left_brake = 0.f;    // [0..1]
        float right_brake = 0.f;   // [0..1]
        float center_brake = 0.f;  // [0..1]
        float parking_brake = 0.f; // [0..1]
        float gear_down = 1.f;     // 1 down, 0 up

        // trims (for convenience RPC endpoints)
        float roll_trim = 0.f;     // [-1..1]
        float pitch_trim = 0.f;    // [-1..1]
        float yaw_trim = 0.f;      // [-1..1]
    };

    struct State {
        Kinematics::State kinematics_estimated;
        GeoPoint gps_location;
        uint64_t timestamp = 0;

        // Commands snapshot
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

        // Articulations (degrees)
        float elevator_pos_deg = 0.f;
        float aileron_left_pos_deg = 0.f;
        float aileron_right_pos_deg = 0.f;
        float rudder_pos_deg = 0.f;

        // Basic speeds/altitudes from FDM (units as provided)
        float cas_kts = 0.f;           // Calibrated AirSpeed
        float gs_kts = 0.f;            // Ground speed
        float total_velocity_kts = 0.f;
        float alt_asl_ft = 0.f;        // altitude above sea level
        float alt_agl_ft = 0.f;        // altitude above ground
        float alt_rate_ftps = 0.f;     // climb rate
        float stall_warning = 0.f;
    };

public:
    FixedWingApiBase(const AirSimSettings::VehicleSetting* vehicle_setting,
                     std::shared_ptr<SensorFactory> sensor_factory,
                     const Kinematics::State& state, const Environment& environment)
    {
        initialize(vehicle_setting, sensor_factory, state, environment);
    }

    virtual ~FixedWingApiBase() = default;

    // sensor helpers
    virtual const SensorCollection& getSensors() const override { return sensors_; }
    SensorCollection& getSensors() { return sensors_; }

    void initialize(const AirSimSettings::VehicleSetting* vehicle_setting,
                    std::shared_ptr<SensorFactory> sensor_factory,
                    const Kinematics::State& state, const Environment& environment)
    {
        sensor_factory_ = sensor_factory;
        sensor_storage_.clear();
        sensors_.clear();
        addSensorsFromSettings(vehicle_setting);
        getSensors().initialize(&state, &environment);
    }

    void addSensorsFromSettings(const AirSimSettings::VehicleSetting* vehicle_setting)
    {
        const auto& sensor_settings = vehicle_setting->sensors;
        sensor_factory_->createSensorsFromSettings(sensor_settings, sensors_, sensor_storage_);
    }

    // controls/state
    virtual void setControls(const Controls& controls) = 0;
    virtual Controls getControls() const = 0;
    virtual State getState() const = 0;
    virtual void updateState(const State& state) = 0;

    // tick
    virtual void update(float delta = 0) override
    {
        VehicleApiBase::update(delta);
        getSensors().update(delta);
    }

    void reportState(StateReporter& reporter) override
    {
        getSensors().reportState(reporter);
    }

protected:
    virtual void resetImplementation() override
    {
        getSensors().reset();
    }

protected:
    std::shared_ptr<const SensorFactory> sensor_factory_;
    SensorCollection sensors_;
    vector<shared_ptr<SensorBase>> sensor_storage_;
};

}} // namespace

#endif
