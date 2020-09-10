set path=%path%;"C:\Program Files (x86)\Windows Kits\8.1\bin\x86"
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VHWebSocket.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VHClassSDK.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallWebRtcSDK.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallWebRtcLive.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallVHMonitorCapture.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallOBSApi.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallOBS.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallNetWork.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallMediaReader.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallDShowPlugin.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/Vhalldshowcapture.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallDecklinkDevice.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallAudioEngine.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/vhall_webrtc_sdk.dll
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/cmd.exe
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p vhall20200701  /hide /c /tr http://timestamp.digicert.com /file ../x64/Release/VhallClassRoom.exe
pause