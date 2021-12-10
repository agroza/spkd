; ---------------------------------------------------------------------------
; - SPKD - PC Speaker Driver (spkd.asm)                                     -
; - Copyright (C) 1998-2021 Alexandru Groza of Microprogramming TECHNIQUES  -
; - All rights reserved.                                                    -
; ---------------------------------------------------------------------------
; - License: GNU General Public License v3.0                                -
; ---------------------------------------------------------------------------

LOCALS @@

P8086
MODEL TINY

CODESEG
  org 100h

STARTUPCODE

  jmp @start

; include section
; ---------------------------------------------------------------------------
include equates.inc
include strings.inc
include vars.inc
include routines.inc
include video.inc
include hal.inc
include program.inc

; Writes the program header.
; ---------------------------------------------------------------------------
writeProgramHeader proc near

  call videoWriteNewln

  mov byte ptr [GBackground],clBlack

  mov byte ptr [GForeground],clWhite
  lea si,sProgram
  call videoWriteln

  mov byte ptr [GForeground],clLightGray
  lea si,sCopyright
  call videoWriteln
  lea si,sAuthor
  call videoWriteln
  lea si,sRights
  call videoWriteln

  ret

writeProgramHeader endp

; Writes the program help.
; ---------------------------------------------------------------------------
writeHelp proc near

  call videoWriteNewln
  lea si,sHelpUsage
  call videoWriteln

  call videoWriteNewln
  lea si,sHelpWhere
  call videoWriteln
  lea si,sHelpParameters
  call videoWriteln

  call videoWriteNewln
  lea si,sHelpCommandsRef
  call videoWriteln
  lea si,sHelpCommands
  call videoWriteln

  ret

writeHelp endp

; Writes the parameter error message.
; ---------------------------------------------------------------------------
writeParameterError proc near

  call videoWriteNewln
  lea si,sParameterMissing
  call videoWriteln

  ret

writeParameterError endp

; Processes the commandline parameters.
; Returns:
;   AL - non-zero = success
;        zero = error
; ---------------------------------------------------------------------------
processParameters proc near

  inc al

@@findHelpParam:
  mov ah,pHelpAlternate
  call findParameter
  jc @@setHelpParam

@@findAltHelpParam:
  mov ah,pHelp
  call findParameter
  jnc @@findAddressParam

@@setHelpParam:
  or byte ptr [GCmdLineParams],kCMD_LINE_HELP

@@findAddressParam:
  mov ah,pAddress
  call findParameter
  jnc @@findCommandParam

  call getMultiDigitValue
  jnc @@errorExit

  or byte ptr [GCmdLineParams],kCMD_LINE_ADDRESS
  mov word ptr [GIOAddress],ax

@@findCommandParam:
  mov ah,pCommand
  call findParameter
  jnc @@findQuietParam

  call getSingleDigitValue
  jnc @@errorExit

  or byte ptr [GCmdLineParams],kCMD_LINE_COMMAND
  mov byte ptr [GCommand],al

@@findQuietParam:
  mov ah,pQuiet
  call findParameter
  jnc @@exit

  or byte ptr [GCmdLineParams],kCMD_LINE_QUIET

  jmp @@exit

@@errorExit:
  call writeParameterError

  xor al,al

@@exit:
  or al,al

  ret

processParameters endp

; Main program entrypoint.
; ---------------------------------------------------------------------------
@start:
  call writeProgramHeader
  call processParameters
  jz @exit

  test byte ptr [GCmdLineParams],kCMD_LINE_HELP
  jnz @showHelp

  test byte ptr [GCmdLineParams],kCMD_LINE_ADDRESS
  jnz @continueProgram

  mov word ptr [GIOAddress],kDefaultIOAddress

@continueProgram:
  call programCheckIOAddress
  jnc @exit

  test byte ptr [GCmdLineParams],kCMD_LINE_COMMAND
  jnz @executeCommand

  call videoDisableCursor
  call programMainLoop
  call videoEnableCursor

  jmp @exit

@showHelp:
  call writeHelp

  jmp @exit

@executeCommand:
  mov al,byte ptr [GCommand]
  call programSendCommand

@exit:
  mov ax,4C00h
  int 21h

end
