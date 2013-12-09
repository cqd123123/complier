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
$$int$0 dd 1
$$int$1 dd 2
$$int$2 dd 0
$$string$3 db "please write a num", 0

.data?

$$temp0@global0 dd ?
$h@global0 dd ?
$jos@1 dd ?
.code
$$jos@1 PROC
PUSH EBX
PUSH EBP
PUSH ESI
PUSH EDI
SUB ESP, 48
MOV EBP, ESP
MOV EAX, dword ptr [ebp] + 68
CMP EAX, $$int$0
MOV dword ptr [ebp] + 68, EAX
JNE @@label0
MOV EAX, $jos@1
MOV EAX, $$int$0
MOV $jos@1, EAX
JMP @@label1
@@label0:
MOV EAX, dword ptr [ebp] + 44
MOV EAX, dword ptr [ebp] + 68
CDQ
IDIV $$int$1
MOV EDX, dword ptr [ebp] + 40
MOV EDX, EAX
IMUL EDX, $$int$1
CMP dword ptr [ebp] + 68, EDX
MOV dword ptr [ebp] + 40, EDX
MOV dword ptr [ebp] + 44, EAX
JNE @@label2
MOV EAX, dword ptr [ebp] + 36
MOV EAX, dword ptr [ebp] + 68
CDQ
IDIV $$int$1
PUSH EAX
MOV dword ptr [ebp] + 36, EAX
CALL $$jos@1
MOV EAX, dword ptr [ebp] + 32
MOV EAX, $jos@1
MOV ECX, $jos@1
MOV ECX, $$int$2
MOV EDX, dword ptr [ebp] + 28
MOV EDX, $$int$1
IMUL EDX, EAX
MOV EBX, dword ptr [ebp] + 24
MOV EBX, EDX
SUB EBX, $$int$0
MOV ECX, EBX
MOV dword ptr [ebp] + 24, EBX
MOV dword ptr [ebp] + 28, EDX
MOV $jos@1, ECX
MOV dword ptr [ebp] + 32, EAX
JMP @@label3
@@label2:
MOV EAX, dword ptr [ebp] + 20
MOV EAX, dword ptr [ebp] + 68
CDQ
IDIV $$int$1
MOV EDX, dword ptr [ebp] + 16
MOV EDX, EAX
IMUL EDX, $$int$1
CMP dword ptr [ebp] + 68, EDX
MOV dword ptr [ebp] + 16, EDX
MOV dword ptr [ebp] + 20, EAX
JLE @@label4
MOV EAX, dword ptr [ebp] + 12
MOV EAX, dword ptr [ebp] + 68
CDQ
IDIV $$int$1
PUSH EAX
MOV dword ptr [ebp] + 12, EAX
CALL $$jos@1
MOV EAX, dword ptr [ebp] + 8
MOV EAX, $jos@1
MOV ECX, $jos@1
MOV ECX, $$int$2
MOV EDX, dword ptr [ebp] + 4
MOV EDX, $$int$1
IMUL EDX, EAX
MOV EBX, dword ptr [ebp]
MOV EBX, EDX
ADD EBX, $$int$0
MOV ECX, EBX
MOV dword ptr [ebp], EBX
MOV dword ptr [ebp] + 4, EDX
MOV $jos@1, ECX
MOV dword ptr [ebp] + 8, EAX
@@label4:
@@label3:
@@label1:
@end$$jos@1:
ADD ESP, 48
POP EDI
POP ESI
POP EBP
POP EBX
RET 4
$$jos@1 ENDP

$$@global@0 PROC
SUB ESP, 0
MOV EBP, ESP
PUSH offset $$string$3
CALL printf
ADD ESP, 4
PUSH offset $$new_line
CALL printf
ADD ESP, 4
@@label5:
SUB ESP, 4
PUSH ESP
PUSH offset $$scanf_int
CALL scanf
ADD ESP, 8
POP dword ptr $h@global0
PUSH dword ptr $h@global0
CALL $$jos@1
MOV EAX, dword ptr $$temp0@global0
MOV EAX, $jos@1
MOV ECX, $jos@1
MOV ECX, $$int$2
MOV $jos@1, ECX
MOV dword ptr $$temp0@global0, EAX
PUSH dword ptr $$temp0@global0
PUSH offset $$printf_int
CALL printf
ADD ESP, 8
PUSH offset $$new_line
CALL printf
ADD ESP, 4
MOV EAX, dword ptr $h@global0
CMP EAX, $$int$2
MOV dword ptr $h@global0, EAX
JGE @@label5
@end$$@global@0:
ADD ESP, 0
RET 0
$$@global@0 ENDP

END $$@global@0
