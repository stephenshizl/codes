@echo off
TITLE Gamepad �Զ����߻����ļ����빤��
color 3f
echo.������USB...
echo.
adb wait-for-device
adb devices
adb root
adb push busybox /sdcard/
If errorlevel 1 (
    Echo ����busybosxʧ�ܣ���������˳�.
    goto end
)
adb push wakeup.sh /sdcard/
If errorlevel 1 (
    Echo ����wakeup.shʧ�ܣ���������˳�.
    goto end
)
echo.
adb shell "cd /data/;cp /sdcard/wakeup.sh ./"
If errorlevel 1 (
    Echo û��Ȩ�ޣ����ROOTȨ��.
    goto end
)
adb shell "chmod 777 /data/wakeup.sh"
start /b adb shell "cd /data/;./wakeup.sh&"
echo.����������γ�USB.
:end
pause >nul
exit
