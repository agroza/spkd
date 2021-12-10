; ---------------------------------------------------------------------------
; - VersaVision Video Driver - Assembler Edition (video.inc)                -
; - Integrant part of VersaVision Framework                                 -
; - Copyright (C) 1998-2021 Alexandru Groza of Microprogramming TECHNIQUES  -
; - All rights reserved.                                                    -
; ---------------------------------------------------------------------------
; - License: GNU General Public License v3.0                                -
; ---------------------------------------------------------------------------

; routines:
;   + videoEnableCursor
;   + videoDisableCursor
;   + videoGetCursor
;   + videoMoveCursor
;   + videoScrollUp
;   + videoGetNormalAttribute
;   + videoGetHighAttribute
;   + videoCalculatePosition
;   + videoWriteRegion
;   + videoWriteNewln
;   + videoWrite
;   + videoWriteln
; ---------------------------------------------------------------------------

; general VGA equates
; ---------------------------------------------------------------------------
  kVGAColumnCount    EQU  80
  kVGARowCount       EQU  25
  kVGARAMAddress     EQU  0B800h

; VGA color equates
; ---------------------------------------------------------------------------
  clBlack            EQU  00h
  clBlue             EQU  01h
  clGreen            EQU  02h
  clCyan             EQU  03h
  clRed              EQU  04h
  clMagenta          EQU  05h
  clBrown            EQU  06h
  clLightGray        EQU  07h
  clDarkGray         EQU  08h
  clLightBlue        EQU  09h
  clLightGreen       EQU  0Ah
  clLightCyan        EQU  0Bh
  clLightRed         EQU  0Ch
  clLightMagenta     EQU  0Dh
  clYellow           EQU  0Eh
  clWhite            EQU  0Fh

; global variables section
; ---------------------------------------------------------------------------
  GForeground        DB   ?
  GHighForeground    DB   ?
  GBackground        DB   ?

; Shows the cursor.
; Input:
;     none
; Output:
;     none
; Affects:
;     AH, CX
; Preserves:
;     none
; ---------------------------------------------------------------------------
videoEnableCursor proc near

  mov ah,01h
  mov cx,0607h
  int 10h

  ret

videoEnableCursor endp

; Hides the cursor.
; Input:
;     none
; Output:
;     none
; Affects:
;     AH, CH
; Preserves:
;     none
; ---------------------------------------------------------------------------
videoDisableCursor proc near

  mov ah,01h
  mov ch,20h
  int 10h

  ret

videoDisableCursor endp

; Returns the current cursor position.
; Input:
;     none
; Output:
;     DH - cursor row position
;     DL - cursor column position
; Affects:
;     none
; Preserves:
;     AX, BX
; ---------------------------------------------------------------------------
videoGetCursor proc near

  push ax
  push bx

  mov ah,03h
  xor bh,bh
  int 10h

  pop bx
  pop ax

  ret

videoGetCursor endp

; Moves the cursor to the specified row and column.
; Input:
;     DH - row
;     DL - column
; Output:
;     none
; Affects:
;     none
; Preserves:
;     AX, BX
; ---------------------------------------------------------------------------
videoMoveCursor proc near

  push ax
  push bx

  mov ah,02h
  xor bh,bh
  int 10h

  pop bx
  pop ax

  ret

videoMoveCursor endp

; Scrolls the entire screen contents up.
; Input:
;     none
; Output:
;     none
; Affects:
;     AX, BH, CX
; Preserves:
;     DX
; ---------------------------------------------------------------------------
videoScrollUp proc near

  push dx

  mov ah,08h
  int 10h
  mov bh,ah

  mov ah,06h
  mov al,01h
  xor cx,cx
  mov dh,kVGARowCount
  dec dh
  mov dl,kVGAColumnCount
  dec dl
  int 10h

  pop dx

  ret

videoScrollUp endp

; Makes an attribute out of [GForeground] and [GBackground].
; Input:
;     none
; Output:
;     AH - video attribute
; Affects:
;     CL
; Preserves:
;     none
; ---------------------------------------------------------------------------
videoGetNormalAttribute proc near

  mov ah,byte ptr [GBackground]
  mov cl,04h
  shl ah,cl
  add ah,byte ptr [GForeground]

  ret

videoGetNormalAttribute endp

; Makes an attribute out of [GHighForeground] and [GBackground].
; Input:
;     none
; Output:
;     AH - video attribute
; Affects:
;     CL
; Preserves:
;     none
; ---------------------------------------------------------------------------
videoGetHighAttribute proc near

  mov ah,byte ptr [GBackground]
  mov cl,04h
  shl ah,cl
  add ah,byte ptr [GHighForeground]

  ret

videoGetHighAttribute endp

; Calculates the new position in VGA RAM.
; Input:
;     DH - row
;     DL - column
; Output:
;     DI - new position
; Affects:
;     FLAGS, BX
; Preserves:
;     AX, CX, DX
; ---------------------------------------------------------------------------
videoCalculatePosition proc near

  push ax
  push cx
  push dx

  mov cx,dx

  xor ah,ah
  mov al,ch
  xor bh,bh
  mov bl,kVGAColumnCount
  mul bx
  shl ax,01h
  xor dh,dh
  mov dl,cl
  shl dx,01h
  add ax,dx

  mov di,ax

  pop dx
  pop cx
  pop ax

  ret

videoCalculatePosition endp

; Writes a region of data (character, attribute) directly in VGA RAM.
; Input:
;     AH - attribute
;     AL - character
;     CH - ending row
;     CL - ending column
;     DH - starting row
;     DL - starting column
; Output:
;     none
; Affects:
;     CX, DX, DI
; Preserves:
;     none
; ---------------------------------------------------------------------------
videoWriteRegion proc near

  LWidthHeight  EQU  word ptr [bp - 02h]
  LWidth        EQU  byte ptr [bp - 02h]
  LHeight       EQU  byte ptr [bp - 01h]

  push bp
  mov bp,sp

  sub sp,02h

  mov LWidthHeight,cx

  mov bx,kVGARAMAddress
  mov es,bx

  xor ch,ch
  dec dh

  cld

@@nextPosition:
  add dh,ch
  call videoCalculatePosition

  dec di
  dec di

  push cx

  mov cl,LWidth

  rep stosw

  pop cx

  inc ch
  cmp ch,LHeight
  jnz @@nextPosition

  mov sp,bp
  pop bp

  ret

videoWriteRegion endp

; Advances the cursor to the next line.
; Input:
;     none
; Output:
;     none
; Affects:
;     DX
; Preserves:
;     none
; ---------------------------------------------------------------------------
videoWriteNewln proc near

  call videoGetCursor

  xor dl,dl

  cmp dh,kVGARowCount - 1
  jz @@scrollUp

  inc dh
  call videoMoveCursor

  jmp @@exit

@@scrollUp:
  call videoMoveCursor
  call videoScrollUp

@@exit:
  ret

videoWriteNewln endp

; Writes a null-terminated string directly in VGA RAM.
; Treats sequence of characters 1,2 as string array separator.
; Any string within characters 3,4 will be highlighted.
; Input:
;     SI - pointer to string
; Output:
;     none
; Affects:
;     none
; Preserves:
;     BX
; ---------------------------------------------------------------------------
videoWrite proc near

  LHighAttribute    EQU  byte ptr [bp - 02h]
  LNormalAttribute  EQU  byte ptr [bp - 01h]

  push bp
  mov bp,sp

  sub sp,02h

  push bx

  call videoGetCursor

  mov ax,kVGARAMAddress
  mov es,ax

  call videoGetHighAttribute
  mov LHighAttribute,ah
  call videoGetNormalAttribute
  mov LNormalAttribute,ah

  cld

@@nextByte:
  call videoCalculatePosition

  lodsb

  or al,al
  jz @@exit
  cmp al,13
  jz @@carriageReturn
  cmp al,10
  jz @@lineFeed
  cmp al,1
  jz @@carriageReturn
  cmp al,2
  jz @@lineFeed
  cmp al,3
  jz @@setHighAttribute
  cmp al,4
  jz @@setNormalAttribute

  jmp @@storeWord

@@setHighAttribute:
  mov ah,LHighAttribute

  jmp @@nextByte

@@setNormalAttribute:
  mov ah,LNormalAttribute

  jmp @@nextByte

@@storeWord:
  stosw

  inc dl

  loop @@nextByte

  jmp @@exit

@@carriageReturn:
  xor dl,dl
  call videoMoveCursor

  jmp @@nextByte

@@lineFeed:
  cmp dh,kVGARowCount - 1
  jz @@scrollUp

  inc dh
  call videoMoveCursor

  jmp @@nextByte

@@scrollUp:
  call videoMoveCursor
  call videoScrollUp

  jmp @@nextByte

@@exit:
  call videoMoveCursor

  pop bx

  mov sp,bp
  pop bp

  ret

videoWrite endp

; Writes a null-terminated string directly in VGA RAM and goes to next line.
; Input:
;     SI - pointer to string
; Output:
;     none
; Affects:
;     none
; Preserves:
;     none
; ---------------------------------------------------------------------------
videoWriteln proc near

  call videoWrite
  call videoWriteNewln

  ret

videoWriteln endp
