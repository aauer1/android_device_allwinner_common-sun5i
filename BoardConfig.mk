# BoardConfig.mk
#
# Product-specific compile-time definitions.
#

# cpu stuff
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT := armv7-a-neon
ARCH_ARM_HAVE_TLS_REGISTER := true

TARGET_BOARD_PLATFORM := sun5i
TARGET_BOOTLOADER_BOARD_NAME := a13

USE_OPENGL_RENDERER := true

# use our own init.rc
TARGET_PROVIDES_INIT_RC :=true

# no hardware camera
USE_CAMERA_STUB := true

#EGL stuff
BOARD_EGL_CFG := device/allwinner/common-sun5i/egl/egl.cfg
USE_OPENGL_RENDERER := true
ENABLE_WEBGL := true
BOARD_USE_SKIA_LCDTEXT := true
TARGET_BOOTANIMATION_PRELOAD := true
TARGET_BOOTANIMATION_TEXTURE_CACHE := true

#audio
HAVE_HTC_AUDIO_DRIVER := true
BOARD_USES_GENERIC_AUDIO := true
#gps 
#"simulator":taget board does not have a gps hardware module;"haiweixun":use the gps module offer by haiweixun 
BOARD_USES_GPS_TYPE := simulator

# Set /system/bin/sh to ash, not mksh, to make sure we can switch back.
# TARGET_SHELL := ash

# audio & camera & cedarx
CEDARX_CHIP_VERSION := F23
CEDARX_USE_SWAUDIO := Y

# use our own su for root
BOARD_USES_ROOT_SU := true

# hardware module include file path
TARGET_HARDWARE_INCLUDE := $(TOP)/device/allwinner/common-sun5i/hardware/include
TARGET_SPECIFIC_HEADER_PATH := device/allwinner/common-sun5i/hardware/include

MALI:
	make -C $(TOP)/device/allwinner/common-sun5i/mali LICHEE_KDIR=${KERNEL_OUT} ARCH=arm CROSS_COMPILE=arm-eabi-
	cp $(TOP)/device/allwinner/common-sun5i/mali/*.ko $(KERNEL_MODULES_OUT)

CEDARX: 
	make -C $(TOP)/device/allwinner/common-sun5i/cedarx LICHEE_KDIR=${KERNEL_OUT} ARCH=arm CROSS_COMPILE=arm-eabi-
	cp $(TOP)/device/allwinner/common-sun5i/cedarx/*.ko $(KERNEL_MODULES_OUT)

TARGET_KERNEL_MODULES := MALI CEDARX
