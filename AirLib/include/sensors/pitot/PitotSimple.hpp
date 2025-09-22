// Copyright (c) OpenAI. All rights reserved.

#pragma once

#include "sensors/pitot/PitotBase.hpp"
#include "common/AirSimSettings.hpp"

namespace msr { namespace airlib {

class PitotSimple : public PitotBase {
public:
    PitotSimple() : PitotBase("Pitot") {}
    PitotSimple(const AirSimSettings::PitotSetting& setting)
        : PitotBase(setting.sensor_name) {}

    virtual void resetImplementation() override { }

    virtual void update(float delta = 0) override
    {
        PitotBase::update(delta);
        Output out;
        const GroundTruth& gt = getGroundTruth();
        // Approximate IAS as magnitude of body velocity (no wind correction here)
        out.airspeed = gt.kinematics ? gt.kinematics->twist.linear.norm() : 0;
        out.time_stamp = clock()->nowNanos();
        setOutput(out);
    }
};

}} // namespace
