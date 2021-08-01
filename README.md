# Simple OS Demo

![image](https://i.imgur.com/KMH6EcY.gif)

## Functions

- Boots with grub in qemu or USB flash
- Printing
- Input string (supports Shift, BackSpace, Enter, Esc)
- Precise nanosleep using PIT
- Move cursor
- Pseudo random number generator
- Shows CPU vendor string
- Shows date and time (updates every second)
- Shows real-time colorful animation

## Nanosleep implementation

The idea behind nanosleep implementation: read PIT counter and calculate number of ticks that happened after the previous read. This results in precise delay, but as interrupt handling is not used, CPU cannot do other work. In this case there is no other work, so it is ok. In real world you should better implement interrupt handlers.

## Background

Based on [this](https://www.youtube.com/watch?v=FkrpUaGThTQ&list=PLZQftyCk7_SeZRitx5MjBKzTtvk0pHMtp&index=1) tutorial by CodePulse, and [this](https://github.com/Roseinabox28/MagickOS) project.

## Setup

Build image for docker env:

- `docker build buildenv -t magickos-buildenv`

## Build

- Linux or MacOS: `docker run --rm -it -v "$pwd":/root/env magickos-buildenv`
- Windows (cmd): `docker run --rm -it -v "%cd%":/root/env magickos-buildenv`
- Windows (Powershell): `docker run --rm -it -v "${pwd}:/root/env" magickos-buildenv`

Build for x86:

- `make build-x86_64`

to leave the build environment type `exit`

## Emulate

Emulate Magick-OS using Qemu

- `qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso`

use this one if you have added to path and the previous doesn't work

- `"C:\Program Files\qemu\qemu-system-x86_64.exe" -cdrom dist/x86_64/kernel.iso`

## How to boot this image on real hardware.

First open real Command Prompt (do not use Far or anything else). Then go to your local folder, where os-series is:

```
cd x:\app\os-series
x:
docker run --rm -it -v "%cd%":/root/env myos-buildenv
make build-x86_64
```

Now dist/x86_64/ will contain kernel.iso file

Insert USB flash drive

Download Rufus

![image](https://user-images.githubusercontent.com/15198808/127573774-8b746020-bbcb-402c-ae2b-91dee1957b5b.png)
![image](https://user-images.githubusercontent.com/15198808/127573784-d2187d4d-2a3c-4287-a297-f8194961b838.png)
