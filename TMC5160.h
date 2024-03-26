/**
 * @file        TMC5160_Registers.h
 * @author      Ilya (https://github.com/EmbededIlya)
 * @brief 		
 * @version 	0.1
 * @date 		2024-03-26
 * 
 * @copyright   Copyright (c) 2024
 * 
 */



#ifndef LIBS_TMC5160_STM32_HAL_TMC5160_STM32_HAL_H_
#define LIBS_TMC5160_STM32_HAL_TMC5160_STM32_HAL_H_



/* Includes	-----------------------------------------------------*/
#include "TMC5160_Registers.h"
#include "main.h"



/* Macros difines -----------------------------------------------*/
//Filter setting macros
#define TMC5160_FIRST_ACCELERATION_LIMIT     0xFFFF 	// 2^16 - 1
#define TMC5160_FIRST_VELOCITY_LIMIT         0xFFFFF  	// 2^20 - 1
#define TMC5160_MAX_ACCELERATION_LIMIT       0xFFFF 	// 2^16 - 1

/*Attention for BELOW: Do not set 0 in positioning mode, even if V1=0!*/
#define TMC5160_SECOND_DECELERATION_LIMIT    0xFFFF  	// 2^16 - 1
#define TMC5160_MAX_DECELERATION_LIMIT       0xFFFF  	// 2^16 - 1

/*Attention BELOW: Do not set 0 in positioning mode, minimum 10 recommend!*/
#define TMC5160_STOP_VELOCITY_LIMIT          0x3FFFF  	// 2^18 - 1
#define TMC5160_MAX_VELOCITY_LIMIT           8388096  	// 2^23 - 512


/* Typedefs	-----------------------------------------------------*/
// RampMode
typedef enum  {
	/* using all A, D and V parameters */
	Positioning = 0,
	/* Velocity mode to positive VMAX (using AMAX acceleration) */
	VelocityPositive = 1,
	/* Velocity mode to negative VMAX (using AMAX acceleration) */
	VelocityNegative = 2,
	/* velocity remains unchanged, unless stop event occurs */
	Hold = 3,
}RampModes;


// Structs for TMC5160
typedef struct{
	// Configuration the mode of the Driver
	union Register_CHOPCONF 	CHOPCONF;
	union Register_IHOLD_IRUN 	IHOLD_IRUN;
	union Register_DRV_STATUS 	DRV_STATUS;
	uint32_t 					TPOWER_DOWN;
	uint32_t 					TSTEP;
	uint32_t 					TPWMTHRS;
	uint32_t 					TCOOLTHRS;
	uint32_t 					THIGH;
	union Register_GCONF 		GCONF;
	uint32_t GLOBALSCALER;
}TMC5160_RegisterOfConfiguration_HandleTypeDef;



typedef struct{
	// Configuration Acceleration and velocity
	uint32_t max_speed;
	uint32_t stop_speed;
	uint32_t first_speed;
	uint32_t first_acceleration;
	uint32_t max_deceleration;
	uint32_t second_deceleration;
	uint32_t max_acceleration;
	uint32_t ramp_mode;
}TMC5160_ConfigurationOfVelosity_HandleTypeDef;



typedef struct{
	// HAL configuration of the MCU
	SPI_HandleTypeDef        *spi;
	GPIO_TypeDef           *GPIOx;
	uint16_t				   CS;
	// Configuration Mode
	TMC5160_RegisterOfConfiguration_HandleTypeDef configuration;
	// Configuration Acceleration and velocity
	TMC5160_ConfigurationOfVelosity_HandleTypeDef configuration_velosity;
}TMC5160_HandleTypeDef;



/* Functions -----------------------------------------------------*/
// Default configuration
HAL_StatusTypeDef TMC5160_Default_Configuration(TMC5160_HandleTypeDef *htmc);
HAL_StatusTypeDef TMC5160_Default_Characteristics_Of_Condition (TMC5160_HandleTypeDef *htmc);
HAL_StatusTypeDef TMC5160_Default_Characteristics_Of_Drive (TMC5160_HandleTypeDef *htmc);



// Motor movement
uint32_t 		  TMC5160_GetPosition(TMC5160_HandleTypeDef *htmc, uint32_t *data);
HAL_StatusTypeDef TMC5160_SetPosition(TMC5160_HandleTypeDef *htmc, int32_t steps);

// R/W functions
HAL_StatusTypeDef TMC5160_WriteRegister(TMC5160_HandleTypeDef *htmc, TMC5160_Reg_Adress reg_addr, uint32_t data_of_register);
HAL_StatusTypeDef TMC5160_ReadRegister(TMC5160_HandleTypeDef *htmc, TMC5160_Reg_Adress reg_addr, uint32_t data_of_register);



// TMC5160_Transform_Functions
void              divide_uint32_t_and_pacckage_in_array(uint32_t value, uint8_t *data);
uint32_t          parsing_data( uint8_t data[]);



#endif /* LIBS_TMC5160_STM32_HAL_TMC5160_STM32_HAL_H_ */
