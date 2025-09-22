#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <cstdlib>
#include <cctype>

#include "vehicles/fixedwing/api/FixedWingRpcLibClient.hpp"

using msr::airlib::FixedWingRpcLibClient;
using msr::airlib::FixedWingApiBase;

static float clamp(float v, float lo, float hi) { return (v < lo) ? lo : (v > hi ? hi : v); }

int main(int argc, char** argv)
{
    const char* ip = (argc > 1) ? argv[1] : "127.0.0.1";
    const uint16_t port = (argc > 2) ? static_cast<uint16_t>(std::atoi(argv[2])) : 41451;
    const std::string vehicle = (argc > 3) ? argv[3] : std::string("FW1");

    try {
        std::cout << "Підключення до AirSim RPC " << ip << ":" << port << " ...\n";
        FixedWingRpcLibClient client(ip, port);
        client.confirmConnection();
        std::cout << "OK. Вмикаю API та озброюю...\n";
        client.enableApiControl(true, vehicle);
        client.armDisarm(true, vehicle);

        // Простий сценарій: розбіг і підйом до цільової висоти, далі AltHold (P‑регулятор по елеватору)
        FixedWingApiBase::Controls ctrl{};
        ctrl.throttle = 0.65f;  // базова тяга для набору висоти (налаштуйте під вашу модель)
        ctrl.elevator = -0.05f; // легкий тангаж вгору (у цій реалізації elevator < 0 піднімає ніс)
        client.setFixedWingControls(ctrl, vehicle);

        // Дочекаємося першого стану і задамо ціль: +400 футів від поточної висоти ASL
        auto st0 = client.getFixedWingState(vehicle);
        float alt_target_ft = st0.alt_asl_ft + 400.f;
        const float Kp_alt = -0.0009f; // elevator = Kp * (alt_target - alt)

        std::cout << "Стартова висота (ASL) = " << st0.alt_asl_ft
                  << " ft, ціль = " << alt_target_ft << " ft\n";

        const auto t_start = std::chrono::steady_clock::now();
        int tick = 0;
        while (true) {
            auto st = client.getFixedWingState(vehicle);

            // AltHold по елеватору
            float err = alt_target_ft - st.alt_asl_ft;
            ctrl.elevator = clamp(Kp_alt * err, -0.25f, 0.25f);
            client.setFixedWingControls(ctrl, vehicle);

            if ((tick++ % 10) == 0) {
                std::cout << "CAS=" << st.cas_kts
                          << " Alt(ft)=" << st.alt_asl_ft
                          << " Thr=" << ctrl.throttle
                          << " Ele=" << ctrl.elevator
                          << "\n";
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            // Завершимо через ~30 секунд демо
            auto dt = std::chrono::steady_clock::now() - t_start;
            if (std::chrono::duration_cast<std::chrono::seconds>(dt).count() > 30)
                break;
        }

        // Безпечно відпустимо керування
        std::cout << "Гашу двигун і відключаю API...\n";
        ctrl.throttle = 0.f;
        client.setFixedWingControls(ctrl, vehicle);
        client.armDisarm(false, vehicle);
        client.enableApiControl(false, vehicle);
        std::cout << "Готово.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "RPC помилка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

