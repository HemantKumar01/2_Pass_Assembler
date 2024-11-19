; Name: Hemant Kumar
; Roll No: 2301CS20
; Linear Search

ldc array
stl 0
ldc 0   ; index counter i
stl 1

loop: ldl 1   ; i
ldc array_size
sub
brlz cont     ; If i < size, continue
br not_found  ; If i >= size, element not found

cont: ldl 0   ; array base address
ldl 1   ; i
add
ldnl 0
ldc target
ldnl 0 
sub
brz found

ldl 1
ldc 1
add
stl 1
br loop

found:      ldl 1   ; Load found index
ldc result
stnl 0 
br end

not_found:  ldc -1  ; Load -1 (not found)
ldc result
stnl 0

end:  HALT

; Data section
array_size: data 5 
target:data 2  
result:data 0 
array: data 3 
       data 7
       data 2
       data 8
       data 4