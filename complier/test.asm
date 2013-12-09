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
$$int$2 dd 3

.data?

$ss@global0 dd ? , ?  , ?  , ?  , ?  , ?  , ?  , ?  , ?  , ?  , ? 
$hse@global0 dd ? , ?  , ?  , ?  , ?  , ?  , ?  , ?  , ?  , ?  , ? 
$$temp0@global0 dd ?
$$temp1@global0 dd ?
$$temp2@global0 dd ?
$$temp3@global0 dd ?
.code
$$@global@0 PROC
SUB ESP, 0
MOV EBP, ESP
MOV EAX, dword ptr $$temp0@global0
MOV EAX, $$int$0
MOV ECX, offset $ss@global0
IMUL EAX, 4
ADD ECX, EAX
MOV EDX, $$int$1
MOV [ECX], EDX
MOV ECX, dword ptr $$temp1@global0
MOV ECX, $$int$0
MOV dword ptr $$temp0@global0, EAX
MOV EAX, offset $ss@global0
IMUL ECX, 4
ADD EAX, ECX
MOV EDX, dword ptr $$temp2@global0
MOV EDX, [EAX]
MOV dword ptr $$temp1@global0, ECX
MOV ECX, offset $hse@global0
IMUL EDX, 4
ADD ECX, EDX
MOV EAX, $$int$2
MOV [ECX], EAX
MOV ECX, dword ptr $$temp3@global0
MOV ECX, $$int$1
MOV dword ptr $$temp2@global0, EDX
MOV dword ptr $$temp3@global0, ECX
MOV ECX, dword ptr $$temp3@global0
MOV EAX, offset $hse@global0
IMUL ECX, 4
ADD EAX, ECX
PUSH [EAX]
PUSH offset $$printf_int
MOV dword ptr $$temp3@global0, ECX
CALL printf
ADD ESP, 8
PUSH offset $$new_line
CALL printf
ADD ESP, 4
@end$$@global@0:
ADD ESP, 0
RET 0
$$@global@0 ENDP

END $$@global@0
