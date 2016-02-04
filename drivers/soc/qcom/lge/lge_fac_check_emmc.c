/* arch/arm/mach-msm/lge/lge_fac_check_emmc.c
 *
 * Interface to calibrate display color temperature.
 *
 * Copyright (C) 2012 LGE
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <soc/qcom/lge/board_lge.h>

static struct factory_check_emmc_platform_data *ffactory_check_emmc_pdata;
static char factory_check_emmc_buf[4096];
unsigned int check_emmc_cnt= 0;

int factory_check_emmc_set_values(int kcal_r, int kcal_g, int kcal_b)
{
	return 0;
}

static int factory_check_emmc_get_values(int *kcal_r, int *kcal_g, int *kcal_b)
{
	return 0;
}

static struct factory_check_emmc_platform_data factory_check_emmc_pdata = {
	.set_values = factory_check_emmc_set_values,
	.get_values = factory_check_emmc_get_values,
};


static struct platform_device factory_check_emmc_platrom_device = {
	.name   = "factory_check_emmc_ctrl",
	.dev = {
		.platform_data = &factory_check_emmc_pdata,
	}
};
/*
void __init lge_add_factory_check_emmc_devices(void)
{
	pr_info(" PRE_SELFD_DEBUG : %s\n", __func__);
	platform_device_register(&factory_check_emmc_platrom_device);
}
*/
static int __init factory_check_emmc_init(void)
{
	return platform_device_register(&factory_check_emmc_platrom_device);
}
//module_init(factory_check_emmc_init);


static ssize_t factory_check_emmc_store(struct device *dev, struct device_attribute *attr,
        const char *buf, size_t count)
{
///*
    char str[512];
    int str_len = 0;
    if ( !lge_get_factory_boot() ) return -EINVAL;

    if ( buf[0] =='$' )
    {
        memset(factory_check_emmc_buf, 0, 4096); check_emmc_cnt = 0;
        return -EINVAL;
    }
    str_len = sprintf(str, "%s", buf);
    if ( (check_emmc_cnt+str_len) > 4096 )
    {
        sprintf((char *) &factory_check_emmc_buf[check_emmc_cnt], "$");
        check_emmc_cnt++;
        return check_emmc_cnt;
    }

    sprintf((char *) &factory_check_emmc_buf[check_emmc_cnt], "%s", buf);
    check_emmc_cnt += str_len;
//*/
    return check_emmc_cnt;
}

static ssize_t factory_check_emmc_show(struct device *dev, struct device_attribute *attr,
        char *buf)
{
    ssize_t ret_len = 0;
    ret_len = snprintf(buf, PAGE_SIZE, "%s", factory_check_emmc_buf);
    return ret_len;

}

static DEVICE_ATTR(factory_check_emmc, 0644, factory_check_emmc_show, factory_check_emmc_store);

int lge_fac_check_emmc(char *drv_bus_code, int func_code, char *dev_code, char *drv_code, int errno)
{
///*
    char str[512];
    int str_len = 0;
    if ( !lge_get_factory_boot() ) return 1;

    str_len = sprintf(str, "%s|%d|%s|%s|%d\n", drv_bus_code, func_code, dev_code, drv_code, errno);
    memset(str, 0, 512);
    str_len = 0;
    if ( (check_emmc_cnt+str_len) > 4096 )
    {
        //sprintf((char *) &factory_check_emmc_buf[check_emmc_cnt], "$");
        //check_emmc_cnt++;
        return 1;
    }

    sprintf((char *) &factory_check_emmc_buf[check_emmc_cnt], "%s|%d|%s|%s|%d\n", drv_bus_code, func_code, dev_code, drv_code, errno);
    memset(factory_check_emmc_buf, 0, 4096);
    check_emmc_cnt += str_len;
//*/
    return 0;
}

static int factory_check_emmc_ctrl_probe(struct platform_device *pdev)
{
    int rc = 0;

    ffactory_check_emmc_pdata = pdev->dev.platform_data;

    rc = device_create_file(&pdev->dev, &dev_attr_factory_check_emmc);
    if (rc != 0)
        return -1;
    return 0;
}

static struct platform_driver this_driver = {
    .probe  = factory_check_emmc_ctrl_probe,
    .driver = {
        .name   = "factory_check_emmc_ctrl",
    },
};

int __init factory_check_emmc_ctrl_init(void)
{
    return platform_driver_register(&this_driver);
}

device_initcall(factory_check_emmc_init);
device_initcall(factory_check_emmc_ctrl_init);

MODULE_DESCRIPTION("LGE Factory Check Emmc driver");
MODULE_LICENSE("GPL v2");
