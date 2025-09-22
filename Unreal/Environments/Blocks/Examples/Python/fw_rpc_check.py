#!/usr/bin/env python3
"""
Простий тест RPC‑підключення до AirSim (FixedWing форк).

Вимагає Python‑пакет `airsim`:
    pip install airsim

Запуск:
    python fw_rpc_check.py --ip 127.0.0.1 --port 41451

Скрипт робить confirmConnection(), зчитує версію сервера і перелік камер.
"""
import argparse
import sys

try:
    import airsim
except ImportError:
    print("[Помилка] Модуль 'airsim' не знайдено. Встановіть: pip install airsim", file=sys.stderr)
    sys.exit(2)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", default="127.0.0.1", help="IP сервера AirSim")
    parser.add_argument("--port", type=int, default=41451, help="RPC порт (ApiServerPort)")
    parser.add_argument("--vehicle", default="FW1", help="Ім'я літака у settings.json")
    args = parser.parse_args()

    # Використовуємо базовий клієнт (через MultirotorClient) лише для confirmConnection/ping
    # Це працює із FixedWing RPC сервером, бо confirmConnection викликає ping.
    client = airsim.MultirotorClient(ip=args.ip, port=args.port)

    try:
        client.confirmConnection()
        ver = client.getServerVersion()
        simmode = client.getSimMode()
        print(f"[OK] Підключено до AirSim RPC {args.ip}:{args.port}, версія={ver}, SimMode={simmode}")
        # Легкий виклик для перевірки RPC каналу
        cams = client.simListSceneObjects("*Camera*")
        print(f"Знайдено об'єктів з 'Camera' у назві: {len(cams)}")
    except Exception as ex:
        print(f"[Помилка RPC] {ex}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()

