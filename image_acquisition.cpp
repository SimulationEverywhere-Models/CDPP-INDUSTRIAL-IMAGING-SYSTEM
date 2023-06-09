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
#include "image_acquisition.h"
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: FGImageAcquisition
* Description: 
********************************************************************/
FGImageAcquisition::FGImageAcquisition( const string &name )
: Atomic( name )
, imageIn( addInputPort( "imageIn" ) )
, IcommandIn( addInputPort( "IcommandIn" ) )
, imageOut( addOutputPort( "imageOut" ) )
, responseOut( addOutputPort( "responseOut" ) )
{
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &FGImageAcquisition::initFunction()
{
    this->response = NONE;
    this->image    = NONE;

    this->passivate();
    return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &FGImageAcquisition::externalFunction( const ExternalMessage &msg )
{
    if( msg.port() ==  imageIn )
    {
        image = msg.value();
        int image_size = abs(image) >> EVENT_DATA_SHIFT;
        if( this->mode != IA_MODE_OFF )
        {
            if ( (image < 0) && ( image_size != this->size ) )
            {
                this->response = ERROR_IMAGE_SIZE_DIFFERENT_THAN_EXPECTED;
            }
            else
            {
                this->response = NONE;
            }
            holdIn( active, Time::Zero );
        }
    }
    else if( msg.port() == IcommandIn )
    {
        int command = ((int)msg.value()) & EVENT_DATA_MASK;
        int data    = ((int)msg.value()) >> EVENT_DATA_SHIFT;
        switch ( command )
        {
            case IA_SET_SIZE :
                this->size = data;
                this->passivate();
                break;
            case IA_GRAB_SINGLE :
                this->mode = IA_MODE_GRAB;
                this->passivate();
                break;
            case IA_GRAB_CONTINUOUS :
                this->mode = IA_MODE_CONT;
                this->passivate();
                break;
            case IA_GRAB_ABORT :
                if ( this->mode != IA_MODE_OFF )
                {
                    this->mode = IA_MODE_OFF;
                    this->response = IA_GRAB_ABORTED;
                }
                else
                {
                    this->response = ERROR_NO_GRAB_IN_PROGRESS;
                }
                holdIn(active, Time::Zero );
                break;
            default :
                this->response = ERROR_UNKNOWN_COMMAND;
                holdIn(active, Time::Zero ); 
        }
    }

    return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &FGImageAcquisition::internalFunction( const InternalMessage & )
{
    if( this->response != NONE )
    {
        this->response = NONE;
    }
    if ( this->image != NONE );
    {
        if ( (this->mode == IA_MODE_GRAB) && (this->image < 0) )
        {
            this->mode = IA_MODE_OFF;
        }
        this->image = NONE;
    }
    this->passivate();
    return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &FGImageAcquisition::outputFunction( const InternalMessage &msg )
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
