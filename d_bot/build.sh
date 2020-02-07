#!/bin/bash

export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=~/zephyr

source ../zephyr/zephyr-env.sh

#if
west build --pristine -b nucleo_f446re -d ../build ./
#then xterm -e minicom -b 115200 -D /dev/ttyACM0
#fi
