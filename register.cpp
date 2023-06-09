/*******************************************************************
*
*  DESCRIPTION: Simulator::registerNewAtomics()
*
*  AUTHOR: Rujun Hu
*
*  STUDENT#: 258902
*
*  EMAIL: rujun_hu@yahoo.ca
*
*  DATE: 16/10/2001
*
*  MODIFIED:  For industrial imaging simulator by Brian Webb 18/10/2007
*******************************************************************/

#include "modeladm.h" 
#include "mainsimu.h"
#include "digital_area_scan_camera_CL.h" // class CameraLinkAreaScanDigitalCamera
#include "storage_buffer.h"
#include "timer.h"
#include "image_acquisition.h"
#include "host_system_interface.h"

void MainSimulator::registerNewAtomics()
{
 
	SingleModelAdm::Instance().registerAtomic( \
		NewAtomicFunction<CameraLinkAreaScanDigitalCamera>(), \
		"CameraLinkAreaScanDigitalCamera" ) ; 
	SingleModelAdm::Instance().registerAtomic( \
		NewAtomicFunction<FGStorageBuffer>(), "FGStorageBuffer" );
	SingleModelAdm::Instance().registerAtomic( \
		NewAtomicFunction<FGTimer>(), "FGTimer" );
	SingleModelAdm::Instance().registerAtomic( \
		NewAtomicFunction<FGImageAcquisition>(), "FGImageAcquisition" );
	SingleModelAdm::Instance().registerAtomic( \
		NewAtomicFunction<FGHostSystemInterface>(), "FGHostSystemInterface" );
} 