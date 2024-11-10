; Name: Hemant Kumar
; Roll No: 2301CS20
; test04.asm
; Program to find minimum and maximum in array
; Numbers are stored starting at 'numbers' location
; N is stored at 'count' location

start:  ldc count     ; Load address of count
        ldnl 0        ; Load value of N
        stl 0         ; Store N in local variable
        
        ; Initialize min and max with first number
        ldc numbers   ; Load address of first number
        ldnl 0        ; Load first number
        stl 1         ; Store as current min
        stl 2         ; Store as current max
        
        ldc 1         ; Initialize counter to 1
        stl 3         ; Store counter

loop:   ldl 3         ; Load counter
        ldl 0         ; Load N
        sub           ; counter - N
        brlz cont     ; If counter < N, continue
        br done       ; Else exit loop

cont:   ldc numbers   ; Load base address of numbers array
        ldl 3         ; Load counter
        add           ; Calculate address of current number
        ldnl 0        ; Load the number
        stl 4         ; Store current number
        
        ; Check if current number < min
        ldl 4         ; Load current number
        ldl 1         ; Load current min
        sub           ; current - min
        brlz new_min  ; If current < min, update min
        br check_max  ; Else check max

new_min:
        ldl 4         ; Load current number
        stl 1         ; Update min

check_max:
        ldl 4         ; Load current number
        ldl 2         ; Load current max
        sub           ; current - max
        brlz next     ; If current < max, continue to next
        ldl 4         ; Load current number
        stl 2         ; Update max

next:   ldl 3         ; Load counter
        ldc 1         ; Load 1
        add           ; Increment counter
        stl 3         ; Store new counter
        br loop       ; Repeat loop

done:   ldl 1         ; Load min
        stl 5         ; Store min in result
        ldl 2         ; Load max
        stl 6         ; Store max in result
        HALT          ; Stop program

count:  data 5        ; N = 5
numbers:data 30       ; Array of numbers
        data 10
        data 50
        data 20
        data 40
result: data 0        ; Min will be stored here
        data 0        ; Max will be stored here