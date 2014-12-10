@echo off
TITLE Gamepad 自动休眠唤醒文件导入工具
color 3f
echo.请连接USB...
echo.
adb wait-for-device
adb devices
adb root
adb push busybox /sdcard/
If errorlevel 1 (
    Echo 拷贝busybosx失败，按任意键退出.
    goto end
)
adb push wakeup.sh /sdcard/
If errorlevel 1 (
    Echo 拷贝wakeup.sh失败，按任意键退出.
    goto end
)
echo.
adb shell "cd /data/;cp /sdcard/wakeup.sh ./"
If errorlevel 1 (
    Echo 没有权限，请打开ROOT权限.
    goto end
)
adb shell "chmod 777 /data/wakeup.sh"
start /b adb shell "cd /data/;./wakeup.sh&"
echo.程序运行请拔出USB.
:end
pause >nul
exit
