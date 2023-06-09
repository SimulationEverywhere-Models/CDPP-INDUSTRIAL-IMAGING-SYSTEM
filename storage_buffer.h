/*******************************************************************
*
*  DESCRIPTION: Atomic Model Frame Grabber Internal Buffer module
*
*  AUTHOR: Brian Webb
*
*  EMAIL: mailto://bwebb@connect.carleton.ca
*
*  DATE: 25/10/2007
*
*******************************************************************/

#ifndef _STORAGE_BUFFER_H_
#define _STORAGE_BUFFER_H_

#include "atomic.h"     // class Atomic
#include "fg_codes.h"

#define LIST_LENGTH 50

class FGStorageBuffer : public Atomic
{
public:
    FGStorageBuffer( const string &name = "FGStorageBuffer" );                    //Default constructor

    virtual string className() const ;
protected:
    Model &initFunction();
    Model &externalFunction( const ExternalMessage & );
    Model &internalFunction( const InternalMessage & );
    Model &outputFunction( const InternalMessage & );

private:
    const Port &BcommandIn;
    Port &imageOut;
    Port &responseOut;
    int bufferList[LIST_LENGTH];
    int bufferSizes[LIST_LENGTH];
    int lastIndex;
    int currentIndex;
    
    int totalSpace;
    int freeSpace;
    
    int response;
    int image;

    float internal_bus_speed;
};  // class FGImageAcquisition

// ** inline ** // 
inline
string FGStorageBuffer::className() const
{
    return "FGStorageBuffer" ;
}

#endif /*_STORAGE_BUFFER_H_*/
