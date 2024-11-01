.model small
.stack 64
.data
msg1 db "Enter the character :$"
msg2 db 0ah, "Character is : $"
.code
mov ax,@data
mov ds,ax

mov dx,offset msg1
mov ah,09h
int 21h

mov ah,01h
int 21h

mov dx,offset msg2
mov ah,09h
int 21h

mov dl,al
mov ah,02h
int 21h

mov ah,4ch
int 21h

end
