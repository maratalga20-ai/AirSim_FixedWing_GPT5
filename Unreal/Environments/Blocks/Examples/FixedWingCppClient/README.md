# FixedWing C++ RPC Client (Example)

This is a minimal C++ client that connects to AirSim's FixedWing RPC server and sends basic controls (throttle/elevator/aileron), then reads back state.

## Prerequisites
- AirSim plugin is built with AirLib RPC enabled (AirLib.lib compiled without AIRLIB_HEADER_ONLY / AIRLIB_NO_RPC).
- Unreal project is running with SimMode = FixedWing and RPC server enabled (default address/port).

## Build (Windows)
1. Open a Developer Command Prompt (or use CMake GUI).
2. From this folder run:
```
cmake -S . -B build -A x64 -DAIRSIM_ROOT="D:/Clear/Cosys-AirSim/Unreal/Environments/Blocks/Plugins/AirSim/Source/AirLib"
cmake --build build --config Release
```
3. Run `build/Release/FixedWingCppClient.exe` while Unreal Editor is playing your scene.

## Notes
- To change the server address/port, edit `main.cpp` constructor: `FixedWingRpcLibClient client("127.0.0.1", RpcLibPort);`
- Vehicle name must match your settings.json (e.g., `FW1`).
- If connection fails, verify that Unreal Output Log shows the RPC server started and `netstat -ano | findstr 41451` shows LISTENING on the expected port.

