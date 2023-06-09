[top]
components : camera@CameraLinkAreaScanDigitalCamera
out : imageDataOut
in : trigIn
Link : imageDataOut@camera imageDataOut
Link : trigIn trigIn@camera

[camera]
mode_continuous : false