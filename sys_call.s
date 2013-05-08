global get_call_num

; according to the parameter passing convention of c language calling 
; assembly language, if the return value of the assembly routine
; is an integer it should be placed in eax. in this case, the number indicate 
; which system call the process is requesting for has already been placed in eax. so, there 
; is nothing to be done here, just an "ret" instruction to return the call_num to the caller.

get_call_num:		
			
	ret;
