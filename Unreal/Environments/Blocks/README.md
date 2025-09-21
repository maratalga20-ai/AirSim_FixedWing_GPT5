# AirSim FixedWing for UE 5.6 (Blocks)

This repository contains an Unreal Engine 5.6 project (Blocks) with AirSim integrated and a new Fixed‑Wing vehicle pipeline. It supports running with a JSBSim‑based plugin (if attached to the fixed‑wing pawn), or a minimal internal kinematic bridge otherwise. It also includes camera/detection logging to make debugging easy.

## Prerequisites
- Unreal Engine 5.6 installed at `D:\UE_5.6`
- Visual Studio 2022 (C++ toolchain)
- Git + Git LFS (for large assets)

## Build
- Generate solution files (optional): run `GenerateProjectFiles.bat`
- Build via Unreal Build Tool (from a shell):
  - `D:\UE_5.6\Engine\Build\BatchFiles\Build.bat BlocksEditor Win64 Development -Project="D:\Clear\Cosys-AirSim\Unreal\Environments\Blocks\Blocks.uproject" -WaitMutex -NoHotReloadFromIDE`
- Launch editor:
  - `D:\UE_5.6\Engine\Binaries\Win64\UnrealEditor.exe "D:\Clear\Cosys-AirSim\Unreal\Environments\Blocks\Blocks.uproject"`

## Fixed‑Wing Setup
- Sim mode: FixedWing
- Pawn class: `AFixedWingPawn` (blueprint path set via settings.json `PawnPath`)
- JSBSim plugin (optional, plugin‑authoritative mode): attach your `UActorComponent` whose class name contains `"JSBSim"` to the pawn blueprint.
  - The plugin must expose UPROPERTY fields (floats/doubles) readable by reflection:
    - Struct `Commands`: Aileron, Elevator, Rudder, Flap, SpeedBrake, Spoiler, LeftBrake, RightBrake, CenterBrake, ParkingBrake, GearDown, (optional) RollTrim, PitchTrim, YawTrim
    - Array `EngineCommands`: each element has `Throttle`
    - Struct `AircraftState`: ElevatorPosition, LeftAileronPosition, RightAileronPosition, RudderPosition, CalibratedAirSpeedKts, GroundSpeedKts, TotalVelocityKts, AltitudeASLFt, AltitudeAGLFt, AltitudeRateFtps, StallWarning
- If the plugin is not present, AirSim uses an internal minimal kinematic bridge so you can still fly and debug sensors.

## Cameras & Detections
- `AFixedWingPawn` expects five `APIPCamera` child actors named exactly:
  - FrontCenterCamera, FrontRightCamera, FrontLeftCamera, BackCenterCamera, BottomCenterCamera
- When camera types are enabled, `UDetectionComponent` is auto‑attached to each capture and receives a render target.
- Logs show camera enable/disable/update with readable image type names (Scene, Segmentation, etc.).
- Use RPC to fetch detections (Python): `client.simGetDetections(camera_name="front_center", vehicle_name="FW1", image_type=airsim.ImageType.Scene)`

## Logging Aids
- Sim mode lifecycle (BeginPlay/EndPlay), pawn initialization, and JSBSim detection vs internal bridge are logged.
- `PIPCamera` logs when capture/detection is enabled/disabled and when `nodisplay` is toggled.
- `WorldSimApi` logs `getDetections` requests/responses with image‑type labels and counts.

## settings.json (SettingsVersion 2.0)
Create `%USERPROFILE%\Documents\AirSim\settings.json`:

```
{
  "SettingsVersion": 2.0,
  "SimMode": "FixedWing",

  "Vehicles": {
    "FW1": {
      "VehicleType": "fixedwing",
      "PawnPath": "YourProject/Blueprints/BP_FixedWingPawn",
      "DefaultVehicleState": "Armed",
      "EnableCollisionPassthrough": false,
      "RC": { "RemoteControlID": 0, "AllowAPIWhenDisconnected": true },
      "Cameras": {
        "front_center":  { "CaptureSettings": [ { "ImageType": 0, "Width": 1280, "Height": 720, "FOV_Degrees": 90 } ] },
        "front_right":   { "CaptureSettings": [ { "ImageType": 0, "Width": 800,  "Height": 600, "FOV_Degrees": 90 } ] },
        "front_left":    { "CaptureSettings": [ { "ImageType": 0, "Width": 800,  "Height": 600, "FOV_Degrees": 90 } ] },
        "back_center":   { "CaptureSettings": [ { "ImageType": 0, "Width": 800,  "Height": 600, "FOV_Degrees": 90 } ] },
        "bottom_center": { "CaptureSettings": [ { "ImageType": 0, "Width": 800,  "Height": 600, "FOV_Degrees": 90 } ] }
      }
    }
  },

  "OriginGeopoint": { "Latitude": 47.641468, "Longitude": -122.140165, "Altitude": 122 }
}
```

Replace `PawnPath` with your actual blueprint path for the pawn derived from `AFixedWingPawn`.

## Pushing to GitHub
This repo is ready for Git LFS. To push:

- HTTPS + Personal Access Token (recommended):
  1) Create a PAT (with `repo` scope) on GitHub.
  2) `git remote set-url origin https://<YOUR_PAT>@github.com/maratalga20-ai/AirSim_FixedWing.git`
  3) `git push -u origin main`

- Or configure Credential Manager and push:
  - `git credential-manager-core configure`
  - `git push -u origin main` (enter username and PAT as password)

- Or SSH:
  - Add your SSH key to GitHub
  - `git remote set-url origin git@github.com:maratalga20-ai/AirSim_FixedWing.git`
  - `git push -u origin main`

## Notes
- We disabled `UserToolBoxBasicCommand` engine plugin in `Blocks.uproject` to avoid a UHT name clash with a JSBSim struct.
- `SimModeWorldFixedWing` uses the base RPC server by default to avoid linking against a missing `FixedWingRpcLibServer`. If you need that server, we can wire it into the AirLib build.
- Large assets are tracked by LFS. Ensure `git lfs install` on any machine cloning this repo.

