# BruCONBadge2019

if you can read this you have been invited here,
the repo is private to avoid spoiling the suprise before the con
We need work on the following areas,
***please under the area where you will work here :***
# Backend
## Tech
mariadb / php / json
( yes, i known but don't care, it should be maintenable with skills we know will be here next year )

## Who
Jegeva

# Art
## Tech
please don't use proprietary formats

## Who

## Tools
The bmp88to12bits.c in the firmware folder (compile it of course).
It produces a C array in the correct format for the badge, 
it expects a 132x132, RGB888 bmp.

# Firmware
## Tech
esp32 - freertos - C
## Who
Jegeva

## Tools

Build env : dockerised it for you guys foresee ~2GB
(that way be all work with the same versions of compiler and IDF)
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

# git and all

please make a branch for your changes,
keep your branches name significant
