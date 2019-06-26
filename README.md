# BruCONBadge2019

if you can read this you have been invited here,
the repo is private to avoid spoiling the suprise before the con

We need work on the following areas,
please under the area where you will work here :

# backend
## Tech
mariadb / php / json
(
 yes, i known but don't care,
 it should be maintenable with skills we know will be here next year
)

## Who
Jegeva

# art
## Tech
please don't use prorietary formats

## Who

## tools
the bmp88to12bits.c in the firmware folder (compile it of course)
produces a C array in the correct format for the badge, it expects
a 132x132, RGB888 bmp

# firmware
## Tech
esp32 - freertos - C
## Who
Jegeva

## Tools
----------------
Build env
dockeriser it for you guys
foresee ~2GB
$cd docker
$chmod u+x ./build.sh
$./build.sh

it will tag it esp32 as scripts will depend on the tag

to open a shell with the project directory and the device for the badge
mounted in the docker
$cd docker
$chmod u+x ./shell.sh
$./shell.sh /dev/ttyUSB0

----------------