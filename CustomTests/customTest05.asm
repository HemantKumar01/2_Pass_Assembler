; Name: Hemant Kumar
; Roll No: 2301CS20
; test05.asm
; Program to calculate power of 2 using left shifts
; N is stored at 'power' location
; Calculates 2^N

start:  ldc power     ; Load address of power
        ldnl 0        ; Load value of N
        stl 0         ; Store N in local variable
        ldc 1         ; Initialize result to 1
        stl 1         ; Store result
        
        ldl 0         ; Load N
        brlz error    ; If N < 0, go to error
        brz done      ; If N = 0, result is 1

loop:   ldl 0         ; Load current power
        brz done      ; If power = 0, we're done
        
        ldl 1         ; Load current result
        ldc 1         ; Load shift amount (1)
        shl           ; Shift left by 1 (multiply by 2)
        stl 1         ; Store new result
        
        ldl 0         ; Load power
        ldc 1         ; Load 1
        sub           ; Decrement power
        stl 0         ; Store new power
        br loop       ; Repeat

error:  ldc -1        ; Load -1 as error code
        stl 1         ; Store error code

done:   ldl 1         ; Load final result
        HALT          ; Stop program

power:  data 4        ; Calculate 2^4 = 16