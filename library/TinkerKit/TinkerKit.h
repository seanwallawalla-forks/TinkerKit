
/*
 *      TinkerKit Library v0.1
 *      
 *
 *
 *      created on Dec 2011
 *      by Federico Vanzati
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *      
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *      
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#include "Arduino.h"

#ifndef TinkerKit_h
#define TinkerKit_h

#include <Esplora.h>

// Minimum Analog In/Out that each platform have
#define I0 A0
#define I1 A1
#define I2 A2
#define I3 A3
#define I4 A4
#define I5 A5

#define O0 11
#define O1 10
#define O2 9
#define O3 6
#define O4 5
#define O5 3

// Mega have more I/O
#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
#define I6 A6
#define I7 A7
#define I8 A8
#define I9 A9

#define D13 13
#define D12 12
#define D8 8
#define D7 7
#define D4 4
#define D2 2
#endif

#define TK_MAX 1023
#define TK_X1 0	// identifies the 1x TKGyro model
#define TK_X4 1	// identifies the 4x TKGyro model
#define NORTH 1	// north pole: used in TKHallSensor
#define SOUTH 0	// south pole: used in TKHallSensor


#if defined ESPLORA_H_
/*
 * Arduino Esplora has no user-serviceable analog inputs;
 * it has a multiplexer that connects to 16 analog
 * or digital peripherals. Two of them are connectors
 * to external peripherals.
 * In order to use those peripherals in a consistent
 * way, the following macros will redirect the
 * analogRead and digitalRead functions to Esplora.
 * readChannel().
 * In order to make this effective, all occurrences of
 * digital/analogRead MUST be in this file and not in
 * the .cpp.
 * The original semantics of these two functions are
 * restored at the end of this file.
 */
#define digitalRead(pin) Esplora.readChannel(pin)
#define analogRead(pin) Esplora.readChannel(pin)

/*
 * Output connectors (wired directly to ATmega -- input
 * objects won't work because of the above redirect
 */
#define OUT_A 3
#define OUT_B 11

/*
 * Input connectors (wired to the multiplexer)
 */
#define IN_A 8
#define IN_B 9

#endif
/* 
-----------------------------------------------------------------------------
                        TinkerKit modules Classes
-----------------------------------------------------------------------------
*/


/*
 * Buttons Class and Methods
 */
  
class TKButton
{
	public:
		TKButton(uint8_t pin);
		inline boolean get() { return digitalRead(_pin); }
		boolean toggle();
		boolean pressed();
		boolean held();
		boolean released();
		
	protected:
		uint8_t _pin;
		boolean _toggleState, _oldState;
		boolean _pressedState, _releasedState;
		
		void update();
};

class TKEsploraButton : public TKButton
{
public:
  TKEsploraButton(uint8_t pin);
  
  inline boolean get() { return !Esplora.readButton(_pin); }
  inline boolean toggle() { return Esplora.buttonToggled(_pin); }
  inline boolean pressed() { return Esplora.buttonPressed(_pin); }
  inline boolean released() { return Esplora.buttonReleased(_pin); }
};

/*
 * Tilt Sensor Class and Methods
 */
 
class TKTiltSensor
{
	public:
		TKTiltSensor(uint8_t pin);
		inline boolean get() { return digitalRead(_pin); }
	
	protected:
		uint8_t _pin;
};


/*
 * Touch Sensor Class and Methods
 */  
 

class TKTouchSensor : public TKButton
{
	public:
		TKTouchSensor(uint8_t pin);
};


/*
 * Leds Class and Methods
 */ 
 
class TKLed 
{
	public: 
		TKLed(uint8_t pin);
		inline void set(boolean value) { digitalWrite(_pin, value); _state = value; }
		inline boolean state() { return _state; }
		inline void on() { TKLed::set(HIGH); }
		inline void off() { TKLed::set(LOW); }
		inline void brightness(int value) 
		{ 
			if( value <= TK_MAX && value >= 0)
				analogWrite(_pin, value/4);
			else
				return;
		}
		
	protected:
		uint8_t _pin;
		boolean _state;
};	

/*
 * Potentiometer Class and Methods
 */
 
class TKPotentiometer
{
	public:
		TKPotentiometer(uint8_t pin);
		inline int get() { return analogRead(_pin); }
		
	protected:
		uint8_t _pin;
};


class TKEsploraPotentiometer : public TKPotentiometer
{
public:
  TKEsploraPotentiometer(uint8_t pin) : TKPotentiometer(pin) { };
  inline int get() { return Esplora.readSlider(); }
};

/*
 * LightSensor Class and Methods
 */

class TKLightSensor
{
	public:
		TKLightSensor(uint8_t pin);
		inline int get() { return analogRead(_pin); }
		
	protected:
		uint8_t _pin;
};

class TKEsploraLightSensor : public TKLightSensor
{
  public:
    TKEsploraLightSensor(uint8_t pin) : 
      TKLightSensor(pin) { };
    inline int get() { return Esplora.readLightSensor(); }
};
  

/*
 * Thermistor Class and Methods
 */
 
class TKThermistor 
{
	public:
		TKThermistor(uint8_t pin);
		inline int get() { return analogRead(_pin); }
		float getCelsius();
		float getFahrenheit();
		
	protected:
		uint8_t _pin;
		const static float ADCres = 1023.0;
		const static int Beta = 3950;			// Beta parameter
		const static float Kelvin = 273.15;	// 0°C = 273.15 K
		const static int Rb = 10000;			// 10 kOhm
		const static float Ginf = 120.6685;	// Ginf = 1/Rinf
														// Rinf = R0*e^(-Beta/T0) = 4700*e^(-3950/298.15)
};	

class TKEsploraTemperatureSensor : public TKThermistor 
{
  public:
    TKEsploraTemperatureSensor(uint8_t pin) 
      : TKThermistor(pin) { };
    inline int get() { return Esplora.readTemperature(DEGREES_C); }
    float getCelsius() {
      // float version of _Esplora::readTemperature
      return (Esplora.readChannel(CH_TEMPERATURE) * 500.0 / 1024) - 50;
    };
    float getFahrenheit() {
      return (Esplora.readChannel(CH_TEMPERATURE) * 450.0 / 512) - 58;
    };
};  


		
/*
 * MosFet Class and Methods
 */
 
class TKMosFet 
{
	public:
		TKMosFet(uint8_t pin);
		inline void set(boolean value) { digitalWrite(_pin, value); _state = value; }
		inline boolean state() { return _state; }
		inline void on() { TKMosFet::set(HIGH); }
		inline void off() { TKMosFet::set(LOW); }
		inline void set(int value) 
		{ 
			if( value <= TK_MAX && value >= 0 )
				analogWrite(_pin, value/4);
			else
				return;
		}

	protected:
		uint8_t _pin;
		boolean _state;
};

/*
 * Relay Class and Methods
 */
 
class TKRelay
{
	public:
		TKRelay(uint8_t pin);
		inline void set(boolean value) { digitalWrite(_pin, value); _state = value; }
		inline boolean state() { return _state; }
		inline void on() { TKRelay::set(HIGH); }
		inline void off() { TKRelay::set(LOW); }

	protected:
		uint8_t _pin;	
		boolean _state;
};

/*
 * Hall Sensor Class and Methods
 */

class TKHallSensor
{
	public:
		TKHallSensor(uint8_t pin);
		inline int get() { return analogRead(_pin); }
		boolean polarity();
			
	protected:
		uint8_t _pin;
		const static uint16_t _zeroValue = 512;
};


/*
 * Joystick Class and Methods
 */
 
 class TKJoystick
 {
 	public:
 		TKJoystick(uint8_t pinX, uint8_t pinY);
 		inline int getXAxis() { return analogRead(_pinX); }
 		inline int getYAxis() { return analogRead(_pinY); }
 		 		
 	protected:
 		uint8_t _pinX, _pinY;
 };
 
 
/*
 * Gyro Sensor Class and Methods
 */

class TKGyro
{	
	public:
		TKGyro(uint8_t pinX, uint8_t pinY, boolean model);
		inline int getXAxis() { return analogRead(_pinX); }
 		inline int getYAxis() { return analogRead(_pinY); }
 		void calibrate();
		long getXAxisRate();
		long getYAxisRate();
		
	protected:
		uint8_t _pinX,_pinY;
		boolean model;
		
		//const static int _ADCresolution = 4880;	// [mV/count]	multiplierd by 1000 to avoid float numbers
		// minimum sensitivity for the 1x module value (from datasheet is 0.167 mV/deg/s but the TinkerKit module has the outputs amplified 2x)
		//const static int _sensitivity = 334;	// Sensitivity is expressed in mV/degree/seconds, multiplierd by 1000 to avoid float numbers. 
															// This value represent the sensitivity of the 1x module. The sensitivity of the 4x module is 4x of this one
	   long _sensitivityInCount;	// we obtain the sensitivity expressed in ADC counts
																										// [counts/dps]
		int _yZeroVoltage;
		int _xZeroVoltage;
};

/*
 * Accelerometer Class and Methods
 */
 
class TKAccelerometer
{	
	public:
		TKAccelerometer(uint8_t pinX, uint8_t pinY);
		inline int getXAxis() { return analogRead(_pinX); }
 		inline int getYAxis() { return analogRead(_pinY); }
 		inline float getXinG() { return (float)(analogRead(_pinX) - _zeroOffset)/96; }
 		inline float getYinG() { return (float)(analogRead(_pinY) - _zeroOffset)/96; }
 		int inclination();
		
	protected:
		uint8_t _pinX, _pinY;
		const static float _gain = 1.414;
		const static int _zeroOffset = 478;
};

/*
 * Esplora builtin peripherals
 */

extern class TKEsploraButton downButton;
extern class TKEsploraButton upButton;
extern class TKEsploraButton leftButton;
extern class TKEsploraButton rightButton;

extern class TKLed redLed;
extern class TKLed greenLed;
extern class TKLed blueLed;

extern class TKEsploraPotentiometer potentiometer;

extern class TKEsploraLightSensor lightSensor;

extern class TKEsploraTemperatureSensor tempSensor;

extern class TKJoystick joystick;


#if defined ESPLORA_H_
#undef digitalRead
#undef analogRead
#endif


#endif


