/*******************************************************************
*
*  DESCRIPTION: Atomic Model Frame Grabber Image Acquisition module
*
*  AUTHOR: Brian Webb
*
*  EMAIL: mailto://bwebb@connect.carleton.ca
*
*  DATE: 25/10/2007
*
*******************************************************************/

#ifndef _TIM_H_
#define _TIM_H_

#include "atomic.h"     // class Atomic
#include "fg_codes.h"

#define PULSE_OFF    0
#define PULSE_DELAY  1
#define PULSE_ACTIVE 2
#define PULSE_INACTIVE 3

class FGTimer : public Atomic
{
public:
    FGTimer( const string &name = "FGImageAcquisition" );                    //Default constructor

    virtual string className() const ;
protected:
    Model &initFunction();
    Model &externalFunction( const ExternalMessage & );
    Model &internalFunction( const InternalMessage & );
    Model &outputFunction( const InternalMessage & );

private:
    const Port &trigIn;
    const Port &TcommandIn;
    Port &timerOut;
    Port &responseOut;
    
    int response;
    Time activeTime;
    Time delayTime;
    Time inactiveTime;

	int mode;
	int pulse_location;
	bool pulse_abort;
};  // class FGTimer

// ** inline ** // 
inline
string FGTimer::className() const
{
    return "FGTimer" ;
}

#endif /*_IMAGE_ACQUISITION_H_*/
