#!/system/bin/busybox sh

echo "do preinstall job"
BUSYBOX="/system/bin/busybox"

if [ ! -e /data/system.notfirstrun ]; then		
	/system/bin/sh /system/bin/pm preinstall /system/preinstall

	# copy android modify tool files
	mkdir /mnt/nanda
	mount -t vfat /dev/block/nanda /mnt/nanda
#	$BUSYBOX cp /mnt/nanda/vendor/initlogo.rle /
	$BUSYBOX cp /mnt/nanda/vendor/system/build.prop /system/
	$BUSYBOX cp /mnt/nanda/vendor/system/media/bootanimation.zip /system/media/
	$BUSYBOX cp /mnt/nanda/vendor/system/usr/keylayout/*.kl /system/usr/keylayout/
	sync
	umount /mnt/nanda
	rmdir /mnt/nanda

	$BUSYBOX touch /data/system.notfirstrun	

fi

echo "preinstall ok"

