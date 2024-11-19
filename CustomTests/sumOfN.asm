; Name: Hemant Kumar
; Roll No: 2301CS20
; test01.asm

start: ldc count
ldnl 0
stl 0
ldc 0 ; init sum to 0
stl 1
ldc 0 ; init i to 0
stl 2

loop:  ldl 2 
ldl 0 
sub 
brlz cont   ; If i < count, continue
br done

cont:  ldc numbers 
ldl 2
add  ; Calculate address of current number
ldnl 0
ldl 1 ; Load current sum
add 
stl 1
ldl 2
ldc 1
add  ; i++
stl 2
br loop

done:  ldl 1 
HALT 

count: data 5
numbers:data 10
data 20
data 30
data 40
data 50