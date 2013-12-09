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
SUB ESP, 268
MOV EBP, ESP
MOV EAX, dword ptr [ebp] + 144
MOV EAX, $$int$0
MOV dword ptr [ebp] + 144, EAX
@@label0:
MOV EAX, dword ptr [ebp] + 144
CMP EAX, $$int$1
MOV dword ptr [ebp] + 144, EAX
JG @@label1
MOV EAX, dword ptr [ebp] + 136
MOV EAX, dword ptr [ebp] + 144
MOV ECX, EBP
ADD ECX, 148
IMUL EAX, 4
ADD ECX, EAX
MOV EDX, dword ptr [ebp] + 144
MOV [ECX], EDX
MOV ECX, dword ptr [ebp] + 132
MOV ECX, EDX
ADD ECX, $$int$2
MOV EDX, ECX
MOV dword ptr [ebp] + 144, EDX
MOV dword ptr [ebp] + 132, ECX
MOV dword ptr [ebp] + 136, EAX
JMP @@label0
@@label1:
MOV EAX, dword ptr [ebp] + 144
MOV EAX, $$int$0
MOV dword ptr [ebp] + 144, EAX
@@label2:
MOV EAX, dword ptr [ebp] + 144
CMP EAX, $$int$1
MOV dword ptr [ebp] + 144, EAX
JG @@label3
MOV EAX, dword ptr [ebp] + 128
MOV EAX, dword ptr [ebp] + 144
MOV dword ptr [ebp] + 128, EAX
MOV EAX, EBP
ADD EAX, 148
MOV ECX, dword ptr [ebp] + 128
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr [ebp] + 124
MOV EDX, [EAX]
MOV dword ptr [ebp] + 128, ECX
MOV ECX, EBP
ADD ECX, 188
IMUL EDX, 4
ADD ECX, EDX
MOV EAX, dword ptr [ebp] + 144
MOV [ECX], EAX
MOV ECX, dword ptr [ebp] + 120
MOV ECX, EAX
ADD ECX, $$int$2
MOV EAX, ECX
MOV dword ptr [ebp] + 124, EDX
MOV dword ptr [ebp] + 120, ECX
MOV dword ptr [ebp] + 144, EAX
JMP @@label2
@@label3:
MOV EAX, dword ptr [ebp] + 144
MOV EAX, $$int$0
MOV dword ptr [ebp] + 144, EAX
@@label4:
MOV EAX, dword ptr [ebp] + 144
CMP EAX, $$int$1
MOV dword ptr [ebp] + 144, EAX
JG @@label5
MOV EAX, dword ptr [ebp] + 116
MOV EAX, dword ptr [ebp] + 144
MOV dword ptr [ebp] + 116, EAX
MOV EAX, EBP
ADD EAX, 148
MOV ECX, dword ptr [ebp] + 116
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr [ebp] + 112
MOV EDX, [EAX]
MOV EAX, EBP
ADD EAX, 188
IMUL EDX, 4
ADD EAX, EDX
MOV EBX, dword ptr [ebp] + 108
MOV EBX, [EAX]
MOV dword ptr [ebp] + 116, ECX
MOV ECX, EBP
ADD ECX, 228
IMUL EBX, 4
ADD ECX, EBX
MOV EAX, dword ptr [ebp] + 144
MOV [ECX], EAX
MOV ECX, dword ptr [ebp] + 104
MOV ECX, EAX
ADD ECX, $$int$2
MOV EAX, ECX
MOV dword ptr [ebp] + 108, EBX
MOV dword ptr [ebp] + 112, EDX
MOV dword ptr [ebp] + 104, ECX
MOV dword ptr [ebp] + 144, EAX
JMP @@label4
@@label5:
MOV EAX, dword ptr [ebp] + 100
MOV EAX, $$int$3
MOV ECX, dword ptr [ebp] + 96
MOV ECX, $$int$2
MOV dword ptr [ebp] + 100, EAX
MOV EAX, EBP
ADD EAX, 148
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr [ebp] + 92
MOV EDX, [EAX]
MOV EAX, EBP
ADD EAX, 148
MOV EBX, dword ptr [ebp] + 100
IMUL EBX, 4
ADD EAX, EBX
MOV ESI, dword ptr [ebp] + 88
MOV ESI, [EAX]
MOV dword ptr [ebp] + 92, EDX
MOV EDX, EBP
ADD EDX, 188
MOV EAX, dword ptr [ebp] + 92
IMUL EAX, 4
ADD EDX, EAX
MOV EDI, dword ptr [ebp] + 84
MOV EDI, ESI
IMUL EDI, [EDX]
MOV EDX, dword ptr [ebp] + 80
MOV EDX, $$int$4
MOV dword ptr [ebp] + 92, EAX
MOV EAX, EBP
ADD EAX, 148
IMUL EDX, 4
ADD EAX, EDX
MOV dword ptr [ebp] + 96, ECX
MOV ECX, dword ptr [ebp] + 76
MOV ECX, [EAX]
MOV EAX, dword ptr [ebp] + 72
MOV EAX, $$int$4
MOV dword ptr [ebp] + 72, EAX
MOV EAX, EBP
ADD EAX, 188
IMUL ECX, 4
ADD EAX, ECX
MOV dword ptr [ebp] + 100, EBX
MOV EBX, dword ptr [ebp] + 68
MOV EBX, [EAX]
MOV dword ptr [ebp] + 80, EDX
MOV EDX, EBP
ADD EDX, 148
MOV EAX, dword ptr [ebp] + 72
IMUL EAX, 4
ADD EDX, EAX
MOV dword ptr [ebp] + 88, ESI
MOV ESI, dword ptr [ebp] + 64
MOV ESI, EBX
IMUL ESI, [EDX]
MOV EDX, dword ptr [ebp] + 60
MOV EDX, ESI
MOV dword ptr [ebp] + 84, EDI
MOV EDI, dword ptr [ebp] + 56
MOV EDI, $$int$4
MOV dword ptr [ebp] + 76, ECX
MOV ECX, dword ptr [ebp] + 52
MOV ECX, $$int$2
MOV dword ptr [ebp] + 72, EAX
MOV EAX, EBP
ADD EAX, 228
IMUL EDI, 4
ADD EAX, EDI
MOV dword ptr [ebp] + 68, EBX
MOV EBX, dword ptr [ebp] + 48
MOV EBX, [EAX]
MOV dword ptr [ebp] + 60, EDX
MOV EDX, EBP
ADD EDX, 228
IMUL ECX, 4
ADD EDX, ECX
MOV EAX, dword ptr [ebp] + 44
MOV EAX, EBX
SUB EAX, [EDX]
MOV dword ptr [ebp] + 52, ECX
MOV ECX, EBP
ADD ECX, 228
MOV EDX, dword ptr [ebp] + 60
IMUL EDX, 4
ADD ECX, EDX
MOV dword ptr [ebp] + 44, EAX
MOV EAX, dword ptr [ebp] + 40
MOV dword ptr [ebp] + 60, EDX
MOV EAX, dword ptr [ECX]
CDQ
IDIV dword ptr [ebp] + 44
MOV ECX, dword ptr [ebp] + 36
MOV ECX, dword ptr [ebp] + 84
ADD ECX, EAX
MOV EDX, dword ptr [ebp] + 32
MOV EDX, $$int$4
MOV dword ptr [ebp] + 32, EDX
MOV EDX, EBP
ADD EDX, 188
MOV dword ptr [ebp] + 64, ESI
MOV ESI, dword ptr [ebp] + 32
IMUL ESI, 4
ADD EDX, ESI
MOV dword ptr [ebp] + 56, EDI
MOV EDI, dword ptr [ebp] + 28
MOV EDI, ECX
SUB EDI, [EDX]
MOV EDX, dword ptr [ebp] + 24
MOV EDX, $$int$5
MOV dword ptr [ebp] + 24, EDX
MOV EDX, EBP
ADD EDX, 188
MOV dword ptr [ebp] + 48, EBX
MOV EBX, dword ptr [ebp] + 24
IMUL EBX, 4
ADD EDX, EBX
MOV dword ptr [ebp] + 40, EAX
MOV EAX, dword ptr [ebp] + 20
MOV EAX, EDI
ADD EAX, [EDX]
MOV EDX, dword ptr [ebp] + 16
MOV EDX, $$int$6
MOV dword ptr [ebp] + 16, EDX
MOV EDX, EBP
ADD EDX, 188
MOV dword ptr [ebp] + 36, ECX
MOV ECX, dword ptr [ebp] + 16
IMUL ECX, 4
ADD EDX, ECX
MOV dword ptr [ebp] + 32, ESI
MOV ESI, dword ptr [ebp] + 12
MOV ESI, EAX
SUB ESI, [EDX]
MOV EDX, dword ptr [ebp] + 8
MOV EDX, ESI
ADD EDX, $$int$2
MOV dword ptr [ebp] + 28, EDI
MOV EDI, dword ptr [ebp] + 140
MOV EDI, EDX
MOV dword ptr [ebp] + 24, EBX
MOV EBX, dword ptr [ebp] + 144
MOV EBX, $$int$0
MOV dword ptr [ebp] + 140, EDI
MOV dword ptr [ebp] + 12, ESI
MOV dword ptr [ebp] + 144, EBX
MOV dword ptr [ebp] + 8, EDX
MOV dword ptr [ebp] + 16, ECX
MOV dword ptr [ebp] + 20, EAX
@@label6:
MOV EAX, dword ptr [ebp] + 144
CMP EAX, $$int$1
MOV dword ptr [ebp] + 144, EAX
JG @@label7
MOV EAX, dword ptr [ebp] + 4
MOV EAX, dword ptr [ebp] + 144
MOV dword ptr [ebp] + 4, EAX
MOV EAX, EBP
ADD EAX, 228
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
MOV EAX, dword ptr [ebp] + 144
ADD EAX, $$int$2
MOV ECX, dword ptr [ebp] + 144
MOV ECX, EAX
MOV dword ptr [ebp] + 144, ECX
MOV dword ptr [ebp], EAX
JMP @@label6
@@label7:
PUSH offset $$string$7
CALL printf
ADD ESP, 4
PUSH dword ptr [ebp] + 140
PUSH offset $$printf_int
CALL printf
ADD ESP, 8
PUSH offset $$new_line
CALL printf
ADD ESP, 4
@end$$ge@1:
ADD ESP, 268
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
