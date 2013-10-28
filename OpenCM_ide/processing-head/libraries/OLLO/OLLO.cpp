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
		pinMode(6, OUTPUT); //RED  (right)
		pinMode(7, OUTPUT); //BLUE (left)
		pinMode(2, INPUT_ANALOG); //ADC input
		break;
	case 2:
		pinMode(8, OUTPUT); //RED  (right)
		pinMode(9, OUTPUT); //BLUE (left)
		pinMode(3, INPUT_ANALOG);//ADC input
		break;
	case 3:
		pinMode(10, OUTPUT); //RED  (right)
		pinMode(11, OUTPUT); //BLUE (left)
		pinMode(0, INPUT_ANALOG);//ADC input
		break;
	case 4:
		pinMode(12, OUTPUT); //RED  (right)
		pinMode(13, OUTPUT); //BLUE (left)
		pinMode(1, INPUT_ANALOG);//ADC input
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
			pinMode(2, INPUT); //digital input
		}else if(device_index == TOUCH_SENSOR){
			pinMode(2, INPUT_PULLUP);
		}else{
			pinMode(2, INPUT_ANALOG); //ADC input
		}

		pinMode(6, OUTPUT); //SIG1
		pinMode(7, OUTPUT); //SIG2
		if(device_index == IR_SENSOR ){
			digitalWrite(6,LOW); //SIG1 set to LOW
			digitalWrite(7,LOW); //SIG2 set to LOW
		}
		break;
	case 2:
		if(device_index == MAGNETIC_SENSOR){
			pinMode(3, INPUT); //digital input
		}else if(device_index == TOUCH_SENSOR){
			pinMode(3, INPUT_PULLUP);
		}else{
			pinMode(3, INPUT_ANALOG);//ADC input
		}


		pinMode(8, OUTPUT); //SIG1
		pinMode(9, OUTPUT); //SIG2
		if(device_index == IR_SENSOR ){
			digitalWrite(8,LOW); //set to LOW
			digitalWrite(9,LOW); //set to LOW
		}
		break;
	case 3:
		if(device_index == MAGNETIC_SENSOR){
			pinMode(0, INPUT); //digital input
		}else if(device_index == TOUCH_SENSOR){
			pinMode(0, INPUT_PULLUP);
		}else{
			pinMode(0, INPUT_ANALOG);//ADC input
		}


		pinMode(10, OUTPUT); //SIG1
		pinMode(11, OUTPUT); //SIG2
		if(device_index == IR_SENSOR ){
			digitalWrite(10,LOW); //set SIG1 to LOW
			digitalWrite(11,LOW); //set SIG2 to LOW
		}
		break;
	case 4:
		if(device_index == MAGNETIC_SENSOR){
			pinMode(1, INPUT); //digital input
		}else if(device_index == TOUCH_SENSOR){
			pinMode(1, INPUT_PULLUP);
		}else{
			pinMode(1, INPUT_ANALOG);//ADC input
		}


		pinMode(12, OUTPUT); //SIG1
		pinMode(13, OUTPUT); //SIG2
		if(device_index == IR_SENSOR ){
			digitalWrite(12,LOW); //set SIG1 to LOW
			digitalWrite(13,LOW); //set SIG2 to LOW
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
			pinMode(2, INPUT_PULLUP);
			attachInterrupt(2,handler, RISING);
		}
		break;
	case 2:
		if(device_index == TOUCH_SENSOR){
			pinMode(3, INPUT_PULLUP);
			attachInterrupt(3,handler, RISING);
		}
		break;
	case 3:
		if(device_index == TOUCH_SENSOR){
			pinMode(0, INPUT_PULLUP);
			attachInterrupt(0,handler, RISING);
		}
		break;
	case 4:
		if(device_index == TOUCH_SENSOR){
			pinMode(1, INPUT_PULLUP);
			attachInterrupt(1,handler, RISING);
		}
		break;
	default:
		break;
	}
}
/*void OLLO::beginButton(int devNum, voidFuncPtr handler){
	if( devNum == 0 || devNum > 4){
		return;
	}
	switch(devNum){
		case 1:
			pinMode(2, INPUT_PULLUP);
			attachInterrupt(2,handler, RISING);
			break;
		case 2:
			pinMode(3, INPUT_PULLUP);
			attachInterrupt(3,handler, RISING);
			break;
		case 3:
			pinMode(0, INPUT_PULLUP);
			attachInterrupt(0,handler, RISING);
			break;
		case 4:
			pinMode(1, INPUT_PULLUP);
			attachInterrupt(1,handler, RISING);
			break;
		default:
			break;
		}
}*/

/*void OLLO::beginIR(int devNum){
	if( devNum == 0 || devNum > 4){
			return;
	}
	switch(devNum){
	case 1:
		pinMode(2, INPUT_ANALOG); //ADC init
		pinMode(6, OUTPUT); //IR SIG1 LOW
		//pinMode(7, PWM); //IR PWM
		pinMode(7, OUTPUT); //IR SIG2

		digitalWrite(6,LOW); //SIG1 set to LOW
		digitalWrite(7,LOW); //SIG2 set to LOW
		//analogWrite(7,40000);
		break;
	case 2:
		pinMode(3, INPUT_ANALOG); //ADC init
		pinMode(8, OUTPUT); //IR SIG1 LOW
		pinMode(9, OUTPUT); //IR SIG2
		//pinMode(9, PWM); //IR PWM

		digitalWrite(8,LOW); //set to LOW
		digitalWrite(9,LOW); //set to LOW
		//analogWrite(9,40000);
		break;
	case 3:
		pinMode(0, INPUT_ANALOG); //ADC init
		pinMode(10, OUTPUT); //IR SIG1 LOW
		pinMode(11, OUTPUT); //IR SIG2
		//pinMode(11, PWM); //IR PWM

		digitalWrite(10,LOW); //set SIG1 to LOW
		digitalWrite(11,LOW); //set SIG2 to LOW
		//analogWrite(11,40000);
		break;
	case 4:
		pinMode(1, INPUT_ANALOG); //ADC init
		pinMode(12, OUTPUT); //IR SIG1 to Output
		pinMode(13, OUTPUT); //IR SIG2 to output
		//pinMode(13, PWM); //IR PWM

		digitalWrite(12,LOW); //set SIG1 to LOW
		digitalWrite(13,LOW); //set SIG2 to LOW
		//analogWrite(13,40000);
		break;
	default:
		break;
	}

}*/


int OLLO::read(int devNum){ // general sensor reading method
	if( devNum == 0 ){
			return 0;
	}
	switch(devNum){
	case 1:
		return (int)analogRead(2);
	case 2:
		return (int)analogRead(3);
	case 3:
		return (int)analogRead(0);
	case 4:
		return (int)analogRead(1);
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
			digitalWrite(7, HIGH);
			delayMicroseconds(15);
			adcValue = analogRead(2);
			digitalWrite(7, LOW);
			return adcValue;
		}else if(device_index == MAGNETIC_SENSOR || device_index == TOUCH_SENSOR  ){
			return digitalRead(2);
		}else{
			return (int)analogRead(2);
		}
		break;
	case 2:
		if(device_index == IR_SENSOR){
			digitalWrite(9, HIGH);
			delayMicroseconds(15);
			adcValue = analogRead(3);
			digitalWrite(9, LOW);
		}else if(device_index == MAGNETIC_SENSOR || device_index == TOUCH_SENSOR ){
			return digitalRead(3);
		}else{
			return (int)analogRead(3);
		}
		break;
	case 3:
		if(device_index == IR_SENSOR){
			digitalWrite(11, HIGH);
			delayMicroseconds(15);
			adcValue = analogRead(0);
			digitalWrite(11, LOW);
		}else if(device_index == MAGNETIC_SENSOR || device_index == TOUCH_SENSOR ){
			return digitalRead(0);
		}else{
			return (int)analogRead(0);
		}
		break;
	case 4:
		if(device_index == IR_SENSOR){
			digitalWrite(13, HIGH);
			delayMicroseconds(15);
			adcValue = analogRead(1);
			digitalWrite(13, LOW);
		}else if(device_index == MAGNETIC_SENSOR || device_index == TOUCH_SENSOR ){
			return digitalRead(1);
		}else{
			return (int)analogRead(1);
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
		 digitalWrite(6,mMot_minus);
		 digitalWrite(7,mMot_plus);
		 delay(40);
		 return (int)analogRead(2);

	case 2:
		digitalWrite(8,mMot_minus);
		digitalWrite(9,mMot_plus);
		delay(40);
		return (int)analogRead(3);

	case 3:
		digitalWrite(10,mMot_minus);
		digitalWrite(11,mMot_plus);
		delay(40);
		return (int)analogRead(0);

	case 4:
		digitalWrite(12,mMot_minus);
		digitalWrite(13,mMot_plus);
		delay(40);
		return (int)analogRead(1);

	default:
		return 0;
	}
}

/*int OLLO::readColor(int devNum, int colorIndex){
	setColor(colorIndex);
	switch(devNum){
		case 1:
			 digitalWrite(6,mMot_minus);
			 digitalWrite(7,mMot_plus);
			 delay(40);
			 return (int)analogRead(2);
			break;
		case 2:
			digitalWrite(8,mMot_minus);
			digitalWrite(9,mMot_plus);
			delay(40);
			return (int)analogRead(3);
			break;
		case 3:
			digitalWrite(10,mMot_minus);
			digitalWrite(11,mMot_plus);
			delay(40);
			return (int)analogRead(0);
			break;
		case 4:
			digitalWrite(12,mMot_minus);
			digitalWrite(13,mMot_plus);
			delay(40);
			return (int)analogRead(1);
			break;
		default:
			break;
	}
	return 0;
}*/
void OLLO::writeLED(int devNum, uint8 leftVal, uint8 rightVal ){
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
