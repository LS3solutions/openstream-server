# Open-App Windows installer 

## Requirements
- All the requirements to build Open-Stream app.
- Qt Installer framework in Msys2 platform. Package name `mingw-w64-qt-installer-framework`.

## Build installer

### Folders structure 

For building the installer, you're going to use the `config` folder and the `package` folder. 

- Copy the openstream app builded files and shared `.dll` into `installer\packages\com.openstream.openstreamapp\data`. 

### Manifest file

Manifest file allows for execution with highest privileges on Windows. 

- Add the `installer\manifest.xml` manifest file to the `openstreamapp.exe`. 

That can be done with using `mt.exe` from the Windows SDK tool. The command executed from within `data` folder looks like: 

- `mt.exe -manifest ..\..\..\manifest.xml -outputresource:openstreamapp.exe;#1`

### AutoStart functionality

- Create a folder `utils` in `installer\packages\com.openstream.openstreamapp\data`
- Copy `AutoLoginTaskSchedulerProject.exe` in `installer\packages\com.openstream.openstreamapp\data\utils` 

AutoStart functionality is encapsulated within an external `.exe` application. Source and instructions on how to compile this file can be found in `AutoLoginTaskSchedulerProject` in openstream-server base folder. 

There's a donwload ready file: https://github.com/m4rkoup/openstream-server/releases/tag/vFeb2021

### Gamepad support

- Create a folder `gamepad_support` in `installer\packages\com.openstream.openstreamapp\data`
- Copy the `ViGEmBusSetup_x64.msi`

Donwload the `ViGEmBusSetup_x64.msi` file from: https://github.com/ViGEm/ViGEmBus/releases

### Build the installer

After previous sections are completed, you can build the installer with one command using Qt installer framework: 

- go to `/openstream-server/installer`
- execute ` /mingw64/bin/binarycreator.exe -c config/config.xml -p packages [your installer name]`

This should produce the installer file in `/openstream-server/installer`.