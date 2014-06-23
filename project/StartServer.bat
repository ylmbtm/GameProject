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
echo.             4.退出命令窗口
echo.
:cho
set choice=
set /p choice=          请选择:
IF NOT "%choice%"=="" SET choice=%choice:~0,1%
if /i "%choice%"=="1" start /D Server\Debug\ /MIN ./Server/Debug/CenterServer.exe & start /D .\Server\Debug\ /MIN ./Server/Debug/LoginServer.exe & start /D .\Server\Debug\ /MIN ./Server/Debug/ProxyServer.exe & start /D .\Server\Debug\ /MIN ./Server/Debug/DBServer.exe & start /D .\Server\Debug\ /MIN ./Server/Debug/GameServer.exe
if /i "%choice%"=="2" start /D Server\Release\ /MIN ./Server/Release/CenterServer.exe & start /D .\Server\Release\ /MIN ./Server/Release/LoginServer.exe & start /D .\Server\Release\ /MIN ./Server/Release/ProxyServer.exe & start /D .\Server\Release\ /MIN ./Server/Release/DBServer.exe & start /D .\Server\Release\ /MIN ./Server/Release/GameServer.exe
if /i "%choice%"=="3" taskkill /im CenterServer.exe & taskkill /im LoginServer.exe & taskkill /im ProxyServer.exe & taskkill /im DBServer.exe & taskkill /im GameServer.exe
if /i "%choice%"=="4" exit
goto cho


