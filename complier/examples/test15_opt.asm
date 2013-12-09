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
$$int$0 dd 0
$$int$1 dd 9
$$int$2 dd 1
$$int$3 dd 5
$$int$4 dd 2
$$int$5 dd 3
$$int$6 dd 4
$$string$7 db "final:", 0

.data?

.code
$$ge@1 PROC
PUSH EBX
PUSH EBP
PUSH ESI
PUSH EDI
SUB ESP, 140
MOV EBP, ESP
MOV EDI, $$int$0
@@label0:
CMP EDI, $$int$1
JG @@label1
MOV EBX, EDI
MOV ECX, EBP
ADD ECX, 20
IMUL EBX, 4
ADD ECX, EBX
MOV [ECX], EDI
MOV EBX, EDI
ADD EBX, $$int$2
MOV EDI, EBX
JMP @@label0
@@label1:
MOV EDI, $$int$0
@@label2:
CMP EDI, $$int$1
JG @@label3
MOV ESI, EDI
MOV EAX, EBP
ADD EAX, 20
IMUL ESI, 4
ADD EAX, ESI
MOV EBX, [EAX]
MOV ECX, EBP
ADD ECX, 60
IMUL EBX, 4
ADD ECX, EBX
MOV [ECX], EDI
MOV EBX, EDI
ADD EBX, $$int$2
MOV EDI, EBX
JMP @@label2
@@label3:
MOV EDI, $$int$0
@@label4:
CMP EDI, $$int$1
JG @@label5
MOV EBX, EDI
MOV EAX, EBP
ADD EAX, 20
IMUL EBX, 4
ADD EAX, EBX
MOV ESI, [EAX]
MOV EAX, EBP
ADD EAX, 60
IMUL ESI, 4
ADD EAX, ESI
MOV EBX, [EAX]
MOV ECX, EBP
ADD ECX, 100
IMUL EBX, 4
ADD ECX, EBX
MOV [ECX], EDI
MOV EBX, EDI
ADD EBX, $$int$2
MOV EDI, EBX
JMP @@label4
@@label5:
MOV ESI, $$int$3
MOV EBX, $$int$2
MOV EAX, EBP
ADD EAX, 20
IMUL EBX, 4
ADD EAX, EBX
MOV EDI, [EAX]
MOV EAX, EBP
ADD EAX, 20
IMUL ESI, 4
ADD EAX, ESI
MOV EBX, [EAX]
MOV EDX, EBP
ADD EDX, 60
IMUL EDI, 4
ADD EDX, EDI
MOV ESI, EBX
IMUL ESI, [EDX]
MOV EBX, $$int$4
MOV EAX, EBP
ADD EAX, 20
IMUL EBX, 4
ADD EAX, EBX
MOV EDX, dword ptr [ebp] + 16
MOV EDX, [EAX]
MOV EAX, dword ptr [ebp] + 12
MOV EAX, $$int$4
MOV dword ptr [ebp] + 12, EAX
MOV EAX, EBP
ADD EAX, 60
IMUL EDX, 4
ADD EAX, EDX
MOV EDI, [EAX]
MOV EDX, EBP
ADD EDX, 20
MOV EAX, dword ptr [ebp] + 12
IMUL EAX, 4
ADD EDX, EAX
MOV EBX, EDI
IMUL EBX, [EDX]
MOV EDX, dword ptr [ebp] + 8
MOV EDX, EBX
MOV ECX, dword ptr [ebp] + 4
MOV ECX, $$int$4
MOV EAX, dword ptr [ebp]
MOV EAX, $$int$2
MOV dword ptr [ebp], EAX
MOV EAX, EBP
ADD EAX, 100
IMUL ECX, 4
ADD EAX, ECX
MOV EDI, [EAX]
MOV dword ptr [ebp] + 8, EDX
MOV EDX, EBP
ADD EDX, 100
MOV EAX, dword ptr [ebp]
IMUL EAX, 4
ADD EDX, EAX
MOV EBX, EDI
SUB EBX, [EDX]
MOV ECX, EBP
ADD ECX, 100
MOV EDX, dword ptr [ebp] + 8
IMUL EDX, 4
ADD ECX, EDX
MOV EAX, EDI
MOV dword ptr [ebp] + 8, EDX
MOV EAX, dword ptr [ECX]
CDQ
IDIV EBX
MOV EBX, ESI
ADD EBX, EAX
MOV ESI, $$int$4
MOV EDX, EBP
ADD EDX, 60
IMUL ESI, 4
ADD EDX, ESI
MOV EDI, EBX
SUB EDI, [EDX]
MOV EBX, $$int$5
MOV EDX, EBP
ADD EDX, 60
IMUL EBX, 4
ADD EDX, EBX
MOV ESI, EDI
ADD ESI, [EDX]
MOV EBX, $$int$6
MOV EDX, EBP
ADD EDX, 60
IMUL EBX, 4
ADD EDX, EBX
MOV EDI, ESI
SUB EDI, [EDX]
MOV EBX, EDI
ADD EBX, $$int$2
MOV ESI, EBX
MOV EDI, $$int$0
@@label6:
CMP EDI, $$int$1
JG @@label7
MOV EBX, EDI
MOV EAX, EBP
ADD EAX, 100
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
JMP @@label6
@@label7:
PUSH offset $$string$7
CALL printf
ADD ESP, 4
PUSH ESI
PUSH offset $$printf_int
CALL printf
ADD ESP, 8
PUSH offset $$new_line
CALL printf
ADD ESP, 4
@end$$ge@1:
ADD ESP, 140
POP EDI
POP ESI
POP EBP
POP EBX
RET 0
$$ge@1 ENDP

$$@global@0 PROC
SUB ESP, 0
MOV EBP, ESP
CALL $$ge@1
@end$$@global@0:
ADD ESP, 0
RET 0
$$@global@0 ENDP

END $$@global@0
