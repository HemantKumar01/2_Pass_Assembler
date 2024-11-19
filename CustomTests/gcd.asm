; Name: Hemant Kumar
; Roll No: 2301CS20
; Linear Search

init: ldc num1
ldnl 0
stl 0
ldc num2 
ldnl 0
stl 1

load: ldl 0
ldl 1

loop: sub
brz done
brlz load_reverse
br mod


load_reverse: ldl 1
ldl 0


mod: sub
brlz load_reverse
stl 0
ldl 0
sub
stl 1
br load

done: HALT

num1: data 15
num2: data 18
