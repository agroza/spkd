; ---------------------------------------------------------------------------
; - PC Speaker Driver - Program Code (program.inc)                          -
; - Copyright (C) 1998-2021 Alexandru Groza of Microprogramming TECHNIQUES  -
; - All rights reserved.                                                    -
; ---------------------------------------------------------------------------
; - License: GNU General Public License v3.0                                -
; ---------------------------------------------------------------------------

; Program main loop.
; Input:
;     none
; Output:
;     none
; Affects:
;     AX, BL, CX, DX, SI
; Preserves:
;     none
; ---------------------------------------------------------------------------
programMainLoop proc near

  LCursorPosition  EQU  word ptr [bp - 02h]

  push bp
  mov bp,sp

  sub sp,02h

  mov byte ptr [GHighForeground],clGreen

  call videoWriteNewln
  lea si,sControls
  call videoWriteln

  call videoWriteNewln
  lea si,sInputPrompt
  call videoWrite

  call videoGetCursor
  mov LCursorPosition,dx

  jmp @@doLoop

@@executeCommand:
  mov dx,LCursorPosition
  call videoMoveCursor

  call videoWrite

  mov al,bl
  call halSendCommand

  mov bx,kBeepDuration
  mov cx,kBeepFrequency
  call emitSound

  call videoGetNormalAttribute
  mov al,kInputPromptClearChar
  mov dx,LCursorPosition
  inc dh
  inc dl
  mov ch,kInputPromptHeight
  mov cl,kInputPromptWidth
  call videoWriteRegion

@@doLoop:
  xor ah,ah
  int 16h

  cmp al,27
  jz @@endLoop
  cmp al,kACCEL_CMD_DOWN
  jz @@doVolumeDown
  cmp al,kACCEL_CMD_UP
  jz @@doVolumeUp

  and al,5Fh

  cmp al,kACCEL_CMD_OTP
  jz @@doOutput
  cmp al,kACCEL_CMD_OTP_ON
  jz @@doOutputOn
  cmp al,kACCEL_CMD_OTP_OFF
  jz @@doOutputOff
  cmp al,kACCEL_CMD_MODE
  jz @@doMode
  cmp al,kACCEL_CMD_MODE_ON
  jz @@doModeOn
  cmp al,kACCEL_CMD_MODE_OFF
  jz @@doModeOff

  jmp @@doLoop

@@doOutput:
  lea si,sOutputFlipped
  mov bl,kHAL_CMD_OTP

  jmp @@executeCommand

@@doOutputOn:
  lea si,sOutputOn
  mov bl,kHAL_CMD_OTP_ON

  jmp @@executeCommand

@@doOutputOff:
  lea si,sOutputOff
  mov bl,kHAL_CMD_OTP_OFF

  jmp @@executeCommand

@@doMode:
  lea si,sModeFlipped
  mov bl,kHAL_CMD_MODE

  jmp @@executeCommand

@@doModeOn:
  lea si,sModeOn
  mov bl,kHAL_CMD_MODE_ON

  jmp @@executeCommand

@@doModeOff:
  lea si,sModeOff
  mov bl,kHAL_CMD_MODE_OFF

  jmp @@executeCommand

@@doVolumeDown:
  lea si,sVolumeDown
  mov bl,kHAL_CMD_DOWN

  jmp @@executeCommand

@@doVolumeUp:
  lea si,sVolumeUp
  mov bl,kHAL_CMD_UP

  jmp @@executeCommand

@@endLoop:
  mov dx,LCursorPosition
  call videoMoveCursor

  lea si,sExit
  call videoWriteln

  mov sp,bp
  pop bp

  ret

programMainLoop endp

; If the quiet parameter is not set, writes a new line then the given message,
; returns otherwise.
; Input:
;     SI - pointer to string
; Output:
;     none
; Affects:
;     none
; Preserves:
;     none
; ---------------------------------------------------------------------------
programWrite proc near

  test byte ptr [GCmdLineParams],kCMD_LINE_QUIET
  jnz @@exit

  call videoWriteNewln
  call videoWriteln

@@exit:
  ret

programWrite endp

; Sends the given command to the hardware abstraction layer and writes the
; command execution status.
; Input:
;     AL - hardware command
; Output:
;     none
; Affects:
;     none
; Preserves:
;     none
; ---------------------------------------------------------------------------
programSendCommand proc near

  cmp al,kHALLowestCommandID
  jb @@commandOutOfRange
  cmp al,kHALHighestCommandID
  ja @@commandOutOfRange

  call halSendCommand

  lea si,sCommandExecuted
  call programWrite

  jmp @@exit

@@commandOutOfRange:
  lea si,sCommandOutOfRange
  call programWrite

@@exit:
  ret

programSendCommand endp

; Verifies whether the address is within acceptable I/O range.
; Input:
;     none
; Output:
;     CF - set = success
;          clear = error
; Affects:
;     none
; Preserves:
;     none
; ---------------------------------------------------------------------------
programCheckIOAddress proc near

  cmp word ptr [GIOAddress],kLowestIOAddress
  jb @@addressOutOfRange
  cmp word ptr [GIOAddress],kHighestIOAddress
  ja @@addressOutOfRange

  stc

  jmp @@exit

@@addressOutOfRange:
  lea si,sAddressOutOfRange
  call programWrite

  clc

@@exit:
  ret

programCheckIOAddress endp
