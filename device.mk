#
# Copyright (C) 2011 The Android Open-Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# full nuclear product config
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

#DEVICE_PACKAGE_OVERLAYS := device/allwinner/common-sun5i/overlay

PRODUCT_PACKAGES += \
	hwcomposer.sun5i \
	camera.exDroid \
	lights.sun5i \
	display.sun5i 

PRODUCT_PACKAGES += \
	audio.primary.exDroid \
	audio.a2dp.default \
	libaudioutils \
	libcedarxbase \
	libcedarxosal \
	libcedarxsftdemux \
	libcedarv \
	libswdrm \
	libstagefright_soft_cedar_h264dec \
	Camera \
	libjni_mosaic \
	FileExplore \
	u3gmonitor \
	chat \
	libjni_pinyinime

PRODUCT_PACKAGES += \
	e2fsck \
	libext2fs \
	libext2_blkid \
	libext2_uuid \
	libext2_profile \
	libext2_com_err \
	libext2_e2p \
	make_ext4fs

PRODUCT_PACKAGES += \
	LiveWallpapersPicker \
	LiveWallpapers \
	android.software.live_wallpaper.xml


# init.rc, kernel
PRODUCT_COPY_FILES += \
	device/allwinner/common-sun5i/init.rc:root/init.rc

# wifi conf
PRODUCT_COPY_FILES += \
	device/allwinner/common-sun5i/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf

# gps.conf
PRODUCT_COPY_FILES += \
	device/allwinner/common-sun5i/gps.conf:system/etc/gps.conf

# mali lib so
PRODUCT_COPY_FILES += \
	device/allwinner/common-sun5i/egl/gralloc.sun5i.so:system/lib/hw/gralloc.sun5i.so \
	device/allwinner/common-sun5i/egl/libMali.so:system/lib/libMali.so \
	device/allwinner/common-sun5i/egl/libUMP.so:system/lib/libUMP.so \
	device/allwinner/common-sun5i/egl/egl.cfg:system/lib/egl/egl.cfg \
	device/allwinner/common-sun5i/egl/libEGL_mali.so:system/lib/egl/libEGL_mali.so \
	device/allwinner/common-sun5i/egl/libGLESv1_CM_mali.so:system/lib/egl/libGLESv1_CM_mali.so \
	device/allwinner/common-sun5i/egl/libGLESv2_mali.so:system/lib/egl/libGLESv2_mali.so \

# preinstall script
PRODUCT_COPY_FILES += \
	device/allwinner/common-sun5i/preinstall.sh:/system/bin/preinstall.sh

#premission feature
PRODUCT_COPY_FILES += \
	device/allwinner/common-sun5i/tablet_core_hardware.xml:system/etc/permissions/tablet_core_hardware.xml \
    device/allwinner/common-sun5i/config/android.hardware.location.xml:system/etc/permissions/android.hardware.location.xml \
    device/allwinner/common-sun5i/config/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    device/allwinner/common-sun5i/config/android.hardware.touchscreen.xml:system/etc/permissions/android.hardware.touchscreen.xml \
    device/allwinner/common-sun5i/config/android.hardware.touchscreen.multitouch.distinct.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.distinct.xml 

#google service
#PRODUCT_COPY_FILES += \
#	$(call find-copy-subdir-files,*,$(LOCAL_PATH)/googleservice,system/app)

PRODUCT_PROPERTY_OVERRIDES += \
	dalvik.vm.heapsize=48m \
	ro.kernel.android.checkjni=0 \
	persist.sys.timezone=Europe/Berlin \
	persist.sys.language=en \
	persist.sys.country=GB \
	wifi.interface=wlan0 \
	wifi.supplicant_scan_interval=15 \
	debug.egl.hw=1 \
	ro.opengles.version=131072 \
	persist.sys.strictmode.visual=0 \
	persist.sys.strictmode.disable=1 \
	hwui.render_dirty_regions=false

# Overrides
PRODUCT_BRAND  := allwinner
PRODUCT_NAME   := common_sun5i
PRODUCT_DEVICE := common-sun5i


