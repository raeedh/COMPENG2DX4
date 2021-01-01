// NAME: RAEED HASSAN
// STUDENT #: 
// MACID: hassam41
// ASSIGNED BUS SPEED: 96 MHz
// ASSIGNED DISTANCE STATUS PIN: PN0
// ASSIGNED DISPLACEMENT STATUS PIN: PL1

#include <stdint.h>
#include "Systick.h"
#include "PLL.h"
#include "tm4c1294ncpdt.h"
#include "vl53l1x_api.h"
#include "uart.h"
#include "onboardLEDs.h"
#include "StepperMotor.h"
#include "I2CInit.h"
#include "OnboardButton.h"
#include "variables.h"

//



uint16_t	dev=0x52;

int status = 0;

volatile int IntCount;

int state = 0;

//capture values from VL53L1X for inspection
uint16_t debugArray[512];


int main(void) {
  uint8_t byteData, sensorState=0, myByteArray[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} , i=0;
  uint16_t wordData;
  uint8_t ToFSensor = 1; // 0=Left, 1=Center(default), 2=Right
  uint16_t SignalRate;
  uint16_t AmbientRate;
  uint16_t SpadNum; 
	uint8_t RangeStatus;
	uint8_t dataReady;
	uint16_t Distance;
  
	//initialize
	PLL_Init();	
	SysTick_Init();
	onboardLEDs_Init();
	I2C_Init();
	UART_Init();
	PortH_Init();
	
		// hello world!
	UART_printf("Program Begins\r\n");
	// int mynumber = 1;
	sprintf(printf_buffer,"2DX4 Project Code\r\n");
	UART_printf(printf_buffer);
	
	/* Those basic I2C read functions can be used to check your own I2C functions */
  status = VL53L1_RdByte(dev, 0x010F, &byteData);					// This is the model ID.  Expected returned value is 0xEA
  myByteArray[i++] = byteData;

  status = VL53L1_RdByte(dev, 0x0110, &byteData);					// This is the module type.  Expected returned value is 0xCC
  myByteArray[i++] = byteData;
	
	status = VL53L1_RdWord(dev, 0x010F, &wordData);
	status = VL53L1X_GetSensorId(dev, &wordData);

	sprintf(printf_buffer,"Model_ID=0x%x , Module_Type=0x%x\r\n",myByteArray[0],myByteArray[1]);
	UART_printf(printf_buffer);

	// Booting ToF chip
	while(sensorState==0){
		status = VL53L1X_BootState(dev, &sensorState);
		SysTick_Wait10ms(10);
  }
	FlashAllLEDs();
	UART_printf("ToF Chip Booted!\r\n");
 	UART_printf("One moment...\r\n");
	
	status = VL53L1X_ClearInterrupt(dev); /* clear interrupt has to be called to enable next interrupt*/

	
  /* This function must to be called to initialize the sensor with the default setting  */
  status = VL53L1X_SensorInit(dev);
	Status_Check("SensorInit", status);

	
  /* Optional functions to be used to change the main ranging parameters according the application requirements to get the best ranging performances */
	//  status = VL53L1X_SetDistanceMode(dev, 2); /* 1=short, 2=long */
	//  status = VL53L1X_SetTimingBudgetInMs(dev, 100); /* in ms possible values [20, 50, 100, 200, 500] */
	//  status = VL53L1X_SetInterMeasurementInMs(dev, 200); /* in ms, IM must be > = TB */

	UART_printf("Press GPIO J1 to start/stop data capture\r\n");
	
	ExternalButton_Init();
	
	while (1) {
		if (state == 1) { // If data capture is on
			status = VL53L1X_StartRanging(dev);   /* This function has to be called to enable the ranging */
			Status_Check("StartRanging", status);	
			
			for(int i = 0; i < 512; i++) {
	//  while(1){ /* read and display data */

			while (dataReady == 0){
				status = VL53L1X_CheckForDataReady(dev, &dataReady);
						//FlashLED3(1);
						FlashLED4(1);
						VL53L1_WaitMs(dev, 5);
			}

			dataReady = 0;
			status = VL53L1X_GetRangeStatus(dev, &RangeStatus);
			status = VL53L1X_GetDistance(dev, &Distance);
			FlashLED4(1);

				debugArray[i] = Distance;
	//	  status = VL53L1X_GetSignalRate(dev, &SignalRate);
	//	  status = VL53L1X_GetAmbientRate(dev, &AmbientRate);
	//	  status = VL53L1X_GetSpadNb(dev, &SpadNum);
			status = VL53L1X_ClearInterrupt(dev); /* clear interrupt has to be called to enable next interrupt*/
	//    sprintf(printf_buffer,"%u, %u, %u, %u, %u\r\n", RangeStatus, Distance, SignalRate, AmbientRate,SpadNum);
	//		double angle = (i/512.0)*360.0; 
			sprintf(printf_buffer,"%u\r\n", Distance); 
				UART_printf(printf_buffer); // transmits distance measurement
	//		SysTick_Wait10ms(5);
			rotate(2,1); // rotates stepper motor 1 step
			if (state == 0) { // If interrupt triggered, exits loop
				break; }
		}
		
		VL53L1X_StopRanging(dev);
		UART_printf("Data capture has stopped, press the button to restart data capture\r\n");
		state = 0;
		}
	
		if (state == 0) { // If data capture is off
			while (state == 0) {
				UART_printf("Press GPIO J1 to start data capture\r\n"); 
				SysTick_Wait10ms(500);
				if (state == 1) break; // If interrupt triggered, exits loop to enter data capture loop
			}
		}
	}
}

