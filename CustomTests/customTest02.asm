; Name: Hemant Kumar
; Roll No: 2301CS20
; test02.asm
; Program to multiply two numbers using repeated addition
; Numbers are stored in 'num1' and 'num2' locations

start:  ldc num1      ; Load address of first number
        ldnl 0        ; Load value of num1
        stl 0         ; Store num1 in local variable
        ldc num2      ; Load address of second number
        ldnl 0        ; Load value of num2
        stl 1         ; Store num2 in local variable
        ldc 0         ; Initialize result to 0
        stl 2         ; Store result
        
        ldl 1         ; Load num2
        brlz neg_mult ; If num2 < 0, handle negative multiplication
        br mult_loop  ; Else proceed with positive multiplication

neg_mult:
        ldc 0         ; Load 0
        ldl 1         ; Load num2
        sub           ; Calculate -num2
        stl 1         ; Store positive num2
        br mult_loop  ; Proceed with multiplication

mult_loop:
        ldl 1         ; Load counter (num2)
        brz done      ; If counter = 0, multiplication complete
        
        ldl 2         ; Load current result
        ldl 0         ; Load num1
        add           ; Add num1 to result
        stl 2         ; Store new result
        
        ldl 1         ; Load counter
        ldc 1         ; Load 1
        sub           ; Decrement counter
        stl 1         ; Store new counter
        br mult_loop  ; Repeat

done:   ldl 2         ; Load final result
        HALT          ; Stop program

num1:   data 5        ; First number
num2:   data 4        ; Second number