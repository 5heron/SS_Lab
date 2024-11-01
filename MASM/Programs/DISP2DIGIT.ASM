.model small
.stack 64
.data
msg1 db "Enter 1st number: $"
msg2 db 0ah, "Enter 2nd number: $"
msg3 db 0ah, "Number is: $"

.code
mov ax,@data
mov ds,ax

mov dx,offset msg1
mov ah,09h
int 21h

mov ah,01h
int 21h
sub al,30h
mov bl,0ah
mul bl
mov bl,al

mov dx,offset msg2
mov ah,09h
int 21h

mov ah,01h
int 21h
sub al,30h
add al,bl
mov bl,0ah
mov ah,00h
div bl
add al,30h
add ah,30h
mov bl,ah

mov dx,offset msg3
mov ah,09h
int 21h
mov ah,02h
mov dl,al
int 21h
mov al,bl
mov ah,02h
mov dl,al
int 21h
mov ah,4ch
int 21h
end
