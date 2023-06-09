/*******************************************************************
*
*  DESCRIPTION: Atomic Model Frame Grabber Host Sytem Interface module
*
*  AUTHOR: Brian Webb 
*
*  EMAIL: mailto://bwebb@connect.carleton.ca
*
*  DATE: 25/10/2007
*
*******************************************************************/

/** include files **/
#include "host_system_interface.h"
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

#include <sstream>

/** public functions **/

/*******************************************************************
* Function Name: FGHostSystemInterface
* Description: 
********************************************************************/
FGHostSystemInterface::FGHostSystemInterface( const string &name )
: Atomic( name )
, HcommandIn( addInputPort( "HcommandIn" ) )
, imageIn( addInputPort( "imageIn" ) )
, responseIn( addInputPort( "responseIn" ) )
, imageOut( addOutputPort( "imageOut" ) )
, responseOut( addOutputPort( "responseOut" ) )
, ImAcqOut( addOutputPort( "ImAcqOut") )
, BufOut( addOutputPort( "BufOut") )
, TimOut( addOutputPort( "TimOut") )
, external_bus_speed( 500.0 )
{
    string speed(MainSimulator::Instance().getParameter( description(), "external_bus_speed" ) );
    if( speed != "" )
    {
        stringstream ss( speed );
        ss >> external_bus_speed;
    }
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &FGHostSystemInterface::initFunction()
{
    this->grab = HSI_GRAB_OFFBOARD_UNBUFFERED;
    this->send_image = NONE;
    this->buf_image = NONE;
    this->response = NONE;
    
    this->off_board_buffered_grab_image_from_IA = false;
    this->retrieving_an_image = false;
    
    this->IACommand  = NONE;
    this->BUFCommand = NONE;
    this->TIMCommand = NONE;
    
    this->passivate();

    return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &FGHostSystemInterface::externalFunction( const ExternalMessage &msg )
{
    if( msg.port() ==  imageIn )
    {
    	if (this->retrieving_an_image )
    	{
    		if ( msg.value() > NONE )
    		{
    			this->send_image = (int)msg.value();
    			holdIn( active, Time::Zero );
    		}
    		else
    		{
    			this->retrieving_an_image = false;
    		}
    	}
    	else
    	{
	        if(this->grab == HSI_GRAB_OFFBOARD_UNBUFFERED)
	        {
	            if (msg.value() > NONE )
	            {
	   	            this->send_image = (int)msg.value();
    	        	holdIn( active, Time::Zero );
	            }
	        }
    	    else if ( this->grab == HSI_GRAB_ONBOARD )
	        {
    	        if ( msg.value() > NONE ) // start of image
	            {
            	    this->buf_image = (int)msg.value() << EVENT_DATA_SHIFT;
        	        this->buf_image += BUF_STORE_IMAGE;
    	            holdIn( active, Time::Zero );
	            }
	        }
    	    else // HSI_GRAB_OFFBOARD_BUFFERED
	        {
	            if (this->off_board_buffered_grab_image_from_IA)
	            {
    	            if ( msg.value() > NONE ) // start of image
	                {
                	    this->buf_image = BUF_STORE_IMAGE + ((int)msg.value() << EVENT_DATA_SHIFT );
            	        holdIn( active, Time::Zero );
        	        }
    	            else // end of image
	                {
                    	this->off_board_buffered_grab_image_from_IA = false;
                	    this->BUFCommand = BUF_RETRIEVE_IMAGE;
            	        holdIn( active, Time::Zero );
        	        }
    	        }
	            else // from the buffer after we're finished buffering it
    	        {

	                if (msg.value() < NONE ) // end of the image
        	        {
    	            	this->off_board_buffered_grab_image_from_IA = true;
	                }
                	else
            	    {
   	    	            this->send_image = msg.value();
		            	holdIn( active, Time::Zero );
	                }

            	}
        	}
    	}
    }
    else
    {
    	if ( msg.port() == responseIn )
	    {
    	    this->response = msg.value();
        	holdIn( active, Time::Zero );
	    }
	    else
	    {
		    if ( msg.port() == HcommandIn ); // commandIn
		    {
    		    int command = ((int)msg.value() & EVENT_DATA_MASK);
		        int data  = (int)(msg.value()) >> EVENT_DATA_SHIFT;

		        switch (command)
		        {
	    	        case COMMAND_FOR_HSI :
	        	        int sub_command;
            	    	sub_command = data & EVENT_DATA_MASK ;
                		data = data >> EVENT_DATA_SHIFT;
	        	        switch (sub_command)
    		            {
		                    case HSI_SET_GRAB_TYPE :
		                        int grab_command;
	        	                grab_command = data;
	    	                    if( grab_command <= HSI_GRAB_LAST_MODE_NUM )
		                        {
		                            this->grab = grab_command;
	                    	        if( grab_command == HSI_GRAB_OFFBOARD_BUFFERED )
	                	            {
		        	                    this->off_board_buffered_grab_image_from_IA = true;
	        	                    }
	    	                        else
		                            {
		                            	this->off_board_buffered_grab_image_from_IA = false;
		                            }
		                            this->passivate();
		                        }
		                        else
		                        {
	    	                        this->response = ERROR_UNKNOWN_COMMAND;
		                            holdIn(active, Time::Zero);
		                        } 
		                        break;
	    	                default:
		                        this->response = ERROR_UNKNOWN_COMMAND;
		                        holdIn(active, Time::Zero);
		                }
		                break;
		            case COMMAND_FOR_IA  :
		                this->IACommand = data;
		                holdIn(active, Time::Zero );
		                break;
		            case COMMAND_FOR_BUF :
		            	if( data == BUF_RETRIEVE_IMAGE )
		            	{
		            		this->retrieving_an_image = true;
		            	}
		                this->BUFCommand = data;
		                holdIn(active, Time::Zero );
		                break;
		            case COMMAND_FOR_TIM :
		                this->TIMCommand = data;
	    	            holdIn(active, Time::Zero );
		                break;
		            default:
	                	this->response = ERROR_UNKNOWN_COMMAND;
	            	    holdIn( active, Time::Zero );
	        	}
	    	}
	    }
    }    
    
    return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &FGHostSystemInterface::internalFunction( const InternalMessage & )
{
    if( this->response != NONE )
    {
        this->response = NONE;
        this->passivate();
    }
    else if ( this->send_image != NONE )
    {
    	if ( this->send_image > NONE )
    	{
        	int size;
	        size = this->send_image >> EVENT_DATA_SHIFT;
    	    this->send_image = -1 * this->send_image;
	        holdIn( active, Time::Time((float)size / this->external_bus_speed) );
    	}
    	else
    	{
    		this->send_image = NONE;
	        this->passivate();
    	}
    }
    else if ( this->buf_image != NONE )
    {
        this->buf_image = NONE;
        this->passivate();
    }
    else if ( this->IACommand != NONE )
    {
        this->IACommand = NONE;
        this->passivate();
    }
    else if ( this->BUFCommand != NONE )
    {
        this->BUFCommand = NONE;
        this->passivate();
    }
    else if ( this->TIMCommand != NONE )
    {
        this->TIMCommand = NONE;
        this->passivate();
    }

    return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &FGHostSystemInterface::outputFunction( const InternalMessage &msg )
{
    if( this->response != NONE )
    {
        sendOutput( msg.time(), responseOut, this->response ) ;
    }
    else if ( this->send_image != NONE )
    {
        sendOutput( msg.time(), imageOut, this->send_image );
    }
    else if ( this->buf_image != NONE )
    {
        sendOutput( msg.time(), BufOut, this->buf_image );
    }
    else if ( this->IACommand != NONE )
    {
        sendOutput( msg.time(), ImAcqOut, this->IACommand );
    }
    else if ( this->BUFCommand != NONE )
    {
        sendOutput( msg.time(), BufOut, this->BUFCommand );
    }
    else if ( this->TIMCommand != NONE )
    {
        sendOutput( msg.time(), TimOut, this->TIMCommand );
    }
    return *this ;
}
