#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>

#include "vehicles/fixedwing/api/FixedWingRpcLibClient.hpp"

static float clamp(float v, float lo, float hi) { return (v < lo) ? lo : (v > hi ? hi : v); }

int main()
{
    using namespace msr::airlib;
    try {
        FixedWingRpcLibClient client("127.0.0.1", RpcLibPort);
        client.confirmConnection();

        const std::string vehicle = "FW1";

        client.enableApiControl(true, vehicle);
        client.armDisarm(true, vehicle);

        FixedWingApiBase::Controls ctrl{};
        ctrl.throttle = 0.3f;
        ctrl.elevator = -0.05f;
        ctrl.aileron  = 0.02f;
        client.setFixedWingControls(ctrl, vehicle);

        bool running = true;
        bool alt_hold = false;
        float alt_target_ft = 0.f;
        const float dth = 0.05f, d_e = 0.01f, d_a = 0.01f, d_r = 0.01f, d_flap = 0.05f;
        const float Kp_alt = -0.0008f; // elevator cmd = Kp*(alt_target - alt)

        std::cout << "Hotkeys: [+/-]=throttle, W/S=elevator, A/D=aileron, Q/E=rudder, [/]=flaps, G=gear, B=park brake, H=AltHold toggle, T=set target alt, X=exit\n";

        int tick = 0;
        while (running) {
            while (_kbhit()) {
                int ch = _getch();
                if (ch == 0 || ch == 224) { (void)_getch(); continue; }
                switch (std::tolower(ch)) {
                case '+': ctrl.throttle = clamp(ctrl.throttle + dth, 0.f, 1.f); break;
                case '-': ctrl.throttle = clamp(ctrl.throttle - dth, 0.f, 1.f); break;
                case 'w': ctrl.elevator = clamp(ctrl.elevator - d_e, -0.5f, 0.5f); alt_hold=false; break;
                case 's': ctrl.elevator = clamp(ctrl.elevator + d_e, -0.5f, 0.5f); alt_hold=false; break;
                case 'a': ctrl.aileron  = clamp(ctrl.aileron  - d_a, -0.7f, 0.7f); break;
                case 'd': ctrl.aileron  = clamp(ctrl.aileron  + d_a, -0.7f, 0.7f); break;
                case 'q': ctrl.rudder   = clamp(ctrl.rudder   - d_r, -0.7f, 0.7f); break;
                case 'e': ctrl.rudder   = clamp(ctrl.rudder   + d_r, -0.7f, 0.7f); break;
                case '[': ctrl.flap     = clamp(ctrl.flap     - d_flap, 0.f, 1.f); break;
                case ']': ctrl.flap     = clamp(ctrl.flap     + d_flap, 0.f, 1.f); break;
                case 'g': ctrl.gear_down = (ctrl.gear_down > 0.5f) ? 0.f : 1.f; break;
                case 'b': ctrl.parking_brake = (ctrl.parking_brake > 0.5f) ? 0.f : 1.f; break;
                case 'h': alt_hold = !alt_hold; std::cout << "AltHold: " << (alt_hold?"ON":"OFF") << "\n"; break;
                case 't': {
                    auto st = client.getFixedWingState(vehicle);
                    alt_target_ft = st.alt_asl_ft;
                    alt_hold = true;
                    std::cout << "AltHold target set to " << alt_target_ft << " ft\n";
                } break;
                case 'x': running = false; break;
                default: break;
                }
                client.setFixedWingControls(ctrl, vehicle);
            }

            auto st = client.getFixedWingState(vehicle);
            if (alt_hold) {
                float e = alt_target_ft - st.alt_asl_ft;
                ctrl.elevator = clamp(Kp_alt * e, -0.25f, 0.25f);
                client.setFixedWingControls(ctrl, vehicle);
            }

            if ((tick++ % 10) == 0) {
                std::cout << "CAS=" << st.cas_kts
                          << " Alt(ft)=" << st.alt_asl_ft
                          << " Thr=" << ctrl.throttle
                          << " Ele=" << ctrl.elevator
                          << " Ail=" << ctrl.aileron
                          << (alt_hold?" [AltHold]":"")
                          << "\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        client.armDisarm(false, vehicle);
        client.enableApiControl(false, vehicle);
    }
    catch (const std::exception& e) {
        std::cerr << "RPC error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
