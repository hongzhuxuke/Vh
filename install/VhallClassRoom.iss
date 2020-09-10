
#define EXE_VER "3.1.0.0"


[Setup]
AppPublisher=vhall.com
AppPublisherURL=http://www.vhall.com/
AppSupportURL=http://www.vhall.com/
AppUpdatesURL=http://www.vhall.com/
SetupIconFile=..\VLive\VhallClassRoom.ico
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
    end;
    tmpFile := axFile + '.1';
    if( FileExists(tmpFile))  then
    begin
      tmp := ' -u  -s \"' + axFile + '.1\"';
      Exec('regsvr32',tmp,'',SW_HIDE, ewWaitUntilTerminated,ResultCode);
      DeleteFile(tmpFile);
    end;

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
    end;
    
  if RegQueryStringValue(HKLM, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\微吼课堂_is1', 'UninstallString', ResultStr) then
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
chs.vname=微吼课堂
en.regist=Register
en.registerInfo=if using all function please register Vhall class accounts.
en.exitVhall=Please Exit Vhall class then press Install  Button.
chs.regist=免费注册
chs.registerInfo=.

en.MyGroupName=Vhall Class Streaming
chs.MyGroupName=微吼课堂
                                 
en.vname=VhallClass
chs.exitVhall=请先退出微吼课堂再进行安装.

en.wrongOS=Please use the operating system which is Windows 7 or higher version! The setup program will exit!
chs.wrongOS=请使用WIN7及以上的操作系统！无法运行程序，本安装程序即将退出！
en.detectNoDX=Your machine don't have the necessary DirectX components! Will you install it right now?
chs.detectNoDX=系统检测到您没有安装必需的DirectX组件，是否立刻安装？
en.DXWarning=DirectX is not installed. Vhall Live Streaming may not be able to run normally!
chs.DXWarning=没有安装DirectX组件，微吼课堂可能无法正常使用！

[Messages]
en.BeveledLabel=Life is not dress rehearsal, living every day!
chs.BeveledLabel=人生没有彩排，每天都是现场直播！

[Tasks]
;Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
;Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}";
[Components]
;Name: "readme"; Description: "Readme File"; Types: full
[Files]

Source: "..\VLive\VhallClassRoom.ico"; BeforeInstall:killprocess('VhallClassRoom.exe');DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\vhall_class_config.ini"; DestDir: "{userdocs}\VhallClass"; Flags: ignoreversion
Source: "..\x64\Release\config.ini"; DestDir: "{userdocs}\VhallClass";  Flags: onlyifdoesntexist uninsneveruninstall

     

                          
; Lib CEF
Source: "..\x64\Release\vh_webview_dll.lib"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\v8_context_snapshot.bin"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\snapshot_blob.bin"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\natives_blob.bin"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\icudtl.dat"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\devtools_resources.pak"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\locales\*.*"; DestDir: "{app}\locales"; Flags: ignoreversion

Source: "..\x64\Release\*.pak"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\swiftshader\*.*"; DestDir: "{app}\swiftshader"; Flags: ignoreversion
Source: "..\x64\Release\icudtl.dat"; DestDir: "{app}\swiftshader"; Flags: ignoreversion
Source: "..\x64\Release\*.bin"; DestDir: "{app}\swiftshader"; Flags: ignoreversion

;QT release

Source: "..\x64\Release\default_pushPic.png"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\timeEnd.mp3"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\bearer\*.*"; DestDir: "{app}\bearer"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\x64\Release\iconengines\*.*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\x64\Release\imageformats\*.*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\x64\Release\platforms\*.*"; DestDir: "{app}\platforms";Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\x64\Release\playlistformats\*.*"; DestDir: "{app}\playlistformats"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\x64\Release\plugins\*.*"; DestDir: "{app}\plugins"; Flags: ignoreversion recursesubdirs createallsubdirs;
;Source: "..\x64\Release\position\*.*"; DestDir: "{app}\position"; Flags: ignoreversion recursesubdirs createallsubdirs;
;Source: "..\x64\Release\printsupport\*.*"; DestDir: "{app}\printsupport"; Flags: ignoreversion recursesubdirs createallsubdirs;
;Source: "..\x64\Release\resources\*.*"; DestDir: "{app}\resources";Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\x64\Release\styles\*.*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\x64\Release\translations\*.*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\x64\Release\audio\*.*"; DestDir: "{app}\audio"; Flags: ignoreversion recursesubdirs createallsubdirs;
Source: "..\x64\Release\mediaservice\*.*"; DestDir: "{app}\mediaservice"; Flags: ignoreversion recursesubdirs createallsubdirs;

Source: "..\x64\Release\yisell_sound_48k.pcm"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\*.exe"; DestDir: "{app}"; Flags: ignoreversion         
Source: "..\x64\Release\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\swiftshader\*.dll"; DestDir: "{app}\swiftshader"; Flags: ignoreversion
Source: "..\x64\Release\GPUCache\*.*"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\x64\Release\pepflash\x64\*.*"; DestDir: "{app}\pepflash\x64"; Flags: ignoreversion
   
   
[Icons]
;Name: "{group}\Vhall {cm:voffline}"; Filename: "{app}\Vhall_{cm:voffline}.exe";IconFilename:"{app}\EventMaker.ico";  Parameters: "/offline"
;Name: "{group}\{cm:vname}"; Filename: "{app}\vbp.exe"; Parameters: "-m console"
;Name: "{group}\uninstall {cm:vname}"; Filename: "{app}\vbp.exe";Parameters: "-m service -u uninstall"

Name: "{group}\微吼课堂"; Filename: "{app}\VhallClassRoom.exe"; IconFilename:"{app}\VhallClassRoom.ico"
Name: "{group}\{cm:UninstallProgram,微吼课堂}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\微吼课堂"; Filename: "{app}\VhallClassRoom.exe"; IconFilename:"{app}\VhallClassRoom.ico"

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
Filename: "taskkill";Parameters: "/IM  cefsimple.exe"; Flags: waituntilterminated
;Filename: "{app}\VhallService.exe ";Parameters: "-d"; Flags: waituntilterminated runhidden


[Run]
;Filename: "{app}\dxsetup\dxwebsetup.exe"; WorkingDir: "{app}\dxsetup"; Parameters: ""; Check: detectDirectX(ExpandConstant('{app}\dxsetup\dxwebsetup.exe')); Flags: waituntilterminated;
;Filename: "{app}\VhallService.exe"; Parameters: "-i"; Description: "{cm:vname}"; Flags: nowait runhidden
Filename: "{app}\VhallClassRoom.exe"; Parameters: "-r restart"; Description: "{cm:vname}"; Flags: nowait runhidden

[ISFormDesigner]
WizardForm=FF0A005457495A415244464F524D0030104601000054504630F10B5457697A617264466F726D0A57697A617264466F726D0C436C69656E74486569676874034C010B436C69656E74576964746803F101134F6E436F6E73747261696E6564526573697A650719506167654465736372697074696F6E4C6162656C436C69636B0D4578706C6963697457696474680301020E4578706C696369744865696768740373010D506978656C73506572496E636802600A54657874486569676874020C00F10C544E65774E6F7465626F6F6B0D4F757465724E6F7465626F6F6B00F110544E65774E6F7465626F6F6B5061676509496E6E65725061676500F1065450616E656C094D61696E50616E656C00F10E544E65775374617469635465787414506167654465736372697074696F6E4C6162656C074F6E436C69636B0719506167654465736372697074696F6E4C6162656C436C69636B000000000000



