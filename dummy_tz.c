/*
 * dummy_tz.c Simple usage of kernel thermal zone API
 *
 * The parts we access in the PCI config space for 00:0.0 is undocumented.
 * This driver just mimics a bash script written by CompuLab.
 *
 * Copyright (c) 2010, Patrik Jakobsson <patrik.jakobsson@home.se>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/thermal.h>
#define TEMP_CRIT	119000
#define TEMP_TRIP	119000



struct dummy_tz {
	int id;
	struct thermal_zone_device *dev;
		};

static struct dummy_tz *tz;

static int get_temp(struct thermal_zone_device *thermal_zone,
		    int *temperature)
{
	u32 temp1 = 50000;
		*temperature = temp1;

	return 0;
}

static int get_crit_temp(struct thermal_zone_device *tz, int *temperature)
{
	*temperature = TEMP_CRIT;

	return 0;
}

static int get_trip_type(struct thermal_zone_device *tz, int trip,
			 enum thermal_trip_type *type)
{
	if (trip == 0)
		*type = THERMAL_TRIP_ACTIVE;

	return 0;
}

static int get_trip_temp(struct thermal_zone_device *tz, int trip,
			 int *temperature)
{
	if (trip == 0)
		*temperature = TEMP_TRIP;

	return 0;
}

struct thermal_zone_device_ops dummy_ops = {
	.get_temp = get_temp,
	.get_crit_temp = get_crit_temp,
	.get_trip_temp = get_trip_temp,
	.get_trip_type = get_trip_type,
};

static int __init dummy_init(void)
	{

	tz = kzalloc(sizeof(struct dummy_tz), GFP_KERNEL);
	if (!tz)
		return -ENOMEM;

	tz->dev = thermal_zone_device_register("DUMMY", 1, 0,
						  NULL, &dummy_ops, 0, 0, 0);
	if (!tz->dev)
		return -EBUSY;

	printk(KERN_NOTICE "Dummy thermal driver loaded!\n");
		return 0;
	}

static void __exit dummy_exit(void)
{
	thermal_zone_device_unregister(tz->dev);

	kfree(tz);
}

module_init(dummy_init);
module_exit(dummy_exit);

MODULE_AUTHOR("Matt Cooper <doopermanofthemountain@gmail.com>");
MODULE_DESCRIPTION("Dummy thermal zone driver");
MODULE_LICENSE("GPL");
