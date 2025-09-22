// Copyright (c) OpenAI. All rights reserved.

#pragma once

#include "sensors/SensorBase.hpp"

namespace msr { namespace airlib {

class PitotBase : public SensorBase {
public:
    PitotBase(const std::string& sensor_name = "") : SensorBase(sensor_name) {}

    struct Output {
        TTimePoint time_stamp = 0;
        real_T airspeed = 0; // m/s (indicated or calibrated per implementation)
    };

    virtual void reportState(StateReporter& reporter) override
    {
        UpdatableObject::reportState(reporter);
        reporter.writeValue("Pitot-IAS", output_.airspeed);
    }

    const Output& getOutput() const { return output_; }

protected:
    void setOutput(const Output& o) { output_ = o; }

private:
    Output output_;
};

}} // namespace

