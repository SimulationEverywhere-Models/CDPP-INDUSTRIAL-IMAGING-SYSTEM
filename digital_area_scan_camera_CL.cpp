/*******************************************************************
*
*  DESCRIPTION: Atomic Model of a digital area scan CameraLink camera
*
*  AUTHOR: Brian Webb
*
*  EMAIL: mailto://bwebb@connect.carleton.ca
*
*  DATE: 16/10/2007
*
*******************************************************************/

/** include files **/
#include "digital_area_scan_camera_CL.h"      // class camera
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include "fg_codes.h"

#include <sstream>

/** public functions **/

/*******************************************************************
* Function Name: CameraLinkAreaScanDigitalCamera
* Description: 
********************************************************************/
CameraLinkAreaScanDigitalCamera::CameraLinkAreaScanDigitalCamera( const string &name )
: Atomic( name )
, trigIn( addInputPort( "trigIn" ) )
, imageDataOut( addOutputPort( "imageDataOut" ) )
, mode_continuous( true )
, configfile( "camera.cfg" )
{
	string mc (MainSimulator::Instance().getParameter( description(), "mode_continuous" ) );
	string mf (MainSimulator::Instance().getParameter( description(), "configfile" ) );
	if ( mc != "" )
	{
		if ( mc == "true" )
		{
			mode_continuous = true;
		}
		else
		{
			mode_continuous = false;
		}
	}
	if ( mf != "" )
	{
		stringstream ss( mf );
		ss >> configfile;
	}
}

/*******************************************************************
* Function Name: initFunction
* Description: Resetea la lista
* Precondition: El tiempo del proximo evento interno es Infinito
********************************************************************/
Model &CameraLinkAreaScanDigitalCamera::initFunction()
{
	FILE *cfg, *im_list;  // config file and image list file
	int imageSizeX;
	int imageSizeY;
	int imageDepth;
	int imageBands;
	int cameraLinkBits;
	int cameraMode;
	int pixelClock;      // in MHz
	int fps;
	int image_size;
	float trans_time;
	float expos_time;

	stringstream configstream ( configfile );
	char file[PATHLEN];

	configstream >> file;
	
	cfg = fopen( file, "r" );
	int i;

	if (cfg == NULL )
	{
		// failed to open file use default settings
		imageSizeX = 640;
		imageSizeY = 480;
		imageDepth = 8;
		imageBands = 1;
		cameraLinkBits = 32;
		pixelClock = 4;
		fps = 30.0;
	}
	else
	{
		fscanf( cfg, "%d\n%d\n%d\n%d\n", &imageSizeX, &imageSizeY, &imageDepth, &imageBands );
		fscanf( cfg, "%d\n%d\n", &cameraLinkBits, &pixelClock );
		fscanf( cfg, "%d\n", &fps );
		fscanf( cfg, "%d\n", &cameraMode );
		fclose( cfg );
		
		if( cameraMode == 1 )
		{
			this->mode_continuous = true;
		}
		else
		{
			this->mode_continuous = false;
		}
	}
	im_list = fopen( "images.txt", "r" );
	if (im_list == NULL)
	{
		this->numImages = 1;
		this->currentImage = 0;
		this->imageList = new char[1][PATHLEN];
		sprintf( imageList[0], "deafult.bmp" );
	}
	else
	{
		fscanf( im_list, "%d\n", &this->numImages );
		this->imageList = new char[numImages][PATHLEN];
		for (i = 0; i < this->numImages; i++ )
		{
			fscanf(im_list, "%s\n", &this->imageList[i] );
		}
		fclose( im_list );
		
	}

	image_size = (imageSizeX * imageSizeY * imageDepth) / 8; // bytes

	expos_time = 1.0 / (float)fps;
	trans_time = (((float)image_size / (float)cameraLinkBits) * (1.0 /(float)(pixelClock * MEGAHURTZ)));

	image_size = image_size / 1024; // KB
	this->Size = image_size << EVENT_DATA_SHIFT;

	this->exposureTime = Time::Time(expos_time);
	this->transmitTime = Time::Time(trans_time);

	this->currentImage = 0;

	if ( this->mode_continuous )
	{
		grabbing = CAM_GRAB_START;
		holdIn( active, Time::Zero );
	}
	else
	{
		grabbing = CAM_GRAB_OFF;
		this->passivate();
	}

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &CameraLinkAreaScanDigitalCamera::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == trigIn  && this->state() == passive) // can't be passive in continuous mode
	{
		if (msg.value() > 0) // -1 is a falling edge which we don't care about
		{
			this->grabbing = CAM_GRAB_START;
			holdIn(active, this->exposureTime ); 
		}
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &CameraLinkAreaScanDigitalCamera::internalFunction( const InternalMessage & )
{
	if (this->state() == active)
	{
		switch (this->grabbing)
		{
			case CAM_GRAB_START :
				this->grabbing = CAM_GRAB_ON;
				holdIn( active, this->exposureTime );
				break;
			case CAM_GRAB_ON :
				this->grabbing = CAM_GRAB_DONE;
				holdIn( active, this->transmitTime );
				break;
			case CAM_GRAB_DONE :
				this->currentImage++;
				if ( this->currentImage >= this->numImages )
				{
					this->currentImage = 0;
				}
				if ( this->mode_continuous )
				{
					this->grabbing = CAM_GRAB_START;
					holdIn( active, Time::Zero );
				}
				else
				{
					this->grabbing = CAM_GRAB_OFF;
					this->passivate();
				}
				break;
			default:  // CAM_GRAB_OFF
				break;
		}
	}
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &CameraLinkAreaScanDigitalCamera::outputFunction( const InternalMessage &msg )
{
	long send_data;

	send_data = this->currentImage + 1;
	send_data = this->Size + send_data;
	switch (this->grabbing)
	{
		case CAM_GRAB_ON :
			sendOutput( msg.time(), this->imageDataOut, send_data );
			break;
		case CAM_GRAB_DONE :
			send_data *= -1;
			sendOutput( msg.time(), this->imageDataOut, send_data );
			break;
		default:
			break;
	}
	return *this ;
}
