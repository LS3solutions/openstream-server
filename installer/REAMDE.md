### Open-App Windows installer 

#### Requirements
- All the requirements from Open-Stream app in the root folder.
- Qt Installer framework in Msys2 platform:  `mingw-w64-qt-installer-framework`.

#### Build installer
- Go to `./packages/com.openstream.openstreamapp/data`
- Extract `data.rar` there. (Contains the `dlls` required by the app to run. They can be statically added later on, but for now they are required).
- Copy the `openstreaapp.exe` file obtained when you build the project (copy into `./packages/com.openstream.openstreamapp/data`).
- Open `MSYS2 MinGW 64` console, and cd to `.` (`installer` folder).
- Run command: `/mingw64/bin/binarycreator.exe -c config/config.xml -p packages [Installer Name]`
- Wait for the command to finish.
