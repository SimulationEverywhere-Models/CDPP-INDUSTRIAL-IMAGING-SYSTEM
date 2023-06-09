[top]
components : timer@FGTimer
out : timerOut
out	: responseOut
in : trigIn
in : commandIn
Link : trigIn trigIn@timer
Link : timerOut@timer timerOut
Link : commandIn TcommandIn@timer
Link : responseOut@timer responseOut

[timer]
