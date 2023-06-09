/*******************************************************************
*
*  DESCRIPTION: Atomic Model Frame Grabber Host System Interface module
*
*  AUTHOR: Brian Webb
*
*  EMAIL: mailto://bwebb@connect.carleton.ca
*
*  DATE: 25/10/2007
*
*******************************************************************/

#ifndef _HOST_SYSTEM_INTERFACE_H_
#define _HOST_SYSTEM_INTERFACE_H_

#include "atomic.h"     // class Atomic
#include "fg_codes.h"

class FGHostSystemInterface : public Atomic
{
public:
    FGHostSystemInterface( const string &name = "FGHostSystemInterface" );                    //Default constructor

    virtual string className() const ;
protected:
    Model &initFunction();
    Model &externalFunction( const ExternalMessage & );
    Model &internalFunction( const InternalMessage & );
    Model &outputFunction( const InternalMessage & );

private:
    const Port &HcommandIn;
    const Port &imageIn;
    const Port &responseIn;
    Port &imageOut;
    Port &responseOut;
    Port &ImAcqOut;
    Port &BufOut;
    Port &TimOut;

    int grab;
    int send_image;
    int buf_image;
    int response;

    bool off_board_buffered_grab_image_from_IA;
    bool retrieving_an_image;
    
    int external_bus_speed;
    int IACommand;
    int BUFCommand;
    int TIMCommand;
};  // class FGHostSystemInterface

// ** inline ** // 
inline
string FGHostSystemInterface::className() const
{
    return "FGHostSystemInterface" ;
}

#endif /*_HOST_SYSTEM_INTERFACE_H_*/
