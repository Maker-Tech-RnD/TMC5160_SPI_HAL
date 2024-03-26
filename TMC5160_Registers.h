/*
 * REg.h
 *
 *  Created on: Mar 25, 2024
 *      Author: Ilya
 */

#ifndef LIB_TMC5160_STM32_HAL_TMC5160_REGISTERS_H_
#define LIB_TMC5160_STM32_HAL_TMC5160_REGISTERS_H_
#include <TMC5160_Registers.h>
#include "main.h"

//Register's addresses
typedef enum{
	//GENERAL CONFIGURATION REGISTERS (0X00…0X0F)
	GCONF			= 0x00,	// RW
	GSTAT			= 0x01,	// R + WC
	IFCNT			= 0x02, // R
	SLAVECONF		= 0x03, // W
	IOIN			= 0x04, // R
	OUTPUT			= 0x04, // W --- In UART mode, SDO_CFG0 is an output.
	X_COMPARE		= 0x05, // W
	OTP_PROG		= 0x06, // W
	OTP_READ		= 0x07, // R
	FACTORY_CONF	= 0x08, // RW
	SHORT_CONF		= 0x09, // W
	DRV_CONF		= 0x0A, // W
	GLOBALSCALER	= 0x0B, // W
	OFFSET_READ		= 0x0C, // R
	//VELOCITY DEPENDENT DRIVER FEATURE CONTROL REGISTER SET (0X10…0X1F)
	IHOLD_IRUN		= 0x10, // W
	TPOWERDOWN		= 0x11, // W
	TSTEP			= 0x12, // R
	TPWMTHRS		= 0x13, // W
	TCOOLTHRS		= 0x14, // W
	THIGH			= 0x15, // W
	//RAMP GENERATOR MOTION CONTROL REGISTER SET (0X20…0X2D)
	RAMPMODE		= 0x20, // RW
	XACTUAL			= 0x21, // RW
	VACTUAL			= 0x22, // R
	VSTART			= 0x23, // W
	A1				= 0x24, // W
	V1				= 0x25, // W
	AMAX			= 0x26, // W
	VMAX			= 0x27, // W
	DMAX			= 0x28, // W
	D1				= 0x2A, // W
	VSTOP			= 0x2B, // W
	TZEROWAIT		= 0x2C, // W
	XTARGET			= 0x2D, // RW
	//RAMP GENERATOR DRIVER FEATURE CONTROL REGISTER SET (0X30…0X36)
	VDCMIN			= 0x33, // W
	SW_MODE			= 0x34, // RW
	RAMP_STAT		= 0x35, // R + WC
	XLATCH			= 0x36, // R
	//ENCODER REGISTER SET (0X38…0X3C)
	ENCMODE			= 0x38, // RW
	X_ENC			= 0x39, // RW
	ENC_CONST		= 0x3A, // W
	ENC_STATUS		= 0x3B, // R + WC
	ENC_LATCH		= 0x3C, // R
	ENC_DEVIATION	= 0x3D, // W
	//DRIVER REGISTER SET (0X6C…0X7F)
	CHOPCONF		= 0x6C, // RW
	COOLCONF		= 0x6D, // W
	DCCTRL			= 0x6E, // W
	DRV_STATUS		= 0x6F, // R
	PWMCONF			= 0x70, // W
	PWM_SCALE		= 0x71, // R
	PWM_AUTO		= 0x72, // R
	LOST_STEPS		= 0x73, // R
}TMC5160_Reg_Adress;

union Register_GCONF
    {
        struct
        {
        	    uint32_t recalibrate			: 1;
				uint32_t faststandstill		 	: 1;
				uint32_t en_pwm_mode	 		: 1;
				uint32_t multistep_filt			: 1;
				uint32_t shaft					: 1;
				uint32_t diag0_error			: 1;
				uint32_t diag0_otpw				: 1;
				uint32_t diag0_stall		 	: 1;
				uint32_t diag1_stall 			: 1;
				uint32_t diag1_index			: 1;
				uint32_t diag1_onstate 			: 1;
				uint32_t diag1_steps_skipped	: 1;
				uint32_t diag0_int_pushpull		: 1;
				uint32_t diag1_poscomp_pushpull	: 1;
				uint32_t small_hysteresis	 	: 1;
				uint32_t stop_enable			: 1;
				uint32_t direct_mode			: 1;
				uint32_t test_mode				: 1;
				uint32_t no_use					: 14;
        } REG;
        uint32_t UINT32;
    };

union Register_CHOPCONF
    {
        struct
        {
        	    uint32_t toff					: 4;
				uint32_t hstrt				 	: 3;
				uint32_t hend					: 4;
				uint32_t fd3					: 1;
				uint32_t disfdcc				: 1;
				uint32_t reserved				: 1;
				uint32_t chm					: 1;
				uint32_t tbl				 	: 2;
				uint32_t reserved2				: 1;
				uint32_t vhighfs				: 1;
				uint32_t vhighchm 				: 1;
				uint32_t tpfd					: 4;
				uint32_t mres					: 4;
				uint32_t intpol					: 1;
				uint32_t dedge				 	: 1;
				uint32_t diss2g					: 1;
				uint32_t diss2vs				: 1;
        } REG;

        struct
		{
				uint32_t toff					: 4;
				uint32_t tfd				 	: 3;
				uint32_t offfset				: 4;
				uint32_t fd3					: 1;
				uint32_t disfdcc				: 1;
				uint32_t reserved				: 1;
				uint32_t chm					: 1;
				uint32_t tbl				 	: 2;
				uint32_t reserved2				: 1;
				uint32_t vhighfs				: 1;
				uint32_t vhighchm 				: 1;
				uint32_t tpfd					: 4;
				uint32_t mres					: 4;
				uint32_t intpol					: 1;
				uint32_t dedge				 	: 1;
				uint32_t diss2g					: 1;
				uint32_t diss2vs				: 1;
		} REG_CHM1;
        uint32_t UINT32;
    };

union Register_IHOLD_IRUN
    {
        struct
        {
        	    uint32_t IHOLD					: 5;
        	    uint32_t IRUN                   : 5;
        	    uint32_t IHOLDDELAY				: 4;
				uint32_t no_use					: 12;
        } REG;
        uint32_t UINT32;
    };

union Register_DRV_STATUS
    {
        struct
        {
        	    uint32_t SG_RESULT 				: 10;
				uint32_t reserved			 	: 2;
				uint32_t s2vsa			 		: 1;
				uint32_t s2vsb					: 1;
				uint32_t stealth				: 1;
				uint32_t fsactive				: 1;
				uint32_t CS_ACTUAL 				: 5;
				uint32_t reserved2			 	: 3;
				uint32_t StallGuard				: 1;
				uint32_t ot						: 1;
				uint32_t otpw		 			: 1;
				uint32_t s2ga					: 1;
				uint32_t s2gb					: 1;
				uint32_t ola					: 1;
				uint32_t olb				 	: 1;
				uint32_t stst					: 1;
        } REG;
        uint32_t UINT32;
    };




#endif /* LIB_TMC5160_STM32_HAL_TMC5160_REGISTERS_H_ */
