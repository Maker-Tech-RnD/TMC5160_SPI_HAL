/* Includes	-----------------------------------------------------*/
#include "TMC5160.h"



/* Functions -----------------------------------------------------*/
/**
 * @brief Transform uint32_t value to array
 * 
 * @param value 
 * @param data 
 */
void  divide_uint32_t_and_pacckage_in_array(uint32_t value, uint8_t *data){
	data[1]  = (uint8_t)(value >> 24);
	data[2]  = (uint8_t)(value >> 16);
	data[3]  = (uint8_t)(value >> 8);
	data[4]  = (uint8_t)value;
}



/**
 * @brief 	Transform array to uint32_t value
 * 
 * @param 	data Buffer, that will be parsed
 * @return 	uint32_t 
 */
uint32_t parsing_data( uint8_t data[]) {
	// Initialization parts of the message
	uint32_t third_byte = (uint32_t)data[1] << 24;
	uint32_t second_byte = (uint32_t)data[2] << 16;
	uint32_t first_byte = (uint32_t)data[3] << 8;
	uint32_t zero_byte = (uint32_t)data[4];
	// Return the uint32_t variable
	return (zero_byte | first_byte | second_byte | third_byte);
}
