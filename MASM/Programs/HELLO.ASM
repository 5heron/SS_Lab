.model small
.stack 64
.data
msg1 db "Hello World ! $"
.code
mov ax,@data
mov ds,ax

mov dx,offset msg1
mov ah,09h
int 21h

mov ah,4ch
int 21h

end
