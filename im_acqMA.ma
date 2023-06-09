[top]
components : grabber@FGImageAcquisition
out : imageOut
out	: responseOut
in : imageIn
in : commandIn
Link : imageIn imageIn@grabber
Link : commandIn IcommandIn@grabber
Link : imageOut@grabber imageOut
Link : responseOut@grabber responseOut

[grabber]
