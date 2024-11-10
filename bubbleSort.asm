; Name: Hemant Kumar
; Roll No: 2301CS20
; Program: Bubble Sort Implementation in Assembly
; Description: Sorts an array in ascending order using bubble sort algorithm

; Initialize stack pointer and program
ldc 1000
a2sp
adj -1
ldc 11        ; Size of array
stl 0 
ldc array     ; Load array address
call sort     ; Call sort subroutine
HALT

; Bubble Sort Subroutine
sort:
    ; Initialize stack frame
    adj -1
    stl 0     ; Save return address
    stl 2     ; Save address of elements
    adj -3

    ; Initialize variables
    ldc 0
    stl 0     ; temp = 0
    ldc 1
    stl 1     ; y = 1
    ldc 0
    stl 2     ; x = 0

    ; Outer loop (x)
loop_out:
    ldl 4     ; Load count
    ldl 2     ; Load x
    sub       ; count - x
    ldc 1
    sub       ; count - 1 - x
    brz done  ; If count-1-x == 0, sorting is done
    
    ; Reset inner loop counter
    ldc 1
    stl 1     ; y = 1

    ; Inner loop (y)
loop_in:
    ; Check inner loop condition
    ldl 4     ; Load count
    ldl 2     ; Load x
    sub       ; count - x
    ldl 1     ; Load y
    sub       ; count - x - y
    brz addx  ; If count-x-y == 0, increment x

    ; Compare adjacent elements
    ldl 5     ; Load array base address
    ldl 1
    ldc 1
    sub
    ldl 5
    add
    stl 6     ; Address of array[y-1]
    
    ldl 1
    ldl 5
    add
    stl 7     ; Address of array[y]

    ; Compare elements
    ldl 7
    ldnl 0    ; Load array[y]
    ldl 6
    ldnl 0    ; Load array[y-1]
    sub       ; array[y] - array[y-1]
    brlz swap ; If array[y] < array[y-1], swap elements
    br addy   ; Else, move to next element

    ; Swap elements if needed
swap:
    ldl 6
    ldnl 0    ; Load array[y-1]
    stl 0     ; temp = array[y-1]
    ldl 7
    ldnl 0    ; Load array[y]
    ldl 6
    stnl 0    ; array[y-1] = array[y]
    ldl 0     ; Load temp
    ldl 7
    stnl 0    ; array[y] = temp

    ; Increment inner loop counter
addy:
    ldc 1
    ldl 1     ; Load y
    add       ; y++
    stl 1     ; Save y
    br loop_in

    ; Increment outer loop counter
addx:
    ldc 1
    ldl 2     ; Load x
    add       ; x++
    stl 2     ; Save x
    ldc 1
    ldl 5
    add       ; array++
    br loop_out

    ; Clean up and return
done:
    ldl 3     ; Load return address
    adj 5     ; Restore stack pointer
    return

; Data section - Initial array values
array:
    data 9
    data 8
    data 7
    data 6
    data 5
    data 4
    data 3
    data 2
    data 1
    data 0
    data 100