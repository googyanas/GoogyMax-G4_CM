#!/bin/sh
export KERNELDIR=`readlink -f .`
export PARENT_DIR=`readlink -f ..`
# export CROSS_COMPILE=/home/googy/Kernel/Googy-Max-G4_CM/toolchain/bin/aarch64-linux-android-
# export CROSS_COMPILE=/usr/bin/aarch64-linux-gnu-
export PATH=/home/googy/Téléchargements/linaro49/bin:$PATH
export CROSS_COMPILE=/home/googy/Téléchargements/linaro49/bin/aarch64-linux-gnu-
# export CROSS_COMPILE=/home/googy/Downloads/arm-cortex-linux-gnueabi-linaro_4.9.4-2015.06/bin/arm-cortex-linux-gnueabi-
export KCONFIG_NOTIMESTAMP=true

VER="\"-Googy-Max-G4_CM-v$1\""
cp -f /home/googy/Kernel/Googy-Max-G4_CM/Kernel/arch/arm64/configs/googymax-g4_defconfig /home/googy/Kernel/Googy-Max-G4_CM/googymax-g4_defconfig
sed "s#^CONFIG_LOCALVERSION=.*#CONFIG_LOCALVERSION=$VER#" /home/googy/Kernel/Googy-Max-G4_CM/googymax-g4_defconfig > /home/googy/Kernel/Googy-Max-G4_CM/Kernel/arch/arm64/configs/googymax-g4_defconfig

export ARCH=arm64
export SUBARCH=arm64

find -name '*.ko' -exec rm -rf {} \;

rm -f /home/googy/Kernel/Googy-Max-G4_CM/Kernel/arch/arm64/boot/Image*.*
rm -f /home/googy/Kernel/Googy-Max-G4_CM/Kernel/arch/arm64/boot/.Image*.*
make googymax-g4_defconfig || exit 1

make -j4 || exit 1

mkdir -p /home/googy/Kernel/Googy-Max-G4_CM/Release/system/lib/modules
rm -rf /home/googy/Kernel/Googy-Max-G4_CM/Release/system/lib/modules/*
find -name '*.ko' -exec cp -av {} /home/googy/Kernel/Googy-Max-G4_CM/Release/system/lib/modules/ \;
${CROSS_COMPILE}strip --strip-unneeded /home/googy/Kernel/Googy-Max-G4_CM/Release/system/lib/modules/*

./tools/dtbToolCM --force-v2 -o /home/googy/Kernel/Googy-Max-G4_CM/Out/dt.img -s 4096 -p ./scripts/dtc/ arch/arm64/boot/dts/

cd /home/googy/Kernel/Googy-Max-G4_CM/Out
./packimg.sh

cd /home/googy/Kernel/Googy-Max-G4_CM/Release
zip -r ../Googy-Max-G4_CM_Kernel_${1}.zip .

adb push /home/googy/Kernel/Googy-Max-G4_CM/Googy-Max-G4_CM_Kernel_${1}.zip /sdcard/Googy-Max-G4_CM_Kernel_${1}.zip

adb kill-server

echo "Googy-Max-G4_CM_Kernel_${1}.zip READY !"
