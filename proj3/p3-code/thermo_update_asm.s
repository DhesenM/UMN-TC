## set_temp_from_ports
.text
.global  set_temp_from_ports

## The function takes a single argument, a pointer in rdi.     
set_temp_from_ports:
        
        movzwl THERMO_SENSOR_PORT(%rip), %eax
        cmpl    $64000, %eax                    ## jump to Ret1 if THERMO_SENSOR_PORT > 64000
        jg      .Ret1
        cmpl    $0,     %eax                    ## jump to Ret1 if THERMO_SENSOR_PORT < 0
        jl      .Ret1

        sarl    $6,     %eax                    ## eax = -500 + (eax / 64)
        subl    $500,   %eax                         

        movzwl THERMO_SENSOR_PORT(%rip), %r8d   ## r8d = THERMO_SENSOR_PORT
        andl    $63,    %r8d                    ## find the remainder = dx & (64 - 1)
        cmpl    $31,    %r8d                    ## jump to Status_port if the remainder < 31
        jl      .Status_port

.Increment:                                     ## eax += 1
        incl    %eax

.Status_port:
        movb THERMO_STATUS_PORT(%rip), %cl      ## cl = THERMO_STATUS_PORT
        andb    $1,     %cl                     ## find the remainder = cl & (2 - 1)
        cmpb    $1,     %cl                     ## jump to Sensor1 if the remainder == 1
        je      .Status_1

        movw    %ax,    0(%rdi)                 ## assign %ax to field tenths_degrees
        movb    $0,     2(%rdi)                 ## assign 0 to field is_fahrenheit
        movl    $0,     %eax                    ## eax = 0
        ret

.Status_1:                                      ## eax = (9 * eax) / 5 + 320
        imull   $9,     %eax                    ## eax *= 9
        cltq
        cqto
        movl    $5,     %ecx                           
        idivl   %ecx                            ## eax /= 5
        addl    $320,   %eax                    ## eax += 320
        movw    %ax,    0(%rdi)                 ## assign %ax to field tenths_degrees
        movb    $1,     2(%rdi)                 ## assign 1 to field is_fahrenheit
        movl    $0,     %eax                    ## eax = 0
        ret

.Ret1:                                          ## eax = 1 and return eax
        movl    $1,     %eax
        ret

## set global array 0-9
.data
digit:
        .int 0b1111110                          ## digit 0
        .int 0b0001100                          ## digit 1
        .int 0b0110111                          ## digit 2 
        .int 0b0011111                          ## digit 3
        .int 0b1001101                          ## digit 4
        .int 0b1011011                          ## digit 5
        .int 0b1111011                          ## digit 6
        .int 0b0001110                          ## digit 7
        .int 0b1111111                          ## digit 8 
        .int 0b1011111                          ## digit 9

## a packed temp struct in rdi
## an integer pointer in rsi
.text
.global  set_display_from_temp
set_display_from_temp:  

        movq    $0,     %rcx                    ## initialize rcx
        movw    %di,    %cx                     ## cx = di
        sarq    $16,    %rdi                    ## find whether if the degree is Fahrenheit 

        cmpb    $1,     %dil                    ## jump to Fahrenheit_1 if dil = 1
        je      .Fahrenheit_1
        cmpb    $0,     %dil                    ## jump to Fahrenheit_0 if dil = 0
        je      .Fahrenheit_0
        jmp     .Ret_1                          ## otherwise return 1

.Fahrenheit_1:
        cmpw    $1220,  %cx                     ## return 1 if cx > 1220
        jg      .Ret_1
        cmpw    $-1220, %cx                     ## return 1 if cx < -1220
        jl      .Ret_1
        jmp     .Continue                       ## jump to Continue

.Fahrenheit_0:
        cmpw    $500,   %cx                     ## return 1 if cx > 500     
        jg      .Ret_1
        cmpw    $-500,  %cx                     ## return 1 if cx < -500
        jl      .Ret_1

.Continue:
        movq    $0,     %r11                    ## initialize %r11
        movq    $0,     %r8                     ## initialize %r8
        cmpw    $0,     %cx                     ## add the negative sign if cx < 0 
        jge     .TempDigits                     ## jump to TempDigits otherwise
        negw    %cx                     

.Negative:  
        movb    $1,     %r11b                   ## add the negative sign in display
        orq     %r11,   %r8                      
        salq    $7,     %r8                                          
           
.TempDigits:
        ## initialize each digit of the degree
        movq    %rcx,   %rax                    ## the remainder after dividing 10 to find each digit
        cqto
        movq    $10,    %r10
        idivq   %r10
        movq    %rdx,   %r12                    ## r12 = temp_tenths
        cqto
        idivq   %r10
        movq    %rdx,   %r13                    ## r13 = temp_ones
        cqto
        idivq   %r10
        movq    %rdx,   %r14                    ## r14 = temp_tens
        cqto
        idivq   %r10
        movq    %rdx,   %r15                    ## r15 = temp_hundreds

        ## index every digit
        leaq    digit(%rip), %r10               ## use the digit in data

        cmpl    $0,     %r15d                   ## jump to Temp_Hundreds if the hundredth digit is unequal to 0
        jne     .Temp_Hundreds

        cmpl    $0,     %r14d                   ## jump to Temp_Tens if the tenth digit is unequal to 0
        jne     .Temp_Tens

        jmp     .Temp_Ones                      ## otherwise jump to the ones digit

.Temp_Hundreds:
        movq    (%r10,%r15,4),  %r11            ## find the hundreds digit and move left by 7 bits
        orq     %r11,           %r8
        salq    $7,             %r8

.Temp_Tens:                                     ## find the tens digit and move left by 7 bits
        movq    (%r10,%r14,4),  %r11   
        orq     %r11,           %r8
        salq    $7,             %r8

.Temp_Ones:                                     ## find the ones digit and move left by 7 bits
        movq    (%r10,%r13,4),  %r11
        orq     %r11,           %r8
        salq    $7,             %r8             ## find the tenths digit and move left by 7 bits
        movq    (%r10,%r12,4),  %r11
        orq     %r11,           %r8

.ForC:                                          ## jump to Celsius if dil = 1, otherwise continue to Fahrenheit
        cmpb    $0,     %dil
        je      .Celsius

.Fahrenheit:
        movq    $1,     %r10                    ## put number 1 in the 28th and jump to Display
        salq    $29,    %r10
        orq     %r10,   %r8
        jmp     .Display

.Celsius:                                       ## put number 1 in the 29th and jump to Display
        movq    $1,     %r10
        salq    $28,    %r10
        orq     %r10,   %r8

.Display:                                       ## pointer (%rsi) = %r8d (the display)
        movl    %r8d,   (%rsi)
        movl    $0,     %eax                    ## eax = 0 ad return
        ret

.Ret_1:
        movl    $1,     %eax                    ## eax = 1 ad return
        ret

.text
.global thermo_update  
thermo_update:
        subq    $20,      %rsp                  ## align spaces for stack
	movw    $0,      (%rsp)                 ## set the value of temp
        movb    $64,    2(%rsp)
        movq    %rsp,     %rdi                  ## rdi = rsp

        call    set_temp_from_ports             ## call set_temp_from_ports
        cmpl    $1,       %eax                  ## jump to Error if fails
        je      .Error

        movq    (%rdi),   %rdi                  ## rdi = *(rdi)
        leaq    THERMO_DISPLAY_PORT(%rip), %rsi ## find the display

        call    set_display_from_temp           ## call set_display_from_temp
        cmpl    $1,       %eax                  ## jump to Error if fails
        je      .Error

        addq    $20,      %rsp                  ## add the spaces back the stack
        movl    $0,       %eax                  ## eax = 0 and return
        ret

.Error:
        movl    $1,       %eax                  ## eax = 1 and return
        addq    $20,      %rsp                  ## add the spaces back the stack
        ret
