
int __svc(0x00) svc_service_add(int x, int y);
int __svc(0x01) svc_service_subtract(int x, int y);
int __svc(0x02) svc_service_multiply(int x, int y);
int __svc(0x03) svc_service_divide(int x, int y);
void SVC_Handler_C(unsigned int * svc_args);

//x and y hold arguments and z contains result of svc service
int x, y, z;
int main(void){
	x = 1;
	y = 5; 
	z = svc_service_add(x,y);
	
	x = 9;
	y = 2;
	z = svc_service_subtract(x,y);
	
	x = 3, y= 4;
	z = svc_service_multiply(x,y);
	
	x = 12, y = 6;
	z = svc_service_divide(x,y);
	
}

__asm void SVC_Handler(void){
	//Check Bit 2 of LR (EXE_RETURN) to see if Process or Main stack
	TST LR, 0x4
	
	//If then else instruction
	ITE EQ
	
	//If Equal to 0 copy value from main stack pointer MSP into R0
	MRSEQ R0, MSP
	
	//If not equal to 0 copy value from program stack pointer PSP into R0
	MRSNE R0, PSP
	
	//Branch to SVC_handler C version
	B		__cpp(SVC_Handler_C)
}

void SVC_Handler_C(unsigned int * svc_args){
	
	unsigned int svc_number;
	
	//To get 1st byte of 
	svc_number = ((char*)svc_args[6])[-2]; 
	
	switch(svc_number) {
		case 0: 
			//R0 and R1 are parameter passing registers and R0 is return register for ARM processors
			svc_args[0] = svc_args[0] + svc_args[1]; 
			break;
		
		case 1:
			svc_args[0] = svc_args[0] - svc_args[1]; 
			break;
		
		case 2:
			svc_args[0] = svc_args[0] * svc_args[1]; 
			break;
		
		case 3:
			svc_args[0] = svc_args[0] / svc_args[1]; 
			break;
		default:
			break;
	}
}

