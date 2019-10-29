# BruCONBadge2019

This is the firmware that was released on the badges at BruCON2019.

It is in dire need of refactoring

## Tools

Build environment : dockerised it for you guys foresee ~2GB

```
$cd docker
$chmod u+x ./build.sh
$./build.sh
```
it will tag it esp32 as scripts will depend on the tag

to open a shell with the project directory and the device for the badge
mounted in the docker
```
$cd docker
$chmod u+x ./shell.sh
$./shell.sh /dev/ttyUSB0
```

