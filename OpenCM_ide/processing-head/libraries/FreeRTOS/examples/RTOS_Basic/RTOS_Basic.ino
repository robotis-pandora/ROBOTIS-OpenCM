/*
 *******************************************************************************
 *  MapleFreeRTOS Basic Example (CM-900 version)
 *******************************************************************************
 *  The program operates 2 tasks, blinking LED and printing "hello world" via SerialUSB
 *  To operate task, just use 2 API, xTaskCreat() and vTaskStartScheduler()    
 *  This is based on MapleFreeRTOS. you can learn it from www.freertos.org    
 *      
 *  Made by ROBOTIS 2012-12-29 Sangmin lee  
 *    
 *    
 *******************************************************************************
 */

//It must be needed to use FreeRTOS
#include <MapleFreeRTOS.h>


void setup(){
  //configMINIMAL_STACK_SIZE is stack size in which insert your task code
  //so it is expressed in bytes. 
  xTaskCreate( vLedTask, ( signed char * ) "Led", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
  xTaskCreate( vUartTask, ( signed char * ) "Uart", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
  //start run tasks
  vTaskStartScheduler();
}
void loop(){
}

void vLedTask( void *pvParameters )
{
  for( ;; ){
    toggleLED();
    delay(100);
  }
}

void vUartTask( void *pvParameters )
{

  SerialUSB.begin();
  for( ;; )
  {
    SerialUSB.println("Hello World");
    delay(100);
  }
}
