/**
 * Copyright (C) 2010 ARM Limited. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 *
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file mali_drv.c
 * Implementation of the Linux device driver entrypoints for Mali DRM
 */
#include <linux/vermagic.h>
#include "drmP.h"
#include "mali_drv.h"

void mali_drm_preclose(struct drm_device *dev)
{
}

void mali_drm_lastclose(struct drm_device *dev)
{
}

static int mali_drm_suspend(struct drm_device *dev)
{
	return 0;
}

static int mali_drm_resume(struct drm_device *dev)
{
	return 0;
}

static int mali_drm_load(struct drm_device *dev, unsigned long chipset)
{
	return 0;
}

static int mali_drm_unload(struct drm_device *dev)
{
	return 0;
}

static struct drm_driver driver = 
{
	.driver_features = DRIVER_USE_PLATFORM_DEVICE,
	.load = mali_drm_load,
	.unload = mali_drm_unload,
	.context_dtor = NULL,
	.reclaim_buffers = NULL,
	.reclaim_buffers_idlelocked = NULL,
	.preclose = mali_drm_preclose,
	.lastclose = mali_drm_lastclose,
	.suspend = mali_drm_suspend,
	.resume = mali_drm_resume,
	.get_map_ofs = drm_core_get_map_ofs,
	.get_reg_ofs = drm_core_get_reg_ofs,
	.ioctls = NULL,
	.fops = {
		 .owner = THIS_MODULE,
		 .open = drm_open,
		 .release = drm_release,
		 .unlocked_ioctl = drm_ioctl,
		 .mmap = drm_mmap,
		 .poll = drm_poll,
		 .fasync = drm_fasync,
	},
	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCHLEVEL,
};

int mali_drm_init(struct platform_device *dev)
{
	printk(KERN_INFO "Mali DRM initialize, driver name: %s, version %d.%d\n", DRIVER_NAME, DRIVER_MAJOR, DRIVER_MINOR);
	driver.num_ioctls = 0;
	driver.platform_device = dev;

	return drm_init(&driver);
}

void mali_drm_exit(void)
{
	drm_exit(&driver);
}

static int __devinit mali_platform_drm_probe(struct platform_device *dev)
{
	return mali_drm_init(dev);
}

static int mali_platform_drm_remove(struct platform_device *dev)
{
	mali_drm_exit();

	return 0;
}

static int mali_platform_drm_suspend(struct platform_device *dev, pm_message_t state)
{
	return 0;
}

static int mali_platform_drm_resume(struct platform_device *dev)
{
	return 0;
}


static struct platform_driver platform_drm_driver = {
	.probe = mali_platform_drm_probe,
	.remove = __devexit_p(mali_platform_drm_remove),
	.suspend = mali_platform_drm_suspend,
	.resume = mali_platform_drm_resume,
	.driver = {
		.owner = THIS_MODULE,
		.name = DRIVER_NAME,
	},
};

static int __init mali_platform_drm_init(void)
{
	return platform_driver_register( &platform_drm_driver );
}

static void __exit mali_platform_drm_exit(void)
{
	platform_driver_unregister( &platform_drm_driver );
}

#ifdef MODULE
module_init(mali_platform_drm_init);
#else
late_initcall(mali_platform_drm_init);
#endif
module_exit(mali_platform_drm_exit);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_LICENSE(DRIVER_LICENCE);
MODULE_ALIAS(DRIVER_ALIAS);
MODULE_INFO(vermagic, VERMAGIC_STRING);
