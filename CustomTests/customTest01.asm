; Name: Hemant Kumar
; Roll No: 2301CS20
; test01.asm
; Program to calculate sum of N numbers
; Numbers are stored starting at 'numbers' location
; N is stored at 'count' location

start:  ldc count     ; Load address of count
        ldnl 0        ; Load value of N
        stl 0         ; Store N in local variable
        ldc 0         ; Initialize sum to 0
        stl 1         ; Store sum in local variable
        ldc 0         ; Initialize counter to 0
        stl 2         ; Store counter

loop:   ldl 2         ; Load counter
        ldl 0         ; Load N
        sub           ; counter - N
        brlz cont     ; If counter < N, continue
        br done       ; Else exit loop

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

done:   ldl 1         ; Load final sum
        HALT          ; Stop program

count:  data 5        ; N = 5
numbers:data 10       ; Array of numbers
        data 20
        data 30
        data 40
        data 51