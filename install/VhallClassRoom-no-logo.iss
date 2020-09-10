#define EXE_VER "2.2.2.7"

[Setup]
AppPublisher=vhall.com
AppPublisherURL=http://www.vhall.com/
AppSupportURL=http://www.vhall.com/
AppUpdatesURL=http://www.vhall.com/
SetupIconFile=..\VLive\class.ico
AppName=  {cm:vname}
DefaultDirName={userdocs}\VhallClassRoom
AppVerName=VhallClassroom{#EXE_VER}
DefaultGroupName={cm:MyGroupName}
OutputDir=.\setup
ArchitecturesInstallIn64BitMode = x64
PrivilegesRequired=admin
ArchitecturesAllowed = x86 x64
OutputBaseFilename=VhallClassRoom{#EXE_VER}
Compression=lzma/ultra
SolidCompression=yes
VersionInfoVersion={#EXE_VER}
;ShowLanguageDialog=no
UsePreviousLanguage=no

[Dirs]  

[Code]

procedure InstallCapture();
var
  axFile:String;
  tmpFile:String;
  tmp:String;
  ResultCode: Integer;
begin

    axFile:= ExpandConstant(('{app}\VhallDesktop.ax'));
    tmpFile := axFile;
    if( FileExists(tmpFile)) then
    begin
      tmp := ' -s \"' + axFile +'\"';
      Exec('regsvr32',tmp,'',SW_HIDE, ewWaitUntilTerminated,ResultCode);
    end
    else
    begin
      tmpFile := axFile + '.1';
      if( FileExists(tmpFile))  then
      begin
        tmp := ' -s \"' + axFile + '.1\"';
        Exec('regsvr32',tmp,'',SW_HIDE, ewWaitUntilTerminated,ResultCode);
      end
    end

end;

procedure UnInstallCapture();
var
  srcFile:String;
  axFile:String;
  tmpFile:String;
  tmp:String;
  ResultCode: Integer;
begin
   axFile:= ExpandConstant(('{app}\VhallDesktop.ax'));
    tmpFile := axFile;
    if( FileExists(tmpFile)) then
    begin
      tmp := ' -u -s \"' + axFile +'\"';
      Exec('regsvr32',tmp,'',SW_HIDE, ewWaitUntilTerminated,ResultCode);
      DeleteFile(tmpFile);
    end

    tmpFile := axFile + '.1';
    if( FileExists(tmpFile))  then
    begin
      tmp := ' -u  -s \"' + axFile + '.1\"';
      Exec('regsvr32',tmp,'',SW_HIDE, ewWaitUntilTerminated,ResultCode);
      DeleteFile(tmpFile);
    end

     tmpFile :=  ExpandConstant(('{sys}\VhallDesktop.ax'));;
    if( FileExists(tmpFile))  then
    begin
      tmp := ' -u -s \"'+tmpFile +'\"';
      Exec('regsvr32',tmp,'',SW_HIDE, ewWaitUntilTerminated,ResultCode)
      DeleteFile(tmpFile);
    end
end;


procedure vbpInstall( param: String);
var
unins:String;
ResultCode: Integer;
begin
  unins:= ExpandConstant(('{app}\vbp.exe'));
  if( FileExists(unins)) then
    begin
      Exec(unins, param, '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
end;

procedure killvbp();
var
unins:String;
ResultCode: Integer;
begin
  unins:= ExpandConstant(('{app}\vbp.exe'));
  if( FileExists(unins)) then
    begin
      Exec('taskkill', '/IM  vbp.exe', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
end;

procedure killprocess(processName: String);
var
unins:String;
taskkillParam:String;
ResultCode: Integer;
begin
  unins:= ExpandConstant(('{app}\')) + processName;
  if( FileExists(unins)) then
    begin
      taskkillParam:=  '/IM ' +  processName;
      Exec('taskkill', taskkillParam, '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
end;

procedure UnInstallWinService(serviceFileName: String);
var
uninservice:String;
ResultCode: Integer;
begin
  uninservice:= ExpandConstant(('{app}\')) + serviceFileName;
  if( FileExists(uninservice)) then
    begin
      Exec(uninservice, ' -d', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
end;

function InitializeSetup: Boolean;
var Isbl: boolean;
var Isstr: string;
var MyProgChecked: Boolean;
var Version:TWindowsVersion;
ResultStr: String;
ResultCode: Integer;
begin
  GetWindowsVersionEx(Version);
  if (Version.Major < 6) or ((Version.Major = 6) and (Version.Minor = 0)) then
    begin
      MsgBox(ExpandConstant('{cm:wrongOS}'),mbInformation,MB_OK);
      Result := false;
      Exit;
    end
  else
    begin
      Result := true;
    end
    
  if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\空中课堂_is1', 'UninstallString', ResultStr) then
    begin
    ResultStr := RemoveQuotes(ResultStr);
    Exec(ResultStr, '/silent', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
    result := true;
end;

procedure btn_setupclick(hBtn:HWND);//立即安装 按钮点击事件
var
  ResultStr: String;
  ResultCode: Integer;
begin
   killprocess('VhallClassRoom.exe');
  WizardForm.NextButton.Click;
end;



[Languages]
Name: "chs"; MessagesFile: "compiler:Languages\ChineseSimp.isl"
Name: "en"; MessagesFile: "compiler:Languages\English.isl"
Name: "chinese_trad"; MessagesFile: "compiler:Languages\ChineseTrad.isl"

[CustomMessages]
chs.vname=空中课堂
en.regist=Register
en.registerInfo=if using all function please register Vhall class accounts.
en.exitVhall=Please Exit Vhall class then press Install  Button.
chs.regist=免费注册
chs.registerInfo=.

en.MyGroupName=Vhall Class Streaming
chs.MyGroupName=空中课堂
                                 
en.vname=VhallClass
chs.exitVhall=请先退出空中课堂再进行安装.

en.wrongOS=Please use the operating system which is Windows 7 or higher version! The setup program will exit!
chs.wrongOS=请使用WIN7及以上的操作系统！无法运行程序，本安装程序即将退出！
en.detectNoDX=Your machine don't have the necessary DirectX components! Will you install it right now?
chs.detectNoDX=系统检测到您没有安装必需的DirectX组件，是否立刻安装？
en.DXWarning=DirectX is not installed. Vhall Live Streaming may not be able to run normally!
chs.DXWarning=没有安装DirectX组件，空中课堂可能无法正常使用！

[Messages]
en.BeveledLabel=Life is not dress rehearsal, living every day!
chs.BeveledLabel=人生没有彩排，每天都是现场直播！

[Tasks]
;Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
;Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}";
[Components]
;Name: "readme"; Description: "Readme File"; Types: full
[Files]

Source: "..\VLive\class.ico"; BeforeInstall:killprocess('VhallClassRoom.exe');DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\vhall_class_config.ini"; BeforeInstall:killprocess('VhallClassRoom.exe');DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\config.ini";  DestDir: "{app}"; Flags: onlyifdoesntexist uninsneveruninstall
     
Source: "..\Win32\Release\VHWebSocket.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VHClassSDK.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallWebRtcSDK.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallWebRtcLive.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallVHMonitorCapture.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallOBSApi.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallOBS.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallNetWork.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallMediaReader.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallDShowPlugin.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Vhalldshowcapture.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallDecklinkDevice.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallAudioEngine.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\vhall_webrtc_sdk.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\glew32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\freeglut.dll"; DestDir: "{app}"; Flags: ignoreversion

;QT release
Source: "..\Win32\Release\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\default_pushPic.png"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Win32\Release\avcodec-58.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\avformat-58.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\avutil-56.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Win32\Release\swresample-3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\swscale-5.dll"; DestDir: "{app}"; Flags: ignoreversion


Source: "..\Win32\Release\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5Multimedia.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5MultimediaWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5Positioning.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5PrintSupport.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5Qml.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5Quick.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5QuickWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5SerialPort.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5WebChannel.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5WebEngineCore.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5WebEngineWidgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\Qt5WinExtras.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\QtWebEngineProcess.exe"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Win32\Release\pepflashplayer.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\cmd.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\timeEnd.mp3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\vhall_ns_process.dll"; DestDir: "{app}"; Flags: ignoreversion




Source: "..\Win32\Release\bearer\*.*"; DestDir: "{app}\bearer"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\iconengines\*.*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\imageformats\*.*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\platforms\*.*"; DestDir: "{app}\platforms";Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\playlistformats\*.*"; DestDir: "{app}\playlistformats"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\plugins\*.*"; DestDir: "{app}\plugins"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\position\*.*"; DestDir: "{app}\position"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\printsupport\*.*"; DestDir: "{app}\printsupport"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\resources\*.*"; DestDir: "{app}\resources";Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\styles\*.*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\translations\*.*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\audio\*.*"; DestDir: "{app}\audio"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\Win32\Release\mediaservice\*.*"; DestDir: "{app}\mediaservice"; Flags: ignoreversion recursesubdirs createallsubdirs;


Source: "..\Win32\Release\api-ms-win-crt-convert-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-environment-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-filesystem-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-heap-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-locale-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-math-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-multibyte-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-runtime-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-stdio-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-string-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-time-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-utility-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\ucrtbase.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Win32\Release\api-ms-win-core-console-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-datetime-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-debug-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-errorhandling-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-file-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-file-l1-2-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-file-l2-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-handle-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-heap-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-interlocked-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-libraryloader-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-localization-l1-2-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-memory-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-namedpipe-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-processenvironment-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-processthreads-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-processthreads-l1-1-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-profile-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-rtlsupport-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-string-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-synch-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-synch-l1-2-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-sysinfo-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-timezone-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-core-util-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\yisell_sound_48k.pcm"; DestDir: "{app}"; Flags: ignoreversion


Source: "..\Win32\Release\swresample-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\msvcp140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\vcruntime140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\api-ms-win-crt-runtime-l1-1-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\swscale-4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
 
Source: "..\Win32\Release\msvcp120.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\msvcr120.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Win32\Release\msvcp100.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\msvcr100.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Win32\Release\msvcp120d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\msvcr120d.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Win32\Release\msvcp100d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\msvcr100d.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\postproc-54.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\nghttp2.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Win32\Release\libvlccore.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\libvlc.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\libssl-1_1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\libssh2.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "..\Win32\Release\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\libcrypto-1_1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\avutil-55.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\avformat-57.dll"; DestDir: "{app}"; Flags: ignoreversion


Source: "..\Win32\Release\avfilter-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\avdevice-57.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\avcodec-57.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Win32\Release\VhallClassRoom.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
;Name: "{group}\Vhall {cm:voffline}"; Filename: "{app}\Vhall_{cm:voffline}.exe";IconFilename:"{app}\EventMaker.ico";  Parameters: "/offline"
;Name: "{group}\{cm:vname}"; Filename: "{app}\vbp.exe"; Parameters: "-m console"
;Name: "{group}\uninstall {cm:vname}"; Filename: "{app}\vbp.exe";Parameters: "-m service -u uninstall"

Name: "{group}\空中课堂"; Filename: "{app}\VhallClassRoom.exe"; IconFilename:"{app}\class.ico"
Name: "{group}\{cm:UninstallProgram,空中课堂}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\空中课堂"; Filename: "{app}\VhallClassRoom.exe"; IconFilename:"{app}\class.ico"

[Registry]
;delete vpb bootstart
Root: HKLM; Subkey: "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; ValueType: string; ValueName: "vbp"; ValueData:  "";Flags:uninsdeletevalue
;Root: HKLM; Subkey: "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; ValueType: string; ValueName: "VhallService"; ValueData:  "{app}\VhallService.exe -i";Flags:uninsdeletevalue
Root: HKLM; Subkey: "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall"; ValueType: string; ValueName: "UninstallString"; ValueData:  "{app}\VhallClassRoom";Flags:uninsdeletevalue
Root: HKCR; Subkey: "VHALLCLASSROOM"; ValueType: string;ValueName: "URL Protocol"; ValueData:  "{app}\VhallClassRoom";Flags:uninsdeletevalue
Root: HKCR; Subkey: "VHALLCLASSROOM\shell\open\command";  ValueType: string; ValueData:  "{app}\VhallClassRoom %1";Flags:uninsdeletevalue

[InstallDelete]
Type: files; Name: "{app}\vbp.log";BeforeInstall:killvbp()
;Type: files; Name: "{app}\VhallService.log";BeforeInstall:UnInstallWinService('VhallService.exe')
[UninstallRun]
Filename: "taskkill";Parameters: "/IM  VhallClassRoom.exe"; Flags: waituntilterminated
;Filename: "{app}\VhallService.exe ";Parameters: "-d"; Flags: waituntilterminated runhidden


[Run]
;Filename: "{app}\dxsetup\dxwebsetup.exe"; WorkingDir: "{app}\dxsetup"; Parameters: ""; Check: detectDirectX(ExpandConstant('{app}\dxsetup\dxwebsetup.exe')); Flags: waituntilterminated;
;Filename: "{app}\VhallService.exe"; Parameters: "-i"; Description: "{cm:vname}"; Flags: nowait runhidden
Filename: "{app}\VhallClassRoom.exe"; Parameters: "-r restart"; Description: "{cm:vname}"; Flags: nowait runhidden

[ISFormDesigner]
WizardForm=FF0A005457495A415244464F524D0030104601000054504630F10B5457697A617264466F726D0A57697A617264466F726D0C436C69656E74486569676874034C010B436C69656E74576964746803F101134F6E436F6E73747261696E6564526573697A650719506167654465736372697074696F6E4C6162656C436C69636B0D4578706C6963697457696474680301020E4578706C696369744865696768740373010D506978656C73506572496E636802600A54657874486569676874020C00F10C544E65774E6F7465626F6F6B0D4F757465724E6F7465626F6F6B00F110544E65774E6F7465626F6F6B5061676509496E6E65725061676500F1065450616E656C094D61696E50616E656C00F10E544E65775374617469635465787414506167654465736372697074696F6E4C6162656C074F6E436C69636B0719506167654465736372697074696F6E4C6162656C436C69636B000000000000



