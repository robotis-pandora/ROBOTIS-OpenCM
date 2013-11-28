/*
 * OLLO.cpp
 *
 *  Created on: 2013. 5. 30.
 *      Author: ROBOTIS,.LTD.
 */

#include "OLLO.h"

#include "Arduino-compatibles.h"

OLLO::OLLO() {
	// TODO Auto-generated constructor stub

}

OLLO::~OLLO() {
	// TODO Auto-generated destructor stub
}

void OLLO::begin(int devNum){
	if( devNum == 0 ){
			return;
	}
	mMot_plus = 0;
	mMot_minus = 0;
	switch(devNum){
	case 1:
		pinMode(PORT1_SIG1, OUTPUT); //RED  (right)
		pinMode(PORT1_SIG2, OUTPUT); //BLUE (left)
		pinMode(PORT1_ADC, INPUT_ANALOG); //ADC input
		break;
	case 2:
		pinMode(PORT2_SIG1, OUTPUT); //RED  (right)
		pinMode(PORT2_SIG2, OUTPUT); //BLUE (left)
		pinMode(PORT2_ADC, INPUT_ANALOG);//ADC input
		break;
	case 3:
		pinMode(PORT3_SIG1, OUTPUT); //RED  (right)
		pinMode(PORT3_SIG2, OUTPUT); //BLUE (left)
		pinMode(PORT3_ADC, INPUT_ANALOG);//ADC input
		break;
	case 4:
		pinMode(PORT4_SIG1, OUTPUT); //RED  (right)
		pinMode(PORT4_SIG2, OUTPUT); //BLUE (left)
		pinMode(PORT4_ADC, INPUT_ANALOG);//ADC input
		break;
	default:
		break;
	}
}
void OLLO::begin(int devNum, int device_index){ //MAGNETIC SENSOR, Button, IR Sensor, and etc...
	if( devNum == 0 ){
			return;
	}
	mMot_plus = 0;
	mMot_minus = 0;
	switch(devNum){
	case 1:
		if(device_index == MAGNETIC_SENSOR){
			pinMode(PORT1_ADC, INPUT); //digital input
		}else if(device_index == TOUCH_SENSOR){
			pinMode(PORT1_ADC, INPUT_PULLUP);
		}else{
			pinMode(PORT1_ADC, INPUT_ANALOG); //ADC input
		}

		pinMode(PORT1_SIG1, OUTPUT); //SIG1
		pinMode(PORT1_SIG2, OUTPUT); //SIG2
		if(device_index == IR_SENSOR ){
			digitalWrite(PORT1_SIG1,LOW); //SIG1 set to LOW
			digitalWrite(PORT1_SIG2,LOW); //SIG2 set to LOW
		}
		break;
	case 2:
		if(device_index == MAGNETIC_SENSOR){
			pinMode(PORT2_ADC, INPUT); //digital input
		}else if(device_index == TOUCH_SENSOR){
			pinMode(PORT2_ADC, INPUT_PULLUP);
		}else{
			pinMode(PORT2_ADC, INPUT_ANALOG);//ADC input
		}


		pinMode(PORT2_SIG1, OUTPUT); //SIG1
		pinMode(PORT2_SIG2, OUTPUT); //SIG2
		if(device_index == IR_SENSOR ){
			digitalWrite(PORT2_SIG1,LOW); //set to LOW
			digitalWrite(PORT2_SIG2,LOW); //set to LOW
		}
		break;
	case 3:
		if(device_index == MAGNETIC_SENSOR){
			pinMode(PORT3_ADC, INPUT); //digital input
		}else if(device_index == TOUCH_SENSOR){
			pinMode(PORT3_ADC, INPUT_PULLUP);
		}else{
			pinMode(PORT3_ADC, INPUT_ANALOG);//ADC input
		}


		pinMode(PORT3_SIG1, OUTPUT); //SIG1
		pinMode(PORT3_SIG2, OUTPUT); //SIG2
		if(device_index == IR_SENSOR ){
			digitalWrite(PORT3_SIG1,LOW); //set SIG1 to LOW
			digitalWrite(PORT3_SIG2,LOW); //set SIG2 to LOW
		}
		break;
	case 4:
		if(device_index == MAGNETIC_SENSOR){
			pinMode(PORT4_ADC, INPUT); //digital input
		}else if(device_index == TOUCH_SENSOR){
			pinMode(PORT4_ADC, INPUT_PULLUP);
		}else{
			pinMode(PORT4_ADC, INPUT_ANALOG);//ADC input
		}


		pinMode(PORT4_SIG1, OUTPUT); //SIG1
		pinMode(PORT4_SIG2, OUTPUT); //SIG2
		if(device_index == IR_SENSOR ){
			digitalWrite(PORT4_SIG1,LOW); //set SIG1 to LOW
			digitalWrite(PORT4_SIG2,LOW); //set SIG2 to LOW
		}
		break;
	default:
		break;
	}
}

void OLLO::begin(int devNum, int device_index, voidFuncPtr handler){ //Button with handler function.
	if( devNum == 0 ){
		return;
	}
	switch(devNum){
	case 1:
		if(device_index == TOUCH_SENSOR){
			pinMode(PORT1_ADC, INPUT_PULLUP);
			attachInterrupt(PORT1_ADC,handler, RISING);
		}
		break;
	case 2:
		if(device_index == TOUCH_SENSOR){
			pinMode(PORT2_ADC, INPUT_PULLUP);
			attachInterrupt(PORT2_ADC,handler, RISING);
		}
		break;
	case 3:
		if(device_index == TOUCH_SENSOR){
			pinMode(PORT3_ADC, INPUT_PULLUP);
			attachInterrupt(PORT3_ADC,handler, RISING);
		}
		break;
	case 4:
		if(device_index == TOUCH_SENSOR){
			pinMode(PORT4_ADC, INPUT_PULLUP);
			attachInterrupt(PORT4_ADC,handler, RISING);
		}
		break;
	default:
		break;
	}
}


int OLLO::read(int devNum){ // general sensor reading method
	if( devNum == 0 ){
			return 0;
	}
	switch(devNum){
	case 1:
		return (int)analogRead(PORT1_ADC);
	case 2:
		return (int)analogRead(PORT2_ADC);
	case 3:
		return (int)analogRead(PORT3_ADC);
	case 4:
		return (int)analogRead(PORT4_ADC);
	default:
		return 0;
	}

}
int OLLO::read(int devNum, int device_index){ // IR SENSOR, Button, MAGNETIC SENSOR, and etc...
	int adcValue = 0;
	if( devNum == 0 ){
		return 0;
	}
	switch(devNum){
	case 1:
		if(device_index == IR_SENSOR){
			digitalWrite(PORT1_SIG2, HIGH);
			delayMicroseconds(15);
			adcValue = analogRead(PORT1_ADC);
			digitalWrite(PORT1_SIG2, LOW);
			return adcValue;
		}else if(device_index == MAGNETIC_SENSOR || device_index == TOUCH_SENSOR  ){
			return digitalRead(PORT1_ADC);
		}else{
			return (int)analogRead(PORT1_ADC);
		}
		break;
	case 2:
		if(device_index == IR_SENSOR){
			digitalWrite(PORT2_SIG2, HIGH);
			delayMicroseconds(15);
			adcValue = analogRead(PORT2_ADC);
			digitalWrite(PORT2_SIG2, LOW);
			return adcValue;
		}else if(device_index == MAGNETIC_SENSOR || device_index == TOUCH_SENSOR ){
			return digitalRead(PORT2_ADC);
		}else{
			return (int)analogRead(PORT2_ADC);
		}
		break;
	case 3:
		if(device_index == IR_SENSOR){
			digitalWrite(11, HIGH);
			delayMicroseconds(15);
			adcValue = analogRead(PORT3_ADC);
			digitalWrite(11, LOW);
			return adcValue;
		}else if(device_index == MAGNETIC_SENSOR || device_index == TOUCH_SENSOR ){
			return digitalRead(PORT3_ADC);
		}else{
			return (int)analogRead(PORT3_ADC);
		}
		break;
	case 4:
		if(device_index == IR_SENSOR){
			digitalWrite(PORT4_SIG2, HIGH);
			delayMicroseconds(15);
			adcValue = analogRead(PORT4_ADC);
			digitalWrite(PORT4_SIG2, LOW);
			return adcValue;
		}else if(device_index == MAGNETIC_SENSOR || device_index == TOUCH_SENSOR ){
			return digitalRead(PORT4_ADC);
		}else{
			return (int)analogRead(PORT4_ADC);
		}
		break;
	default:
		return 0;
	}
	return 0;
}

int OLLO::read(int devNum, int device_index, int sub_index){ //COLOR SENSOR
	//int adcValue = 0;
	if( devNum == 0 ){
		return 0;
	}
	if(device_index == COLOR_SENSOR){
		setColor(sub_index);
	}else{
		return 0;
	}

	switch(devNum){
	case 1:
		 digitalWrite(PORT1_SIG1, mMot_minus);
		 digitalWrite(PORT1_SIG2, mMot_plus);
		 delay(20); // after 20ms, read analog
		 return (int)analogRead(PORT1_ADC);

	case 2:
		digitalWrite(PORT2_SIG1, mMot_minus);
		digitalWrite(PORT2_SIG2, mMot_plus);
		delay(20);
		return (int)analogRead(PORT2_ADC);

	case 3:
		digitalWrite(PORT3_SIG1, mMot_minus);
		digitalWrite(PORT3_SIG2, mMot_plus);
		delay(20);
		return (int)analogRead(PORT3_ADC);

	case 4:
		digitalWrite(PORT4_SIG1, mMot_minus);
		digitalWrite(PORT4_SIG2, mMot_plus);
		delay(20);
		return (int)analogRead(PORT4_ADC);

	default:
		return 0;
	}
}
/*
uint8 OLLO::detectColor(uint8 port){

	int temp_red,temp_green,temp_blue;

	temp_red = 0;
	temp_green = 0;
	temp_blue= 0;
	int lColor[3]= {0,0,0};
	int lRed,lGreen,lBlue;
	uint8 bMaxColor, bMinColor, bColorResult;
	bMaxColor=0;
	bMinColor=0;
	bColorResult=0;
	//int i=0,j=0;
	//uint8 bColorResult=0;
	//for(i=0; i < 3; i++)
	{
		lRed = this->read(port, COLOR_SENSOR, RED);
	}
	//for(i=0; i < 3; i++)
	{
		lGreen = (this->read(port, COLOR_SENSOR, GREEN)+1) * 11 / 10;
	}
	//for(i=0; i < 3; i++)
	{
		lBlue = this->read(port, COLOR_SENSOR, BLUE);
	}
	Serial2.print("lRed = "); Serial2.print(lRed);
	Serial2.print("	 lGreen = "); Serial2.print(lGreen);
	Serial2.print("  lBlue = "); Serial2.print(lBlue);
    if(lRed >= lGreen && lRed >= lBlue)
    {
           bMaxColor = 1;
           lColor[0] = lRed;
    }
    else if(lGreen >= lRed && lGreen >= lBlue)
    {
           bMaxColor = 2;
           lColor[0] = lGreen;
    }
    else if(lBlue >= lRed && lBlue >= lGreen)
    {
           bMaxColor = 3;
           lColor[0] = lBlue;
    }
    if(lRed <= lGreen && lRed <= lBlue)
    {
           bMinColor = 1;
           lColor[2] = lRed;
    }
    else if(lGreen <= lRed && lGreen <= lBlue)
    {
           bMinColor = 2;
           lColor[2] = lGreen;
    }
    else if(lBlue <= lRed && lBlue <= lGreen)
    {
           bMinColor = 3;
           lColor[2] = lBlue;
    }
    lColor[1] = lRed + lGreen + lBlue - lColor[0] - lColor[2];

    if(lColor[2] > 300 || (lColor[2] > 250 && (lColor[0] / (lColor[0] - lColor[2])) > 4))// && lColor[0] < 200)
    {
           bColorResult = 1; // white
           //TxDString("WHITE\r\n");
    }
    else if(bMaxColor == 3 && lBlue > lGreen + 10 && lColor[0] / (lColor[0] - lColor[2]) < 5 && lColor[2] < 200 )// && lBlue > lRed + 3 && lRed < 20 && lGreen < 20)
    {
           bColorResult = 5; // blue
           //TxDString("BLUE\r\n");
    }
    else if(lColor[0] < 60 || (lColor[0] < 120 && lColor[0] / (lColor[0] - lColor[2]) > 5))//lRed < 5 && lGreen < 5 && lBlue < 5)// && lColor[0] - lColor[2] < 30)
    {
           bColorResult = 2; // black
          // TxDString("BLACK\r\n");
    }
    else if(bMaxColor == 1 && lColor[0] / (lColor[2] + 1) >= 2 && lColor[1] - lColor[2] < 50lRed > lGreen + 40 && lGreen < lBlue + 5 && lRed > 80 )
    {
           bColorResult = 3; // red
          // TxDString("RED\r\n");
    }
    else if(bMaxColor == 2 && lColor[0] - lColor[2] > 2 && lColor[0] < 300lGreen > lBlue + 3 && lGreen > lRed + 3 && lRed < 20)//lGreen < 5000)
    {
           bColorResult = 4; // green
          // TxDString("GREEN\r\n");
    }

    else if(bMinColor == 3 && (lColor[1] / lColor[2] > 1 || lColor[1] - lColor[2] >= 50))//lRed-lBlue > 20 && lGreen-lBlue > 20 && lRed < 120)
    {
           bColorResult = 6; // yellow
           //TxDString("YELLOW\r\n");
    }
    else
    {
           bColorResult = 0; // unknown
          // TxDString("\r\n");
    }

    Serial2.print("  bColorResult = "); Serial2.println(bColorResult);


	return bColorResult;

}*/
void OLLO::writeLED(int devNum, uint8 leftVal, uint8 rightVal ){
	if( leftVal >1 || rightVal >1 || devNum == 0 ){
		return;
	}

	switch(devNum){
		case 1:
			 digitalWrite(PORT1_SIG1,rightVal);
			 digitalWrite(PORT1_SIG2,leftVal);
			break;
		case 2:
			digitalWrite(PORT2_SIG1,rightVal);
			digitalWrite(PORT2_SIG2,leftVal);
			break;
		case 3:
			digitalWrite(PORT3_SIG1,rightVal);
			digitalWrite(PORT3_SIG2,leftVal);
			break;
		case 4:
			digitalWrite(PORT4_SIG1,rightVal);
			digitalWrite(PORT4_SIG2,leftVal);
			break;
		default:
			break;
		}

}
void OLLO::write(int devNum, uint8 leftVal, uint8 rightVal ){
	if( leftVal >1 || rightVal >1 || devNum == 0 ){
		return;
	}

	switch(devNum){
		case 1:
			 digitalWrite(6,rightVal);
			 digitalWrite(7,leftVal);
			break;
		case 2:
			digitalWrite(8,rightVal);
			digitalWrite(9,leftVal);
			break;
		case 3:
			digitalWrite(10,rightVal);
			digitalWrite(11,leftVal);
			break;
		case 4:
			digitalWrite(12,rightVal);
			digitalWrite(13,leftVal);
			break;
		default:
			break;
		}

}
void OLLO::write(int devNum, uint8 leftVal, uint8 centerVal, uint8 rightVal){

	if( leftVal >1 || rightVal >1 || centerVal > 1 || devNum == 0){
		return;
	}

	switch(devNum){
		case 1:
			 digitalWrite(6,rightVal);
			 digitalWrite(2,centerVal);
			 digitalWrite(7,leftVal);
			break;
		case 2:
			digitalWrite(8,rightVal);
			digitalWrite(3,centerVal);
			digitalWrite(9,leftVal);
			break;
		case 3:
			digitalWrite(10,rightVal);
			digitalWrite(0,centerVal);
			digitalWrite(11,leftVal);
			break;
		case 4:
			digitalWrite(12,rightVal);
			digitalWrite(1,centerVal);
			digitalWrite(13,leftVal);
			break;
		default:
			break;
	}

}

void OLLO::setColor(int colorIndex){
	switch(colorIndex){
			case RED: //Red
				mMot_minus = LOW;
				mMot_plus = LOW;
				break;
			case GREEN://Green
				mMot_minus = LOW;
				mMot_plus = HIGH;
				break;
			case BLUE://Blue
				mMot_minus = HIGH;
				mMot_plus = LOW;
				break;
			default:
				break;
		}

}
