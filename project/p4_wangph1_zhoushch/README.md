# Project 4: Longan Nano minigame

## How to play!!
Attention!!!
The up botton is broken, so when moving, use SW2 to instead U

After started:
- U/D to select level(U = press down)
- A/D to select number of stones
- BOOT0 to confirm settings
- SW1 to quit the game

Level start!
- press SW1 to pause

When pausing:
- press SW1 to continue the game
- press down the direction buttom to restart this level
- press BOOT0 to quit the level(go to level selection part)

When the ranking list appears,
- press SW1 to get to another level
- press BOOT0 to leave the game

Level 4:
Enter the Patrick level?
Maybe unfinished.

## how to burn
- make
- sudo ./dfu-util -l | grep 28e9:0189
- sudo ./dfu-util -a 0 --dfuse-address 0x08000000:leave -D .pio/build/sipeed-longan-nano/firmware.bin

if dfu-util command not found:
- chmod +x ./dfu-util



## original introduction
This repo contains the framework code for your Project 4. 

In this project, you will be implementing a Pong game using the Longan Nano development board we've given to you. We've provided you a framework here.

You can also choose to implement your own minigame.

We hope this project will help you experience programming RISC-V on a real machine rather than in a simulator.

For full details of this project, please see course website

## PLEASE READ

The projects are part of your design project worth 2 credit points. As such they run in parallel to the actual course. So be aware that the due date for project and homework might be very close to each other! Start early and do not procrastinate.

## Download Framework

Download the framework from gitlab and Check if you already have the following files after pulling.

```
.
├── LICENSE
├── Makefile
├── README.md
├── dfu-util
├── include
│   ├── README
│   ├── fatfs
│   │   ├── diskio.h
│   │   ├── ff.h
│   │   ├── ffconf.h
│   │   └── tf_card.h
│   ├── gd32v_pjt_include.h
│   ├── gd32vf103_libopt.h
│   ├── lcd
│   │   ├── bmp.h
│   │   ├── lcd.h
│   │   └── oledfont.h
│   ├── systick.h
│   └── utils.h
├── platformio.ini
└── src
    ├── assembly
    │   └── example.S
    ├── fatfs
    │   ├── 00history.txt
    │   ├── 00readme.txt
    │   ├── ff.c
    │   ├── ffsystem.c
    │   ├── ffunicode.c
    │   └── tf_card.c
    ├── lcd
    │   └── lcd.c
    ├── main.c
    ├── systick.c
    └── utils.c
```
