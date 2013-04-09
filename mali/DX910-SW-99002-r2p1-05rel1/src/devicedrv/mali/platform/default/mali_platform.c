/*
 * Copyright (C) 2010-2011 ARM Limited. All rights reserved.
 * 
 * This program is free software and is provided to you under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation, and any use by you of this program is subject to the terms of such GNU licence.
 * 
 * A copy of the licence is included with the program, and can also be obtained from Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * @file mali_platform.c
 * Platform specific Mali driver functions for a default platform
 */
#include "mali_kernel_common.h"
#include "mali_osk.h"
#include "mali_platform.h"

#include <linux/module.h>  
#include <linux/clk.h>
#include <mach/gpio_v2.h>
#include <mach/irqs.h>
#include <mach/script_v2.h>


int mali_clk_div = 3;
module_param(mali_clk_div, int, S_IRUSR | S_IWUSR | S_IWGRP | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(mali_clk_div, "Clock divisor for mali");

struct clk *h_ahb_mali, *h_mali_clk, *h_ve_pll;

_mali_osk_errcode_t mali_platform_init(_mali_osk_resource_t *resource)
{
	unsigned long rate;
	int clk_div;
	int mali_used = 0;
	
	//get mali ahb clock
	h_ahb_mali = clk_get(NULL, "ahb_mali");
	if(!h_ahb_mali){
		MALI_PRINT(("try to get ahb mali clock failed!\n"));
	}
	//get mali clk
	h_mali_clk = clk_get(NULL, "mali");
	if(!h_mali_clk){
		MALI_PRINT(("try to get mali clock failed!\n"));
	}

	h_ve_pll = clk_get(NULL, "ve_pll");
	if(!h_ve_pll){
		MALI_PRINT(("try to get ve pll clock failed!\n"));
	}

	//set mali parent clock
	if(clk_set_parent(h_mali_clk, h_ve_pll)){
		MALI_PRINT(("try to set mali clock source failed!\n"));
	}
	
	//set mali clock
	rate = clk_get_rate(h_ve_pll);

	if(!script_parser_fetch("mali_para", "mali_used", &mali_used, 1)) {
		if (mali_used == 1) {
			if (!script_parser_fetch("mali_para", "mali_clkdiv", &clk_div, 1)) {
				if (clk_div > 0) {
					pr_info("mali: use config clk_div %d\n", clk_div);
					mali_clk_div = clk_div;
				}
			}
		}
	}

	pr_info("mali: clk_div %d\n", mali_clk_div);
	rate /= mali_clk_div;

	if(clk_set_rate(h_mali_clk, rate)){
		MALI_PRINT(("try to set mali clock failed!\n"));
	}
	
	if(clk_enable(h_ahb_mali)){
		MALI_PRINT(("try to enable mali ahb failed!\n"));
	}
	if(clk_enable(h_mali_clk)){
		MALI_PRINT(("try to enable mali clock failed!\n"));
	}
	if(clk_reset(h_mali_clk,0)){
		MALI_PRINT(("try to reset release failed!\n"));
	}
	MALI_PRINT(("mali clock set completed, clock is  %d Mhz\n", rate));
    MALI_SUCCESS;
}

_mali_osk_errcode_t mali_platform_deinit(_mali_osk_resource_type_t *type)
{
	//close clock
	MALI_PRINT(("free mali clk\n"));
	clk_disable(h_mali_clk);
	clk_disable(h_ahb_mali);
    MALI_SUCCESS;
}

_mali_osk_errcode_t mali_platform_powerdown(u32 cores)
{
    MALI_SUCCESS;
}

_mali_osk_errcode_t mali_platform_powerup(u32 cores)
{
    MALI_SUCCESS;
}

void mali_gpu_utilization_handler(u32 utilization)
{
}

#if MALI_POWER_MGMT_TEST_SUITE
u32 pmu_get_power_up_down_info(void)
{
	return 4095;

}
#endif
