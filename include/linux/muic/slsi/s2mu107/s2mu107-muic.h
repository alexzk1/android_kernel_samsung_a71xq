/*
 * Copyright (C) 2019 Samsung Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#ifndef __S2MU107_MUIC_H__
#define __S2MU107_MUIC_H__

#include <linux/wakelock.h>
#include <linux/muic/common/muic.h>
#include <linux/muic/common/muic_interface.h>
#if defined(CONFIG_HV_MUIC_S2MU107_AFC)
#include <linux/muic/slsi/s2mu107/s2mu107-muic-hv.h>
#endif

#define MUIC_DEV_NAME	"muic-s2mu107"

enum s2mu107_muic_registers {
	S2MU107_REG_AFC_INT =		0x00,
	S2MU107_REG_MUIC_INT1 =		0x01,
	S2MU107_REG_MUIC_INT2 =		0x02,
	S2MU107_REG_AFC_INT_MASK =	0x08,
	S2MU107_REG_MUIC_INT1_MASK =	0x09,
	S2MU107_REG_MUIC_INT2_MASK =	0x0A,
	S2MU107_REG_AFC_STATUS =		0x10,
	S2MU107_REG_AFC_CTRL1 =		0x2D,
	S2MU107_REG_AFC_CTRL2 =		0x2E,
	S2MU107_REG_AFC_TX_BYTE =	0x2F,
	S2MU107_REG_AFC_RX_BYTE1 =	0x30,
	S2MU107_REG_AFC_RX_BYTE2 =	0x31,
	S2MU107_REG_AFC_RX_BYTE3 =	0x32,
	S2MU107_REG_AFC_RX_BYTE4 =	0x33,
	S2MU107_REG_AFC_RX_BYTE5 =	0x34,
	S2MU107_REG_AFC_RX_BYTE6 =	0x35,
	S2MU107_REG_AFC_RX_BYTE7 =	0x36,
	S2MU107_REG_AFC_RX_BYTE8 =	0x37,
	S2MU107_REG_AFC_RX_BYTE9 =	0x38,
	S2MU107_REG_AFC_RX_BYTE10 =	0x39,
	S2MU107_REG_AFC_RX_BYTE11 =	0x3A,
	S2MU107_REG_AFC_RX_BYTE12 =	0x3B,
	S2MU107_REG_AFC_RX_BYTE13 =	0x3C,
	S2MU107_REG_AFC_RX_BYTE14 =	0x3D,
	S2MU107_REG_AFC_RX_BYTE15 =	0x3E,
	S2MU107_REG_AFC_RX_BYTE16 =	0x3F,
	S2MU107_REG_AFC_LOGIC_RDATA1 =	0x40,
	S2MU107_REG_AFC_LOGIC_RDATA2 =	0x41,
	S2MU107_REG_AFC_LOGIC_CTRL1 =	0x42,
	S2MU107_REG_AFC_LOGIC_CTRL2 =	0x43,
	S2MU107_REG_AFC_LOGIC_RDATA3 =	0x44,
	S2MU107_REG_QC30_DP_STEP =	0x45,
	S2MU107_REG_QC30_DM_STEP =	0x46,
	S2MU107_REG_AFC_MONITOR =	0x47,
	S2MU107_REG_ADC_VALUE =		0x48,
	S2MU107_REG_DEVICE_TYP1 =	0x49,
	S2MU107_REG_DEVICE_TYP2 =	0x4A,
	S2MU107_REG_DEVICE_TYP3 =	0x4B,
	S2MU107_REG_RESET =		0x4E,
	S2MU107_REG_CHG_TYP =		0x4F,
	S2MU107_REG_DEVICE_APPLE =	0x50,
	S2MU107_REG_BCD_RESCAN =		0x51,
	S2MU107_REG_TEST1 =		0x52,
	S2MU107_REG_TEST2 =		0x53,
	S2MU107_REG_TEST3 =		0x54,
	S2MU107_REG_WATER_WARN_FLAG =	0x55,
	S2MU107_REG_AFC_OTP0 =		0x64,
	S2MU107_REG_AFC_OTP1 =		0x65,
	S2MU107_REG_AFC_OTP2 =		0x66,
	S2MU107_REG_AFC_OTP3 =		0x67,
	S2MU107_REG_AFC_OTP4 =		0x68,
	S2MU107_REG_AFC_OTP5 =		0x69,
	S2MU107_REG_AFC_OTP6 =		0x6A,
	S2MU107_REG_AFC_OTP7 =		0x6B,
	S2MU107_REG_MUIC_CTRL1 =		0x6C,
	S2MU107_REG_TIMER_SET1 =		0x6D,
	S2MU107_REG_TIMER_SET2 =		0x6E,
	S2MU107_REG_MANUAL_SW_CTRL =	0x6F,
	S2MU107_REG_TIMER_SET3 =		0x70,
	S2MU107_REG_MUIC_CTRL2 =		0x71,
	S2MU107_REG_MUIC_CTRL3 =		0x72,
	S2MU107_REG_CHARGER_DET_OTP =	0x73,
	S2MU107_REG_ADC_VREF_OTP =	0x74,
	S2MU107_REG_ADCBIAS_OTP1 =	0x75,
	S2MU107_REG_ADCBIAS_OTP2 =	0x76,
	S2MU107_REG_ADCBIAS_OTP3 =	0x77,
	S2MU107_REG_ADCBIAS_OTP4 =	0x78,
	S2MU107_REG_LDOADC_VSETL =	0x79,
	S2MU107_REG_LDOADC_VSETH =	0x7A,
	S2MU107_REG_LDOPCP_VSET_OTP =	0x7B,
	S2MU107_REG_MONITOR_SEL =	0x7C,
	S2MU107_REG_RID_WATER_PROOF =	0x7D,
	S2MU107_REG_RID_DISCHARGE =	0x7E,
	S2MU107_REG_MUIC_RSVD1 =		0x7F,
	S2MU107_REG_MUIC_RSVD2 =		0x80,
	S2MU107_REG_MUIC_RSVD3 =		0x81,
	S2MU107_REG_MUIC_RSVD4 =		0x82,
	S2MU107_REG_MUIC_RSVD5 =		0x83,
};

/* s2mu107 muic register read/write related information defines. */
/* S2MU107 Control register */
/* S2MU107 MUIC_INT1 register (0x01)*/
#define S2MU107_WAKE_UP_I_SHIFT			7
#define S2MU107_USB_KILLER_I_SHIFT		6
#define S2MU107_RID_CHG_I_SHIFT			5
#define S2MU107_LKR_I_SHIFT                     4
#define S2MU107_LKP_I_SHIFT                     3
#define S2MU107_KP_I_SHIFT                      2
#define S2MU107_DETACH_I_SHIFT			1
#define S2MU107_ATTACH_I_SHIFT			0

#define S2MU107_WAKE_UP_I_MASK			(0x1 << S2MU107_WAKE_UP_I_SHIFT)
#define S2MU107_USB_KILLER_I_MASK		(0x1 << S2MU107_USB_KILLER_I_SHIFT)
#define S2MU107_RID_CHG_I_MASK			(0x1 << S2MU107_RID_CHG_I_SHIFT)
#define S2MU107_LKR_I_MASK			(0x1 << S2MU107_LKR_I_SHIFT)
#define S2MU107_LKP_I_MASK			(0x1 << S2MU107_LKP_I_SHIFT)
#define S2MU107_KP_I_MASK			(0x1 << S2MU107_KP_I_SHIFT)
#define S2MU107_DETACH_I_MASK			(0x1 << S2MU107_DETACH_I_SHIFT)
#define S2MU107_ATTACH_I_MASK			(0x1 << S2MU107_ATTACH_I_SHIFT)

/* S2MU107 MUIC_INT2 register (0x02)*/
#define S2MU107_VBUS_OFF_I_SHIFT		7
#define S2MU107_AV_CHARGE_I_SHIFT		6
#define S2MU107_MHDL_I_SHIFT			5
#define S2MU107_STUCKRCV_I_SHIFT		4
#define S2MU107_STUCK_I_SHIFT			3
#define S2MU107_ADCCHANGE_I_SHIFT		2
#define S2MU107_RSVD_ATTACH_I_SHIFT		1
#define S2MU107_VBUS_ON_I_SHIFT			0

#define S2MU107_VBUS_OFF_I_MASK			(0x1 << S2MU107_VBUS_OFF_I_SHIFT)
#define S2MU107_AV_CHARGE_I_MASK		(0x1 << S2MU107_AV_CHARGE_I_SHIFT)
#define S2MU107_MHDL_I_MASK			(0x1 << S2MU107_MHDL_I_SHIFT)
#define S2MU107_STUCKRCV_I_MASK			(0x1 << S2MU107_STUCKRCV_I_SHIFT)
#define S2MU107_STUCK_I_MASK			(0x1 << S2MU107_STUCK_I_SHIFT)
#define S2MU107_ADCCHANGE_I_MASK		(0x1 << S2MU107_ADCCHANGE_I_SHIFT)
#define S2MU107_RSVD_ATTACH_I_MASK 		(0x1 << S2MU107_RSVD_ATTACH_I_SHIFT)
#define S2MU107_VBUS_ON_I_MASK			(0x1 << S2MU107_VBUS_ON_I_SHIFT)

/* S2MU107 MUIC_INT1_MASK register (0x09)*/
#define S2MU107_WAKE_UP_Im_SHIFT		7
#define S2MU107_USB_KILLER_Im_SHIFT		6
#define S2MU107_RID_CHG_Im_SHIFT		5
#define S2MU107_LKR_Im_SHIFT			4
#define S2MU107_LKP_Im_SHIFT			3
#define S2MU107_KP_Im_SHIFT			2
#define S2MU107_DETACH_Im_SHIFT			1
#define S2MU107_ATTACH_Im_SHIFT			0

#define S2MU107_WAKE_UP_Im_MASK			(0x1 << S2MU107_WAKE_UP_Im_SHIFT)
#define S2MU107_USB_KILLER_Im_MASK		(0x1 << S2MU107_USB_KILLER_Im_SHIFT)
#define S2MU107_RID_CHG_Im_MASK			(0x1 << S2MU107_RID_CHG_Im_SHIFT)
#define S2MU107_LKR_Im_MASK			(0x1 << S2MU107_LKR_Im_SHIFT)
#define S2MU107_LKP_Im_MASK			(0x1 << S2MU107_LKP_Im_SHIFT)
#define S2MU107_KP_Im_MASK			(0x1 << S2MU107_KP_Im_SHIFT)
#define S2MU107_DETACH_Im_MASK			(0x1 << S2MU107_DETACH_Im_SHIFT)
#define S2MU107_ATTACH_Im_MASK			(0x1 << S2MU107_ATTACH_Im_SHIFT)

/* S2MU107 MUIC_INT2_MASK register (0x0A)*/
#define S2MU107_VBUS_OFF_Im_SHIFT		7
#define S2MU107_AV_CHARGE_Im_SHIFT		6
#define S2MU107_MHDL_Im_SHIFT			5
#define S2MU107_STUCKRCV_Im_SHIFT		4
#define S2MU107_STUCK_Im_SHIFT			3
#define S2MU107_ADCCHANGE_Im_SHIFT		2
#define S2MU107_RSVD_ATTACH_Im_SHIFT	        1
#define S2MU107_VBUS_ON_Im_SHIFT		0

#define S2MU107_VBUS_OFF_Im_MASK		(0x1 << S2MU107_VBUS_OFF_Im_SHIFT)
#define S2MU107_AV_CHARGE_Im_MASK		(0x1 << S2MU107_AV_CHARGE_Im_SHIFT)
#define S2MU107_MHDL_Im_MASK			(0x1 << S2MU107_MHDL_Im_SHIFT)
#define S2MU107_STUCKRCV_Im_MASK		(0x1 << S2MU107_STUCKRCV_Im_SHIFT)
#define S2MU107_STUCK_Im_MASK			(0x1 << S2MU107_STUCK_Im_SHIFT)
#define S2MU107_ADCCHANGE_Im_MASK		(0x1 << S2MU107_ADCCHANGE_Im_SHIFT)
#define S2MU107_RSVD_ATTACH_Im_MASK 	        (0x1 << S2MU107_RSVD_ATTACH_Im_SHIFT)
#define S2MU107_VBUS_ON_Im_MASK			(0x1 << S2MU107_VBUS_ON_Im_SHIFT)

/*
 * S2MU107 MUIC Interrupt Maksing for pdic
 * Use Attach, Detach, VBUS On, ADC Change, VBUS Off.
 */
#define S2MU107_INT_MUIC_MASK1			(0xFC)
#define S2MU107_INT_MUIC_MASK2			(0x7A)

#define S2MU107_INT_USBKILLER_MASK1		(0xBD)
#define S2MU107_ADC_MASK			(0x1f)

/* S2MU107 ADC_VALUE register (0x48) */
#define S2MU107_ADCERR_SHIFT			7
#define S2MU107_ADCVAL_SHIFT			0

#define S2MU107_ADCERR_MASK			(0x1 << S2MU107_ADCERR_SHIFT)
#define S2MU107_ADCVAL_MASK			(0x1F << S2MU107_ADCVAL_SHIFT)

/* S2MU107 DEVICE_TYP1 register (0x49)*/
#define S2MU107_USBOTG_SHIFT		7
#define S2MU107_DCPCHG_SHIFT		6
#define S2MU107_CDPCHG_SHIFT		5
#define S2MU107_CARKIT_SHIFT		4
#define S2MU107_UART_SHIFT		3
#define S2MU107_USB_SHIFT		2
#define S2MU107_AUDIOTYP2_SHIFT		1
#define S2MU107_AUDIOTYP1_SHIFT		0

#define S2MU107_USBOTG_MASK		(0x1 << S2MU107_USBOTG_SHIFT)
#define S2MU107_DCPCHG_MASK		(0x1 << S2MU107_DCPCHG_SHIFT)
#define S2MU107_CDPCHG_MASK		(0x1 << S2MU107_CDPCHG_SHIFT)
#define S2MU107_CARKIT_MASK		(0x1 << S2MU107_CARKIT_SHIFT)
#define S2MU107_UART_MASK		(0x1 << S2MU107_UART_SHIFT)
#define S2MU107_USB_MASK		(0x1 << S2MU107_USB_SHIFT)
#define S2MU107_AUDIOTYP2_MASK		(0x1 << S2MU107_AUDIOTYP2_SHIFT)
#define S2MU107_AUDIOTYP1_MASK		(0x1 << S2MU107_AUDIOTYP1_SHIFT)

/* S2MU107 DEVICE_TYP2 register (0x4A) */
#define S2MU107_SDP_1P8S_SHIFT		7
#define S2MU107_AV_SHIFT		6
#define S2MU107_TTY_SHIFT		5
#define S2MU107_PPD_SHIFT		4
#define S2MU107_JIGUARTOFF_SHIFT	3
#define S2MU107_JIGUARTON_SHIFT		2
#define S2MU107_JIGUSBOFF_SHIFT		1
#define S2MU107_JIGUSBON_SHIFT		0

#define S2MU107_SDP_1P8S_MASK		(0x1 << S2MU107_SDP_1P8S_SHIFT)
#define S2MU107_AV_MASK			(0x1 << S2MU107_AV_SHIFT)
#define S2MU107_TTY_MASK		(0x1 << S2MU107_TTY_SHIFT)
#define S2MU107_PPD_MASK		(0x1 << S2MU107_PPD_SHIFT)
#define S2MU107_JIGUARTOFF_MASK		(0x1 << S2MU107_JIGUARTOFF_SHIFT)
#define S2MU107_JIGUARTON_MASK		(0x1 << S2MU107_JIGUARTON_SHIFT)
#define S2MU107_JIGUSBOFF_MASK		(0x1 << S2MU107_JIGUSBOFF_SHIFT)
#define S2MU107_JIGUSBON_MASK		(0x1 << S2MU107_JIGUSBON_SHIFT)

/* S2MU107 DEVICE_TYP3 register (0x4B) */
#define S2MU107_U200_CHG_SHIFT		7
#define S2MU107_USB_KILLER_SHIFT	6
#define S2MU107_AV_VBUS_SHIFT		4
#define S2MU107_CARKIT_TYPE_SHIFT	2
#define S2MU107_VBUS_R255_SHIFT		1
#define S2MU107_MHDL_SHIFT		0

#define S2MU107_U200_CHG_MASK		(0x1 << S2MU107_U200_CHG_SHIFT)
#define S2MU107_USB_KILLER_MASK		(0x1 << S2MU107_USB_KILLER_SHIFT)
#define S2MU107_AV_VBUS_MASK		(0x1 << S2MU107_AV_VBUS_SHIFT)
#define S2MU107_CARKIT_TYPE_MASK	(0x3 << S2MU107_CARKIT_TYPE_SHIFT)
#define S2MU107_VBUS_R255_MASK		(0x1 << S2MU107_VBUS_R255_SHIFT)
#define S2MU107_MHDL_MASK		(0x1 << S2MU107_MHDL_SHIFT)

/* S2MU107 MUIC CHG_TYP Register (0x4F) */
#define S2MU107_CHG_TYP_VBUS_R255_SHIFT		7
#define S2MU107_CHG_TYP_OPPO_DCP_SHIFT		6
#define S2MU107_CHG_TYP_DP_3V_SDP_SHIFT		5
#define S2MU107_CHG_TYP_U200_SHIFT		4
#define S2MU107_CHG_TYP_SDP_1P8S_SHIFT		3
#define S2MU107_CHG_TYP_USB_SHIFT		2
#define S2MU107_CHG_TYP_CDPCHG_SHIFT		1
#define S2MU107_CHG_TYP_DCPCHG_SHIFT		0

#define S2MU107_CHG_TYP_VBUS_R255_MASK		(0x1 << S2MU107_CHG_TYP_VBUS_R255_SHIFT)
#define S2MU107_CHG_TYP_OPPO_DCP_MASK		(0x1 << S2MU107_CHG_TYP_OPPO_DCP_SHIFT)
#define S2MU107_CHG_TYP_DP_3V_SDP_MASK		(0x1 << S2MU107_CHG_TYP_DP_3V_SDP_SHIFT)
#define S2MU107_CHG_TYP_U200_MASK		(0x1 << S2MU107_CHG_TYP_U200_SHIFT)
#define S2MU107_CHG_TYP_SDP_1P8S_MASK		(0x1 << S2MU107_CHG_TYP_SDP_1P8S_SHIFT)
#define S2MU107_CHG_TYP_USB_MASK		(0x1 << S2MU107_CHG_TYP_USB_SHIFT)
#define S2MU107_CHG_TYP_CDPCHG_MASK		(0x1 << S2MU107_CHG_TYP_CDPCHG_SHIFT)
#define S2MU107_CHG_TYP_DCPCHG_MASK		(0x1 << S2MU107_CHG_TYP_DCPCHG_SHIFT)
#define S2MU107_DEV_TYPE_CHG_TYPE		(S2MU107_CHG_TYP_VBUS_R255_MASK | S2MU107_CHG_TYP_U200_MASK | S2MU107_CHG_TYP_SDP_1P8S_MASK)

/* S2MU107 MUIC DEVICE_APPLE Register (0x50) */
#define S2MU107_APPLE0P5A_CHG_SHIFT	7
#define S2MU107_APPLE1A_CHG_SHIFT	6
#define S2MU107_APPLE2A_CHG_SHIFT	5
#define S2MU107_APPLE2P4A_CHG_SHIFT	4
#define S2MU107_SDP_DCD_OUT_SHIFT	3
#define S2MU107_RID_WAKEUP_SHIFT	2
#define S2MU107_VBUS_WAKEUP_SHIFT	1
#define S2MU107_BCV1P2_OR_OPEN_SHIFT	0

#define S2MU107_APPLE0P5A_CHG_MASK	(0x1 << S2MU107_APPLE0P5A_CHG_SHIFT)
#define S2MU107_APPLE1A_CHG_MASK	(0x1 << S2MU107_APPLE1A_CHG_SHIFT)
#define S2MU107_APPLE2A_CHG_MASK	(0x1 << S2MU107_APPLE2A_CHG_SHIFT)
#define S2MU107_APPLE2P4A_CHG_MASK	(0x1 << S2MU107_APPLE2P4A_CHG_SHIFT)
#define S2MU107_SDP_DCD_OUT_MASK	(0x1 << S2MU107_SDP_DCD_OUT_SHIFT)
#define S2MU107_RID_WAKEUP_MASK		(0x1 << S2MU107_RID_WAKEUP_SHIFT)
#define S2MU107_VBUS_WAKEUP_MASK	(0x1 << S2MU107_VBUS_WAKEUP_SHIFT)
#define S2MU107_BCV1P2_OR_OPEN_MASK	(0x1 << S2MU107_BCV1P2_OR_OPEN_SHIFT)

/* S2MU107 MUIC BCD_RESCAN Register (0x51) */
#define S2MU107_BCD_RESCAN_SHIFT	0

#define S2MU107_BCD_RESCAN_MASK		(0x1 << S2MU107_BCD_RESCAN_SHIFT)

/* S2MU107 MUIC WATER_WARN_FLAG Register (0x55) */
#define S2MU107_WATER_WARN_FLAG_SHIFT	0

#define S2MU107_WATER_WARN_FLAG_MASK	(0x1 << S2MU107_WATER_WARN_FLAG_SHIFT)

/* S2MU107 MUIC AFC_OTP3 Register (0x67) */
#define S2MU107_COMP_REF_SEL_SHIFT	        5
#define S2MU107_HCOMP_REF_SEL_SHIFT	        0

#define S2MU107_COMP_REF_SEL_MASK	        (0x7 << S2MU107_COMP_REF_SEL_SHIFT)
#define S2MU107_HCOMP_REF_SEL_MASK	        (0x1f << S2MU107_HCOMP_REF_SEL_SHIFT)

#define S2MU107_HCOMP_REF_SEL_1p65_MASK	        (0x10 << S2MU107_HCOMP_REF_SEL_SHIFT)
#define S2MU107_HCOMP_REF_SEL_1p2_MASK		(0x8 << S2MU107_HCOMP_REF_SEL_SHIFT)
#define S2MU107_HCOMP_REF_SEL_0p6_MASK		(0x4 << S2MU107_HCOMP_REF_SEL_SHIFT)
#define S2MU107_HCOMP_REF_SEL_0p55_MASK 	(0x2 << S2MU107_HCOMP_REF_SEL_SHIFT)
#define S2MU107_HCOMP_REF_SEL_0p5_MASK		(0x1 << S2MU107_HCOMP_REF_SEL_SHIFT)

/* S2MU107 MUIC MUIC_CTRL1 Register (0x6C) */
#define S2MU107_TX_DPDM_SHORT_SHIFT	7
#define S2MU107_TX_DP_RDN_SHIFT		6
#define S2MU107_TX_DM_RDN_SHIFT		5
#define S2MU107_SWITCH_OPEN_SHIFT	4
#define S2MU107_RAW_DATA_SHIFT		3
#define S2MU107_MANUAL_SW_SHIFT		2
#define S2MU107_WAIT_SHIFT		1
#define S2MU107_INT_MASK_SHIFT		0

#define S2MU107_TX_DPDM_SHORT_MASK	(0x1 << S2MU107_TX_DPDM_SHORT_SHIFT)
#define S2MU107_TX_DP_RDN_MASK 		(0x1 << S2MU107_TX_DP_RDN_SHIFT)
#define S2MU107_TX_DM_RDN_MASK 		(0x1 << S2MU107_TX_DM_RDN_SHIFT)
#define S2MU107_SWITCH_OPEN_MASK 	(0x1 << S2MU107_SWITCH_OPEN_SHIFT)
#define S2MU107_RAW_DATA_MASK		(0x1 << S2MU107_RAW_DATA_SHIFT)
#define S2MU107_MANUAL_SW_MASK 		(0x1 << S2MU107_MANUAL_SW_SHIFT)
#define S2MU107_WAIT_MASK 		(0x1 << S2MU107_WAIT_SHIFT)
#define S2MU107_INT_MASK_MASK 		(0x1 << S2MU107_INT_MASK_SHIFT)

/* S2MU107 MUIC TIMER_SET1 Register (0x6D) */
#define S2MU107_KEY_PRESS_SHIFT 		4
#define S2MU107_VBUS_RISE_DEB_SEL_SHIFT 	3
#define S2MU107_VBUS_FALL_DEB_SEL_SHIFT 	2
#define S2MU107_CLK_DIV2_SHIFT 		        1
#define S2MU107_EOC2x_SHIFT 			0

#define S2MU107_KEY_PRESS_MASK 		        (0xF << S2MU107_KEY_PRESS_SHIFT)
#define S2MU107_VBUS_RISE_DEB_SEL_MASK 	        (0x1 << S2MU107_VBUS_RISE_DEB_SEL_SHIFT)
#define S2MU107_VBUS_FALL_DEB_SEL_MASK   	(0x1 << S2MU107_VBUS_FALL_DEB_SEL_SHIFT)
#define S2MU107_CLK_DIV2_MASK	        	(0x1 << S2MU107_CLK_DIV2_SHIFT)
#define S2MU107_EOC2x_MASK 			(0x1 << S2MU107_EOC2x_SHIFT)

/* S2MU107 MUIC TIMER_SET2 Register (0x6E) */
#define S2MU107_SWITCHING_WAIT_SHIFT		4
#define S2MU107_LONG_KEY_PRESS_SHIFT 	        0

#define S2MU107_SWITCHING_WAIT_MASK 		(0xF << S2MU107_SWITCHING_WAIT_SHIFT)
#define S2MU107_LONG_KEY_PRESS_MASK 		(0xF << S2MU107_LONG_KEY_PRESS_SHIFT)

/* S2MU107 MUIC MANUAL_SW_CTRL Register (0x6F) */
#define S2MU107_DM_SWITCHING_SHIFT 	        5
#define S2MU107_DP_SWITCHING_SHIFT 	        2
#define S2MU107_JIG_SHIFT 		        0

#define S2MU107_DM_SWITCHING_MASK 	        (0x7 << S2MU107_DM_SWITCHING_SHIFT)
#define S2MU107_DP_SWITCHING_MASK 	        (0x7 << S2MU107_DP_SWITCHING_SHIFT)
#define S2MU107_JIG_MASK 		        (0x1 << S2MU107_JIG_SHIFT)
/*
 * Manual Switch Control
 * D- [7:5] / D+ [4:2]
 * 000: Open all / 001: USB / 010: AUDIO / 011: UART / 100: V_AUDIO
 * rsvd[1] / JIG[0]
 * 00: Vbus to Open / 01: Vbus to Charger / 10: Vbus to MIC / 11: Vbus to VBout
 */
#define S2MU107_MANSW_OPEN	0
#define S2MU107_MANSW_USB	1
#define S2MU107_MANSW_UART1	2
#define S2MU107_MANSW_UART2	3

#define S2MU107_MANSW_OPEN_MASK	(S2MU107_MANSW_OPEN << S2MU107_DM_SWITCHING_SHIFT | S2MU107_MANSW_OPEN << S2MU107_DP_SWITCHING_SHIFT)
#define S2MU107_MANSW_USB_MASK	(S2MU107_MANSW_USB << S2MU107_DM_SWITCHING_SHIFT | S2MU107_MANSW_USB << S2MU107_DP_SWITCHING_SHIFT)
#define S2MU107_MANSW_UART1_MASK	(S2MU107_MANSW_UART1 << S2MU107_DM_SWITCHING_SHIFT | S2MU107_MANSW_UART1 << S2MU107_DP_SWITCHING_SHIFT)
#define S2MU107_MANSW_UART2_MASK	(S2MU107_MANSW_UART2 << S2MU107_DM_SWITCHING_SHIFT | S2MU107_MANSW_UART2 << S2MU107_DP_SWITCHING_SHIFT)

/* S2MU107 MUIC TIMER_SET3 Register (0x70) */
#define S2MU107_JIG_WAIT_TIME_SHIFT	5
#define S2MU107_ADC_PERIOD_SHIFT 	3
#define S2MU107_DCDTMRSET_SHIFT 	0

#define S2MU107_JIG_WAIT_TIME_MASK 	(0x7 << S2MU107_JIG_WAIT_TIME_SHIFT)
#define S2MU107_ADC_PERIOD_MASK 	(0x3 << S2MU107_ADC_PERIOD_SHIFT)
#define S2MU107_DCDTMRSET_MASK 		(0x7 << S2MU107_DCDTMRSET_SHIFT)

#define S2MU107_DCD_TIME_250MS 		(0)
#define S2MU107_DCD_TIME_300MS		(1)
#define S2MU107_DCD_TIME_350MS		(2)
#define S2MU107_DCD_TIME_400MS		(3)
#define S2MU107_DCD_TIME_600MS		(4)
#define S2MU107_DCD_TIME_1200MS		(5)
#define S2MU107_DCD_TIME_1800MS		(6)
#define S2MU107_DCD_TIME_2400MS		(7)

#define S2MU107_DCDTMR_250MS_MASK 	(S2MU107_DCD_TIME_250MS << S2MU107_DCDTMRSET_SHIFT)
#define S2MU107_DCDTMR_300MS_MASK 	(S2MU107_DCD_TIME_300MS << S2MU107_DCDTMRSET_SHIFT)
#define S2MU107_DCDTMR_350MS_MASK 	(S2MU107_DCD_TIME_400MS << S2MU107_DCDTMRSET_SHIFT)
#define S2MU107_DCDTMR_400MS_MASK 	(S2MU107_DCD_TIME_600MS << S2MU107_DCDTMRSET_SHIFT)
#define S2MU107_DCDTMR_600MS_MASK 	(S2MU107_DCD_TIME_1200MS << S2MU107_DCDTMRSET_SHIFT)
#define S2MU107_DCDTMR_1200MS_MASK 	(S2MU107_DCD_TIME_1800MS << S2MU107_DCDTMRSET_SHIFT)
#define S2MU107_DCDTMR_2400MS_MASK 	(S2MU107_DCD_TIME_2400MS << S2MU107_DCDTMRSET_SHIFT)

/* S2MU107 MUIC MUIC_CTRL2 Register (0x71) */
#define S2MU107_WAKEUP_LOOP_SEL_SHIFT	7
#define S2MU107_ADCEN_CNTR_SHIFT 	4
#define S2MU107_DCDTMR_OUT_EN_SHIFT 	3
#define S2MU107_USB_2ND_EN_SHIFT 	2
#define S2MU107_ADC_OFF_SHIFT 		1
#define S2MU107_CPEN_SHIFT 		0

#define S2MU107_WAKEUP_LOOP_SEL_MASK	(0x1 << S2MU107_WAKEUP_LOOP_SEL_SHIFT)
#define S2MU107_ADCEN_CNTR_MASK 	(0x7 << S2MU107_ADCEN_CNTR_SHIFT)
#define S2MU107_DCDTMR_OUT_EN_MASK 	(0x1 << S2MU107_DCDTMR_OUT_EN_SHIFT)
#define S2MU107_USB_2ND_EN_MASK 	(0x1 << S2MU107_USB_2ND_EN_SHIFT)
#define S2MU107_ADC_OFF_MASK 		(0x1 << S2MU107_ADC_OFF_SHIFT)
#define S2MU107_CPEN_MASK 		(0x1 << S2MU107_CPEN_SHIFT)

#define S2MU107_ADCEN_CNTR_1TIME	(1)
#define S2MU107_ADCEN_CNTR_2TIME	(2)
#define S2MU107_ADCEN_CNTR_3TIME	(3)
#define S2MU107_ADCEN_CNTR_4TIME	(4)
#define S2MU107_ADCEN_CNTR_5TIME	(5)
#define S2MU107_ADCEN_CNTR_6TIME	(6)
#define S2MU107_ADCEN_CNTR_7TIME	(7)

#define S2MU107_ADCEN_CNTR_1TIME_MASK 	(S2MU107_ADCEN_CNTR_1TIME << S2MU107_ADCEN_CNTR_SHIFT)
#define S2MU107_ADCEN_CNTR_2TIME_MASK 	(S2MU107_ADCEN_CNTR_2TIME << S2MU107_ADCEN_CNTR_SHIFT)
#define S2MU107_ADCEN_CNTR_3TIME_MASK 	(S2MU107_ADCEN_CNTR_3TIME << S2MU107_ADCEN_CNTR_SHIFT)
#define S2MU107_ADCEN_CNTR_4TIME_MASK 	(S2MU107_ADCEN_CNTR_4TIME << S2MU107_ADCEN_CNTR_SHIFT)
#define S2MU107_ADCEN_CNTR_5TIME_MASK 	(S2MU107_ADCEN_CNTR_5TIME << S2MU107_ADCEN_CNTR_SHIFT)
#define S2MU107_ADCEN_CNTR_6TIME_MASK 	(S2MU107_ADCEN_CNTR_6TIME << S2MU107_ADCEN_CNTR_SHIFT)
#define S2MU107_ADCEN_CNTR_7TIME_MASK 	(S2MU107_ADCEN_CNTR_7TIME << S2MU107_ADCEN_CNTR_SHIFT)

/* S2MU107 MUIC MUIC_CTRL3 Register (0x72) */
#define S2MU107_ADC_DECISION_PERI_SHIFT 6
#define S2MU107_ADC_DECISION_ONE_SHIFT 	4
#define S2MU107_Auto_rescan_SHIFT 	3
#define S2MU107_ONE_SHOT_SHIFT		2
#define S2MU107_LOWPWR_SHIFT 		0

#define S2MU107_ADC_DECISION_PERI_MASK	(0x3 << S2MU107_ADC_DECISION_PERI_SHIFT)
#define S2MU107_ADC_DECISION_ONE_MASK 	(0x3 << S2MU107_ADC_DECISION_ONE_SHIFT)
#define S2MU107_Auto_rescan_MASK 	(0x1 << S2MU107_Auto_rescan_SHIFT)
#define S2MU107_ONE_SHOT_MASK 		(0x1 << S2MU107_ONE_SHOT_SHIFT)
#define S2MU107_LOWPWR_MASK 		(0x1 << S2MU107_LOWPWR_SHIFT)

#define S2MU107_ADC_DECISION_1TIME	(1)
#define S2MU107_ADC_DECISION_2TIME	(2)
#define S2MU107_ADC_DECISION_3TIME	(3)

#define S2MU107_ADC_DECISION_PERI_1TIME_MASK	(S2MU107_ADC_DECISION_1TIME << S2MU107_ADC_DECISION_PERI_SHIFT)
#define S2MU107_ADC_DECISION_PERI_2TIME_MASK	(S2MU107_ADC_DECISION_2TIME << S2MU107_ADC_DECISION_PERI_SHIFT)
#define S2MU107_ADC_DECISION_PERI_3TIME_MASK	(S2MU107_ADC_DECISION_3TIME << S2MU107_ADC_DECISION_PERI_SHIFT)
#define S2MU107_ADC_DECISION_ONE_1TIME_MASK	(S2MU107_ADC_DECISION_1TIME << S2MU107_ADC_DECISION_ONE_SHIFT)
#define S2MU107_ADC_DECISION_ONE_2TIME_MASK	(S2MU107_ADC_DECISION_2TIME << S2MU107_ADC_DECISION_ONE_SHIFT)
#define S2MU107_ADC_DECISION_ONE_3TIME_MASK	(S2MU107_ADC_DECISION_3TIME << S2MU107_ADC_DECISION_ONE_SHIFT)

/* S2MU107 MUIC CHARGER_DET_OTP Register (0x73) */
#define S2MU107_MAN_VBUSTMR_ON_SHIFT	7
#define S2MU107_VTH_APPLE_SHIFT 	4
#define S2MU107_VTRIM_DATA_SHIFT	0

#define S2MU107_MAN_VBUSTMR_ON_MASK	(0x1 << S2MU107_MAN_VBUSTMR_ON_SHIFT)
#define S2MU107_VTH_APPLE_MASK 		(0x7 << S2MU107_VTH_APPLE_SHIFT)
#define S2MU107_VTRIM_DATA_MASK		(0xF << S2MU107_VTRIM_DATA_SHIFT)

/* S2MU107 MUIC ADC_VREF_OTP Register (0x74) */
#define S2MU107_ADC_VREF_TRIM_SHIFT 	0

#define S2MU107_ADC_VREF_TRIM_MASK	(0xFF << S2MU107_ADC_VREF_TRIM_SHIFT)

/* S2MU107 MUIC ADCBIAS_OTP1 Register (0x75) */
#define S2MU107_WAKEUP_LOW_VTH_SEL_SHIFT	7
#define S2MU107_TRM_RID2U_2ND_SHIFT 		0

#define S2MU107_WAKEUP_LOW_VTH_SEL_MASK 	(0x1 << S2MU107_WAKEUP_LOW_VTH_SEL_SHIFT)
#define S2MU107_TRM_RID2U_2ND_MASK 		(0x3F << S2MU107_TRM_RID2U_2ND_SHIFT)

/* S2MU107 MUIC ADCBIAS_OTP2 Register (0x76) */
#define S2MU107_RID_PUP_RES_SEL1_SHIFT	7
#define S2MU107_TRM_RID11U_2ND_SHIFT 	0

#define S2MU107_RID_PUP_RES_SEL1_MASK 	(0x1 << S2MU107_RID_PUP_RES_SEL1_SHIFT)
#define S2MU107_TRM_RID11U_2ND_MASK 	(0x3F << S2MU107_TRM_RID11U_2ND_SHIFT)

/* S2MU107 MUIC ADCBIAS_OTP3 Register (0x77) */
#define S2MU107_RID_PUP_RES_SEL0_SHIFT	7
#define S2MU107_TRM_RID64U_2ND_SHIFT 	0

#define S2MU107_RID_PUP_RES_SEL0_MASK 	(0x1 << S2MU107_RID_PUP_RES_SEL0_SHIFT)
#define S2MU107_TRM_RID64U_2ND_MASK 	(0x3F << S2MU107_TRM_RID64U_2ND_SHIFT)

/* S2MU107 MUIC ADCBIAS_OTP4 Register (0x78) */
#define S2MU107_ADC_STDBY_BIAS_SEL_SHIFT	7
#define S2MU107_TRM_RID260U_2ND_SHIFT		0

#define S2MU107_ADC_STDBY_BIAS_SEL_MASK	        (0x1 << S2MU107_ADC_STDBY_BIAS_SEL_SHIFT)
#define S2MU107_TRM_RID260U_2ND_MASK 		(0x3F << S2MU107_TRM_RID260U_2ND_SHIFT)

/* S2MU107 MUIC LDOADC_VSETL Register (0x79) */
#define S2MU107_UART_SEL_SHIFT 			7
#define S2MU107_MUIC_CLK_O_SHIFT 		6
#define S2MU107_CHG_TYPE_DET_ALWAYS_ONB_SHIFT	5
#define S2MU107_LDOADC_VSETL_SHIFT 		0

#define S2MU107_UART_SEL_MASK			(0x1 << S2MU107_UART_SEL_SHIFT)
#define S2MU107_MUIC_CLK_O_MASK 		(0x1 << S2MU107_MUIC_CLK_O_SHIFT)
#define S2MU107_CHG_TYPE_DET_ALWAYS_ONB_MASK	(0x1 << S2MU107_CHG_TYPE_DET_ALWAYS_ONB_SHIFT)
#define S2MU107_LDOADC_VSETL_MASK 		(0xF << S2MU107_LDOADC_VSETL_SHIFT)

/* S2MU107 MUIC LDOADC_VSETH Register (0x7A) */
#define S2MU107_WAKEUP_DLY_SHIFT 		7
#define S2MU107_WAKE_HYS_SHIFT 			6
#define S2MU107_MANUAL_JIGB_RID_SHIFT		5
#define S2MU107_LDOADC_VSETH_SHIFT 		0

#define S2MU107_WAKEUP_DLY_MASK 		(0x1 << S2MU107_WAKEUP_DLY_SHIFT)
#define S2MU107_WAKE_HYS_MASK 			(0x1 << S2MU107_WAKE_HYS_SHIFT)
#define S2MU107_MANUAL_JIGB_RID_MASK		(0x1 << S2MU107_MANUAL_JIGB_RID_SHIFT)
#define S2MU107_LDOADC_VSETH_MASK 		(0x1F << S2MU107_LDOADC_VSETH_SHIFT)

#define S2MU107_LDOADC_VSET_3V		0x1F
#define S2MU107_LDOADC_VSET_2_7V	0x1C
#define S2MU107_LDOADC_VSET_2_6V	0x0E
#define S2MU107_LDOADC_VSET_2_4V	0x0C
#define S2MU107_LDOADC_VSET_2_2V	0x0A
#define S2MU107_LDOADC_VSET_2_0V	0x08
#define S2MU107_LDOADC_VSET_1_8V	0x06
#define S2MU107_LDOADC_VSET_1_7V	0x05
#define S2MU107_LDOADC_VSET_1_6V	0x04
#define S2MU107_LDOADC_VSET_1_5V	0x03
#define S2MU107_LDOADC_VSET_1_4V	0x02
#define S2MU107_LDOADC_VSET_1_2V	0x00
#define S2MU107_LDOADC_VSETH_WAKE_HYS_SHIFT	6
#define S2MU107_LDOADC_VSETH_WAKE_HYS_MASK	(0x1 << S2MU107_LDOADC_VSETH_WAKE_HYS_SHIFT)

/* S2MU107 MUIC LDOPCP_VSET_OTP Register (0x7B) */
#define S2MU107_LDOPCP_USB_SET_SHIFT 		4
#define S2MU107_JIGB_DLY_AB_SEL_SHIFT 		3
#define S2MU107_VBUS_DET_TMR_MASK_CTRL_SHIFT	2
#define S2MU107_DETACH_TIME_SET_SHIFT 		0

#define S2MU107_LDOPCP_USB_SET_MASK 		(0xF << S2MU107_LDOPCP_USB_SET_SHIFT)
#define S2MU107_JIGB_DLY_AB_SEL_MASK 		(0x3 << S2MU107_JIGB_DLY_AB_SEL_SHIFT)
#define S2MU107_VBUS_DET_TMR_MASK_CTRL_MASK	(0x1 << S2MU107_VBUS_DET_TMR_MASK_CTRL_SHIFT)
#define S2MU107_DETACH_TIME_SET_MASK 		(0x3 << S2MU107_DETACH_TIME_SET_SHIFT)

/* S2MU107 MUIC MONITOR_SEL Register (0x7C) */
#define S2MU107_MONITOR_SEL_SHIFT		7

#define S2MU107_MONITOR_SEL_MASK		(0x1 << S2MU107_MONITOR_SEL_SHIFT)

/* S2MU107 MUIC RID_WATER_PROOF Register (0x7D) */
#define S2MU107_ADC_STD_BIAS_CTRL_SHIFT 	4
#define S2MU107_DETACH_VALID_TSET_SHIFT 	1
#define S2MU107_WATER_WARN_SEL_SHIFT 		0

#define S2MU107_ADC_STD_BIAS_CTRL_MASK		(0xF << S2MU107_ADC_STD_BIAS_CTRL_SHIFT)
#define S2MU107_DETACH_VALID_TSET_MASK 		(0x3 << S2MU107_DETACH_VALID_TSET_SHIFT)
#define S2MU107_WATER_WARN_SEL_MASK 		(0x1 << S2MU107_WATER_WARN_SEL_SHIFT)

/* S2MU107 MUIC RID_DISCHARGE Register (0x7E) */
#define S2MU107_DCD_DAATTACH_OPTION_BACKUP_SHIFT	7
#define S2MU107_CHG_DET_OFF_SHIFT			6
#define S2MU107_USBKILL_OTG_OPTION_SHIFT 		5
#define S2MU107_RID_DISCHG_MAN_OFF_WC_SHIFT 	        4
#define S2MU107_RID_DISCHG_MAN_ON_SHIFT 		3
#define S2MU107_RID_DISCHARGE_ON_SHIFT 		        2
#define S2MU107_RID_DISCHG_TIME_SHIFT 		        0

#define S2MU107_DCD_DAATTACH_OPTION_BACKUP_MASK         (0x1 << S2MU107_DCD_DAATTACH_OPTION_BACKUP_SHIFT)
#define S2MU107_CHG_DET_OFF_MASK 			(0x1 << S2MU107_CHG_DET_OFF_SHIFT)
#define S2MU107_USBKILL_OTG_OPTION_MASK 		(0x1 << S2MU107_USBKILL_OTG_OPTION_SHIFT)
#define S2MU107_RID_DISCHG_MAN_OFF_WC_MASK 	        (0x1 << S2MU107_RID_DISCHG_MAN_OFF_WC_SHIFT)
#define S2MU107_RID_DISCHG_MAN_ON_MASK 		        (0x1 << S2MU107_RID_DISCHG_MAN_ON_SHIFT)
#define S2MU107_RID_DISCHARGE_ON_MASK 		        (0x1 << S2MU107_RID_DISCHARGE_ON_SHIFT)
#define S2MU107_RID_DISCHG_TIME_MASK 		        (0x3 << S2MU107_RID_DISCHG_TIME_SHIFT)

/* S2MU107 MUIC MUIC_RSVD1 Register (0x7F) */
#define S2MU107_HICCUP_UARX_CP_SHIFT			(2)
#define S2MU107_HICCUP_UATX_CP_SHIFT			(1)
#define S2MU107_FAC_WAIT_SHIFT			        (0)

#define S2MU107_HICCUP_UARX_CP_MASK			(0x1 << S2MU107_HICCUP_UARX_CP_SHIFT)
#define S2MU107_HICCUP_UATX_CP_MASK			(0x1 << S2MU107_HICCUP_UATX_CP_SHIFT)
#define S2MU107_FAC_WAIT_MASK			        (0x1 << S2MU107_FAC_WAIT_SHIFT)

enum s2mu107_muic_enable {
	S2MU107_DISABLE,
	S2MU107_ENABLE,
};

enum s2mu107_muic_adc_mode {
	S2MU107_ADC_ONESHOT,
	S2MU107_ADC_PERIODIC,
};

enum s2mu107_muic_detect_dev_read_val {
	DEVICE_TYPE1 = 0,
	DEVICE_TYPE2,
	DEVICE_TYPE3,
	ADC,
	DEVICE_APPLE,
	CHG_TYPE,
	SC_STATUS2,
	MAX_NUM,
};

typedef enum {
	S2MU107_MODE_NONE = -1,
	S2MU107_MODE_AUTO,
	S2MU107_MODE_MANUAL,
	S2MU107_MODE_MAX,
} t_mode_data;

typedef enum {
	S2MU107_PATH_NONE = -1,
	S2MU107_PATH_OPEN,
	S2MU107_PATH_USB,
	S2MU107_PATH_UART_AP,
	S2MU107_PATH_UART_CP,
	S2MU107_PATH_RSVD,
	S2MU107_PATH_MAX,
} t_path_data;

typedef enum {
	S2MU107_DETECT_NONE,
	S2MU107_DETECT_DONE,
	S2MU107_DETECT_SKIP,
} t_detect_status;

typedef enum {
	S2MU107_IRQ_CHECK_DONE,
	S2MU107_IRQ_SKIP,
} t_irq_status;

/* muic chip specific internal data structure
 * that setted at muic-xxxx.c file
 */
struct s2mu107_muic_data {
	struct device *dev;
	struct i2c_client *i2c;	/* i2c addr: 0x7C; MUIC */
	struct i2c_client *i2c_common;	/* i2c addr: 0x74; common */
	struct s2mu107_dev *s2mu107_dev;

	struct mutex muic_mutex;
	struct mutex switch_mutex;
	struct mutex bcd_rescan_mutex;
#if defined(CONFIG_HV_MUIC_S2MU107_AFC)
	struct mutex afc_mutex;
#endif

	/* struct wakeup_source wakeup_src; */
	struct wake_lock wake_lock;

	/* model dependant mfd platform data */
	struct s2mu107_platform_data *mfd_pdata;

	/* model dependant muic platform data */
	struct muic_platform_data *pdata;

	void *if_data;

	int irq_attach;
	int irq_detach;
	int irq_rid_chg;
	int irq_usb_killer;
	int irq_vbus_on;
	int irq_rsvd_attach;
	int irq_adc_change;
	int irq_av_charge;
	int irq_vbus_off;
#if defined(CONFIG_HV_MUIC_S2MU107_AFC)
	int irq_vdnmon;
	int irq_mpnack;
	int irq_mrxtrf;
	int irq_mrxrdy;
	struct power_supply *psy_pm;
#endif
	bool afc_check;
#if defined(CONFIG_HV_MUIC_S2MU107_AFC)
	bool is_dp_drive;
	bool is_hvcharger_detected;
	bool is_requested_step_down;
#endif
#if IS_ENABLED(CONFIG_HICCUP_CHARGER)
	bool is_hiccup_mode;
#endif
	muic_attached_dev_t new_dev;

	int adc;
	int vbvolt;
	int vmid;
	int reg[MAX_NUM];
#if defined(CONFIG_HV_MUIC_S2MU107_AFC)
	int mrxrdy_cnt;
	int mping_cnt;
	int qc_retry_cnt;
	int tx_data;
	int qc_retry_wait_cnt;

	muic_hv_state_t hv_state;
#endif

	/*
	 * Distinguisher is needed,
	 * if it doesn't have the muic manager.
	 */

	/* W/A waiting for the charger ic */
	struct delayed_work dcd_recheck;
#if defined(CONFIG_HV_MUIC_S2MU107_AFC)
	struct delayed_work reset_work;
	struct delayed_work mping_retry_work;
	struct delayed_work qc_retry_work;
#endif
};

extern struct muic_platform_data muic_pdata;
extern unsigned int lpcharge;

int s2mu107_i2c_read_byte(struct i2c_client *client, u8 command);
int s2mu107_i2c_write_byte(struct i2c_client *client, u8 command, u8 value);
int s2mu107_muic_recheck_adc(struct s2mu107_muic_data *muic_data);
void s2mu107_muic_control_vbus_det(struct s2mu107_muic_data *muic_data,
		bool enable);
extern void s2mu107_muic_dcd_rescan(struct s2mu107_muic_data *muic_data);
extern int s2mu107_muic_control_rid_adc(struct s2mu107_muic_data *muic_data,
		bool enable);
extern int s2mu107_muic_bcd_rescan(struct s2mu107_muic_data *muic_data);
int s2mu107_muic_jig_on(struct s2mu107_muic_data *muic_data);
void s2mu107_muic_control_vbus_det(struct s2mu107_muic_data *muic_data,
		bool enable);
int s2mu107_set_gpio_uart_sel(struct s2mu107_muic_data *muic_data,
		int uart_sel);
int s2mu107_init_interface(struct s2mu107_muic_data *muic_data,
		struct muic_interface_t *muic_if);
extern int _s2mu107_muic_get_vbus_state(struct s2mu107_muic_data *muic_data);
#if IS_ENABLED(CONFIG_PDIC_S2MU107)
int s2mu107_muic_refresh_adc(struct s2mu107_muic_data *muic_data);
#endif
#if defined(CONFIG_HV_MUIC_S2MU107_AFC)
int s2mu107_hv_muic_get_vdnmon_status(struct s2mu107_muic_data *muic_data);
#endif
#endif /* __S2MU107_MUIC_H__ */
