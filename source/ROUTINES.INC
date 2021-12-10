; ---------------------------------------------------------------------------
; - PC Speaker Driver - Routines (routines.inc)                             -
; - Copyright (C) 1998-2021 Alexandru Groza of Microprogramming TECHNIQUES  -
; - All rights reserved.                                                    -
; ---------------------------------------------------------------------------
; - License: GNU General Public License v3.0                                -
; ---------------------------------------------------------------------------

; routines:
;   + delay
;   + emitSound
;   + checkDelimiter
;   + skipDelimiters
;   + findParameter
;   + getSingleDigitValue
;   + getMultiDigitValue
;   + charToIntDef
;   + hexStrToIntDef
; ---------------------------------------------------------------------------

; Delay by a given value in milliseconds.
; Input:
;     BX - number of milliseconds
; Output:
;     none
; Affects:
;     AX, CX, DX
; Preserves:
;     none
; ---------------------------------------------------------------------------
delay proc near

  mov ax,1000
  mul bx
  mov cx,dx
  mov dx,ax
  mov ah,86h

  int 15h

  ret

delay endp

; Emits a sound through the PC Speaker.
; Input:
;     BX - duration in milliseconds
;     CX - frequency
; Output:
;     none
; Affects:
;     AX
; Preserves:
;     none
; ---------------------------------------------------------------------------
emitSound proc near

  mov al,182
  out 43h,al

  mov ax,cx
  out 42h,al
  mov al,ah
  out 42h,al

  in al,61h
  or al,00000011b
  out 61h,al

  call delay

  in al,61h
  and al,11111100b
  out 61h,al

  ret

emitSound endp

; Checks whether the given character is a commandline delimiter.
; Input:
;     AL - character to test
; Output:
;     ZF - set = given character is a delimiter
;          clear = the given character is not a delimiter
; Affects:
;     none
; Preserves:
;     none
; ---------------------------------------------------------------------------
checkDelimiter proc near

  cmp al,' '
  jz @@exitCheckDelimiter
  cmp al,','
  jz @@exitCheckDelimiter
  cmp al,09h
  jz @@exitCheckDelimiter
  cmp al,';'
  jz @@exitCheckDelimiter
  cmp al,'='
  jz @@exitCheckDelimiter
  cmp al,0Dh

@@exitCheckDelimiter:
  ret

checkDelimiter endp

; Skips all delimiters at the current position within the commandline.
; Input:
;     SI - pointer to commandline
; Output:
;     none
; Affects:
;     AL, SI
; Preserves:
;     none
; ---------------------------------------------------------------------------
skipDelimiters proc near

  cld

@@nextByte:
  lodsb

  cmp al,0Dh
  jz @@exitSkipDelimiters

  call checkDelimiter
  jz @@nextByte

@@exitSkipDelimiters:
  dec si

  ret

skipDelimiters endp

; Searches for the given commandline parameter.
; Input:
;     AH - parameter
; Output:
;     CF - set = success
;          clear = error
;     CX - length of the commandline string, zero if none
;     SI - last scanned position within the commandline
; Affects:
;     AL
; Preserves:
;     none
; ---------------------------------------------------------------------------
findParameter proc near

  clc

  xor ch,ch
  mov cl,byte ptr ds:[80h]
  jcxz @@exit

  mov si,81h

  cld

@@nextParameter:
  call skipDelimiters

@@nextByte:
  lodsb

  call checkDelimiter
  jz @@parameterEnd

  cmp al,'-'
  jnz @@nextByte

  lodsb

  call checkDelimiter
  jz @@parameterEnd

  cmp al,ah
  jz @@parameterFound

  and al,5Fh
  cmp al,ah
  jz @@parameterFound

  jmp @@nextParameter

@@parameterEnd:
  cmp al,0Dh
  jnz @@nextParameter

  jmp @@exit

@@parameterFound:
  stc

@@exit:
  ret

findParameter endp

; Returns the single-digit parameter value converted from character to integer .
; Input:
;     SI - pointer to current position within the commandline
; Output:
;     CF - set = success
;          clear = error
;     AL - commandline parameter value as 8-bit integer
; Affects:
;     AH
; Preserves:
;     none
; ---------------------------------------------------------------------------
getSingleDigitValue proc near

  cld

  lodsb

  cmp al,'='
  jnz @@exitError

  lodsb

  call checkDelimiter
  jz @@exitError

  xor ah,ah
  call charToIntDef

  jmp @@exit

@@exitError:
  clc

@@exit:
  ret

getSingleDigitValue endp

; Returns the multi-digit parameter value converted from string to integer .
; Input:
;     SI - pointer to current position within the commandline
; Output:
;     CF - set = success
;          clear = error
;     AX - commandline parameter value as 16-bit integer
; Affects:
;     CL
; Preserves:
;     none
; ---------------------------------------------------------------------------
getMultiDigitValue proc near

  cld

  lodsb

  cmp al,'='
  jnz @@exitError

  lodsb

  call checkDelimiter
  jz @@exitError

  dec si

  xor ax,ax
  mov cl,03h
  call hexStrToIntDef

  jmp @@exit

@@exitError:
  clc

@@exit:
  ret

getMultiDigitValue endp

; Converts the given character to an 8-bit integer.
; Input:
;     AH - default value in case of AL not in range 0..9
;     AL - character number
; Output:
;     CF - set = success
;          clear = error
;     AL - integer representation
; Affects:
;     none
; Preserves:
;     none
; ---------------------------------------------------------------------------
charToIntDef proc near

  sub al,'0'
  jb @@notNumber
  cmp al,09h
  ja @@notNumber

  stc

  jmp @@exit

@@notNumber:
  mov al,ah

  clc

@@exit:
  ret

charToIntDef endp

; Converts the given hexadecimal string to a 16-bit integer.
; Input:
;     SI - pointer to string
;     AX - default value in case SI doesn't point to a number
;     CL - how many digits
; Output:
;     CF - set = success
;          clear = error
;     AX - integer representation
; Affects:
;     BL, CH, SI
; Preserves:
;     none
; ---------------------------------------------------------------------------
hexStrToIntDef proc near

  LDefaultValue  EQU  word ptr [bp - 02h]

  push bp
  mov bp,sp

  sub sp,02h

  mov LDefaultValue,ax

  xor ax,ax
  xor ch,ch

@@nextByte:
  shl ax,04h

  mov bl,ds:[si]
  inc si

  cmp bl,'a'
  jb @@checkDigit

  sub bl,20h

@@checkDigit:
  cmp bl,'0'
  jb @@notNumber
  cmp bl,'F'
  ja @@notNumber
  cmp bl,'9'
  jbe @@digit09
  cmp bl,'A'
  jae @@digitAF

@@digit09:
  sub bl,'0'

  jmp @@continue

@@digitAF:
  sub bl,'7'

@@continue:
  or al,bl

  loop @@nextByte

  stc

  jmp @@exit

@@notNumber:
  mov ax,LDefaultValue

  clc

@@exit:
  mov sp,bp
  pop bp

  ret

hexStrToIntDef endp
