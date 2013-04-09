#!/bin/bash
function cdevice()
{	
	cd $DEVICE
}

function cout()
{
	cd $OUT	
}

function extract-bsp()
{
	LICHEE_DIR=$ANDROID_BUILD_TOP/../lichee
	LINUXOUT_DIR=$LICHEE_DIR/out/android
	LINUXOUT_MODULE_DIR=$LICHEE_DIR/out/android/lib/modules/*/*
	CURDIR=$PWD

	cd $DEVICE

	#extract kernel
	if [ -f kernel ]; then
		rm kernel
	fi
	cp $LINUXOUT_DIR/bImage kernel
	echo "cp $LINUXOUT_DIR/bImage kernel"
	echo "$DEVICE/bImage copied!"

	#extract linux modules
	if [ -d modules ]; then
		rm -rf modules
	fi
	mkdir -p modules/modules
	cp -rf $LINUXOUT_MODULE_DIR modules/modules
	echo "$DEVICE/modules copied!"
	chmod 0755 modules/modules/*

# create modules.mk
(cat << EOF) > ./modules/modules.mk 
# modules.mk generate by extract-files.sh , do not edit it !!!!
PRODUCT_COPY_FILES += \\
	\$(call find-copy-subdir-files,*,\$(LOCAL_PATH)/modules,system/vendor/modules)

EOF

	cd $CURDIR
}

function make-all()
{
	# check lichee dir
	LICHEE_DIR=$ANDROID_BUILD_TOP/../lichee
	if [ ! -d $LICHEE_DIR ] ; then
		echo "$LICHEE_DIR not exists!"
		return
	fi

	extract-bsp
	m -j8
} 

# copy files for AndroidModify tool
function copy_android_modify_tool_files()
{
# sun4i chip
	SUN4I_DIR=$(gettop)/../lichee/tools/pack/chips/sun4i
	if [ -d $SUN4I_DIR ] ; then
		VENDOR=$(gettop)/../lichee/tools/pack/chips/sun4i/wboot/bootfs/vendor
		rm $VENDOR -rf
		mkdir $VENDOR
		mkdir $VENDOR/system
		mkdir $VENDOR/system/media
		mkdir $VENDOR/system/usr
		mkdir $VENDOR/system/usr/keylayout
		cp $OUT/system/build.prop $VENDOR/system
		cp $OUT/system/media/bootanimation.zip $VENDOR/system/media
		cp $OUT/system/usr/keylayout/*.kl $VENDOR/system/usr/keylayout
		echo "android modify tools files sun4i copied!"
	fi

# sun5i chip
	SUN5I_DIR=$(gettop)/../lichee/tools/pack/chips/sun5i
	if [ -d $SUN5I_DIR ] ; then
		VENDOR=$(gettop)/../lichee/tools/pack/chips/sun5i/wboot/bootfs/vendor
		rm $VENDOR -rf
		mkdir $VENDOR
		mkdir $VENDOR/system
		mkdir $VENDOR/system/media
		mkdir $VENDOR/system/usr
		mkdir $VENDOR/system/usr/keylayout
		cp $OUT/system/build.prop $VENDOR/system
		cp $OUT/system/media/bootanimation.zip $VENDOR/system/media
		cp $OUT/system/usr/keylayout/*.kl $VENDOR/system/usr/keylayout
		echo "android modify tools files sun5i copied!"
	fi

}

function pack()
{
	T=$(gettop)
	export CRANE_IMAGE_OUT=$OUT
	export PACKAGE=$T/../lichee/tools/pack

	copy_android_modify_tool_files
	echo "Device: $DEVICE"
	sh $DEVICE/package.sh $1
}

function exdroid_diff()
{
	echo "please check v1, v2 in build/tools/exdroid_diff.sh (^C to break)"
	read
	repo forall -c '$ANDROID_BUILD_TOP/build/tools/exdroid_diff.sh'	
}

function exdroid_patch()
{
	echo "please confirm this is v1 (^C to break)"
	read
	repo forall -c '$ANDROID_BUILD_TOP/build/tools/exdroid_patch.sh'	
}
