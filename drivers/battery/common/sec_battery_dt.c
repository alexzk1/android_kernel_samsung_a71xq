/*
 *  sec_battery_dt.c
 *  Samsung Mobile Battery Driver
 *
 *  Copyright (C) 2018 Samsung Electronics
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include "sec_battery.h"
#include "sec_battery_dt.h"


#ifdef CONFIG_OF
int sec_bat_parse_dt(struct device *dev,
		struct sec_battery_info *battery)
{
	struct device_node *np;
	sec_battery_platform_data_t *pdata = battery->pdata;
	int ret = 0, len = 0;
	unsigned int i = 0;
	const u32 *p;
	u32 temp = 0;

	np = of_find_node_by_name(NULL, "cable-info");
	if (!np) {
		pr_err ("%s : np NULL\n", __func__);
	} else {
		struct device_node *child;
		u32 input_current = 0, charging_current = 0;

		ret = of_property_read_u32(np, "default_input_current", &input_current);
		ret = of_property_read_u32(np, "default_charging_current", &charging_current);
		ret = of_property_read_u32(np, "full_check_current_1st", &pdata->full_check_current_1st);
		ret = of_property_read_u32(np, "full_check_current_2nd", &pdata->full_check_current_2nd);

		pdata->default_input_current = input_current;
		pdata->default_charging_current = charging_current;
		pdata->charging_current =
			kzalloc(sizeof(sec_charging_current_t) * SEC_BATTERY_CABLE_MAX,
				GFP_KERNEL);

		for (i = 0; i < SEC_BATTERY_CABLE_MAX; i++) {
			pdata->charging_current[i].input_current_limit = (unsigned int)input_current;
			pdata->charging_current[i].fast_charging_current = (unsigned int)charging_current;
		}

		for_each_child_of_node(np, child) {
			ret = of_property_read_u32(child, "input_current", &input_current);
			ret = of_property_read_u32(child, "charging_current", &charging_current);
			p = of_get_property(child, "cable_number", &len);
			if (!p)
				return 1;

			len = len / sizeof(u32);

			for (i = 0; i <= len; i++) {
				ret = of_property_read_u32_index(child, "cable_number", i, &temp);
				pdata->charging_current[temp].input_current_limit = (unsigned int)input_current;
				pdata->charging_current[temp].fast_charging_current = (unsigned int)charging_current;
			}
		}
	}

	for (i = 0; i < SEC_BATTERY_CABLE_MAX; i++) {
		pr_info("%s : CABLE_NUM(%d) INPUT(%d) CHARGING(%d)\n",
			__func__, i,
			pdata->charging_current[i].input_current_limit,
			pdata->charging_current[i].fast_charging_current);
	}

	pr_info("%s : TOPOFF_1ST(%d), TOPOFF_2ND(%d)\n",
		__func__, pdata->full_check_current_1st, pdata->full_check_current_2nd);

	pdata->default_usb_input_current = pdata->charging_current[SEC_BATTERY_CABLE_USB].input_current_limit;
	pdata->default_usb_charging_current = pdata->charging_current[SEC_BATTERY_CABLE_USB].fast_charging_current;
#ifdef CONFIG_SEC_FACTORY
	pdata->default_charging_current = 1500;
	pdata->charging_current[SEC_BATTERY_CABLE_TA].fast_charging_current = 1500;
#endif
	np = of_find_node_by_name(NULL, "battery");
	if (!np) {
		pr_info("%s: np NULL\n", __func__);
		return 1;
	}

	ret = of_property_read_u32(np, "battery,battery_full_capacity",
			&pdata->battery_full_capacity);

	if (ret)
		pr_info("%s : battery_full_capacity is Empty\n", __func__);
#if defined(CONFIG_BATTERY_CISD)	
	else {
		pr_info("%s : battery_full_capacity : %d\n", __func__, pdata->battery_full_capacity);
		pdata->cisd_cap_high_thr = pdata->battery_full_capacity + 1000; /* battery_full_capacity + 1000 */
		pdata->cisd_cap_low_thr = pdata->battery_full_capacity + 500; /* battery_full_capacity + 500 */
		pdata->cisd_cap_limit = (pdata->battery_full_capacity * 11) / 10; /* battery_full_capacity + 10% */
	}

	ret = of_property_read_u32(np, "battery,cisd_max_voltage_thr",
		&pdata->max_voltage_thr);
	if (ret) {
		pr_info("%s : cisd_max_voltage_thr is Empty\n", __func__);
		pdata->max_voltage_thr = 4400;
	}

	ret = of_property_read_u32(np, "battery,cisd_alg_index",
			&pdata->cisd_alg_index);
	if (ret) {
		pr_info("%s : cisd_alg_index is Empty. Defalut set to six\n", __func__);
		pdata->cisd_alg_index = 6;
	} else {
		pr_info("%s : set cisd_alg_index : %d\n", __func__, pdata->cisd_alg_index);
	}
#endif

	ret = of_property_read_u32(np,
				   "battery,expired_time", &temp);
	if (ret) {
		pr_info("expired time is empty\n");
		pdata->expired_time = 3 * 60 * 60;
	} else {
		pdata->expired_time = (unsigned int) temp;
	}
	pdata->expired_time *= 1000;
	battery->expired_time = pdata->expired_time;

	ret = of_property_read_u32(np,
				   "battery,recharging_expired_time", &temp);
	if (ret) {
		pr_info("expired time is empty\n");
		pdata->recharging_expired_time = 90 * 60;
	} else {
		pdata->recharging_expired_time = (unsigned int) temp;
	}
	pdata->recharging_expired_time *= 1000;

	ret = of_property_read_u32(np,
				   "battery,standard_curr", &pdata->standard_curr);
	if (ret) {
		pr_info("standard_curr is empty\n");
		pdata->standard_curr = 2150;
	}

	ret = of_property_read_string(np,
		"battery,vendor", (char const **)&pdata->vendor);
	if (ret)
		pr_info("%s: Vendor is Empty\n", __func__);

	ret = of_property_read_string(np,
		"battery,charger_name", (char const **)&pdata->charger_name);
	if (ret)
		pr_info("%s: Charger name is Empty\n", __func__);

	ret = of_property_read_string(np,
		"battery,fuelgauge_name", (char const **)&pdata->fuelgauge_name);
	if (ret)
		pr_info("%s: Fuelgauge name is Empty\n", __func__);

	ret = of_property_read_string(np,
		"battery,wireless_charger_name", (char const **)&pdata->wireless_charger_name);
	if (ret)
		pr_info("%s: Wireless charger name is Empty\n", __func__);

	ret = of_property_read_string(np,
		"battery,fgsrc_switch_name", (char const **)&pdata->fgsrc_switch_name);
	if (ret) {
		pdata->support_fgsrc_change = false;
		pr_info("%s: fgsrc_switch_name is Empty\n", __func__);
	}
	else
		pdata->support_fgsrc_change = true;

	ret = of_property_read_string(np,
		"battery,chip_vendor", (char const **)&pdata->chip_vendor);
	if (ret)
		pr_info("%s: Chip vendor is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,technology",
		&pdata->technology);
	if (ret)
		pr_info("%s : technology is Empty\n", __func__);

	ret = of_property_read_u32(np,
		"battery,wireless_cc_cv", &pdata->wireless_cc_cv);

	pdata->fake_capacity = of_property_read_bool(np,
						     "battery,fake_capacity");

	p = of_get_property(np, "battery,polling_time", &len);
	if (!p)
		return 1;

	len = len / sizeof(u32);
	pdata->polling_time = kzalloc(sizeof(*pdata->polling_time) * len, GFP_KERNEL);
	ret = of_property_read_u32_array(np, "battery,polling_time",
					 pdata->polling_time, len);
	if (ret)
		pr_info("%s : battery,polling_time is Empty\n", __func__);

	/* battery thermistor */
	ret = of_property_read_u32(np, "battery,thermal_source",
		&pdata->thermal_source);
	if (ret)
		pr_info("%s : Thermal source is Empty\n", __func__);

	if (pdata->thermal_source == SEC_BATTERY_THERMAL_SOURCE_ADC) {
		p = of_get_property(np, "battery,temp_table_adc", &len);
		if (!p)
			return 1;

		len = len / sizeof(u32);

		pdata->temp_adc_table_size = len;
		pdata->temp_amb_adc_table_size = len;

		pdata->temp_adc_table =
			kzalloc(sizeof(sec_bat_adc_table_data_t) *
				pdata->temp_adc_table_size, GFP_KERNEL);
		pdata->temp_amb_adc_table =
			kzalloc(sizeof(sec_bat_adc_table_data_t) *
				pdata->temp_adc_table_size, GFP_KERNEL);

		for(i = 0; i < pdata->temp_adc_table_size; i++) {
			ret = of_property_read_u32_index(np,
					 "battery,temp_table_adc", i, &temp);
			pdata->temp_adc_table[i].adc = (int)temp;
			if (ret)
				pr_info("%s : Temp_adc_table(adc) is Empty\n",
					__func__);

			ret = of_property_read_u32_index(np,
							 "battery,temp_table_data", i, &temp);
			pdata->temp_adc_table[i].data = (int)temp;
			if (ret)
				pr_info("%s : Temp_adc_table(data) is Empty\n",
					__func__);

			ret = of_property_read_u32_index(np,
							 "battery,temp_table_adc", i, &temp);
			pdata->temp_amb_adc_table[i].adc = (int)temp;
			if (ret)
				pr_info("%s : Temp_amb_adc_table(adc) is Empty\n",
					__func__);

			ret = of_property_read_u32_index(np,
							 "battery,temp_table_data", i, &temp);
			pdata->temp_amb_adc_table[i].data = (int)temp;
			if (ret)
				pr_info("%s : Temp_amb_adc_table(data) is Empty\n",
					__func__);
		}
	}
	ret = of_property_read_u32(np, "battery,temp_check_type",
		&pdata->temp_check_type);
	if (ret)
		pr_info("%s : Temp check type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,temp_check_count",
		&pdata->temp_check_count);
	if (ret)
		pr_info("%s : Temp check count is Empty\n", __func__);

	/* usb thermistor */
	ret = of_property_read_u32(np, "battery,usb_thermal_source",
		&pdata->usb_thermal_source);
	if (ret)
		pr_info("%s : usb_thermal_source is Empty\n", __func__);

	if(pdata->usb_thermal_source == SEC_BATTERY_THERMAL_SOURCE_ADC) {
		p = of_get_property(np, "battery,usb_temp_table_adc", &len);
		if (!p)
			return 1;

		len = len / sizeof(u32);

		pdata->usb_temp_adc_table_size = len;

		pdata->usb_temp_adc_table =
			kzalloc(sizeof(sec_bat_adc_table_data_t) *
				pdata->usb_temp_adc_table_size, GFP_KERNEL);

		for(i = 0; i < pdata->usb_temp_adc_table_size; i++) {
			ret = of_property_read_u32_index(np,
							 "battery,usb_temp_table_adc", i, &temp);
			pdata->usb_temp_adc_table[i].adc = (int)temp;
			if (ret)
				pr_info("%s : Usb_Temp_adc_table(adc) is Empty\n",
					__func__);

			ret = of_property_read_u32_index(np,
							 "battery,usb_temp_table_data", i, &temp);
			pdata->usb_temp_adc_table[i].data = (int)temp;
			if (ret)
				pr_info("%s : Usb_Temp_adc_table(data) is Empty\n",
					__func__);
		}
	}
	ret = of_property_read_u32(np, "battery,usb_temp_check_type",
		&pdata->usb_temp_check_type);
	if (ret)
		pr_info("%s : usb_temp_check_type is Empty\n", __func__);

	/* chg thermistor */
	ret = of_property_read_u32(np, "battery,chg_thermal_source",
		&pdata->chg_thermal_source);
	if (ret)
		pr_info("%s : chg_thermal_source is Empty\n", __func__);

	if(pdata->chg_thermal_source == SEC_BATTERY_THERMAL_SOURCE_ADC) {
		p = of_get_property(np, "battery,chg_temp_table_adc", &len);
		if (!p)
			return 1;

		len = len / sizeof(u32);

		pdata->chg_temp_adc_table_size = len;

		pdata->chg_temp_adc_table =
			kzalloc(sizeof(sec_bat_adc_table_data_t) *
				pdata->chg_temp_adc_table_size, GFP_KERNEL);

		for(i = 0; i < pdata->chg_temp_adc_table_size; i++) {
			ret = of_property_read_u32_index(np,
							 "battery,chg_temp_table_adc", i, &temp);
			pdata->chg_temp_adc_table[i].adc = (int)temp;
			if (ret)
				pr_info("%s : CHG_Temp_adc_table(adc) is Empty\n",
					__func__);

			ret = of_property_read_u32_index(np,
							 "battery,chg_temp_table_data", i, &temp);
			pdata->chg_temp_adc_table[i].data = (int)temp;
			if (ret)
				pr_info("%s : CHG_Temp_adc_table(data) is Empty\n",
					__func__);
		}
	}
	ret = of_property_read_u32(np, "battery,chg_temp_check_type",
		&pdata->chg_temp_check_type);
	if (ret)
		pr_info("%s : chg_temp_check_type is Empty\n", __func__);

#if defined(CONFIG_DIRECT_CHARGING)
	/* direct chg thermistor */
	ret = of_property_read_u32(np, "battery,dchg_thermal_source",
		&pdata->dchg_thermal_source);
	if (ret)
		pr_info("%s : dchg_thermal_source is Empty\n", __func__);

	if(pdata->dchg_thermal_source == SEC_BATTERY_THERMAL_SOURCE_CHG_ADC) {
		p = of_get_property(np, "battery,dchg_temp_table_adc", &len);
		if (!p)
			return 1;

		len = len / sizeof(u32);

		pdata->dchg_temp_adc_table_size = len;

		pdata->dchg_temp_adc_table =
			kzalloc(sizeof(sec_bat_adc_table_data_t) *
				pdata->dchg_temp_adc_table_size, GFP_KERNEL);

		for(i = 0; i < pdata->dchg_temp_adc_table_size; i++) {
			ret = of_property_read_u32_index(np,
							 "battery,dchg_temp_table_adc", i, &temp);
			pdata->dchg_temp_adc_table[i].adc = (int)temp;
			if (ret)
				pr_info("%s : DIRECT CHG_Temp_adc_table(adc) is Empty\n",
					__func__);

			ret = of_property_read_u32_index(np,
							 "battery,dchg_temp_table_data", i, &temp);
			pdata->dchg_temp_adc_table[i].data = (int)temp;
			if (ret)
				pr_info("%s : DIRECT CHG_Temp_adc_table(data) is Empty\n",
					__func__);
		}
	}
	ret = of_property_read_u32(np, "battery,dchg_temp_check_type",
		&pdata->dchg_temp_check_type);
	if (ret)
		pr_info("%s : dchg_temp_check_type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,dchg_high_temp", &temp);
	pdata->dchg_high_temp = (int)temp;
	if (ret) {
		pr_info("%s : dchg_high_temp is Empty\n", __func__);
		pdata->dchg_high_temp = pdata->chg_high_temp;
	}
	ret = of_property_read_u32(np, "battery,dchg_high_temp_recovery",
			&temp);
	pdata->dchg_high_temp_recovery = (int)temp;
	if (ret) {
		pr_info("%s : dchg_temp_recovery is Empty\n", __func__);
		pdata->dchg_high_temp_recovery = pdata->chg_high_temp_recovery;
	}

	ret = of_property_read_u32(np, "battery,dchg_high_batt_temp",
			&temp);
	pdata->dchg_high_batt_temp = (int)temp;
	if (ret) {
		pr_info("%s : dchg_high_batt_temp is Empty\n", __func__);
		pdata->dchg_high_batt_temp = pdata->chg_high_temp;
	}
	ret = of_property_read_u32(np, "battery,dchg_high_batt_temp_recovery",
			&temp);
	pdata->dchg_high_batt_temp_recovery = (int)temp;
	if (ret) {
		pr_info("%s : dchg_high_batt_temp_recovery is Empty\n", __func__);
		pdata->dchg_high_batt_temp_recovery = pdata->chg_high_temp_recovery;
	}
#endif
	/* wpc thermistor */
	ret = of_property_read_u32(np, "battery,wpc_thermal_source",
		&pdata->wpc_thermal_source);
	if (ret)
		pr_info("%s : wpc_thermal_source is Empty\n", __func__);

	if(pdata->wpc_thermal_source == SEC_BATTERY_THERMAL_SOURCE_ADC) {
		p = of_get_property(np, "battery,wpc_temp_table_adc", &len);
		if (!p) {
			pr_info("%s : wpc_temp_table_adc(adc) is Empty\n",__func__);
		} else {
			len = len / sizeof(u32);

			pdata->wpc_temp_adc_table_size = len;

			pdata->wpc_temp_adc_table =
				kzalloc(sizeof(sec_bat_adc_table_data_t) *
					pdata->wpc_temp_adc_table_size, GFP_KERNEL);

			for(i = 0; i < pdata->wpc_temp_adc_table_size; i++) {
				ret = of_property_read_u32_index(np,
								 "battery,wpc_temp_table_adc", i, &temp);
				pdata->wpc_temp_adc_table[i].adc = (int)temp;
				if (ret)
					pr_info("%s : WPC_Temp_adc_table(adc) is Empty\n",
						__func__);

				ret = of_property_read_u32_index(np,
								 "battery,wpc_temp_table_data", i, &temp);
				pdata->wpc_temp_adc_table[i].data = (int)temp;
				if (ret)
					pr_info("%s : WPC_Temp_adc_table(data) is Empty\n",
						__func__);
			}
		}
	}
	ret = of_property_read_u32(np, "battery,wpc_temp_check_type",
		&pdata->wpc_temp_check_type);
	if (ret)
		pr_info("%s : wpc_temp_check_type is Empty\n", __func__);
	
	/* sub bat thermistor */
	ret = of_property_read_u32(np, "battery,sub_bat_thermal_source",
		&pdata->sub_bat_thermal_source);
	if (ret)
		pr_info("%s : sub_bat_thermal_source is Empty\n", __func__);

	if(pdata->sub_bat_thermal_source) {
		p = of_get_property(np, "battery,sub_bat_temp_table_adc", &len);
		if (!p) {
			pr_info("%s : sub_bat_temp_table_adc(adc) is Empty\n",__func__);
		} else {
			len = len / sizeof(u32);

			pdata->sub_bat_temp_adc_table_size = len;

			pdata->sub_bat_temp_adc_table =
				kzalloc(sizeof(sec_bat_adc_table_data_t) *
					pdata->sub_bat_temp_adc_table_size, GFP_KERNEL);

			for(i = 0; i < pdata->sub_bat_temp_adc_table_size; i++) {
				ret = of_property_read_u32_index(np,
								 "battery,sub_bat_temp_table_adc", i, &temp);
				pdata->sub_bat_temp_adc_table[i].adc = (int)temp;
				if (ret)
					pr_info("%s : sub_bat_adc_table(adc) is Empty\n",
						__func__);

				ret = of_property_read_u32_index(np,
								 "battery,sub_bat_temp_table_data", i, &temp);
				pdata->sub_bat_temp_adc_table[i].data = (int)temp;
				if (ret)
					pr_info("%s : sub_bat_temp_adc_table(data) is Empty\n",
						__func__);
			}
		}
	}
	ret = of_property_read_u32(np, "battery,sub_bat_temp_check_type",
		&pdata->sub_bat_temp_check_type);
	if (ret)
		pr_info("%s : sub_bat_temp_check_type is Empty\n", __func__);

	/* slave thermistor */
	ret = of_property_read_u32(np, "battery,slave_thermal_source",
		&pdata->slave_thermal_source);
	if (ret)
		pr_info("%s : slave_thermal_source is Empty\n", __func__);

	if(pdata->slave_thermal_source == SEC_BATTERY_THERMAL_SOURCE_ADC) {
		p = of_get_property(np, "battery,slave_chg_temp_table_adc", &len);
		if (!p)
			return 1;

		len = len / sizeof(u32);

		pdata->slave_chg_temp_adc_table_size = len;

		pdata->slave_chg_temp_adc_table =
			kzalloc(sizeof(sec_bat_adc_table_data_t) *
				pdata->slave_chg_temp_adc_table_size, GFP_KERNEL);

		for(i = 0; i < pdata->slave_chg_temp_adc_table_size; i++) {
			ret = of_property_read_u32_index(np,
							 "battery,slave_chg_temp_table_adc", i, &temp);
			pdata->slave_chg_temp_adc_table[i].adc = (int)temp;
			if (ret)
				pr_info("%s : slave_chg_temp_adc_table(adc) is Empty\n",
					__func__);

			ret = of_property_read_u32_index(np,
							 "battery,slave_chg_temp_table_data", i, &temp);
			pdata->slave_chg_temp_adc_table[i].data = (int)temp;
			if (ret)
				pr_info("%s : slave_chg_temp_adc_table(data) is Empty\n",
					__func__);
		}
	}
	ret = of_property_read_u32(np, "battery,slave_chg_temp_check_type",
		&pdata->slave_chg_temp_check_type);
	if (ret)
		pr_info("%s : slave_chg_temp_check_type is Empty\n", __func__);
	
	/* blkt thermistor */
	ret = of_property_read_u32(np, "battery,blkt_thermal_source",
		&pdata->blkt_thermal_source);
	if (ret)
		pr_info("%s : blkt_thermal_source is Empty\n", __func__);

	if(pdata->blkt_thermal_source == SEC_BATTERY_THERMAL_SOURCE_ADC) {
		p = of_get_property(np, "battery,blkt_temp_table_adc", &len);
		if (!p)
			return 1;

		len = len / sizeof(u32);

		pdata->blkt_temp_adc_table_size = len;

		pdata->blkt_temp_adc_table =
			kzalloc(sizeof(sec_bat_adc_table_data_t) *
				pdata->blkt_temp_adc_table_size, GFP_KERNEL);

		for(i = 0; i < pdata->blkt_temp_adc_table_size; i++) {
			ret = of_property_read_u32_index(np,
							 "battery,blkt_temp_table_adc", i, &temp);
			pdata->blkt_temp_adc_table[i].adc = (int)temp;
			if (ret)
				pr_info("%s : blkt_temp_adc_table(adc) is Empty\n",
					__func__);

			ret = of_property_read_u32_index(np,
							 "battery,blkt_temp_table_data", i, &temp);
			pdata->blkt_temp_adc_table[i].data = (int)temp;
			if (ret)
				pr_info("%s : blkt_temp_adc_table(data) is Empty\n",
					__func__);
		}
	}
	ret = of_property_read_u32(np, "battery,blkt_temp_check_type",
		&pdata->blkt_temp_check_type);
	if (ret)
		pr_info("%s : blkt_temp_check_type is Empty\n", __func__);

	if (pdata->chg_temp_check_type) {
		ret = of_property_read_u32(np, "battery,chg_12v_high_temp",
					   &temp);
		pdata->chg_12v_high_temp = (int)temp;
		if (ret)
			pr_info("%s : chg_12v_high_temp is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,chg_high_temp",
					   &temp);
		pdata->chg_high_temp = (int)temp;
		if (ret)
			pr_info("%s : chg_high_temp is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,chg_high_temp_recovery",
					   &temp);
		pdata->chg_high_temp_recovery = (int)temp;
		if (ret)
			pr_info("%s : chg_temp_recovery is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,chg_charging_limit_current",
					   &pdata->chg_charging_limit_current);
		if (ret)
			pr_info("%s : chg_charging_limit_current is Empty\n", __func__);
		
		ret = of_property_read_u32(np, "battery,chg_input_limit_current",
					   &pdata->chg_input_limit_current);
		if (ret)
			pr_info("%s : chg_input_limit_current is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,dchg_charging_limit_current",
					   &pdata->dchg_charging_limit_current);
		if (ret) {
			pr_info("%s : dchg_charging_limit_current is Empty\n", __func__);
			pdata->dchg_charging_limit_current = pdata->chg_charging_limit_current;
		}

		ret = of_property_read_u32(np, "battery,dchg_input_limit_current",
					   &pdata->dchg_input_limit_current);
		if (ret) {
			pr_info("%s : dchg_input_limit_current is Empty\n", __func__);
			pdata->dchg_input_limit_current = pdata->chg_input_limit_current;
		}

		ret = of_property_read_u32(np, "battery,mix_high_temp",
					   &temp);
		pdata->mix_high_temp = (int)temp;
		if (ret)
			pr_info("%s : mix_high_temp is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,mix_high_chg_temp",
					   &temp);
		pdata->mix_high_chg_temp = (int)temp;
		if (ret)
			pr_info("%s : mix_high_chg_temp is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,mix_high_temp_recovery",
					   &temp);
		pdata->mix_high_temp_recovery = (int)temp;
		if (ret)
			pr_info("%s : mix_high_temp_recovery is Empty\n", __func__);
	}

	if (pdata->wpc_temp_check_type) {
		ret = of_property_read_u32(np, "battery,wpc_temp_control_source",
				&pdata->wpc_temp_control_source);
		if (ret) {
			pr_info("%s : wpc_temp_control_source is Empty\n", __func__);
			pdata->wpc_temp_control_source = TEMP_CONTROL_SOURCE_CHG_THM;
		}

		ret = of_property_read_u32(np, "battery,wpc_temp_lcd_on_control_source",
				&pdata->wpc_temp_lcd_on_control_source);
		if (ret) {
			pr_info("%s : wpc_temp_lcd_on_control_source is Empty\n", __func__);
			pdata->wpc_temp_lcd_on_control_source = TEMP_CONTROL_SOURCE_CHG_THM;
		}

		ret = of_property_read_u32(np, "battery,wpc_high_temp",
				&pdata->wpc_high_temp);
		if (ret)
			pr_info("%s : wpc_high_temp is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,wpc_high_temp_recovery",
				&pdata->wpc_high_temp_recovery);
		if (ret)
			pr_info("%s : wpc_high_temp_recovery is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,wpc_input_limit_current",
				&pdata->wpc_input_limit_current);
		if (ret)
			pr_info("%s : wpc_input_limit_current is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,wpc_charging_limit_current",
				&pdata->wpc_charging_limit_current);
		if (ret)
			pr_info("%s : wpc_charging_limit_current is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,wpc_lcd_on_high_temp",
				&pdata->wpc_lcd_on_high_temp);
		if (ret)
			pr_info("%s : wpc_lcd_on_high_temp is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,wpc_lcd_on_high_temp_rec",
				&pdata->wpc_lcd_on_high_temp_rec);
		if (ret)
			pr_info("%s : wpc_lcd_on_high_temp_rec is Empty\n", __func__);

		ret = of_property_read_u32(np, "battery,wpc_lcd_on_input_limit_current",
				&pdata->wpc_lcd_on_input_limit_current);
		if (ret) {
			pr_info("%s : wpc_lcd_on_input_limit_current is Empty\n", __func__);
			pdata->wpc_lcd_on_input_limit_current =
				pdata->wpc_input_limit_current;
		}
	}

	ret = of_property_read_u32(np, "battery,wc_full_input_limit_current",
		&pdata->wc_full_input_limit_current);
	if (ret)
		pr_info("%s : wc_full_input_limit_current is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,wc_hero_stand_cc_cv",
		&pdata->wc_hero_stand_cc_cv);
	if (ret) {
		pr_info("%s : wc_hero_stand_cc_cv is Empty\n", __func__);
		pdata->wc_hero_stand_cc_cv = 70;
	}
	ret = of_property_read_u32(np, "battery,wc_hero_stand_cv_current",
		&pdata->wc_hero_stand_cv_current);
	if (ret) {
		pr_info("%s : wc_hero_stand_cv_current is Empty\n", __func__);
		pdata->wc_hero_stand_cv_current = 600;
	}
	ret = of_property_read_u32(np, "battery,wc_hero_stand_hv_cv_current",
		&pdata->wc_hero_stand_hv_cv_current);
	if (ret) {
		pr_info("%s : wc_hero_stand_hv_cv_current is Empty\n", __func__);
		pdata->wc_hero_stand_hv_cv_current = 450;
	}

	ret = of_property_read_u32(np, "battery,sleep_mode_limit_current",
			&pdata->sleep_mode_limit_current);
	if (ret)
		pr_info("%s : sleep_mode_limit_current is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,inbat_voltage",
			&pdata->inbat_voltage);
	if (ret)
		pr_info("%s : inbat_voltage is Empty\n", __func__);

	if (pdata->inbat_voltage) {
		p = of_get_property(np, "battery,inbat_voltage_table_adc", &len);
		if (!p)
			return 1;

		len = len / sizeof(u32);

		pdata->inbat_adc_table_size = len;

		pdata->inbat_adc_table =
			kzalloc(sizeof(sec_bat_adc_table_data_t) *
					pdata->inbat_adc_table_size, GFP_KERNEL);

		for(i = 0; i < pdata->inbat_adc_table_size; i++) {
			ret = of_property_read_u32_index(np,
							 "battery,inbat_voltage_table_adc", i, &temp);
			pdata->inbat_adc_table[i].adc = (int)temp;
			if (ret)
				pr_info("%s : inbat_adc_table(adc) is Empty\n",
						__func__);

			ret = of_property_read_u32_index(np,
							 "battery,inbat_voltage_table_data", i, &temp);
			pdata->inbat_adc_table[i].data = (int)temp;
			if (ret)
				pr_info("%s : inbat_adc_table(data) is Empty\n",
						__func__);
		}
	}

	ret = of_property_read_u32(np, "battery,pre_afc_input_current",
		&pdata->pre_afc_input_current);
	if (ret) {
		pr_info("%s : pre_afc_input_current is Empty\n", __func__);
		pdata->pre_afc_input_current = 1000;
	}

	ret = of_property_read_u32(np, "battery,pre_afc_work_delay",
			&pdata->pre_afc_work_delay);
	if (ret) {
		pr_info("%s : pre_afc_work_delay is Empty\n", __func__);
		pdata->pre_afc_work_delay = 2000;
	}

	ret = of_property_read_u32(np, "battery,pre_wc_afc_input_current",
		&pdata->pre_wc_afc_input_current);
	if (ret) {
		pr_info("%s : pre_wc_afc_input_current is Empty\n", __func__);
		pdata->pre_wc_afc_input_current = 500; /* wc input default */
	}

	ret = of_property_read_u32(np, "battery,pre_wc_afc_work_delay",
			&pdata->pre_wc_afc_work_delay);
	if (ret) {
		pr_info("%s : pre_wc_afc_work_delay is Empty\n", __func__);
		pdata->pre_wc_afc_work_delay = 4000;
	}

	ret = of_property_read_u32(np, "battery,prepare_ta_delay",
			&pdata->prepare_ta_delay);
	if (ret) {
		pr_info("%s : prepare_ta_delay is Empty\n", __func__);
		pdata->prepare_ta_delay = 500;
	}

	ret = of_property_read_u32(np, "battery,tx_stop_capacity",
		&pdata->tx_stop_capacity);
	if (ret) {
		pr_info("%s : tx_stop_capacity is Empty\n", __func__);
		pdata->tx_stop_capacity = 30;
	}

	ret = of_property_read_u32(np, "battery,adc_check_count",
		&pdata->adc_check_count);
	if (ret)
		pr_info("%s : Adc check count is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,temp_adc_type",
		&pdata->temp_adc_type);
	if (ret)
		pr_info("%s : Temp adc type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,cable_check_type",
		&pdata->cable_check_type);
	if (ret)
		pr_info("%s : Cable check type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,cable_source_type",
		&pdata->cable_source_type);
	if (ret)
		pr_info("%s: Cable_source_type is Empty\n", __func__);
#if defined(CONFIG_CHARGING_VZWCONCEPT)
	pdata->cable_check_type &= ~SEC_BATTERY_CABLE_CHECK_NOUSBCHARGE;
	pdata->cable_check_type |= SEC_BATTERY_CABLE_CHECK_NOINCOMPATIBLECHARGE;
#endif
	ret = of_property_read_u32(np, "battery,polling_type",
		&pdata->polling_type);
	if (ret)
		pr_info("%s : Polling type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,monitor_initial_count",
		&pdata->monitor_initial_count);
	if (ret)
		pr_info("%s : Monitor initial count is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,battery_check_type",
		&pdata->battery_check_type);
	if (ret)
		pr_info("%s : Battery check type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,check_count",
		&pdata->check_count);
	if (ret)
		pr_info("%s : Check count is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,check_adc_max",
		&pdata->check_adc_max);
	if (ret)
		pr_info("%s : Check adc max is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,check_adc_min",
		&pdata->check_adc_min);
	if (ret)
		pr_info("%s : Check adc min is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,ovp_uvlo_check_type",
		&pdata->ovp_uvlo_check_type);
	if (ret)
		pr_info("%s : Ovp Uvlo check type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,overheatlimit_threshold", &temp);
	battery->overheatlimit_threshold = (int)temp;
	if (ret) {
		pr_info("%s : overheatlimit_threshold is Empty\n", __func__);
		battery->overheatlimit_threshold = 700;
	}

	ret = of_property_read_u32(np, "battery,overheatlimit_recovery", &temp);
	battery->overheatlimit_recovery = (int)temp;
	if (ret) {
		pr_info("%s : overheatlimit_recovery is Empty\n", __func__);
		battery->overheatlimit_recovery = 680;
	}

#if defined(CONFIG_PREVENT_USB_CONN_OVERHEAT)
	ret = of_property_read_u32(np, "battery,usb_protection_temp", &temp);
	battery->usb_protection_temp = (int)temp;
	if (ret) {
		pr_info("%s : usb protection temp value is Empty\n", __func__);
		battery->usb_protection_temp = 610;
	}

	ret = of_property_read_u32(np, "battery,temp_gap_bat_usb", &temp);
	battery->temp_gap_bat_usb = (int)temp;
	if (ret) {
		pr_info("%s : temp gap value is Empty\n", __func__);
		battery->temp_gap_bat_usb = 200;
	}
#endif

	ret = of_property_read_u32(np, "battery,wire_warm_overheat_thresh", &temp);
	pdata->wire_warm_overheat_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wire_warm_overheat_thresh is Empty\n", __func__);
		pdata->wire_warm_overheat_thresh = 500;
	}

	ret = of_property_read_u32(np, "battery,wire_normal_warm_thresh", &temp);
	pdata->wire_normal_warm_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wire_normal_warm_thresh is Empty\n", __func__);
		pdata->wire_normal_warm_thresh = 410;
	}

	ret = of_property_read_u32(np, "battery,wire_cool1_normal_thresh", &temp);
	pdata->wire_cool1_normal_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wire_cool1_normal_thresh is Empty\n", __func__);
		pdata->wire_cool1_normal_thresh = 180;
	}

	ret = of_property_read_u32(np, "battery,wire_cool2_cool1_thresh", &temp);
	pdata->wire_cool2_cool1_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wire_cool2_cool1_thresh is Empty\n", __func__);
		pdata->wire_cool2_cool1_thresh = 150;
	}

	ret = of_property_read_u32(np, "battery,wire_cool3_cool2_thresh", &temp);
	pdata->wire_cool3_cool2_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wire_cool3_cool2_thresh is Empty\n", __func__);
		pdata->wire_cool3_cool2_thresh = 50;
	}

	ret = of_property_read_u32(np, "battery,wire_cold_cool3_thresh", &temp);
	pdata->wire_cold_cool3_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wire_cold_cool3_thresh is Empty\n", __func__);
		pdata->wire_cold_cool3_thresh = 0;
	}

	ret = of_property_read_u32(np, "battery,wireless_warm_overheat_thresh", &temp);
	pdata->wireless_warm_overheat_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wireless_warm_overheat_thresh is Empty\n", __func__);
		pdata->wireless_warm_overheat_thresh = 450;
	}

	ret = of_property_read_u32(np, "battery,wireless_normal_warm_thresh", &temp);
	pdata->wireless_normal_warm_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wireless_normal_warm_thresh is Empty\n", __func__);
		pdata->wireless_normal_warm_thresh = 410;
	}

	ret = of_property_read_u32(np, "battery,wireless_cool1_normal_thresh", &temp);
	pdata->wireless_cool1_normal_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wireless_cool1_normal_thresh is Empty\n", __func__);
		pdata->wireless_cool1_normal_thresh = 180;
	}

	ret = of_property_read_u32(np, "battery,wireless_cool2_cool1_thresh", &temp);
	pdata->wireless_cool2_cool1_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wireless_cool2_cool1_thresh is Empty\n", __func__);
		pdata->wireless_cool2_cool1_thresh = 150;
	}

	ret = of_property_read_u32(np, "battery,wireless_cool3_cool2_thresh", &temp);
	pdata->wireless_cool3_cool2_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wireless_cool3_cool2_thresh is Empty\n", __func__);
		pdata->wireless_cool3_cool2_thresh = 50;
	}

	ret = of_property_read_u32(np, "battery,wireless_cold_cool3_thresh", &temp);
	pdata->wireless_cold_cool3_thresh = (int)temp;
	if (ret) {
		pr_info("%s : wireless_cold_cool3_thresh is Empty\n", __func__);
		pdata->wireless_cold_cool3_thresh = 0;
	}

	ret = of_property_read_u32(np, "battery,wire_warm_current", &temp);
	pdata->wire_warm_current = (int)temp;
	if (ret) {
		pr_info("%s : wire_warm_current is Empty\n", __func__);
		pdata->wire_warm_current = 500;
	}

	ret = of_property_read_u32(np, "battery,wire_cool1_current", &temp);
	pdata->wire_cool1_current = (int)temp;
	if (ret) {
		pr_info("%s : wire_cool1_current is Empty\n", __func__);
		pdata->wire_cool1_current = 500;
	}

	ret = of_property_read_u32(np, "battery,wire_cool2_current", &temp);
	pdata->wire_cool2_current = (int)temp;
	if (ret) {
		pr_info("%s : wire_cool2_current is Empty\n", __func__);
		pdata->wire_cool2_current = 500;
	}

	ret = of_property_read_u32(np, "battery,wire_cool3_current", &temp);
	pdata->wire_cool3_current = (int)temp;
	if (ret) {
		pr_info("%s : wire_cool3_current is Empty\n", __func__);
		pdata->wire_cool3_current = 500;
	}

	ret = of_property_read_u32(np, "battery,wireless_warm_current", &temp);
	pdata->wireless_warm_current = (int)temp;
	if (ret) {
		pr_info("%s : wireless_warm_current is Empty\n", __func__);
		pdata->wireless_warm_current = 500;
	}

	ret = of_property_read_u32(np, "battery,wireless_cool1_current", &temp);
	pdata->wireless_cool1_current = (int)temp;
	if (ret) {
		pr_info("%s : wireless_cool1_current is Empty\n", __func__);
		pdata->wireless_cool1_current = 500;
	}

	ret = of_property_read_u32(np, "battery,wireless_cool2_current", &temp);
	pdata->wireless_cool2_current = (int)temp;
	if (ret) {
		pr_info("%s : wireless_cool2_current is Empty\n", __func__);
		pdata->wireless_cool2_current = 500;
	}

	ret = of_property_read_u32(np, "battery,wireless_cool3_current", &temp);
	pdata->wireless_cool3_current = (int)temp;
	if (ret) {
		pr_info("%s : wireless_cool3_current is Empty\n", __func__);
		pdata->wireless_cool3_current = 500;
	}

	ret = of_property_read_u32(np, "battery,high_temp_topoff", &temp);
	pdata->high_temp_topoff = (int)temp;
	if (ret) {
		pr_info("%s : high_temp_topoff is Empty\n", __func__);
		pdata->high_temp_topoff = 250;
	}

	ret = of_property_read_u32(np, "battery,low_temp_topoff", &temp);
	pdata->low_temp_topoff = (int)temp;
	if (ret) {
		pr_info("%s : low_temp_topoff is Empty\n", __func__);
		pdata->low_temp_topoff = 250;
	}

	ret = of_property_read_u32(np, "battery,high_temp_float", &temp);
	pdata->high_temp_float = (int)temp;
	if (ret) {
		pr_info("%s : high_temp_float is Empty\n", __func__);
		pdata->high_temp_float = 4150;
	}

	ret = of_property_read_u32(np, "battery,low_temp_float", &temp);
	pdata->low_temp_float = (int)temp;
	if (ret) {
		pr_info("%s : low_temp_float is Empty\n", __func__);
		pdata->low_temp_float = 4350;
	}

	ret = of_property_read_u32(np, "battery,recharge_condition_vcell", &pdata->recharge_condition_vcell);
	if (ret) {
		pr_info("%s : recharge_condition_vcell is Empty\n", __func__);
		pdata->recharge_condition_vcell = 4270;
	}

	ret = of_property_read_u32(np, "battery,swelling_high_rechg_voltage", &temp);
	pdata->swelling_high_rechg_voltage = (int)temp;
	if (ret) {
		pr_info("%s : swelling_high_rechg_voltage is Empty\n", __func__);
		pdata->swelling_high_rechg_voltage = 4000;
	}

	ret = of_property_read_u32(np, "battery,swelling_low_rechg_voltage", &temp);
	pdata->swelling_low_rechg_voltage = (int)temp;
	if (ret) {
		pr_info("%s : swelling_low_rechg_voltage is Empty\n", __func__);
		pdata->swelling_low_rechg_voltage = 4200;
	}

	ret = of_property_read_u32(np, "battery,tx_high_threshold",
				   &temp);
	pdata->tx_high_threshold = (int)temp;
	if (ret) {
		pr_info("%s : tx_high_threshold is Empty\n", __func__);
		pdata->tx_high_threshold = 450;
	}

	ret = of_property_read_u32(np, "battery,tx_high_recovery",
				   &temp);
	pdata->tx_high_recovery = (int)temp;
	if (ret) {
		pr_info("%s : tx_high_recovery is Empty\n", __func__);
		pdata->tx_high_recovery = 400;
	}

	ret = of_property_read_u32(np, "battery,tx_low_threshold",
				   &temp);
	pdata->tx_low_threshold = (int)temp;
	if (ret) {
		pr_info("%s : tx_low_threshold is Empty\n", __func__);
		pdata->tx_low_recovery = 0;
	}
	ret = of_property_read_u32(np, "battery,tx_low_recovery",
				   &temp);
	pdata->tx_low_recovery = (int)temp;
	if (ret) {
		pr_info("%s : tx_low_recovery is Empty\n", __func__);
		pdata->tx_low_recovery = 50;
	}

	ret = of_property_read_u32(np, "battery,charging_limit_by_tx_check",
				   &pdata->charging_limit_by_tx_check);
	if (ret)
		pr_info("%s : charging_limit_by_tx_check is Empty\n", __func__);

	if(pdata->charging_limit_by_tx_check) {
		ret = of_property_read_u32(np, "battery,charging_limit_current_by_tx",
					   &pdata->charging_limit_current_by_tx);
		if (ret) {
			pr_info("%s : charging_limit_current_by_tx is Empty\n", __func__);
			pdata->charging_limit_current_by_tx = 1000;
		}
	}

	ret = of_property_read_u32(np, "battery,wpc_input_limit_by_tx_check",
				   &pdata->wpc_input_limit_by_tx_check);
	if (ret)
		pr_info("%s : wpc_input_limit_by_tx_check is Empty\n", __func__);
		
	if(pdata->wpc_input_limit_by_tx_check) {
		ret = of_property_read_u32(np, "battery,wpc_input_limit_current_by_tx",
				&pdata->wpc_input_limit_current_by_tx);
		if (ret) {
			pr_info("%s : wpc_input_limit_current_by_tx is Empty\n", __func__);
			pdata->wpc_input_limit_current_by_tx =
				pdata->wpc_input_limit_current;
		}
	}	

	ret = of_property_read_u32(np, "battery,full_check_type",
		&pdata->full_check_type);
	if (ret)
		pr_info("%s : Full check type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,full_check_type_2nd",
		&pdata->full_check_type_2nd);
	if (ret)
		pr_info("%s : Full check type 2nd is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,full_check_count",
		&pdata->full_check_count);
	if (ret)
		pr_info("%s : Full check count is Empty\n", __func__);

        ret = of_property_read_u32(np, "battery,chg_gpio_full_check",
                &pdata->chg_gpio_full_check);
	if (ret)
		pr_info("%s : Chg gpio full check is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,chg_polarity_full_check",
		&pdata->chg_polarity_full_check);
	if (ret)
		pr_info("%s : Chg polarity full check is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,full_condition_type",
		&pdata->full_condition_type);
	if (ret)
		pr_info("%s : Full condition type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,full_condition_soc",
		&pdata->full_condition_soc);
	if (ret)
		pr_info("%s : Full condition soc is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,full_condition_vcell",
		&pdata->full_condition_vcell);
	if (ret)
		pr_info("%s : Full condition vcell is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,recharge_check_count",
		&pdata->recharge_check_count);
	if (ret)
		pr_info("%s : Recharge check count is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,recharge_condition_type",
		&pdata->recharge_condition_type);
	if (ret)
		pr_info("%s : Recharge condition type is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,recharge_condition_soc",
		&pdata->recharge_condition_soc);
	if (ret)
		pr_info("%s : Recharge condition soc is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,charging_total_time",
		(unsigned int *)&pdata->charging_total_time);
	if (ret)
		pr_info("%s : Charging total time is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,hv_charging_total_time",
				   &pdata->hv_charging_total_time);
	if (ret) {
		pdata->hv_charging_total_time = 3 * 60 * 60;
		pr_info("%s : HV Charging total time is %d\n",
			__func__, pdata->hv_charging_total_time);
	}

	ret = of_property_read_u32(np, "battery,normal_charging_total_time",
				   &pdata->normal_charging_total_time);
	if (ret) {
		pdata->normal_charging_total_time = 5 * 60 * 60;
		pr_info("%s : Normal(WC) Charging total time is %d\n",
			__func__, pdata->normal_charging_total_time);
	}

	ret = of_property_read_u32(np, "battery,usb_charging_total_time",
				   &pdata->usb_charging_total_time);
	if (ret) {
		pdata->usb_charging_total_time = 10 * 60 * 60;
		pr_info("%s : USB Charging total time is %d\n",
			__func__, pdata->usb_charging_total_time);
	}

	ret = of_property_read_u32(np, "battery,recharging_total_time",
		(unsigned int *)&pdata->recharging_total_time);
	if (ret)
		pr_info("%s : Recharging total time is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,charging_reset_time",
		(unsigned int *)&pdata->charging_reset_time);
	if (ret)
		pr_info("%s : Charging reset time is Empty\n", __func__);

	ret = of_property_read_u32(np, "battery,chg_float_voltage",
		(unsigned int *)&pdata->chg_float_voltage);
	if (ret) {
		pr_info("%s: chg_float_voltage is Empty\n", __func__);
		pdata->chg_float_voltage = 43500;
	}

	ret = of_property_read_u32(np, "battery,chg_float_voltage_conv",
				   &pdata->chg_float_voltage_conv);
	if (ret) {
		pr_info("%s: chg_float_voltage_conv is Empty\n", __func__);
		pdata->chg_float_voltage_conv = 1;
	}

#if defined(CONFIG_CALC_TIME_TO_FULL)
	ret = of_property_read_u32(np, "battery,ttf_hv_12v_charge_current",
					&pdata->ttf_hv_12v_charge_current);
	if (ret) {
		pdata->ttf_hv_12v_charge_current =
			pdata->charging_current[SEC_BATTERY_CABLE_12V_TA].fast_charging_current;
		pr_info("%s: ttf_hv_12v_charge_current is Empty, Defualt value %d \n",
			__func__, pdata->ttf_hv_12v_charge_current);
	}
	ret = of_property_read_u32(np, "battery,ttf_hv_charge_current",
					&pdata->ttf_hv_charge_current);
	if (ret) {
		pdata->ttf_hv_charge_current =
			pdata->charging_current[SEC_BATTERY_CABLE_9V_TA].fast_charging_current;
		pr_info("%s: ttf_hv_charge_current is Empty, Defualt value %d \n",
			__func__, pdata->ttf_hv_charge_current);
	}

	ret = of_property_read_u32(np, "battery,ttf_hv_wireless_charge_current",
					&pdata->ttf_hv_wireless_charge_current);
	if (ret) {
		pr_info("%s: ttf_hv_wireless_charge_current is Empty, Defualt value 0 \n", __func__);
		pdata->ttf_hv_wireless_charge_current =
			pdata->charging_current[SEC_BATTERY_CABLE_HV_WIRELESS].fast_charging_current - 300;
	}

	ret = of_property_read_u32(np, "battery,ttf_hv_12v_wireless_charge_current",
					&pdata->ttf_hv_12v_wireless_charge_current);
	if (ret) {
		pr_info("%s: ttf_hv_12v_wireless_charge_current is Empty, Defualt value 0 \n", __func__);
		pdata->ttf_hv_12v_wireless_charge_current =
			pdata->charging_current[SEC_BATTERY_CABLE_HV_WIRELESS_20].fast_charging_current - 300;
	}

	ret = of_property_read_u32(np, "battery,ttf_wireless_charge_current",
					&pdata->ttf_wireless_charge_current);
	if (ret) {
		pr_info("%s: ttf_wireless_charge_current is Empty, Defualt value 0 \n", __func__);
		pdata->ttf_wireless_charge_current =
			pdata->charging_current[SEC_BATTERY_CABLE_WIRELESS].input_current_limit;
	}
#endif

	/* wpc_en */
	ret = pdata->wpc_en = of_get_named_gpio(np, "battery,wpc_en", 0);
	if (ret < 0) {
		pr_info("%s : can't get wpc_en\n", __func__);
		pdata->wpc_en = 0;
	}
#if defined(CONFIG_BATTERY_AGE_FORECAST)
	p = of_get_property(np, "battery,age_data", &len);
	if (p) {
		battery->pdata->num_age_step = len / sizeof(sec_age_data_t);
		battery->pdata->age_data = kzalloc(len, GFP_KERNEL);
		ret = of_property_read_u32_array(np, "battery,age_data",
				 (u32 *)battery->pdata->age_data, len/sizeof(u32));
		if (ret) {
			pr_err("%s failed to read battery->pdata->age_data: %d\n",
					__func__, ret);
			kfree(battery->pdata->age_data);
			battery->pdata->age_data = NULL;
			battery->pdata->num_age_step = 0;
		}
		pr_err("%s num_age_step : %d\n", __func__, battery->pdata->num_age_step);
		for (len = 0; len < battery->pdata->num_age_step; ++len) {
			pr_err("[%d/%d]cycle:%d, float:%d, full_v:%d, recharge_v:%d, soc:%d\n",
				len, battery->pdata->num_age_step-1,
				battery->pdata->age_data[len].cycle,
				battery->pdata->age_data[len].float_voltage,
				battery->pdata->age_data[len].full_condition_vcell,
				battery->pdata->age_data[len].recharge_condition_vcell,
				battery->pdata->age_data[len].full_condition_soc);
		}
	} else {
		battery->pdata->num_age_step = 0;
		pr_err("%s there is not age_data\n", __func__);
	}
#endif
	p = of_get_property(np, "battery,health_condition", &len);
	if (p || (len / sizeof(battery_health_condition)) == BATTERY_HEALTH_MAX) {
		battery->pdata->health_condition = kzalloc(len, GFP_KERNEL);
		ret = of_property_read_u32_array(np, "battery,health_condition",
				(u32 *)battery->pdata->health_condition, len/sizeof(u32));
		if (ret) {
			pr_err("%s failed to read battery->pdata->health_condition: %d\n",
					__func__, ret);
			kfree(battery->pdata->health_condition);
			battery->pdata->health_condition = NULL;
		} else {
			for (i = 0; i < BATTERY_HEALTH_MAX; i++) {
				pr_err("%s: [BATTERY_HEALTH] %d: Cycle(~ %d), ASoC(~ %d)\n",
						__func__, i,
						battery->pdata->health_condition[i].cycle,
						battery->pdata->health_condition[i].asoc);
			}
		}
	} else {
		battery->pdata->health_condition = NULL;
		pr_err("%s there is not health_condition, len(%d)\n", __func__, len);
	}

	ret = of_property_read_u32(np, "battery,siop_input_limit_current",
			&pdata->siop_input_limit_current);
	if (ret)
		pdata->siop_input_limit_current = SIOP_INPUT_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_charging_limit_current",
			&pdata->siop_charging_limit_current);
	if (ret)
		pdata->siop_charging_limit_current = SIOP_CHARGING_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_hv_12v_input_limit_current",
			&pdata->siop_hv_12v_input_limit_current);
	if (ret)
		pdata->siop_hv_12v_input_limit_current = SIOP_HV_12V_INPUT_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_hv_12v_charging_limit_current",
			&pdata->siop_hv_12v_charging_limit_current);
	if (ret)
		pdata->siop_hv_12v_charging_limit_current = SIOP_HV_12V_CHARGING_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_hv_input_limit_current",
			&pdata->siop_hv_input_limit_current);
	if (ret)
		pdata->siop_hv_input_limit_current = SIOP_HV_INPUT_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_hv_input_limit_current_2nd",
			&pdata->siop_hv_input_limit_current_2nd);
	if (ret)
		pdata->siop_hv_input_limit_current_2nd = SIOP_HV_INPUT_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_hv_charging_limit_current",
			&pdata->siop_hv_charging_limit_current);
	if (ret)
		pdata->siop_hv_charging_limit_current = SIOP_HV_CHARGING_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_apdo_input_limit_current",
			&pdata->siop_apdo_input_limit_current);
	if (ret)
		pdata->siop_apdo_input_limit_current = SIOP_APDO_INPUT_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_apdo_charging_limit_current",
			&pdata->siop_apdo_charging_limit_current);
	if (ret)
		pdata->siop_apdo_charging_limit_current = SIOP_APDO_CHARGING_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_wireless_input_limit_current",
			&pdata->siop_wireless_input_limit_current);
	if (ret)
		pdata->siop_wireless_input_limit_current = SIOP_WIRELESS_INPUT_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_wireless_charging_limit_current",
			&pdata->siop_wireless_charging_limit_current);
	if (ret)
		pdata->siop_wireless_charging_limit_current = SIOP_WIRELESS_CHARGING_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_hv_wireless_input_limit_current",
			&pdata->siop_hv_wireless_input_limit_current);
	if (ret)
		pdata->siop_hv_wireless_input_limit_current = SIOP_HV_WIRELESS_INPUT_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,siop_hv_wireless_charging_limit_current",
			&pdata->siop_hv_wireless_charging_limit_current);
	if (ret)
		pdata->siop_hv_wireless_charging_limit_current = SIOP_HV_WIRELESS_CHARGING_LIMIT_CURRENT;

	ret = of_property_read_u32(np, "battery,wireless_otg_input_current",
			&pdata->wireless_otg_input_current);
	if (ret)
		pdata->wireless_otg_input_current = WIRELESS_OTG_INPUT_CURRENT;

	ret = of_property_read_u32(np, "battery,max_input_voltage",
			&pdata->max_input_voltage);
	if (ret)
		pdata->max_input_voltage = 12000;

	ret = of_property_read_u32(np, "battery,max_input_current",
			&pdata->max_input_current);
	if (ret)
		pdata->max_input_current = 3000;

	ret = of_property_read_u32(np, "battery,pd_charging_charge_power",
			&pdata->pd_charging_charge_power);
	if (ret) {
		pr_err("%s: pd_charging_charge_power is Empty\n", __func__);
		pdata->pd_charging_charge_power = 15000;
	}

	ret = of_property_read_u32(np, "battery,rp_current_rp1",
			&pdata->rp_current_rp1);
	if (ret) {
		pr_err("%s: rp_current_rp1 is Empty\n", __func__);
		pdata->rp_current_rp1 = 500;
	}

	ret = of_property_read_u32(np, "battery,rp_current_rp2",
			&pdata->rp_current_rp2);
	if (ret) {
		pr_err("%s: rp_current_rp2 is Empty\n", __func__);
		pdata->rp_current_rp2 = 1500;
	}

	ret = of_property_read_u32(np, "battery,rp_current_rp3",
			&pdata->rp_current_rp3);
	if (ret) {
		pr_err("%s: rp_current_rp3 is Empty\n", __func__);
		pdata->rp_current_rp3 = 3000;
	}

	ret = of_property_read_u32(np, "battery,rp_current_rdu_rp3",
			&pdata->rp_current_rdu_rp3);
	if (ret) {
		pr_err("%s: rp_current_rdu_rp3 is Empty\n", __func__);
		pdata->rp_current_rdu_rp3 = 2100;
	}

	ret = of_property_read_u32(np, "battery,rp_current_abnormal_rp3",
			&pdata->rp_current_abnormal_rp3);
	if (ret) {
		pr_err("%s: rp_current_abnormal_rp3 is Empty\n", __func__);
		pdata->rp_current_rdu_rp3 = 1800;
	}

	ret = of_property_read_u32(np, "battery,nv_charge_power",
			&pdata->nv_charge_power);
	if (ret) {
		pr_err("%s: nv_charge_power is Empty\n", __func__);
		pdata->nv_charge_power =
			SEC_INPUT_VOLTAGE_5V * pdata->default_input_current;
	}

	ret = of_property_read_u32(np, "battery,tx_minduty_default",
			&pdata->tx_minduty_default);
	if (ret) {
		pdata->tx_minduty_default = 20;
		pr_err("%s: tx minduty is Empty. set %d\n", __func__, pdata->tx_minduty_default);
	}

	ret = of_property_read_u32(np, "battery,tx_minduty_5V",
			&pdata->tx_minduty_5V);
	if (ret) {
		pdata->tx_minduty_5V = 50;
		pr_err("%s: tx minduty 5V is Empty. set %d\n", __func__, pdata->tx_minduty_5V);
	}

	ret = of_property_read_u32(np, "battery,tx_uno_iout",
			&pdata->tx_uno_iout);
	if (ret) {
		pdata->tx_uno_iout = 1500;
		pr_err("%s: tx uno iout is Empty. set %d\n", __func__, pdata->tx_uno_iout);
	}

	ret = of_property_read_u32(np, "battery,tx_mfc_iout_gear",
			&pdata->tx_mfc_iout_gear);
	if (ret) {
		pdata->tx_mfc_iout_gear = 1500;
		pr_err("%s: tx mfc iout gear is Empty. set %d\n", __func__, pdata->tx_mfc_iout_gear);
	}

	ret = of_property_read_u32(np, "battery,tx_mfc_iout_phone",
			&pdata->tx_mfc_iout_phone);
	if (ret) {
		pdata->tx_mfc_iout_phone = 1100;
		pr_err("%s: tx mfc iout phone is Empty. set %d\n", __func__, pdata->tx_mfc_iout_phone);
	}

	ret = of_property_read_u32(np, "battery,tx_mfc_iout_phone_5v",
			&pdata->tx_mfc_iout_phone_5v);
	if (ret) {
		pdata->tx_mfc_iout_phone_5v = 300;
		pr_err("%s: tx mfc iout phone 5v is Empty. set %d\n", __func__, pdata->tx_mfc_iout_phone_5v);
	}

	ret = of_property_read_u32(np, "battery,tx_mfc_iout_lcd_on",
			&pdata->tx_mfc_iout_lcd_on);
	if (ret) {
		pdata->tx_mfc_iout_lcd_on = 900;
		pr_err("%s: tx mfc iout lcd on is Empty. set %d\n", __func__, pdata->tx_mfc_iout_lcd_on);
	}

	pr_info("%s: vendor : %s, technology : %d, cable_check_type : %d\n"
		"cable_source_type : %d, temp_adc_type: %d\n"
		"polling_type : %d, initial_count : %d, check_count : %d\n"
		"check_adc_max : %d, check_adc_min : %d\n"
		"ovp_uvlo_check_type : %d, thermal_source : %d\n"
		"temp_check_type : %d, temp_check_count : %d, nv_charge_power : %d\n"
		"full_condition_type : %d, recharge_condition_type : %d, full_check_type : %d\n",
		__func__,
		pdata->vendor, pdata->technology,pdata->cable_check_type,
		pdata->cable_source_type, pdata->temp_adc_type,
		pdata->polling_type, pdata->monitor_initial_count,
		pdata->check_count, pdata->check_adc_max, pdata->check_adc_min,
		pdata->ovp_uvlo_check_type, pdata->thermal_source,
		pdata->temp_check_type, pdata->temp_check_count, pdata->nv_charge_power,
		pdata->full_condition_type, pdata->recharge_condition_type, pdata->full_check_type
		);

#if defined(CONFIG_STEP_CHARGING)
	sec_step_charging_init(battery, dev);
#endif
	ret = of_property_read_u32(np, "battery,max_charging_current",
			&pdata->max_charging_current);
	if (ret) {
		pr_err("%s: max_charging_current is Empty\n", __func__);
		pdata->max_charging_current = 3000;
	}

	ret = of_property_read_u32(np, "battery,max_charging_charge_power",
			&pdata->max_charging_charge_power);
	if (ret) {
		pr_err("%s: max_charging_charge_power is Empty\n", __func__);
		pdata->max_charging_charge_power = 25000;
	}

#if defined(CONFIG_DUAL_BATTERY)
	ret = of_property_read_u32(np,
			"battery,support_dual_battery", &temp);
	if (ret) {
		pr_info("support_dual_battery is empty\n");
		pdata->support_dual_battery = 0;
	} else
		pdata->support_dual_battery = temp;
	
	if (pdata->support_dual_battery) {
		ret = of_property_read_string(np,
			"battery,dual_battery_name", (char const **)&pdata->dual_battery_name);
		if (ret)
			pr_info("%s: Dual battery name is Empty\n", __func__);

		ret = of_property_read_string(np,
			"battery,main_limiter_name", (char const **)&pdata->main_limiter_name);
		if (ret)
			pr_info("%s: Main limiter name is Empty\n", __func__);

		ret = of_property_read_string(np,
			"battery,sub_limiter_name", (char const **)&pdata->sub_limiter_name);
		if (ret)
			pr_info("%s: Sub limiter name is Empty\n", __func__);		
	}

	np = of_find_node_by_name(NULL, "sec-dual-battery");
	if (!np) {
		pr_info("%s: np NULL\n", __func__);
	} else {
		/* MAIN_BATTERY_SW_EN_AP */
		ret = pdata->main_bat_enb_gpio = of_get_named_gpio(np, "battery,main_bat_enb_gpio", 0);
		if (ret < 0) {
			pr_info("%s : can't get main_bat_enb_gpio\n", __func__);
		}
		/* SUB_BATTERY_SW_EN */
		ret = pdata->sub_bat_enb_gpio = of_get_named_gpio(np, "battery,sub_bat_enb_gpio", 0);
		if (ret < 0) {
			pr_info("%s : can't get sub_bat_enb_gpio\n", __func__);
		}
		ret = of_property_read_u32(np, "battery,main_charging_rate",
				&pdata->main_charging_rate);
		if (ret) {
			pr_err("%s: main_charging_rate is Empty\n", __func__);
			pdata->main_charging_rate = 60;
		}
		ret = of_property_read_u32(np, "battery,sub_charging_rate",
				&pdata->sub_charging_rate);
		if (ret) {
			pr_err("%s: sub_charging_rate is Empty\n", __func__);
			pdata->sub_charging_rate = 50;
		}
		ret = of_property_read_u32(np, "battery,force_recharge_margin",
				&pdata->force_recharge_margin);
		if (ret) {
			pr_err("%s: force_recharge_margin is Empty\n", __func__);
			pdata->force_recharge_margin = 150;
		}
		ret = of_property_read_u32(np, "battery,max_main_charging_current",
				&pdata->max_main_charging_current);
		if (ret) {
			pr_err("%s: max_main_charging_current is Empty\n", __func__);
			pdata->max_main_charging_current = 1550;
		}
		ret = of_property_read_u32(np, "battery,min_main_charging_current",
				&pdata->min_main_charging_current);
		if (ret) {
			pr_err("%s: min_main_charging_current is Empty\n", __func__);
			pdata->min_main_charging_current = 450;
		}		
		ret = of_property_read_u32(np, "battery,max_sub_charging_current",
				&pdata->max_sub_charging_current);
		if (ret) {
			pr_err("%s: max_sub_charging_current is Empty\n", __func__);
			pdata->max_sub_charging_current = 1300;
		}
		ret = of_property_read_u32(np, "battery,min_sub_charging_current",
				&pdata->min_sub_charging_current);
		if (ret) {
			pr_err("%s: min_sub_charging_current is Empty\n", __func__);
			pdata->min_sub_charging_current = 450;
		}
		pr_info("%s : main rate:%d, sub rate:%d, recharge marging:%d, "
				"max main curr:%d, min main curr:%d, max sub curr:%d, min sub curr:%d \n",
				__func__, pdata->main_charging_rate, pdata->sub_charging_rate,
				pdata->force_recharge_margin, pdata->max_main_charging_current, pdata->min_main_charging_current,
				pdata->max_sub_charging_current, pdata->min_sub_charging_current);
	}
#endif

#if defined(CONFIG_BATTERY_CISD)
	p = of_get_property(np, "battery,ignore_cisd_index", &len);
	pdata->ignore_cisd_index = kzalloc(sizeof(*pdata->ignore_cisd_index) * 2, GFP_KERNEL);
	if (p) {
		len = len / sizeof(u32);
		ret = of_property_read_u32_array(np, "battery,ignore_cisd_index",
	                     pdata->ignore_cisd_index, len);
	} else {
		pr_info("%s : battery,ignore_cisd_index is Empty\n", __func__);
	}

	p = of_get_property(np, "battery,ignore_cisd_index_d", &len);
	pdata->ignore_cisd_index_d = kzalloc(sizeof(*pdata->ignore_cisd_index_d) * 2, GFP_KERNEL);
	if (p) {
		len = len / sizeof(u32);
		ret = of_property_read_u32_array(np, "battery,ignore_cisd_index_d",
	                     pdata->ignore_cisd_index_d, len);
	} else {
		pr_info("%s : battery,ignore_cisd_index_d is Empty\n", __func__);
	}
#endif

#if defined(CONFIG_DIRECT_CHARGING)
	sec_direct_chg_init(battery, dev);
#endif
	return 0;
}

void sec_bat_parse_mode_dt(struct sec_battery_info *battery)
{
	struct device_node *np;
	sec_battery_platform_data_t *pdata = battery->pdata;
	int ret = 0;
	u32 temp = 0;

	np = of_find_node_by_name(NULL, "battery");
	if (!np) {
		pr_err("%s np NULL\n", __func__);
		return;
	}

	if (battery->store_mode) {
		ret = of_property_read_u32(np, "battery,store_mode_max_input_power",
			&pdata->store_mode_max_input_power);
		if (ret) {
			pr_info("%s : store_mode_max_input_power is Empty\n", __func__);
			pdata->store_mode_max_input_power = 4000;
		}

		if (pdata->wpc_temp_check_type) {
			ret = of_property_read_u32(np, "battery,wpc_store_high_temp",
			   &temp);
			if (!ret)
				pdata->wpc_high_temp = temp;

			ret = of_property_read_u32(np, "battery,wpc_store_high_temp_recovery",
			   &temp);
			if (!ret)
				pdata->wpc_high_temp_recovery = temp;

			ret = of_property_read_u32(np, "battery,wpc_store_charging_limit_current",
			   &temp);
			if (!ret)
				pdata->wpc_input_limit_current = temp;

			ret = of_property_read_u32(np, "battery,wpc_store_lcd_on_high_temp",
			   &temp);
			if (!ret)
				pdata->wpc_lcd_on_high_temp = (int)temp;

			ret = of_property_read_u32(np, "battery,wpc_store_lcd_on_high_temp_rec",
			   &temp);
			if (!ret)
				pdata->wpc_lcd_on_high_temp_rec = (int)temp;

			ret = of_property_read_u32(np, "battery,wpc_store_lcd_on_charging_limit_current",
				&temp);
			if (!ret)
				pdata->wpc_lcd_on_input_limit_current = (int)temp;

			pr_info("%s: update store_mode - wpc high_temp(t:%d, r:%d), lcd_on_high_temp(t:%d, r:%d), curr(%d, %d)\n",
				__func__,
				pdata->wpc_high_temp, pdata->wpc_high_temp_recovery,
				pdata->wpc_lcd_on_high_temp, pdata->wpc_lcd_on_high_temp_rec,
				pdata->wpc_input_limit_current,
				pdata->wpc_lcd_on_input_limit_current);
		}

		ret = of_property_read_u32(np, "battery,siop_store_hv_wireless_input_limit_current",
			&temp);
		if (!ret)
			pdata->siop_hv_wireless_input_limit_current = temp;
		else
			pdata->siop_hv_wireless_input_limit_current = SIOP_STORE_HV_WIRELESS_CHARGING_LIMIT_CURRENT;
		pr_info("%s: update siop_hv_wireless_input_limit_current(%d)\n",
			__func__, pdata->siop_hv_wireless_input_limit_current);
	}
}

void sec_bat_parse_mode_dt_work(struct work_struct *work)
{
	struct sec_battery_info *battery = container_of(work,
		struct sec_battery_info, parse_mode_dt_work.work);

	sec_bat_parse_mode_dt(battery);

	if (is_hv_wire_type(battery->cable_type) ||
		is_hv_wireless_type(battery->cable_type)) {
		sec_bat_set_charging_current(battery);
	}

	wake_unlock(&battery->parse_mode_dt_wake_lock);
}
#endif
