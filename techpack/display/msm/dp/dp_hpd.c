// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2012-2019, The Linux Foundation. All rights reserved.
 */

#include <linux/slab.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/err.h>

#include "dp_hpd.h"
#include "dp_usbpd.h"
#include "dp_gpio_hpd.h"
#include "dp_lphw_hpd.h"
#include "dp_debug.h"
#ifdef CONFIG_SEC_DISPLAYPORT
#include "secdp.h"
#endif

static void dp_hpd_host_init(struct dp_hpd *dp_hpd,
		struct dp_catalog_hpd *catalog)
{
	if (!catalog) {
		DP_ERR("invalid input\n");
		return;
	}
	catalog->config_hpd(catalog, true);
}

static void dp_hpd_host_deinit(struct dp_hpd *dp_hpd,
		struct dp_catalog_hpd *catalog)
{
	if (!catalog) {
		DP_ERR("invalid input\n");
		return;
	}
	catalog->config_hpd(catalog, false);
}

static void dp_hpd_isr(struct dp_hpd *dp_hpd)
{
}

struct dp_hpd *dp_hpd_get(struct device *dev, struct dp_parser *parser,
		struct dp_catalog_hpd *catalog, struct dp_hpd_cb *cb)
{
	struct dp_hpd *dp_hpd;

	pr_debug("+++, no_aux_switch<%d>\n", parser->no_aux_switch);

	if (parser->no_aux_switch && parser->lphw_hpd) {
#ifndef CONFIG_SEC_DISPLAYPORT
		dp_hpd = dp_lphw_hpd_get(dev, parser, catalog, cb);
		if (IS_ERR(dp_hpd)) {
			DP_ERR("failed to get lphw hpd\n");
			return dp_hpd;
		}
		dp_hpd->type = DP_HPD_LPHW;
#endif
	} else if (parser->no_aux_switch) {
		dp_hpd = dp_gpio_hpd_get(dev, cb);
		if (IS_ERR(dp_hpd)) {
			DP_ERR("failed to get gpio hpd\n");
			return dp_hpd;
		}
		dp_hpd->type = DP_HPD_GPIO;
	} else {
#ifndef CONFIG_SEC_DISPLAYPORT
		dp_hpd = dp_usbpd_get(dev, cb);
#else
		dp_hpd = secdp_usbpd_get(dev, cb);
#endif
		if (IS_ERR(dp_hpd)) {
			DP_ERR("failed to get usbpd\n");
			return dp_hpd;
		}
		dp_hpd->type = DP_HPD_USBPD;
	}

	pr_debug("type<%d>\n", dp_hpd->type);

	if (!dp_hpd->host_init)
		dp_hpd->host_init	= dp_hpd_host_init;
	if (!dp_hpd->host_deinit)
		dp_hpd->host_deinit	= dp_hpd_host_deinit;
	if (!dp_hpd->isr)
		dp_hpd->isr		= dp_hpd_isr;

	return dp_hpd;
}

void dp_hpd_put(struct dp_hpd *dp_hpd)
{
	if (!dp_hpd)
		return;

	pr_debug("+++\n");

	switch (dp_hpd->type) {
	case DP_HPD_USBPD:
		dp_usbpd_put(dp_hpd);
		break;
	case DP_HPD_GPIO:
		dp_gpio_hpd_put(dp_hpd);
		break;
#ifndef CONFIG_SEC_DISPLAYPORT
	case DP_HPD_LPHW:
		dp_lphw_hpd_put(dp_hpd);
		break;
#endif
	default:
		DP_ERR("unknown hpd type %d\n", dp_hpd->type);
		break;
	}
}
