[top]
components : interface@FGHostSystemInterface
out : imageOut
out : responseOut
out : buf
out : ia
out : tim
in : commandIn
in : responseIn
in : imageIn

Link : imageOut@interface imageOut
Link : responseOut@interface responseOut
Link : BufOut@interface buf
Link : ImAcqOut@interface ia
Link : TimOut@interface tim
Link : commandIn HcommandIn@interface
Link : responseIn responseIn@interface
Link : imageIn imageIn@interface

[interface]
external_bus_speed : 500.0