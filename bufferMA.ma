[top]
components : buffer@FGStorageBuffer
out : imageOut
out	: responseOut
in : commandIn
Link : commandIn BcommandIn@buffer
Link : imageOut@buffer imageOut
Link : responseOut@buffer responseOut

[buffer]
totalSpace : 700
internal_bus_speed : 500.0
