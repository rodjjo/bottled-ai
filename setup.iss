[Setup]
AppName=Bottled AI
AppVersion=1.0
DefaultDirName={userappdata}\Bottled AI
DefaultGroupName=Bottled AI
UninstallDisplayIcon={app}\uninstall.ico
Compression=lzma2
SolidCompression=yes
OutputBaseFilename=Setup
OutputDir=.\build\bin
PrivilegesRequiredOverridesAllowed=commandline dialog

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; The python files should be installed first as we replace python configs after
Source: "temp\python3.10\*.*"; DestDir: "{app}\bin"; Flags: ignoreversion recursesubdirs
Source: "build\bin\bottled-ai.exe"; DestDir: "{app}\bin"; Flags: ignoreversion 
Source: "build\bin\bottled-ai-win.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "build\mingw64\bin\libstdc++-6.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "build\mingw64\opt\bin\libgcc_s_seh-1.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "build\mingw64\opt\bin\libwinpthread-1.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "data\images\app.ico"; DestDir: "{app}\bin"
Source: "data\images\uninstall.ico"; DestDir: "{app}\bin"
Source: "python_stuff\*.*"; DestDir: "{app}\python_stuff"; Flags: ignoreversion recursesubdirs
Source: "dependencies\python-future\*.*"; DestDir: "{app}\python_deps\python-future"; Flags: ignoreversion recursesubdirs
Source: "python_configs\*.*"; DestDir: "{app}\bin"; Flags: ignoreversion recursesubdirs


[Icons]
Name: "{group}\Bottled AI"; Filename: "{app}\bin\bottled-ai-win.exe"; IconFilename: {app}\bin\app.ico ; WorkingDir: "{app}\bin"
Name: "{userdesktop}\Bottled AI"; Filename: "{app}\bin\bottled-ai-win.exe"; IconFilename: {app}\bin\app.ico; Tasks: desktopicon ; WorkingDir: "{app}\bin"

