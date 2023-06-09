[top]
components : camera@CameraLinkAreaScanDigitalCamera
components : FrameGrabber

out : imageBufOut respOut
in : triggerIn cmndIn

Link : triggerIn trigIn@FrameGrabber
Link : cmndIn cIn@FrameGrabber
Link : responseOut@FrameGrabber respOut
Link : imageOut@FrameGrabber imageBufOut
Link : timerOut@FrameGrabber trigIn@camera
Link : imageDataOut@camera imageIn@FrameGrabber

[camera]
mode_continuous : true
configfile : slow.cfg

[FrameGrabber]
components : interface@FGHostSystemInterface im_acq@FGImageAcquisition timer@FGTimer buffer@FGStorageBuffer

in : trigIn imageIn cIn
out : imageOut responseOut timerOut

Link : imageIn imageIn@im_acq

Link : trigIn trigIn@timer
Link : timerOut@timer timerOut

Link : imageOut@buffer imageIn@interface
Link : imageOut@im_acq imageIn@interface

Link : responseOut@buffer responseIn@interface
Link : responseOut@im_acq responseIn@interface
Link : responseOut@timer responseIn@interface

Link : TimOut@interface TcommandIn@timer
Link : BufOut@interface BcommandIn@buffer
Link : ImAcqOut@interface IcommandIn@im_acq

Link : imageOut@interface imageOut
Link : cIn HcommandIn@interface
Link : responseOut@interface responseOut


[buffer]
totalSpace : 700
internal_bus_speed : 10000.0

[interface]
external_bus_speed : 10000.0

[timer]

[im_acq]