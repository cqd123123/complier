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
SUB ESP, 48
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
POP ESI
PUSH offset $$string$1
CALL printf
ADD ESP, 4
PUSH offset $$new_line
CALL printf
ADD ESP, 4
MOV EDI, $$int$2
@@label8:
CMP EDI, ESI
JG @@label9
SUB ESP, 4
PUSH ESP
PUSH offset $$scanf_int
CALL scanf
ADD ESP, 8
POP dword ptr [ebp] + 4
MOV EBX, EDI
SUB EBX, $$int$2
MOV EAX, dword ptr [ebp]
MOV EAX, EBX
MOV ECX, EBP
ADD ECX, 8
IMUL EAX, 4
ADD ECX, EAX
MOV EDX, dword ptr [ebp] + 4
MOV [ECX], EDX
MOV EBX, EDI
ADD EBX, $$int$2
MOV EDI, EBX
JMP @@label8
@@label9:
MOV EBX, ESI
SUB EBX, $$int$2
PUSH EBX
PUSH $$int$3
PUSH EBP
CALL $$qsort@2
MOV EDI, $$int$3
@@label10:
MOV EBX, ESI
SUB EBX, $$int$2
CMP EDI, EBX
JG @@label11
MOV EBX, EDI
MOV EAX, EBP
ADD EAX, 8
IMUL EBX, 4
ADD EAX, EBX
PUSH [EAX]
PUSH offset $$printf_int
CALL printf
ADD ESP, 8
PUSH offset $$new_line
CALL printf
ADD ESP, 4
MOV EBX, EDI
ADD EBX, $$int$2
MOV EDI, EBX
JMP @@label10
@@label11:
@end$$hes@1:
ADD ESP, 48
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
SUB ESP, 72
MOV EBP, ESP
MOV ESI, dword ptr [ebp] + 96
MOV EDI, dword ptr [ebp] + 100
MOV EAX, dword ptr [ebp] + 68
MOV EAX, ESI
ADD EAX, EDI
MOV dword ptr [ebp] + 68, EAX
MOV EAX, dword ptr [ebp] + 64
MOV EAX, dword ptr [ebp] + 68
CDQ
IDIV $$int$4
MOV EDX, dword ptr [ebp] + 60
MOV EDX, EAX
MOV EAX, dword ptr [ebp] + 92
ADD EAX, 8
IMUL EDX, 4
ADD EAX, EDX
MOV ECX, dword ptr [ebp] + 56
MOV ECX, [EAX]
MOV EBX, ECX
@@label0:
MOV EAX, dword ptr [ebp] + 52
MOV EAX, ESI
MOV dword ptr [ebp] + 52, EAX
MOV EAX, dword ptr [ebp] + 92
ADD EAX, 8
MOV ECX, dword ptr [ebp] + 52
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr [ebp] + 48
MOV EDX, [EAX]
CMP EDX, EBX
MOV dword ptr [ebp] + 48, EDX
JGE @@label1
@@label2:
MOV EAX, dword ptr [ebp] + 44
MOV EAX, ESI
ADD EAX, $$int$2
MOV ESI, EAX
MOV ECX, dword ptr [ebp] + 40
MOV ECX, ESI
MOV EAX, dword ptr [ebp] + 92
ADD EAX, 8
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr [ebp] + 36
MOV EDX, [EAX]
CMP EDX, EBX
MOV dword ptr [ebp] + 36, EDX
JL @@label2
@@label1:
MOV EAX, dword ptr [ebp] + 32
MOV EAX, EDI
MOV dword ptr [ebp] + 32, EAX
MOV EAX, dword ptr [ebp] + 92
ADD EAX, 8
MOV ECX, dword ptr [ebp] + 32
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr [ebp] + 28
MOV EDX, [EAX]
CMP EBX, EDX
MOV dword ptr [ebp] + 28, EDX
JGE @@label3
@@label4:
MOV EAX, dword ptr [ebp] + 24
MOV EAX, EDI
SUB EAX, $$int$2
MOV EDI, EAX
MOV ECX, dword ptr [ebp] + 20
MOV ECX, EDI
MOV EAX, dword ptr [ebp] + 92
ADD EAX, 8
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr [ebp] + 16
MOV EDX, [EAX]
CMP EBX, EDX
MOV dword ptr [ebp] + 16, EDX
JL @@label4
@@label3:
CMP ESI, EDI
JG @@label5
MOV EAX, dword ptr [ebp] + 12
MOV EAX, EDI
MOV dword ptr [ebp] + 12, EAX
MOV EAX, dword ptr [ebp] + 92
ADD EAX, 8
MOV ECX, dword ptr [ebp] + 12
IMUL ECX, 4
ADD EAX, ECX
PUSH EAX
MOV EAX, dword ptr [ebp] + 8
MOV EAX, ESI
MOV dword ptr [ebp] + 8, EAX
MOV EAX, dword ptr [ebp] + 92
ADD EAX, 8
MOV EDX, dword ptr [ebp] + 8
IMUL EDX, 4
ADD EAX, EDX
PUSH EAX
PUSH dword ptr [ebp] + 92
PUSH EBP
CALL $$swap@3
MOV EAX, dword ptr [ebp] + 4
MOV EAX, ESI
ADD EAX, $$int$2
MOV ESI, EAX
MOV ECX, dword ptr [ebp]
MOV ECX, EDI
SUB ECX, $$int$2
MOV EDI, ECX
@@label5:
CMP ESI, EDI
JLE @@label0
CMP EDI, dword ptr [ebp] + 96
JLE @@label6
PUSH EDI
PUSH dword ptr [ebp] + 96
PUSH dword ptr [ebp] + 92
CALL $$qsort@2
@@label6:
CMP ESI, dword ptr [ebp] + 100
JGE @@label7
PUSH dword ptr [ebp] + 100
PUSH ESI
PUSH dword ptr [ebp] + 92
CALL $$qsort@2
@@label7:
@end$$qsort@2:
ADD ESP, 72
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
SUB ESP, 0
MOV EBP, ESP
MOV EAX, EBP
ADD EAX, 28
MOV EAX, [EAX]
MOV EBX, [EAX]
MOV EDI, EBX
MOV EAX, EBP
ADD EAX, 32
MOV EAX, [EAX]
MOV EBX, [EAX]
MOV ECX, EBP
ADD ECX, 28
MOV ECX, [ECX]
MOV [ECX], EBX
MOV ECX, EBP
ADD ECX, 32
MOV ECX, [ECX]
MOV [ECX], EDI
@end$$swap@3:
ADD ESP, 0
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
