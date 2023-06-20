## Burn the borad

If "./dfu-util command not found":
chmod +x ./dfu-util

In the terminal:

Boot0 - reset - reset - Boot0

1.

    make

2.

    sudo ./dfu-util -l | grep 28e9:0189

3.

    sudo ./dfu-util -a 0 --dfuse-address 0x08000000:leave -D .pio/build/sipeed-longan-nano/firmware.bin
