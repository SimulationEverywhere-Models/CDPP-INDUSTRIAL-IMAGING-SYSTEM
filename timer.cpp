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

/** include files **/
#include "timer.h"
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: FGImageAcquisition
* Description: 
********************************************************************/
FGTimer::FGTimer( const string &name )
: Atomic( name )
, trigIn( addInputPort( "trigIn" ) )
, TcommandIn( addInputPort( "TcommandIn" ) )
, timerOut( addOutputPort( "timerOut" ) )
, responseOut( addOutputPort( "responseOut" ) )
{
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &FGTimer::initFunction()
{
	this->response = NONE;
	
	this->activeTime   = Time::Zero;
	this->delayTime    = Time::Zero;
	this->inactiveTime = Time::Zero;
	
	this->mode = TIM_MODE_OFF;
	this->pulse_location = PULSE_OFF;
	this->pulse_abort = false;
	
    this->passivate();
    return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &FGTimer::externalFunction( const ExternalMessage &msg )
{
    if( msg.port() ==  trigIn )
    {
    	if ( this->mode == TIM_MODE_TRIGGERED && this->pulse_location == PULSE_OFF)
    	{
    		this->pulse_location = PULSE_DELAY;
    		holdIn( active, delayTime );
    	}
    }
    if( msg.port() == TcommandIn )
    {
    	int command = (int)msg.value() & EVENT_DATA_MASK;
    	int data;
    	data = (int)msg.value() >> EVENT_DATA_SHIFT;;

	   	switch( command )
    	{
    		case TIM_SET_ACTIVE :
    			activeTime = Time::Time( (float)data / MSEC );
    			break;
    		case TIM_SET_DELAY :
    			delayTime = Time::Time( (float)data / MSEC );
    			break;
    		case TIM_SET_INACTIVE :
    			inactiveTime = Time::Time( (float)data / MSEC );
    			break;
    		case TIM_START_TIMER :
    			if ( activeTime == Time::Zero )
				{
					this->response = ERROR_TIMER_NOT_SET;
					holdIn( active, Time::Zero );
				}
				else if ( (delayTime == Time::Zero) && (inactiveTime == Time::Zero) )
				{
					this->response = ERROR_TIMER_DELAY_OR_INACTIVE_NOT_SET;
					holdIn( active, Time::Zero );
				}
				else
				{
						this->mode = TIM_MODE_TRIGGERED;
				}
    			break;
    		case TIM_STOP_TIMER :
				this->mode = TIM_MODE_OFF;
				this->pulse_location = PULSE_OFF;
				this->passivate();
    			break;
    		case TIM_START_PERIODIC_TIMER :
    			FILE *debug;
    			debug = fopen ( "debug.txt", "a" );
    			fprintf( debug, "delay time %lf active_time %lf inactive %lf\n", this->delayTime.asMsecs(), this->activeTime.asMsecs(), this->inactiveTime.asMsecs() );
				fclose( debug );
    			if ( activeTime == Time::Zero )
    			{
    				this->response = ERROR_TIMER_NOT_SET;
    				holdIn( active, Time::Zero );
    			}
    			else if ( (delayTime == Time::Zero) && (inactiveTime == Time::Zero) )
    			{
    				this->response = ERROR_TIMER_DELAY_OR_INACTIVE_NOT_SET;
    				holdIn( active, Time::Zero );
    			}
    			else
    			{
    				this->mode = TIM_MODE_PERIODIC;
    				this->pulse_location = PULSE_DELAY;
    				holdIn( active, this->delayTime );
    			}
    			break;
    		default:
    			break;
    	}
    }
    return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &FGTimer::internalFunction( const InternalMessage & )
{
	if( this->response != NONE )
	{
		this->response = NONE;
		this->passivate();
	}
	if ( this->mode != TIM_MODE_OFF )
	{
		int location = this->pulse_location;
		switch (location)
		{
			case PULSE_DELAY :
				this->pulse_location = PULSE_ACTIVE;
				holdIn( active, this->activeTime );
				break;
			case PULSE_ACTIVE :
				this->pulse_location = PULSE_INACTIVE;
				holdIn( active, this->inactiveTime );
				break;
			case PULSE_INACTIVE :
				if (this->mode == TIM_MODE_PERIODIC)
				{
					this->pulse_location = PULSE_DELAY;
					holdIn( active, this->delayTime );
				}
				else // has to be ON, or we wouldn't get here
				{
					this->pulse_location = PULSE_OFF;
					this->passivate();
				}
				break;
			default: // shouldn't get here
				break;
		}
	}
    return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &FGTimer::outputFunction( const InternalMessage &msg )
{
	if ( this->response != NONE )
	{
		sendOutput( msg.time(), responseOut, this->response );
	}
	if ( this->pulse_location == PULSE_DELAY )
	{
		sendOutput( msg.time(), timerOut, 1.0 );
	}
	if ( this->pulse_location == PULSE_ACTIVE )
	{
		sendOutput( msg.time(), timerOut, -1.0 );
	}
    return *this ;
}
