; ---------------------------------------------------------------------------
; - PC Speaker Driver - Hardware Abstraction Layer (hal.inc)                -
; - Copyright (C) 1998-2021 Alexandru Groza of Microprogramming TECHNIQUES  -
; - All rights reserved.                                                    -
; ---------------------------------------------------------------------------
; - License: GNU General Public License v3.0                                -
; ---------------------------------------------------------------------------

; Sends the given command to the PC Speaker Driver ISA card command decoder.
; Input:
;     AL - hardware command
; Output:
;     none
; Affects:
;     BX, DX
; Preserves:
;     none
; ---------------------------------------------------------------------------
halSendCommand proc near

  mov dx,word ptr [GIOAddress]
  out dx,al

  mov bx,kCommandDelay
  call delay

  ret

halSendCommand endp
