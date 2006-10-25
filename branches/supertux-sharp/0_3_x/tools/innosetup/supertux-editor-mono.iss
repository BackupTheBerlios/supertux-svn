; $Id$
; Bugs:
;  * Does not include data directory
;  * Messy code
;  * Probably lots more
; Script generated by VSPrj2make Script Wizard.
; 25.10.2006 16:55:34

#define MyAppName "SuperTux Editor"
#define MyAppVer "0.3.0"
#define MyAppVerName "SuperTux Editor 0.3.0"
#define MyAppMajVerName "SuperTux Editor 0.3"
#define MyAppPublisher "SuperTux Development Team"
#define MyAppURL "http://supertux.berlios.de"
#define MyAppBaseName "supertux-editor"

[Setup]
AppName={#MyAppName}
AppVerName={#MyAppVerName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppMajVerName}
DefaultGroupName=SuperTux
AllowNoIcons=true
VersionInfoVersion={#MyAppVer}
AppVersion={#MyAppVer}
LicenseFile=..\..\COPYING
OutputBaseFilename={#MyAppBaseName}-{#MyAppVer}-mono-setup
Compression=Lzma
SolidCompression=true
ShowLanguageDialog=yes
AppID={{3db3f2b4-7ba2-4d6e-bb23-6e968bd20ac1}
UninstallDisplayName={#MyAppVerName}


[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: ..\..\{#MyAppBaseName}.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\..\gtkgl-sharp.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\..\libeditor.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\..\Lisp.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\..\LispReader.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\..\Resources.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\..\SDL.dll; DestDir: {app}; Flags: ignoreversion
Source: ..\..\SDL_image.dll; DestDir: {app}; Flags: ignoreversion
Source: InstallerMaker\monoLaunchC.exe; DestDir: {app}; Flags: ignoreversion
Source: InstallerMaker\monoLaunchW.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\..\supertux-editor\resources\supertux-editor.ico; DestDir: {app}; Flags: ignoreversion

[Icons]
Name: {group}\SuperTux Editor; Filename: {app}\monoLaunchW.exe; Parameters: {code:QuoteMainExe|supertux-editor.exe}; WorkingDir: {app}; IconFilename: {app}\supertux-editor.ico; IconIndex: 0; Tasks: 
Name: {group}\{cm:UninstallProgram,SuperTux Editor}; Filename: {uninstallexe}
Name: {userdesktop}\SuperTux Editor; Filename: {app}\monoLaunchW.exe; Tasks: desktopicon; Parameters: {code:QuoteMainExe|supertux-editor.exe}; WorkingDir: {app}; IconFilename: {app}\supertux-editor.ico; IconIndex: 0

[Code]
// This checks if another app that installed using Inno Setup is already installed.
function GetPathInstalled( AppID: String ): String;
var
	sPrevPath: String;
begin
	sPrevPath := '';
	if not RegQueryStringValue(HKLM,
	                           'Software\Microsoft\Windows\CurrentVersion\Uninstall\'+AppID+'_is1',
	                           'Inno Setup: App Path', sPrevpath) then
		RegQueryStringValue(HKCU, 'Software\Microsoft\Windows\CurrentVersion\Uninstall\'+AppID+'_is1' ,
		                    'Inno Setup: App Path', sPrevpath);

  Result := sPrevPath;
end;

	// AppID for SuperTux, if you change it in the .iss for supertux change here too!
	SuperTuxID = '{4BEF4147-E17A-4848-BDC4-60A0AAC70F2A}';

// Global variable that will contain path to supertux if supertux was installed using Inno Setup
var
	SuperTuxPath: String;


Function SayMessage(const strMsg: String; const typMsgBox: TMsgBoxType) : Integer;
begin
  Result := MsgBox(strMsg, typMsgBox, MB_OK);
end;

Function QuoteMainExe(const mainExeFile: String) : String;
begin
  Result := '"' + ExpandConstant('{app}') + '\' +  mainExeFile + '"';
end;

Function GetMonoBasePath(strParam1 : string): string;
var
    strMonoBaseDir: String;
    bRc: Boolean;
    strDefaultCLR: String;
begin
    // Get current CLR version
    bRc := RegQueryStringValue(HKLM, 'SOFTWARE\Novell\Mono', 'DefaultCLR', strDefaultCLR);
    If bRc = true Then
    begin
      // Get the registry value
      bRc := RegQueryStringValue(HKLM, 'SOFTWARE\Novell\Mono\' + strDefaultCLR, 'SdkInstallRoot',
      strMonoBaseDir
      );
    end;

    If bRc = true Then
    begin
        Result := strMonoBaseDir;
    end;
end;

// Checks to see if Mono Runtime is Installed
Function IsMonoRuntimeInstalled() : Boolean;
var
strMonoBasePath: String;
nLen: LongInt;
begin
    Result := true;
    strMonoBasePath:= GetMonoBasePath('');
    nLen := Length(strMonoBasePath);
    IF nLen < 1 then
    begin
      Result := false;
    end;
end;

// Checks to see if Mono is Installed
Function IsMonoInstalled() : Boolean;
var
strMonoBasePath: String;
nLen: LongInt;
begin
    Result := true;
    strMonoBasePath:= GetMonoBasePath('');
    nLen := Length(strMonoBasePath);
    IF nLen < 1 then
    begin
      Result := false;
    end;
end;

Function IsNet1dot1RunInstalled() : Boolean;
begin
	Result := RegKeyExists(HKLM, 'SOFTWARE\Microsoft\.NETFramework\policy\v1.1');
end;

Function IsNet2dot0RunInstalled() : Boolean;
begin
	Result := RegKeyExists(HKLM, 'SOFTWARE\Microsoft\.NETFramework\policy\v2.0');
end;

Function IsGtkSDKInstalled() : Boolean;
begin
	Result := RegKeyExists(HKLM, 'SOFTWARE\Novell\Gtk-Sharp');
end;

Function IsGtkRunInstalled() : Boolean;
begin
	Result := RegKeyExists(HKLM, 'SOFTWARE\Novell\Gtk-SharpRuntime');
end;


// Checks to see if there is a previous installation of the Add-in
Function IsCurrentlyInstalled() : Boolean;
begin
	Result := RegKeyExists(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{3db3f2b4-7ba2-4d6e-bb23-6e968bd20ac1}_is1');
end;


Function Net1dot1RuntimeDownload : Boolean;
var
    ErrorCode: Integer;
begin
  Result := false;
    ShellExec('open',
  'http://download.microsoft.com/download/a/a/c/aac39226-8825-44ce-90e3-bf8203e74006/dotnetfx.exe',
    '',
  '',
  SW_SHOWNORMAL,
  ewNoWait,
  ErrorCode
  );
	exit;
end;

Function Net2dot0RuntimeDownload : Boolean;
var
    ErrorCode: Integer;
begin
  Result := false;
    ShellExec('open',
  'http://download.microsoft.com/download/5/6/7/567758a3-759e-473e-bf8f-52154438565a/dotnetfx.exe',
    '',
  '',
  SW_SHOWNORMAL,
  ewNoWait,
  ErrorCode
  );
	exit;
end;

Function MonoRuntimeDownload : Boolean;
var
    ErrorCode: Integer;
begin
  Result := false;
    ShellExec('open',
  'http://forge.novell.com/modules/xfcontent/private.php/monowin32/Mono%20Runtime%20Installer%20for%20Windows/v1.1.13.x/monoruntime-1.1.13-exp-0.1.exe',
    '',
  '',
  SW_SHOWNORMAL,
  ewNoWait,
  ErrorCode
  );
	exit;
end;

Function MonoCombinedDownload : Boolean;
var
    ErrorCode: Integer;
begin
  Result := false;
    ShellExec('open',
  'ftp://www.go-mono.com/archive/1.1.15/windows-installer/1/mono-1.1.15-gtksharp-2.8.2-win32-1.exe',
    '',
  '',
  SW_SHOWNORMAL,
  ewNoWait,
  ErrorCode
  );
	exit;
end;

Function GtkSharpRuntimeDownload : Boolean;
var
    ErrorCode: Integer;
begin
  Result := false;
    ShellExec('open',
  'http://forge.novell.com/modules/xfcontent/private.php/gtks-inst4win/Win32%20Runtime%20Installer/v2.7.1/gtksharp-runtime-2.7.1-win32-0.4.exe',
    '',
  '',
  SW_SHOWNORMAL,
  ewNoWait,
  ErrorCode
  );
	exit;
end;

Function GtkSharpSDKDownload : Boolean;
var
    ErrorCode: Integer;
begin
  Result := false;
    ShellExec('open',
  'http://forge.novell.com/modules/xfcontent/private.php/gtks-inst4win/Win32%20Installer/v2.7.1/gtksharp-2.7.1-win32-0.4.exe',
    '',
  '',
  SW_SHOWNORMAL,
  ewNoWait,
  ErrorCode
  );
	exit;
end;
Function InitializeSetup : Boolean;
var
    ErrorCode: Integer;
begin
	// Check requirements before Installation
	If IsCurrentlyInstalled() = true Then
	begin
	 Result := false;
	 SayMessage('SuperTux Editor seems to be installed.' + #13 + #10 + 'Please uninstall it and run this setup again.', mbError);
	 exit;
	end
if IsMonoInstalled() = false then
begin
	Result := MsgBox(
		'This setup requires the Mono for Windows. Please download and install the Mono for Windows and run this setup again. Do you want to download it now?',
		mbConfirmation, MB_YESNO) = idYes;
	if Result = false then
	begin
		Result := false;
		exit;
	end
	else
	begin
		// Runtime download function invocation
		Result := MonoCombinedDownload();
		exit;
	end

	// Check that SuperTux is installed
	SuperTuxPath := GetPathInstalled(SuperTuxID);
	if (Length(SuperTuxPath) = 0) then
	begin
		ResultSuperTux := MsgBox('{#MyAppVerName} requires SuperTux 0.3 to be installed. Are you sure you want to continue without installing SuperTux-0.3?',
		                         mbConfirmation, MB_YESNO) = idYes;
		if ResultSuperTux = false then
			Result := false;
	end;
end;
	Result := true;
end;
