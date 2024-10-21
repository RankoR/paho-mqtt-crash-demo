# Paho MQTT C++ library crash demo

Minimal demo of crashing Paho MQTT library with release build.

## HOWTOConnecting to the MQTT server...OK
Sending test message
Waiting for messages on topic: 'paho/client/test/crash12345'
paho/client/test/crash12345: I will crash your app
paho/client/test/crash12345: I will crash your app

```shell
git submodule update --init --recursive
```

Build a release build and push it to Android device:

```shell
adb push build/android/x86_64/release/paho_mqtt_crash /data/local/tmp/
adb push /home/user/Android/Sdk/ndk/27.1.12297006/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/x86_64-linux-android/libc++_shared.so /data/local/tmp/
```

```shell
adb shell
```

On device:
```shell
cd /data/local/tmp
LD_LIBRARY_PATH=. ./paho_mqtt_crash
```

You will get a crash:

```text
Connecting to the MQTT server...OK
Sending test message
Segmentation fault 
```

But with a debug build it works fine:

```text
Connecting to the MQTT server...OK
Sending test message
Waiting for messages on topic: 'paho/client/test/crash12345'
paho/client/test/crash12345: I will crash your app
paho/client/test/crash12345: I will crash your app
```