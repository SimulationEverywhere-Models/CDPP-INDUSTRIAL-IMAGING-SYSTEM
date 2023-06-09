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

#ifndef _IMAGE_ACQUISITION_H_
#define _IMAGE_ACQUISITION_H_

#include "atomic.h"     // class Atomic
#include "fg_codes.h"

#define IA_MODE_OFF     0x0
#define IA_MODE_GRAB    0x1
#define IA_MODE_CONT    0x2

class FGImageAcquisition : public Atomic
{
public:
    FGImageAcquisition( const string &name = "FGImageAcquisition" );                    //Default constructor

    virtual string className() const ;
protected:
    Model &initFunction();
    Model &externalFunction( const ExternalMessage & );
    Model &internalFunction( const InternalMessage & );
    Model &outputFunction( const InternalMessage & );

private:
    const Port &imageIn;
    const Port &IcommandIn;
    Port &imageOut;
    Port &responseOut;
    int size;
    int mode;
    int response;
    int image;
    
};  // class FGImageAcquisition

// ** inline ** // 
inline
string FGImageAcquisition::className() const
{
    return "FGImageAcquisition" ;
}

#endif /*_IMAGE_ACQUISITION_H_*/
