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
#include "storage_buffer.h"
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

#include <sstream>

/** public functions **/

/*******************************************************************
* Function Name: FGStorageBuffer
* Description: 
********************************************************************/
FGStorageBuffer::FGStorageBuffer( const string &name )
: Atomic( name )
, BcommandIn( addInputPort( "BcommandIn" ) )
, imageOut( addOutputPort( "imageOut" ) )
, responseOut( addOutputPort( "responseOut" ) )
, totalSpace( 4096 )
, internal_bus_speed( 500.0 )
{
    string space(MainSimulator::Instance().getParameter( description(), "totalSpace" ) );
    string speed(MainSimulator::Instance().getParameter( description(), "internal_bus_speed" ) );
    
    if (space != "" )
    {
        stringstream ss( space );
        ss >> totalSpace;
        freeSpace = totalSpace;
    }
    if (speed != "" )
    {
        stringstream ss( speed );
        ss >> internal_bus_speed;
    }
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &FGStorageBuffer::initFunction()
{
    int i;
    this->response = NONE;
    this->image    = NONE;

    for( i = 0; i < LIST_LENGTH; i++ )
    {
        this->bufferList[i]  = 0.0;
        this->bufferSizes[i] = 0;
    }
    this->lastIndex    = 0;
    this->currentIndex = 0;

    this->passivate();

    return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &FGStorageBuffer::externalFunction( const ExternalMessage &msg )
{
    if( msg.port() ==  BcommandIn )
    {
        int command = (int)msg.value() & EVENT_DATA_MASK;
        int data  = (int)msg.value() >> EVENT_DATA_SHIFT;
        switch (command)
        {
            case BUF_CREATE_BUFFER:
                if ((data < this->freeSpace) && ( this->lastIndex < LIST_LENGTH ))
                {
                    this->bufferSizes[lastIndex] = (int)data;
                    this->lastIndex++;
                    this->freeSpace -= data;
                    this->passivate();
                }
                else
                {
                    this->response = ERROR_NOT_ENOUGH_FREE_SPACE;
                    holdIn( active, Time::Zero ); 
                }
                break;
            case BUF_SET_INDEX:
                if (lastIndex <= 0)
                {
                    this->response = ERROR_NO_BUFFERS_DEFINED;
                    holdIn( active, Time::Zero );
                }
                else if ((data >= 0) && (data < this->lastIndex ))
                {
                    this->currentIndex = data;
                    this->passivate();
                }
                else
                {
                    this->response = ERROR_INDEX_OUT_OF_RANGE;
                    holdIn( active, Time::Zero );
                }
                break;
            case BUF_STORE_IMAGE:
                int store_size;
                store_size = data >> EVENT_DATA_SHIFT;
                if( store_size > this->bufferSizes[this->currentIndex] )
                {
                    response = ERROR_IMAGE_BUFFER_SIZE_MISMATCH;
                    holdIn( active, Time::Zero );
                }
                else
                {
                    this->bufferList[this->currentIndex] = data;
                    this->passivate();
                }
                break;
            case BUF_RETRIEVE_IMAGE:
            	this->image = this->bufferList[ this->currentIndex ];
                holdIn( active, Time::Zero );
                break;
            default :
                this->response = ERROR_UNKNOWN_COMMAND;
                holdIn( active, Time::Zero );
        }
    }

    return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &FGStorageBuffer::internalFunction( const InternalMessage & )
{
	int size;

    if( this->response != NONE )
    {
        this->response = NONE;
        this->passivate();
    }
    if ( this->image != NONE )
    {
	    if ( this->image > NONE )
    	{
    	    this->image = -1.0 * this->image;
    		size = this->bufferSizes[ this->currentIndex ];
	        holdIn( active, Time::Time((float)size/this->internal_bus_speed) );
    	}
	    else // this->image < NONE
    	{
    	    this->image = NONE;
        	this->passivate();
	    }
    }

    return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &FGStorageBuffer::outputFunction( const InternalMessage &msg )
{
    if( this->response != NONE )
    {
        sendOutput( msg.time(), responseOut, this->response ) ;
    }
    if ( this->image != NONE )
    {
        sendOutput( msg.time(), imageOut, this->image );
    }

    return *this ;
}
