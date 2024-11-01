.MODEL SMALL
.STACK 64
.DATA
    msg1 DB "Enter a single alphabet character: $"
    msg_vowel DB 0ah,"It's a vowel.$"
    msg_consonant DB 0ah,"It's a consonant.$"
    msg_invalid DB 0ah,"Invalid input. Please enter an alphabet character.$"
    vowels DB "AEIOUaeiou"

.CODE
    MOV AX, @DATA
    MOV DS, AX

    MOV DX, OFFSET msg1
    MOV AH, 09H
    INT 21H

    MOV AH, 01H
    INT 21H
    MOV CL, AL

CHECK_ALPHABET:
    CMP CL, 'A'
    JB INVALID
    CMP CL, 'Z'
    JBE CHECK
    CMP CL, 'z'
    JA INVALID
    CMP CL, 'a'
    JAE CHECK
    JMP INVALID

CHECK:
    MOV SI, OFFSET vowels 
    MOV CX, 10
CHECK_LOOP:               
    MOV BL, [SI]          
    CMP AL, BL      
    JE DISPLAY_VOWEL    
    INC SI
    LOOP CHECK_LOOP
    JMP DISPLAY_CONSONANT

DISPLAY_VOWEL:
    MOV DX, OFFSET msg_vowel
    MOV AH, 09H
    INT 21H
    JMP END_PROGRAM

DISPLAY_CONSONANT:
    MOV DX, OFFSET msg_consonant
    MOV AH, 09H
    INT 21H
    JMP END_PROGRAM

INVALID:
    MOV DX, OFFSET msg_invalid
    MOV AH, 09H
    INT 21H

END_PROGRAM:
    MOV AH, 4CH
    INT 21H
END