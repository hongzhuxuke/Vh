set path=%path%;"C:\Program Files (x86)\Windows Kits\8.1\bin\x86"
wosigncodecmd  sign /dig sha256 /pfx vhall20200701.pfx /p  vhall20200701 /hide /c /tr http://timestamp.digicert.com /file ./setup/VhallClassRoom3.0.2.0.exe


