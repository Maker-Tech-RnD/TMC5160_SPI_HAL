/* Includes	------------------------------------------------------*/
#include "TMC5160.h"



/* Functions -----------------------------------------------------*/
/**
 * @brief  Default configuration of the TMC5160 Driver
 * 		   Use it in main.c before start the movement
 * 
 * @param  htmc Pointer to a TMC5160_HandleTypeDef structure, that contains
 * 			   the configuration configuration information for the specified TMC5160 Driver
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef TMC5160_Default_Configuration(TMC5160_HandleTypeDef *htmc){

	HAL_StatusTypeDef result = TMC5160_Default_Characteristics_Of_Condition(htmc);
	if(result != HAL_OK){
		return result;
	}

	result = TMC5160_Default_Characteristics_Of_Drive(htmc);
		if(result != HAL_OK){
			return result;
		}
	return result;
}



/**
 * @brief  Set the position of the rotor: 1 step according to configuration 
 * 		   Use it in main.c to move the motor
 * 
 * @param  htmc pointer to a TMC5160_HandleTypeDef structure, that contains
 * 			   the configuration configuration information for the specified TMC5160 Driver 
 * @param  steps total amount of the steps. The maximum possible displacement is +/- ((2^31)-1).
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef TMC5160_SetPosition(TMC5160_HandleTypeDef *htmc, int32_t steps){
	if(steps > 2147483648){
		return HAL_ERROR;
	}
	HAL_StatusTypeDef result;
	result = TMC5160_WriteRegister(htmc, XTARGET, steps);
	if(result != HAL_OK){
		return result;
	}

	return HAL_OK;
}



/**
 * @brief  Get the position of the rotor: 1 step according to configuration 
 * 		   Use it in main.c to know about position of the motor
 * @param  htmc Pointer to a TMC5160_HandleTypeDef structure, that contains
 * 			   the configuration configuration information for the specified TMC5160 Driver
 * @param  data The value of rotor's position
 * @return uint32_t 
 */
uint32_t TMC5160_GetPosition(TMC5160_HandleTypeDef *htmc, uint32_t *data){
	HAL_StatusTypeDef result = TMC5160_ReadRegister(htmc, XACTUAL, *data);
		if(result != HAL_OK){
			return result;
		}
	return *data;
}



/**
 * @brief  write a value to the register. 
 * 
 * @param  htmc 
 * @param  reg_addr 
 * @param  data_of_register 
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef TMC5160_WriteRegister(TMC5160_HandleTypeDef *htmc, TMC5160_Reg_Adress reg_addr, uint32_t data_of_register){
	//reg_addr + bit_of_flow - first bit show, that it's write_function or read_function -> In read case it's number: 1
	reg_addr |= 0b10000000;
	uint8_t buff[5];
	buff[0] = reg_addr;
	//Transform in uint32_t variable and receive buff
	divide_uint32_t_and_pacckage_in_array(data_of_register, buff);
	//The default configuration of STM's SPI transmit only 8 bits due to CS_Pin, so we initialize a CS_Pin manually
	HAL_GPIO_WritePin(htmc->GPIOx,htmc->CS, GPIO_PIN_RESET);
	HAL_StatusTypeDef result = HAL_SPI_Transmit(htmc->spi, buff, 5, 100);
	HAL_GPIO_WritePin(htmc->GPIOx,htmc->CS, GPIO_PIN_SET);
	return result;
}



/**
 * @brief  read a value from the register.
 * 
 * @param  htmc pointer to a TMC5160_HandleTypeDef structure, that contains
 * 			   the configuration configuration information for the specified TMC5160 Driver
 * @param  reg_addr address of the register
 * @param  data_of_register value, that wil be transmitted
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef TMC5160_ReadRegister(TMC5160_HandleTypeDef *htmc, TMC5160_Reg_Adress reg_addr, uint32_t data_of_register){
	//reg_addr + bit_of_flow - first bit show, that it's write_function or read_function -> In read case it's number: 0. We didn't have to do anything, just left.
	uint8_t buff[5];
	buff[0] = reg_addr;
	// sending buffer and fill it, and after that transform in uint32_t variable
	//The default configuration of STM's SPI transmit only 8 bits due to CS_Pin, so we initialize a CS_Pin manually
	HAL_GPIO_WritePin(htmc->GPIOx,htmc->CS, GPIO_PIN_RESET);
	HAL_SPI_Transmit(htmc->spi, buff, 5, 100);
	HAL_StatusTypeDef result = HAL_SPI_TransmitReceive(htmc->spi, buff, buff, 5, 100);
	HAL_GPIO_WritePin(htmc->GPIOx,htmc->CS, GPIO_PIN_SET);
	data_of_register = parsing_data(buff);
	return result;
}



/**
 * @brief  Set the default values of the speed, acceleration and write them to all neccesery registers
 * 
 * @param  htmc Pointer to a TMC5160_HandleTypeDef structure, that contains
 * 			   the configuration configuration information for the specified TMC5160 Driver
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef TMC5160_Default_Characteristics_Of_Drive (TMC5160_HandleTypeDef *htmc){
	// Set the default values of the speed, acceleration
	htmc->configuration_velosity.max_speed 				= 200000;
	htmc->configuration_velosity.stop_speed 			= 10;
	htmc->configuration_velosity.first_speed 			= 50000;
	htmc->configuration_velosity.first_acceleration 	= 1000;
	htmc->configuration_velosity.max_deceleration 		= 700;
	htmc->configuration_velosity.second_deceleration 	= 1400;
	htmc->configuration_velosity.max_acceleration 		= 500;
	htmc->configuration_velosity.ramp_mode 				= Positioning;
	// Write values to all neccesery registers
	HAL_StatusTypeDef result;
	result = TMC5160_WriteRegister(htmc, A1, htmc->configuration_velosity.first_acceleration);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, V1, htmc->configuration_velosity.first_speed);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, AMAX, htmc->configuration_velosity.max_acceleration);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, VMAX, htmc->configuration_velosity.max_speed);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, DMAX, htmc->configuration_velosity.max_deceleration);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, D1, htmc->configuration_velosity.second_deceleration);
	if(result != HAL_OK){
				return result;
	}
	result = TMC5160_WriteRegister(htmc, VSTOP, htmc->configuration_velosity.stop_speed);
	if(result != HAL_OK){
				return result;
	}
	result = TMC5160_WriteRegister(htmc, RAMPMODE, htmc->configuration_velosity.ramp_mode);
	if(result != HAL_OK){
				return result;
	}
	return HAL_OK;
}



/**
 * @brief  Set the default values of configuration of the TMC5160 Driver and write them to all neccesery registers
 * 
 * @param  htmc Pointer to a TMC5160_HandleTypeDef structure, that contains
 * 			   the configuration configuration information for the specified TMC5160 Driver
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef TMC5160_Default_Characteristics_Of_Condition (TMC5160_HandleTypeDef *htmc){
	// Fill the bits of the registers in TMC5160_HandleTypeDef structure(from Getting started of the Datasheet's page 116)
	// CHOPCONF
	htmc->configuration.CHOPCONF.REG.toff					= 3;
	htmc->configuration.CHOPCONF.REG.hstrt				 	= 4;
	htmc->configuration.CHOPCONF.REG.hend					= 1;
	htmc->configuration.CHOPCONF.REG.fd3					= 0;
	htmc->configuration.CHOPCONF.REG.disfdcc				= 0;
	htmc->configuration.CHOPCONF.REG.reserved				= 0;
	htmc->configuration.CHOPCONF.REG.chm					= 0;
	htmc->configuration.CHOPCONF.REG.tbl				 	= 2;
	htmc->configuration.CHOPCONF.REG.reserved2				= 0;
	htmc->configuration.CHOPCONF.REG.vhighfs				= 0;
	htmc->configuration.CHOPCONF.REG.vhighchm 				= 0;
	htmc->configuration.CHOPCONF.REG.tpfd					= 0;
	htmc->configuration.CHOPCONF.REG.mres					= 0;
	htmc->configuration.CHOPCONF.REG.intpol					= 0;
	htmc->configuration.CHOPCONF.REG.dedge				 	= 0;
	htmc->configuration.CHOPCONF.REG.diss2g					= 0;
	htmc->configuration.CHOPCONF.REG.diss2vs				= 0;
	// IHOLD_IRUN
	htmc->configuration.IHOLD_IRUN.REG.IHOLD				= 10;
	htmc->configuration.IHOLD_IRUN.REG.IRUN                 = 31;
	htmc->configuration.IHOLD_IRUN.REG.IHOLDDELAY			= 6;
	htmc->configuration.IHOLD_IRUN.REG.no_use				= 0;
	// TPOWERDOWN
	htmc->configuration.TPOWER_DOWN 						= 10;
	// EN_PWM_MODE
	htmc->configuration.GCONF.REG.recalibrate 				= 0;
	htmc->configuration.GCONF.REG.faststandstill 			= 0;
	htmc->configuration.GCONF.REG.en_pwm_mode				= 1;
	htmc->configuration.GCONF.REG.multistep_filt			= 0;
	htmc->configuration.GCONF.REG.shaft						= 0;
	htmc->configuration.GCONF.REG.diag0_error				= 0;
	htmc->configuration.GCONF.REG.diag0_otpw				= 0;
	htmc->configuration.GCONF.REG.diag0_stall		 		= 0;
	htmc->configuration.GCONF.REG.diag1_stall 				= 0;
	htmc->configuration.GCONF.REG.diag1_index				= 0;
	htmc->configuration.GCONF.REG.diag1_onstate 			= 0;
	htmc->configuration.GCONF.REG.diag1_steps_skipped		= 0;
	htmc->configuration.GCONF.REG.diag0_int_pushpull		= 0;
	htmc->configuration.GCONF.REG.diag1_poscomp_pushpull	= 0;
	htmc->configuration.GCONF.REG.small_hysteresis	 		= 0;
	htmc->configuration.GCONF.REG.stop_enable				= 0;
	htmc->configuration.GCONF.REG.direct_mode				= 0;
	htmc->configuration.GCONF.REG.test_mode					= 0;
	htmc->configuration.GCONF.REG.no_use					= 0;
	// TPWM_THRS
	htmc->configuration.TPWMTHRS 							= 500;
	// Write to all necessary registers
	HAL_StatusTypeDef result;
	/**
	 * @brief The "easy way" that sends the first parcel of data, 
	 * 		  since the first parcel is not sent once (Alert: IT'S a BUG!!!)
	 * 
	 */
	result = TMC5160_WriteRegister(htmc, CHOPCONF, htmc->configuration.CHOPCONF.UINT32);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, CHOPCONF, htmc->configuration.CHOPCONF.UINT32);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, IHOLD_IRUN, htmc->configuration.IHOLD_IRUN.UINT32);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, TPOWERDOWN, htmc->configuration.TPOWER_DOWN);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, GCONF, htmc->configuration.GCONF.UINT32);
	if(result != HAL_OK){
		return result;
	}
	result = TMC5160_WriteRegister(htmc, TPWMTHRS, htmc->configuration.TPWMTHRS);
	if(result != HAL_OK){
		return result;
	}
	return result;
}