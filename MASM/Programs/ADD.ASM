.model small
.stack 64
.data
msg1 db "Enter the number : $"
msg2 db 0ah, "Enter the 2nd number : $"
msg3 db 0ah, "Sum is : $"
.code
mov ax,@data
mov ds,ax

mov dx,offset msg1
mov ah,09h
int 21h

mov ah,01h
int 21h
sub al,30h
mov bl,al

mov dx,offset msg2
mov ah,09h
int 21h

mov ah,01h
int 21h
sub al,30h
add al,bl
add al,30h

mov dx,offset msg3
mov ah,09h
int 21h
mov dl,al

mov ah,02h
int 21h

mov ah,4ch
int 21h

end
