; Script generated by the Inno Setup Script Wizard.
; modified by Matthias Braun (thanks go to fEnio for the netpanzer script which I took as a base)

[Setup]
AppName=SuperTux
AppVerName=SuperTux 0.1.0
AppPublisher=SuperTux Development Team
AppPublisherURL=http://super-tux.sourceforge.net
AppSupportURL=http://super-tux.sourceforge.net
AppUpdatesURL=http://super-tux.sourceforge.net
DefaultDirName={pf}\SuperTux
DefaultGroupName=SuperTux
AllowNoIcons=yes
Compression=lzma
SolidCompression=yes

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\supertux\supertux.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\AUTHORS"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\ChangeLog"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\COPYING"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\jpeg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\libpng1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\NEWS"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\README"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\SDL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\SDL_image.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\SDL_mixer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\TODO"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\zlib.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\supertux\data\*"; DestDir: "{app}\data"; Flags: ignoreversion recursesubdirs
Source: "D:\supertux\contrib\innosetup\supertux.ico"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\SuperTux"; Filename: "{app}\supertux.exe"; WorkingDir: "{app}"; IconFilename: "{app}\supertux.ico"
Name: "{group}\{cm:UninstallProgram,SuperTux}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\SuperTux"; Filename: "{app}\supertux.exe"; WorkingDir: "{app}"; IconFilename: "{app}\supertux.ico"; Tasks: desktopicon

[Run]
Filename: "{app}\supertux.exe"; Description: "{cm:LaunchProgram,SuperTux}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{app}\.supertux"
Type: filesandordirs; Name: "{app}\stdout.txt"
Type: filesandordirs; Name: "{app}\stderr.txt"
