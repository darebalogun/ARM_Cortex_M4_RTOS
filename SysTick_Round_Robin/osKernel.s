	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXTERN current
	EXPORT SysTick_Handler
	EXPORT osSchedulerLaunch				

SysTick_Handler						; Save remaining registers not saved automatically
	CPSID	I
	PUSH	{R4-R11}				
	LDR		R0, =current				; Store current thread pointer in R0
	LDR		R1, [R0]					; Store current thread pointer in R1
	STR		SP, [R1]					; Store current thread pointer in SP register
	LDR		R1, [R1,#4]				; Store next pointer in R1
	STR		R1, [R0]					; Store R1 in current i.e point to next thread
	LDR		SP, [R1]					; Store stack pointer of current thread in SP
	POP		{R4-R11}				; Restore manually saved registers
	CPSIE	I
	BX		LR
	
osSchedulerLaunch
	LDR		R0, =current
	LDR		R2, [R0]					; Store current thread pointer in R2
	LDR		SP, [R2]					; Store stack pointer of current thread in SP register
	POP		{R4-R11}
	POP		{R0-R3}
	POP		{R12}
	ADD		SP,SP,#4
	POP		{LR}
	ADD		SP,SP,#4
	CPSIE	I
	BX		LR
	
	ALIGN
	END