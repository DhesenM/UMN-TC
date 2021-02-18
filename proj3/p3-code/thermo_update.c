#include <stdio.h>
#include "thermo.h"

int set_temp_from_ports(temp_t *temp){
	if (THERMO_SENSOR_PORT > 64000 || THERMO_SENSOR_PORT < 0){  
		return 1;                         // return 1 if out of bound
	}

	// initialize and set sensor and degree
	int sensor = -500;
	int degree = THERMO_SENSOR_PORT / 64; // 64 units of value in the sensor are 0.1 degree
	sensor = sensor + degree;             // starting from -50.0 deg C and plus degrees

	if (THERMO_SENSOR_PORT % 64 > 31){
		sensor += 1;
	}

	// converting from Celsius to Fahrenheit if needed
	if (THERMO_STATUS_PORT % 2 == 1){     // convert if the last digit of THERMO_STATUS_PORT is 1
		sensor = (sensor * 9) / 5 + 320;
	}

	// set fields of temp
	temp -> tenths_degrees = sensor;

	// set is_fahrenheit 0 if the last digit of THERMO_STATUS_PORT is 0
	if (THERMO_STATUS_PORT % 2 == 0){     
		temp -> is_fahrenheit = 0;
	}
	// set is_fahrenheit 1 if the last digit of THERMO_STATUS_PORT is 1
	else{ 
		temp -> is_fahrenheit = 1;
	}
	return 0;                             // return 0 if succeeded
}

// sets global array 0-9
int digit_arr[10] = {0b1111110,0b0001100,0b0110111,0b0011111,0b1001101,
                     0b1011011,0b1111011,0b0001110,0b1111111,0b1011111};

int set_display_from_temp(temp_t temp, int *display){
	if(temp.is_fahrenheit == 1){        // return 1 if out of bound
		if (temp.tenths_degrees < -1220 || temp.tenths_degrees > 1220){
			return 1;
		}
	}
	else if (temp.is_fahrenheit == 0){  // return 1 if out of bound
		if(temp.tenths_degrees < -500 || temp.tenths_degrees > 500){ 
			return 1;
		}
	}
	else{                               // return 1 if neither f or c
		return 1;
	}

	// initialize display to 0 and sensor to the degree in temp
	int temp_display = 0;
	int temp_sensor = temp.tenths_degrees;

	if (temp.tenths_degrees < 0){       // set the negative sign if < 0
		temp_display |= (0b1 << 7);
		temp_sensor = temp_sensor * -1;
	}

	// initialize local variables to determine each digits of the degree
	int temp_tenths = temp_sensor % 10; 
	temp_sensor /= 10;
	int temp_ones = temp_sensor % 10;
	temp_sensor /= 10;
	int temp_tens = temp_sensor % 10;
	temp_sensor /= 10;
	int temp_hundreds = temp_sensor % 10;

	// index the hundreds digit if has it and shift left by 7 bits
	if (temp_hundreds != 0){
		temp_display |= digit_arr[temp_hundreds];
		temp_display = temp_display << 7;
	}
	
	// index the tens digit if has it and shift left by 7 bits
	if (temp_hundreds != 0 || temp_tens != 0){
		temp_display |= digit_arr[temp_tens];
		temp_display = temp_display << 7;
	}

	// index the ones digit if has it and shift left by 7 bits then index the tenths digit
	temp_display |= digit_arr[temp_ones];
	temp_display = temp_display << 7;
	temp_display |= digit_arr[temp_tenths];

	// index the degree sign Fahrenheit or Celsius
	if (temp.is_fahrenheit == 0){
		temp_display |= (0b1 << 28);
	}
	else{
		temp_display |= (0b1 << 29);
	}

	*display = temp_display;   // replace the given display
	return 0;                  // return 0 if succeeded
}

int thermo_update()
// Called to update the thermometer display.  Makes use of
// set_temp_from_ports() and set_display_from_temp() to access
// temperature sensor then set the display. Checks these functions and
// if they indicate an error, makes not changes to the display.
// Otherwise modifies THERMO_DISPLAY_PORT to set the display.
// 
// CONSTRAINT: Does not allocate any heap memory as malloc() is NOT
// available on the target microcontroller.  Uses stack and global
// memory only.
{
	// initialize temp, display, and error
	temp_t temp = {};
	int display = 0;
	int error = set_temp_from_ports(&temp);
	if (error == 1){   // return 1 if set_temp_from_ports returns 1 
		return 1;
	}
	error = set_display_from_temp(temp,&display);
	if (error == 1){   // return 1 if set_display_from_temp returns 1
		return 1;
	}
	// change the display if there are no errors and return 0
	THERMO_DISPLAY_PORT = display;   
	return 0;
}
