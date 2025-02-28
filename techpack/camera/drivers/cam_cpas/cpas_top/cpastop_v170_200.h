/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2019-2020, The Linux Foundation. All rights reserved.
 */

#ifndef _CPASTOP_V170_200_H_
#define _CPASTOP_V170_200_H_

#define TEST_IRQ_ENABLE 0
#define TCSR_CONN_RESET 0x0
#define TCSR_CONN_SET  0x3

static struct cam_camnoc_irq_sbm cam_cpas_v170_200_irq_sbm = {
	.sbm_enable = {
		.access_type = CAM_REG_TYPE_READ_WRITE,
		.enable = true,
		.offset = 0x2040, /* SBM_FAULTINEN0_LOW */
		.value = 0x1 | /* SBM_FAULTINEN0_LOW_PORT0_MASK*/
			0x2 | /* SBM_FAULTINEN0_LOW_PORT1_MASK */
			0x4 | /* SBM_FAULTINEN0_LOW_PORT2_MASK */
			0x8 | /* SBM_FAULTINEN0_LOW_PORT3_MASK */
			0x10 | /* SBM_FAULTINEN0_LOW_PORT4_MASK */
			0x20 | /* SBM_FAULTINEN0_LOW_PORT5_MASK */
			(TEST_IRQ_ENABLE ?
			0x100 : /* SBM_FAULTINEN0_LOW_PORT8_MASK */
			0x0),
	},
	.sbm_status = {
		.access_type = CAM_REG_TYPE_READ,
		.enable = true,
		.offset = 0x2048, /* SBM_FAULTINSTATUS0_LOW */
	},
	.sbm_clear = {
		.access_type = CAM_REG_TYPE_WRITE,
		.enable = true,
		.offset = 0x2080, /* SBM_FLAGOUTCLR0_LOW */
		.value = TEST_IRQ_ENABLE ? 0x6 : 0x2,
	}
};

static struct cam_camnoc_irq_err
	cam_cpas_v170_200_irq_err[] = {
	{
		.irq_type = CAM_CAMNOC_HW_IRQ_SLAVE_ERROR,
		.enable = true,
		.sbm_port = 0x1, /* SBM_FAULTINSTATUS0_LOW_PORT0_MASK */
		.err_enable = {
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.enable = true,
			.offset = 0x3D88, /* ERRLOGGER_MAINCTL_LOW */
			.value = 1,
		},
		.err_status = {
			.access_type = CAM_REG_TYPE_READ,
			.enable = true,
			.offset = 0x3D90, /* ERRLOGGER_ERRVLD_LOW */
		},
		.err_clear = {
			.access_type = CAM_REG_TYPE_WRITE,
			.enable = true,
			.offset = 0x3D98, /* ERRLOGGER_ERRCLR_LOW */
			.value = 1,
		},
	},
	{
		.irq_type = CAM_CAMNOC_HW_IRQ_IFE01_UBWC_ENCODE_ERROR,
		.enable = true,
		.sbm_port = 0x2, /* SBM_FAULTINSTATUS0_LOW_PORT1_MASK */
		.err_enable = {
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.enable = true,
			.offset = 0x25a0, /* SPECIFIC_IFE01_ENCERREN_LOW */
			.value = 1,
		},
		.err_status = {
			.access_type = CAM_REG_TYPE_READ,
			.enable = true,
			.offset = 0x2590, /* SPECIFIC_IFE01_ENCERRSTATUS_LOW */
		},
		.err_clear = {
			.access_type = CAM_REG_TYPE_WRITE,
			.enable = true,
			.offset = 0x2598, /* SPECIFIC_IFE01_ENCERRCLR_LOW */
			.value = 1,
		},
	},
	{
		.irq_type = CAM_CAMNOC_HW_IRQ_IFE23_UBWC_ENCODE_ERROR,
		.enable = true,
		.sbm_port = 0x4, /* SBM_FAULTINSTATUS0_LOW_PORT2_MASK */
		.err_enable = {
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.enable = true,
			.offset = 0x27a0, /* SPECIFIC_IFE23_ENCERREN_LOW */
			.value = 1,
		},
		.err_status = {
			.access_type = CAM_REG_TYPE_READ,
			.enable = true,
			.offset = 0x2790, /* SPECIFIC_IFE23_ENCERRSTATUS_LOW */
		},
		.err_clear = {
			.access_type = CAM_REG_TYPE_WRITE,
			.enable = true,
			.offset = 0x2798, /* SPECIFIC_IFE23_ENCERRCLR_LOW */
			.value = 1,
		},
	},
	{
		.irq_type = CAM_CAMNOC_HW_IRQ_IPE_BPS_UBWC_DECODE_ERROR,
		.enable = true,
		.sbm_port = 0x8, /* SBM_FAULTINSTATUS0_LOW_PORT3_MASK */
		.err_enable = {
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.enable = true,
			.offset = 0x2920, /* SPECIFIC_IBL_RD_DECERREN_LOW */
			.value = 1,
		},
		.err_status = {
			.access_type = CAM_REG_TYPE_READ,
			.enable = true,
			.offset = 0x2910, /* SPECIFIC_IBL_RD_DECERRSTATUS_LOW */
		},
		.err_clear = {
			.access_type = CAM_REG_TYPE_WRITE,
			.enable = true,
			.offset = 0x2918, /* SPECIFIC_IBL_RD_DECERRCLR_LOW */
			.value = 1,
		},
	},
	{
		.irq_type = CAM_CAMNOC_HW_IRQ_IPE_BPS_UBWC_ENCODE_ERROR,
		.enable = true,
		.sbm_port = 0x10, /* SBM_FAULTINSTATUS0_LOW_PORT4_MASK */
		.err_enable = {
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.enable = true,
			.offset = 0x2ba0, /* SPECIFIC_IBL_WR_ENCERREN_LOW */
			.value = 1,
		},
		.err_status = {
			.access_type = CAM_REG_TYPE_READ,
			.enable = true,
			.offset = 0x2b90,
			/* SPECIFIC_IBL_WR_ENCERRSTATUS_LOW */
		},
		.err_clear = {
			.access_type = CAM_REG_TYPE_WRITE,
			.enable = true,
			.offset = 0x2b98, /* SPECIFIC_IBL_WR_ENCERRCLR_LOW */
			.value = 1,
		},
	},
	{
		.irq_type = CAM_CAMNOC_HW_IRQ_AHB_TIMEOUT,
		.enable = true,
		.sbm_port = 0x20, /* SBM_FAULTINSTATUS0_LOW_PORT5_MASK */
		.err_enable = {
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.enable = true,
			.offset = 0x2088, /* SBM_FLAGOUTSET0_LOW */
			.value = 0x1,
		},
		.err_status = {
			.access_type = CAM_REG_TYPE_READ,
			.enable = true,
			.offset = 0x2090, /* SBM_FLAGOUTSTATUS0_LOW */
		},
		.err_clear = {
			.enable = false,
		},
	},
	{
		.irq_type = CAM_CAMNOC_HW_IRQ_RESERVED1,
		.enable = false,
	},
	{
		.irq_type = CAM_CAMNOC_HW_IRQ_RESERVED2,
		.enable = false,
	},
	{
		.irq_type = CAM_CAMNOC_HW_IRQ_CAMNOC_TEST,
		.enable = TEST_IRQ_ENABLE ? true : false,
		.sbm_port = 0x100, /* SBM_FAULTINSTATUS0_LOW_PORT8_MASK */
		.err_enable = {
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.enable = true,
			.offset = 0x2088, /* SBM_FLAGOUTSET0_LOW */
			.value = 0x5,
		},
		.err_status = {
			.access_type = CAM_REG_TYPE_READ,
			.enable = true,
			.offset = 0x2090, /* SBM_FLAGOUTSTATUS0_LOW */
		},
		.err_clear = {
			.enable = false,
		},
	},
};

static struct cam_camnoc_specific
	cam_cpas_v170_200_camnoc_specific[] = {
	{
		.port_type = CAM_CAMNOC_CDM,
		.enable = true,
		.priority_lut_low = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2230, /* SPECIFIC_CDM_PRIORITYLUT_LOW */
			.value = 0x33333333,
		},
		.priority_lut_high = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2234, /* SPECIFIC_CDM_PRIORITYLUT_HIGH */
			.value = 0x33333333,
		},
		.urgency = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 1,
			.offset = 0x2238, /* SPECIFIC_CDM_URGENCY_LOW */
			.mask = 0x7, /* SPECIFIC_CDM_URGENCY_LOW_READ_MASK */
			.shift = 0x0, /* SPECIFIC_CDM_URGENCY_LOW_READ_SHIFT */
			.value = 0x3,
		},
		.danger_lut = {
			.enable = false,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2240, /* SPECIFIC_CDM_DANGERLUT_LOW */
			.value = 0x0,
		},
		.safe_lut = {
			.enable = false,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2248, /* SPECIFIC_CDM_SAFELUT_LOW */
			.value = 0x0,
		},
		.ubwc_ctl = {
			.enable = false,
		},
	},
	{
		.port_type = CAM_CAMNOC_IFE01,
		.enable = true,
		.priority_lut_low = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2430, /* SPECIFIC_IFE01_PRIORITYLUT_LOW */
			.value = 0x66665643,
		},
		.priority_lut_high = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2434, /* SPECIFIC_IFE01_PRIORITYLUT_HIGH */
			.value = 0x66666666,
		},
		.urgency = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 1,
			.offset = 0x2438, /* SPECIFIC_IFE01_URGENCY_LOW */
			/* SPECIFIC_IFE01_URGENCY_LOW_WRITE_MASK */
			.mask = 0x70,
			/* SPECIFIC_IFE01_URGENCY_LOW_WRITE_SHIFT */
			.shift = 0x4,
			.value = 3,
		},
		.danger_lut = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.offset = 0x2440, /* SPECIFIC_IFE01_DANGERLUT_LOW */
			.value = 0xFFFFFF00,
		},
		.safe_lut = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.offset = 0x2448, /* SPECIFIC_IFE01_SAFELUT_LOW */
			.value = 0x0000000f,
		},
		.ubwc_ctl = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2588, /* SPECIFIC_IFE01_ENCCTL_LOW */
			.value = 5,
		},
	},
	{
		.port_type = CAM_CAMNOC_IFE23,
		.enable = true,
		.priority_lut_low = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2630, /* SPECIFIC_IFE23_PRIORITYLUT_LOW */
			.value = 0x66665643,
		},
		.priority_lut_high = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2634, /* SPECIFIC_IFE23_PRIORITYLUT_HIGH */
			.value = 0x66666666,
		},
		.urgency = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 1,
			.offset = 0x2638, /* SPECIFIC_IFE23_URGENCY_LOW */
			/* SPECIFIC_IFE23_URGENCY_LOW_WRITE_MASK */
			.mask = 0x70,
			/* SPECIFIC_IFE23_URGENCY_LOW_WRITE_SHIFT */
			.shift = 0x4,
			.value = 3,
		},
		.danger_lut = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.offset = 0x2640, /* SPECIFIC_IFE23_DANGERLUT_LOW */
			.value = 0xFFFFFF00,
		},
		.safe_lut = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.offset = 0x2648, /* SPECIFIC_IFE23_SAFELUT_LOW */
			.value = 0x0000000f,
		},
		.ubwc_ctl = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2788, /* SPECIFIC_IFE23_ENCCTL_LOW */
			.value = 5,
		},
	},
	{
		.port_type = CAM_CAMNOC_IPE_BPS_LRME_READ,
		.enable = true,
		.priority_lut_low = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2830, /* SPECIFIC_IBL_RD_PRIORITYLUT_LOW */
			.value = 0x33333333,
		},
		.priority_lut_high = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2834, /* SPECIFIC_IBL_RD_PRIORITYLUT_HIGH */
			.value = 0x33333333,
		},
		.urgency = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 1,
			.offset = 0x2838, /* SPECIFIC_IBL_RD_URGENCY_LOW */
			/* SPECIFIC_IBL_RD_URGENCY_LOW_READ_MASK */
			.mask = 0x7,
			/* SPECIFIC_IBL_RD_URGENCY_LOW_READ_SHIFT */
			.shift = 0x4,
			.value = 3,
		},
		.danger_lut = {
			.enable = false,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2840, /* SPECIFIC_IBL_RD_DANGERLUT_LOW */
			.value = 0x0,
		},
		.safe_lut = {
			.enable = false,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2848, /* SPECIFIC_IBL_RD_SAFELUT_LOW */
			.value = 0x0,
		},
		.ubwc_ctl = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2908, /* SPECIFIC_IBL_RD_DECCTL_LOW */
			.value = 1,
		},
	},
	{
		.port_type = CAM_CAMNOC_IPE_BPS_LRME_WRITE,
		.enable = true,
		.priority_lut_low = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2A30, /* SPECIFIC_IBL_WR_PRIORITYLUT_LOW */
			.value = 0x33333333,
		},
		.priority_lut_high = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2A34, /* SPECIFIC_IBL_WR_PRIORITYLUT_HIGH */
			.value = 0x33333333,
		},
		.urgency = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 1,
			.offset = 0x2A38, /* SPECIFIC_IBL_WR_URGENCY_LOW */
			/* SPECIFIC_IBL_WR_URGENCY_LOW_WRITE_MASK */
			.mask = 0x70,
			/* SPECIFIC_IBL_WR_URGENCY_LOW_WRITE_SHIFT */
			.shift = 0x4,
			.value = 3,
		},
		.danger_lut = {
			.enable = false,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2A40, /* SPECIFIC_IBL_WR_DANGERLUT_LOW */
			.value = 0x0,
		},
		.safe_lut = {
			.enable = false,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2A48, /* SPECIFIC_IBL_WR_SAFELUT_LOW */
			.value = 0xFFF,
		},
		.ubwc_ctl = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2B88, /* SPECIFIC_IBL_WR_ENCCTL_LOW */
			.value = 5,
		},
	},
	{
		.port_type = CAM_CAMNOC_JPEG,
		.enable = true,
		.priority_lut_low = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2C30, /* SPECIFIC_JPEG_PRIORITYLUT_LOW */
			.value = 0x22222222,
		},
		.priority_lut_high = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2C34, /* SPECIFIC_JPEG_PRIORITYLUT_HIGH */
			.value = 0x22222222,
		},
		.urgency = {
			.enable = true,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2C38, /* SPECIFIC_JPEG_URGENCY_LOW */
			.value = 0x22,
		},
		.danger_lut = {
			.enable = false,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2C40, /* SPECIFIC_JPEG_DANGERLUT_LOW */
			.value = 0x0,
		},
		.safe_lut = {
			.enable = false,
			.access_type = CAM_REG_TYPE_READ_WRITE,
			.masked_value = 0,
			.offset = 0x2C48, /* SPECIFIC_JPEG_SAFELUT_LOW */
			.value = 0x0,
		},
		.ubwc_ctl = {
			.enable = false,
		},
	},
	{
		.port_type = CAM_CAMNOC_ICP,
		.enable = true,
		.flag_out_set0_low = {
			.enable = true,
			.access_type = CAM_REG_TYPE_WRITE,
			.masked_value = 0,
			.offset = 0x2088,
			.value = 0x100000,
		},
	},
};

static struct cam_camnoc_err_logger_info cam170_cpas200_err_logger_offsets = {
	.mainctrl     =  0x2708, /* ERRLOGGER_MAINCTL_LOW */
	.errvld       =  0x2710, /* ERRLOGGER_ERRVLD_LOW */
	.errlog0_low  =  0x2720, /* ERRLOGGER_ERRLOG0_LOW */
	.errlog0_high =  0x2724, /* ERRLOGGER_ERRLOG0_HIGH */
	.errlog1_low  =  0x2728, /* ERRLOGGER_ERRLOG1_LOW */
	.errlog1_high =  0x272c, /* ERRLOGGER_ERRLOG1_HIGH */
	.errlog2_low  =  0x2730, /* ERRLOGGER_ERRLOG2_LOW */
	.errlog2_high =  0x2734, /* ERRLOGGER_ERRLOG2_HIGH */
	.errlog3_low  =  0x2738, /* ERRLOGGER_ERRLOG3_LOW */
	.errlog3_high =  0x273c, /* ERRLOGGER_ERRLOG3_HIGH */
};

static struct cam_cpas_hw_errata_wa_list cam170_cpas200_errata_wa_list = {
	.camnoc_flush_slave_pending_trans = {
		.enable = false,
		.data.reg_info = {
			.access_type = CAM_REG_TYPE_READ,
			.offset = 0x2100, /* SidebandManager_SenseIn0_Low */
			.mask = 0xE0000, /* Bits 17, 18, 19 */
			.value = 0, /* expected to be 0 */
		},
	},
};

static struct cam_camnoc_info cam170_cpas200_camnoc_info = {
	.specific = &cam_cpas_v170_200_camnoc_specific[0],
	.specific_size = ARRAY_SIZE(cam_cpas_v170_200_camnoc_specific),
	.irq_sbm = &cam_cpas_v170_200_irq_sbm,
	.irq_err = &cam_cpas_v170_200_irq_err[0],
	.irq_err_size = ARRAY_SIZE(cam_cpas_v170_200_irq_err),
	.err_logger = &cam170_cpas200_err_logger_offsets,
	.errata_wa_list = &cam170_cpas200_errata_wa_list,
};

#endif /* _CPASTOP_V170_200_H_ */
