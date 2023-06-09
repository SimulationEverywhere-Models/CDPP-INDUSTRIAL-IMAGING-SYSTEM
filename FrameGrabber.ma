[top]
components : interface@FGHostSystemInterface
components : im_acq@FGImageAcquisition
components : timer@FGTimer
components : buffer@FGStorageBuffer

in : commandIn
in : trigIn
in : imageIn
out : imageOut
out : responseOut
out : timerOut

Link : commandIn HcommandIn@interface
Link : imageIn imageIn@im_acq
Link : imageOut@im_acq imageIn@interface
Link : trigIn trigIn@timer

Link : imageOut@interface imageOut
Link : responseOut@interface responseOut
Link : timerOut@timer timerOut

Link : responseOut@im_acq responseIn@interface
Link : responseOut@timer responseIn@interface
Link : responseOut@buffer responseIn@interface

Link : BufOut@interface BcommandIn@buffer
Link : ImAcqOut@interface IcommandIn@im_acq
Link : TimOut@interface TcommandIn@timer

Link : imageOut@buffer imageIn@interface

[buffer]
totalSpace : 700
internal_bus_speed : 500.0

[interface]
external_bus_speed : 500.0

[timer]

[im_acq]
