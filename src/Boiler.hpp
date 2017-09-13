#include "Arduino.h"

#ifndef Boiler_h
#define Boiler_h

class Boiler
{
	public:
		Boiler( int pPin );
		Boiler( int pPin, bool pOn );
		void on();
		void off();
		bool isOn();
		void setOn( bool pOn );
		void loop();
	private:
		int thePin;
		bool theState;
		unsigned long theLastSend;
		unsigned long theLastPulse;
		unsigned int thePulses;

		void send( const bool pData[], int pLength );
		void reset();
};

#endif
