#include "Arduino.h"
#include "Boiler.h"

#define REPEATTIME 6000
#define REPEATTIME 60000
#define PULSETIME 1900
#define PULSES 3

const bool theOnString[] =   { 1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,0,1,0,0,1,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,0,1,0,1 };
const bool theOffString[] =  { 1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,0,1,0,0,1,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,1,1,0,1,0,1,0,0,1,0,1 };
const bool theBootString[] = { 1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,0,1,0,0,1,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,1,0,1,0,1,0,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,1 };

Boiler::Boiler( int pPin )
{
  thePin = pPin;
  theState = false;
  pinMode( thePin, OUTPUT );
  digitalWrite( thePin, HIGH );
  reset();
}

Boiler::Boiler( int pPin, bool pOn )
{
  thePin = pPin;
  theState = pOn;
  pinMode( thePin, OUTPUT );
  digitalWrite( thePin, HIGH );
  reset();
}

void Boiler::on()
{
  if( !theState )
  {
    theState = true;
    reset();
  }
}

void Boiler::off()
{
  if( theState )
  {
    theState = false;
    reset();
  }
}

bool Boiler::isOn()
{
  return theState;
}

void Boiler::setOn( bool pOn )
{
  if( pOn )
  {
    on();
  }
  else
  {
    off();
  }
}

void Boiler::reset()
{
  theLastPulse = millis() - ( PULSETIME + 1 );
  theLastSend = millis() - ( REPEATTIME + 1 );
  thePulses = 0;
}

void Boiler::send( const bool pData[], int pLength )
{
  for( int j = 0; j < pLength; j ++ )
  {
    digitalWrite( thePin, pData[ j ] );
    delayMicroseconds( 500 );
  }
  digitalWrite( thePin, LOW );
  theLastPulse = millis();
  thePulses++;
}


void Boiler::doComms()
{
  unsigned long lDiff = millis() - theLastSend;
  if( lDiff > REPEATTIME )
  {
    lDiff = millis() - theLastPulse;

    if( lDiff > PULSETIME )
    {
      if( theState )
      {
        send( theOnString, sizeof( theOnString ) / sizeof( bool ) );
      }
      else
      {
        send( theOffString, sizeof( theOffString ) / sizeof( bool ) );
      }
    }
    if( thePulses >= PULSES )
    {
      theLastSend = millis();
      thePulses = 0;
    }
  }
}

