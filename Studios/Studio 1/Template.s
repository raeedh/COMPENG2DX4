;Template - Defining an Assembly File 
;
; McMaster 2DX4 
; Read Text Chapters 3 and 4 for more info
 
; Original: Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu


;ADDRESS SETUP
;Define your I/O Port Addresses Here

SYSCTL_RCGCGPIO_R             EQU     0x400FE608         ;General-Purpose Input/Output Run Mode Clock Gating Control Register
GPIO_PORTF_DIR_R              EQU     0x4005D400		 ;GPIO Port F DIR Register
GPIO_PORTF_DEN_R              EQU     0x4005D51C         ;GPIO Port F DEN Register
GPIO_PORTF_DATA_R             EQU     0x4005D3FC         ;GPIO Port F DATA Register 

                              



        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
        EXPORT Start

;Function PortF_Init
PortF_Init 
		;STEP 1
		 LDR R1, =SYSCTL_RCGCGPIO_R
		 LDR R0, [R1]
		 ORR R0,R0, #0x20
		 STR R0, [R1]
		 NOP
		 NOP
		
		;STEP 5
		 LDR R1, =GPIO_PORTF_DIR_R
		 LDR R0, [R1]
		 ORR R0,R0, #0x10
		 STR R0, [R1]
		;STEP 7
		 LDR R1, =GPIO_PORTF_DEN_R
		 LDR R0, [R1]
		 ORR R0,R0, #0x10
		 STR R0, [R1]
        BX LR               ; return from function 
       
Start 
	    BL PortF_Init       ; calls and execute your PortN_Init function
		;STEP 8    
         LDR R1, =GPIO_PORTF_DATA_R
		 LDR R0, [R1]
		 ORR R0,R0, #0x10
		 STR R0, [R1]
		ALIGN               ; directive for assembly			
        END                 ; End of function 