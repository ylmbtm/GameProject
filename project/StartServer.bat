:start
@echo off
color 0A
echo.
echo            ================================
echo             请选择要进行的操作，然后按回车
echo            ================================
echo.
echo.             1.启动全部服务器(DEBUG)
echo.             
echo.             2.启动全部服务器(RELEASE)
echo.     
echo.             3.关闭全部服务器
echo.
echo.             4.显示服务器配制
echo.
echo.             5.清除屏幕
echo.
echo.             6.退出命令窗口
echo.
echo.             
set DebugDir=%cd%\Server\Debug\
set ReleaseDir=%cd%\Server\Release\
echo.
echo Debug = %DebugDir%
echo Release= %ReleaseDir%
echo.


:cho
set choice=
set /p choice=          请选择:
IF NOT "%choice%"=="" SET choice=%choice:~0,1%
if /i "%choice%"=="1" start /D %DebugDir% /MIN %DebugDir%CenterServer.exe & start /D %DebugDir% /MIN %DebugDir%LoginServer.exe & start /D %DebugDir% /MIN %DebugDir%ProxyServer.exe & start /D %DebugDir% /MIN %DebugDir%DBServer.exe & start /D %DebugDir% /MIN %DebugDir%GameServer.exe
if /i "%choice%"=="2" start /D %DebugDir% /MIN %DebugDir%CenterServer.exe & start /D %DebugDir% /MIN %DebugDir%LoginServer.exe & start /D %DebugDir% /MIN %DebugDir%ProxyServer.exe & start /D %DebugDir% /MIN %DebugDir%DBServer.exe & start /D %DebugDir% /MIN %DebugDir%GameServer.exe
if /i "%choice%"=="3" taskkill /im CenterServer.exe & taskkill /im LoginServer.exe & taskkill /im ProxyServer.exe & taskkill /im DBServer.exe & taskkill /im GameServer.exe
if /i "%choice%"=="4" type %DebugDir%GameServer.ini & type %DebugDir%LoginServer.ini & type %DebugDir%ProxyServer.ini & type %DebugDir%CenterServer.ini & type %DebugDir%DBServer.ini
if /i "%choice%"=="5" cls & goto start
if /i "%choice%"=="6" exit
echo.
goto cho


