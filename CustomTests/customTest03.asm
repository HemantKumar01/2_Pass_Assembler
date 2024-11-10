; Name: Hemant Kumar
; Roll No: 2301CS20
; test03.asm
; Program to calculate average of N numbers using bit shifts
; This will calculate integer average by dividing by power of 2
; Numbers are stored starting at 'numbers' location
; N must be a power of 2 (e.g., 2, 4, 8, 16)

start:  ldc count     ; Load address of count
        ldnl 0        ; Load value of N (must be power of 2)
        stl 0         ; Store N in local variable
        ldc 0         ; Initialize sum to 0
        stl 1         ; Store sum in local variable
        ldc 0         ; Initialize counter to 0
        stl 2         ; Store counter

; Calculate sum
loop:   ldl 2         ; Load counter
        ldl 0         ; Load N
        sub           ; counter - N
        brlz cont     ; If counter < N, continue
        br avg        ; Else calculate average

cont:   ldc numbers   ; Load base address of numbers array
        ldl 2         ; Load counter
        add           ; Calculate address of current number
        ldnl 0        ; Load the number
        ldl 1         ; Load current sum
        add           ; Add number to sum
        stl 1         ; Store new sum
        ldl 2         ; Load counter
        ldc 1         ; Load 1
        add           ; Increment counter
        stl 2         ; Store new counter
        br loop       ; Repeat loop

; Calculate average by right shifting (dividing by power of 2)
avg:    ldl 1         ; Load sum
        ldc 2         ; Load shift amount (2 for divide by 4)
        shr           ; Shift right to divide by 4
        HALT          ; Stop program

count:  data 4        ; N = 4 (must be power of 2)
numbers:data 10       ; Array of numbers
        data 20
        data 30
        data 40