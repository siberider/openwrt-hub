/*
 *  Atheros AR71xx SoC device definitions
 *
 *  Copyright (C) 2008-2009 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2008 Imre Kaloz <kaloz@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#ifndef __AR71XX_DEVICES_H
#define __AR71XX_DEVICES_H

#include <asm/mach-ar71xx/platform.h>

#include <linux/leds.h>
#include <linux/gpio_buttons.h>
#include <net/dsa.h>

void ar71xx_add_device_spi(struct ar71xx_spi_platform_data *pdata,
			   struct spi_board_info const *info,
			   unsigned n) __init;

void ar71xx_set_mac_base(unsigned char *mac) __init;
void ar71xx_parse_mac_addr(char *mac_str) __init;

extern struct ag71xx_platform_data ar71xx_eth0_data;
extern struct ag71xx_platform_data ar71xx_eth1_data;
void ar71xx_add_device_eth(unsigned int id) __init;

void ar71xx_add_device_mdio(u32 phy_mask) __init;

void ar71xx_add_device_leds_gpio(int id,
				 unsigned num_leds,
				 struct gpio_led *leds) __init;

void ar71xx_add_device_gpio_buttons(int id,
				    unsigned poll_interval,
				    unsigned nbuttons,
				    struct gpio_button *buttons) __init;

void ar71xx_add_device_usb(void) __init;

#ifdef CONFIG_AR71XX_EARLY_SERIAL
static inline void ar71xx_add_device_uart(void) {}
#else
void ar71xx_add_device_uart(void) __init;
#endif

void ar71xx_add_device_wdt(void) __init;

void ar91xx_add_device_wmac(void) __init;

void ar71xx_add_device_dsa(unsigned int id,
			   struct dsa_platform_data *d) __init;

#endif /* __AR71XX_DEVICES_H */