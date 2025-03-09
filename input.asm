a:
.asciz "abcdef12Aajfhjlddddddd"
 .half 0 , 20,30    #data
# text segment
.text
label2 :    jal x1, label1
c:# comment
.dword 9223372036854775807
addi x1, x2, 10
d :  beq x2,x3,label3
andi x3, x4, 255
ori x5, x6, 7
label3: 
.data
#
 b: .dword 1 
.text
lb x8, 100(x9)
ld x10, 200(x11)
lh x12, 300(x13)
lw x14, 400(x15)
jalr x16,200(x17)
add  x3, x1, x2
and  x3, x1, x2
or   x3, x1, x2
sll  x3, x1, x2
beq x2,x3,label2
slt  x3, x1, x2
sra  x3, x1, x2
srl  x3, x1, x2
sub  x3, x1, x2
xor  x3, x1, x2
mul  x3, x1, x2
div  x3, x1, x2
rem  x3, x1, x2
label1:
sb x8, 100(x9)
sd x10, 200(x11)
sh x12, 300(x13)
sw x14, 400(x15)
beq x2,x3,label1
bne x10,x11,label2
blt x20,x31,label3
bge x0,x30,label1
lui x15,123456
auipc x20,1234
exit:
.data
.half 5 6