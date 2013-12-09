.386
.model flat, stdcall
option casemap :none

includelib msvcrt.lib	;vc运行时环境
printf PROTO C:ptr sbyte, :vararg
scanf PROTO C:ptr sbyte, :vararg
.data

$$printf_char db "%c", 0
$$printf_int db "%d", 0
$$new_line db 10, 13, 0
$$scanf_char db "%c", 0
$$scanf_int db "%d", 0
$$int$4 dd 2
$$int$2 dd 1
$$string$0 db "please input num <10", 0
$$string$1 db "please input number", 0
$$int$3 dd 0

.data?

.code
$$hes@1 PROC
PUSH EBX
PUSH EBP
PUSH ESI
PUSH EDI
SUB ESP, 80
MOV EBP, ESP
PUSH offset $$string$0
CALL printf
ADD ESP, 4
PUSH offset $$new_line
CALL printf
ADD ESP, 4
SUB ESP, 4
PUSH ESP
PUSH offset $$scanf_int
CALL scanf
ADD ESP, 8
POP dword ptr [ebp] + 32
PUSH offset $$string$1
CALL printf
ADD ESP, 4
PUSH offset $$new_line
CALL printf
ADD ESP, 4
MOV EAX, dword ptr [ebp] + 36
MOV EAX, $$int$2
MOV dword ptr [ebp] + 36, EAX
@@label8:
MOV EAX, dword ptr [ebp] + 36
CMP EAX, dword ptr [ebp] + 32
MOV dword ptr [ebp] + 36, EAX
JG @@label9
SUB ESP, 4
PUSH ESP
PUSH offset $$scanf_int
CALL scanf
ADD ESP, 8
POP dword ptr [ebp] + 28
MOV EAX, dword ptr [ebp] + 24
MOV EAX, dword ptr [ebp] + 36
SUB EAX, $$int$2
MOV ECX, dword ptr [ebp] + 20
MOV ECX, EAX
MOV dword ptr [ebp] + 20, ECX
MOV ECX, EBP
ADD ECX, 40
MOV EDX, dword ptr [ebp] + 20
IMUL EDX, 4
ADD ECX, EDX
MOV EBX, dword ptr [ebp] + 28
MOV [ECX], EBX
MOV ECX, dword ptr [ebp] + 16
MOV ECX, dword ptr [ebp] + 36
ADD ECX, $$int$2
MOV ESI, dword ptr [ebp] + 36
MOV ESI, ECX
MOV dword ptr [ebp] + 36, ESI
MOV dword ptr [ebp] + 28, EBX
MOV dword ptr [ebp] + 20, EDX
MOV dword ptr [ebp] + 16, ECX
MOV dword ptr [ebp] + 24, EAX
JMP @@label8
@@label9:
MOV EAX, dword ptr [ebp] + 12
MOV EAX, dword ptr [ebp] + 32
SUB EAX, $$int$2
PUSH EAX
PUSH $$int$3
MOV dword ptr [ebp] + 12, EAX
PUSH EBP
CALL $$qsort@2
MOV EAX, dword ptr [ebp] + 36
MOV EAX, $$int$3
MOV dword ptr [ebp] + 36, EAX
@@label10:
MOV EAX, dword ptr [ebp] + 8
MOV EAX, dword ptr [ebp] + 32
SUB EAX, $$int$2
CMP dword ptr [ebp] + 36, EAX
MOV dword ptr [ebp] + 8, EAX
JG @@label11
MOV EAX, dword ptr [ebp] + 4
MOV EAX, dword ptr [ebp] + 36
MOV dword ptr [ebp] + 4, EAX
MOV EAX, EBP
ADD EAX, 40
MOV ECX, dword ptr [ebp] + 4
IMUL ECX, 4
ADD EAX, ECX
PUSH [EAX]
PUSH offset $$printf_int
MOV dword ptr [ebp] + 4, ECX
CALL printf
ADD ESP, 8
PUSH offset $$new_line
CALL printf
ADD ESP, 4
MOV EAX, dword ptr [ebp]
MOV EAX, dword ptr [ebp] + 36
ADD EAX, $$int$2
MOV ECX, dword ptr [ebp] + 36
MOV ECX, EAX
MOV dword ptr [ebp] + 36, ECX
MOV dword ptr [ebp], EAX
JMP @@label10
@@label11:
@end$$hes@1:
ADD ESP, 80
POP EDI
POP ESI
POP EBP
POP EBX
RET 0
$$hes@1 ENDP

$$qsort@2 PROC
PUSH EBX
PUSH EBP
PUSH ESI
PUSH EDI
SUB ESP, 88
MOV EBP, ESP
MOV EAX, dword ptr [ebp] + 84
MOV EAX, dword ptr [ebp] + 112
MOV ECX, dword ptr [ebp] + 80
MOV ECX, dword ptr [ebp] + 116
MOV EDX, dword ptr [ebp] + 68
MOV EDX, EAX
ADD EDX, ECX
MOV dword ptr [ebp] + 84, EAX
MOV EAX, dword ptr [ebp] + 64
MOV dword ptr [ebp] + 68, EDX
MOV EAX, dword ptr [ebp] + 68
CDQ
IDIV $$int$4
MOV EDX, dword ptr [ebp] + 60
MOV EDX, EAX
MOV dword ptr [ebp] + 64, EAX
MOV EAX, dword ptr [ebp] + 108
ADD EAX, 40
IMUL EDX, 4
ADD EAX, EDX
MOV EBX, dword ptr [ebp] + 56
MOV EBX, [EAX]
MOV EAX, dword ptr [ebp] + 72
MOV EAX, EBX
MOV dword ptr [ebp] + 56, EBX
MOV dword ptr [ebp] + 60, EDX
MOV dword ptr [ebp] + 80, ECX
MOV dword ptr [ebp] + 72, EAX
@@label0:
MOV EAX, dword ptr [ebp] + 52
MOV EAX, dword ptr [ebp] + 84
MOV dword ptr [ebp] + 52, EAX
MOV EAX, dword ptr [ebp] + 108
ADD EAX, 40
MOV ECX, dword ptr [ebp] + 52
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr [ebp] + 48
MOV EDX, [EAX]
CMP EDX, dword ptr [ebp] + 72
MOV dword ptr [ebp] + 48, EDX
MOV dword ptr [ebp] + 52, ECX
JGE @@label1
@@label2:
MOV EAX, dword ptr [ebp] + 44
MOV EAX, dword ptr [ebp] + 84
ADD EAX, $$int$2
MOV ECX, dword ptr [ebp] + 84
MOV ECX, EAX
MOV EDX, dword ptr [ebp] + 40
MOV EDX, ECX
MOV dword ptr [ebp] + 44, EAX
MOV EAX, dword ptr [ebp] + 108
ADD EAX, 40
IMUL EDX, 4
ADD EAX, EDX
MOV EBX, dword ptr [ebp] + 36
MOV EBX, [EAX]
CMP EBX, dword ptr [ebp] + 72
MOV dword ptr [ebp] + 36, EBX
MOV dword ptr [ebp] + 40, EDX
MOV dword ptr [ebp] + 84, ECX
JL @@label2
@@label1:
MOV EAX, dword ptr [ebp] + 32
MOV EAX, dword ptr [ebp] + 80
MOV dword ptr [ebp] + 32, EAX
MOV EAX, dword ptr [ebp] + 108
ADD EAX, 40
MOV ECX, dword ptr [ebp] + 32
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr [ebp] + 28
MOV EDX, [EAX]
CMP dword ptr [ebp] + 72, EDX
MOV dword ptr [ebp] + 28, EDX
MOV dword ptr [ebp] + 32, ECX
JGE @@label3
@@label4:
MOV EAX, dword ptr [ebp] + 24
MOV EAX, dword ptr [ebp] + 80
SUB EAX, $$int$2
MOV ECX, dword ptr [ebp] + 80
MOV ECX, EAX
MOV EDX, dword ptr [ebp] + 20
MOV EDX, ECX
MOV dword ptr [ebp] + 24, EAX
MOV EAX, dword ptr [ebp] + 108
ADD EAX, 40
IMUL EDX, 4
ADD EAX, EDX
MOV EBX, dword ptr [ebp] + 16
MOV EBX, [EAX]
CMP dword ptr [ebp] + 72, EBX
MOV dword ptr [ebp] + 16, EBX
MOV dword ptr [ebp] + 20, EDX
MOV dword ptr [ebp] + 80, ECX
JL @@label4
@@label3:
MOV EAX, dword ptr [ebp] + 84
CMP EAX, dword ptr [ebp] + 80
MOV dword ptr [ebp] + 84, EAX
JG @@label5
MOV EAX, dword ptr [ebp] + 12
MOV EAX, dword ptr [ebp] + 80
MOV dword ptr [ebp] + 12, EAX
MOV EAX, dword ptr [ebp] + 108
ADD EAX, 40
MOV ECX, dword ptr [ebp] + 12
IMUL ECX, 4
ADD EAX, ECX
PUSH EAX
MOV EAX, dword ptr [ebp] + 8
MOV EAX, dword ptr [ebp] + 84
MOV dword ptr [ebp] + 8, EAX
MOV EAX, dword ptr [ebp] + 108
ADD EAX, 40
MOV EDX, dword ptr [ebp] + 8
IMUL EDX, 4
ADD EAX, EDX
PUSH EAX
MOV dword ptr [ebp] + 8, EDX
MOV dword ptr [ebp] + 12, ECX
PUSH dword ptr [ebp] + 108
PUSH EBP
CALL $$swap@3
MOV EAX, dword ptr [ebp] + 4
MOV EAX, dword ptr [ebp] + 84
ADD EAX, $$int$2
MOV ECX, dword ptr [ebp] + 84
MOV ECX, EAX
MOV EDX, dword ptr [ebp]
MOV EDX, dword ptr [ebp] + 80
SUB EDX, $$int$2
MOV EBX, dword ptr [ebp] + 80
MOV EBX, EDX
MOV dword ptr [ebp] + 80, EBX
MOV dword ptr [ebp], EDX
MOV dword ptr [ebp] + 84, ECX
MOV dword ptr [ebp] + 4, EAX
@@label5:
MOV EAX, dword ptr [ebp] + 84
CMP EAX, dword ptr [ebp] + 80
MOV dword ptr [ebp] + 84, EAX
JLE @@label0
MOV EAX, dword ptr [ebp] + 80
CMP EAX, dword ptr [ebp] + 112
MOV dword ptr [ebp] + 80, EAX
JLE @@label6
PUSH dword ptr [ebp] + 80
PUSH dword ptr [ebp] + 112
PUSH dword ptr [ebp] + 108
CALL $$qsort@2
@@label6:
MOV EAX, dword ptr [ebp] + 84
CMP EAX, dword ptr [ebp] + 116
MOV dword ptr [ebp] + 84, EAX
JGE @@label7
PUSH dword ptr [ebp] + 116
PUSH dword ptr [ebp] + 84
PUSH dword ptr [ebp] + 108
CALL $$qsort@2
@@label7:
@end$$qsort@2:
ADD ESP, 88
POP EDI
POP ESI
POP EBP
POP EBX
RET 12
$$qsort@2 ENDP

$$swap@3 PROC
PUSH EBX
PUSH EBP
PUSH ESI
PUSH EDI
SUB ESP, 12
MOV EBP, ESP
MOV EAX, EBP
ADD EAX, 40
MOV EAX, [EAX]
MOV ECX, dword ptr [ebp] + 4
MOV ECX, [EAX]
MOV EAX, dword ptr [ebp] + 8
MOV EAX, ECX
MOV dword ptr [ebp] + 8, EAX
MOV EAX, EBP
ADD EAX, 44
MOV EAX, [EAX]
MOV EDX, dword ptr [ebp]
MOV EDX, [EAX]
MOV dword ptr [ebp] + 4, ECX
MOV ECX, EBP
ADD ECX, 40
MOV ECX, [ECX]
MOV [ECX], EDX
MOV ECX, EBP
ADD ECX, 44
MOV ECX, [ECX]
MOV EAX, dword ptr [ebp] + 8
MOV [ECX], EAX
MOV dword ptr [ebp], EDX
MOV dword ptr [ebp] + 8, EAX
@end$$swap@3:
ADD ESP, 12
POP EDI
POP ESI
POP EBP
POP EBX
RET 16
$$swap@3 ENDP

$$@global@0 PROC
SUB ESP, 0
MOV EBP, ESP
CALL $$hes@1
@end$$@global@0:
ADD ESP, 0
RET 0
$$@global@0 ENDP

END $$@global@0
