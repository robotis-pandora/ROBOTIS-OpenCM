#include <Keypad.h>


const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long loopCount = 0;
unsigned long timer_t = 0;

void setup(){
	Serial2.begin(9600);
	
	// Try playing with different debounceTime settings to see how it affects
	// the number of times per second your loop will run. The library prevents
	// setting it to anything below 1 millisecond.
	kpd.setDebounceTime(10);	// setDebounceTime(mS)
}

void loop(){
	char key = kpd.getKey();
	
	// Report the number of times through the loop in 1 second. This will give
	// you a relative idea of just how much the debounceTime has changed the
	// speed of your code. If you set a high debounceTime your loopCount will
	// look good but your keypresses will start to feel sluggish.
	if ((millis() - timer_t) > 1000) {
		Serial2.print("Your loop code ran ");
		Serial2.print(loopCount);
		Serial2.println(" times over the last second");
		loopCount = 0;
		timer_t = millis();
	}
	loopCount++;
	if(key)
		Serial2.println(key);
}
