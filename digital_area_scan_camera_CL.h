/*******************************************************************
*
*  DESCRIPTION: Atomic Model of a digital area scan CameraLink camera
*
*  AUTHOR: Brian Webb
*
*  EMAIL: mailto://bwebb@connect.carleton.ca
*
*  DATE: 15/10/2007
*
*******************************************************************/

#ifndef __AS_DIG_CAM_CL_H
#define __AS_DIG_CAM_CL_H


#include "atomic.h"     // class Atomic

#define MEGAHURTZ 1000000
#define PATHLEN        50

#define CAM_GRAB_OFF	0
#define CAM_GRAB_START	1
#define CAM_GRAB_ON		2
#define CAM_GRAB_DONE	3

class CameraLinkAreaScanDigitalCamera : public Atomic
{
public:
	CameraLinkAreaScanDigitalCamera( const string &name = "CameraLinkAreaScanDigitalCamera" );					//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &trigIn;   // get trigger signal
	Port &imageDataOut;   // send image index on to what wants it
	Time exposureTime;     // how long it takes for the CCD to expose
	Time transmitTime;		// how long it takes for the CCD to transmit the data
	string configfile;
	bool mode_continuous;
	int Size;
	int currentImage;
	int numImages;
	int grabbing;
	char (*imageList)[PATHLEN];

};	// class CameraLinkAreaScanDigitalCamera

// ** inline ** // 
inline
string CameraLinkAreaScanDigitalCamera::className() const
{
	return "CameraLinkAreaScanDigitalCamera" ;
}

#endif   //__AS_DIG_CAM_CL_H
