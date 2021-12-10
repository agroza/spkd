; ---------------------------------------------------------------------------
; - PC Speaker Driver - Equates (equates.inc)                               -
; - Copyright (C) 1998-2021 Alexandru Groza of Microprogramming TECHNIQUES  -
; - All rights reserved.                                                    -
; ---------------------------------------------------------------------------
; - License: GNU General Public License v3.0                                -
; ---------------------------------------------------------------------------

; general program equates
; ---------------------------------------------------------------------------
  kCommandDelay          EQU  15
  kBeepDuration          EQU  100
  kBeepFrequency         EQU  2280

  kDefaultIOAddress      EQU  300h
  kLowestIOAddress       EQU  300h
  kHighestIOAddress      EQU  31Fh

  kInputPromptWidth      EQU  20
  kInputPromptHeight     EQU  1
  kInputPromptClearChar  EQU  ' '

; commandline parameters configuration bits
; ---------------------------------------------------------------------------
  kCMD_LINE_HELP         EQU  00000001b
  kCMD_LINE_ADDRESS      EQU  00000010b
  kCMD_LINE_COMMAND      EQU  00000100b
  kCMD_LINE_QUIET        EQU  00001000b

; program commandline parameters
; ---------------------------------------------------------------------------
  pHelpAlternate         EQU  '?'
  pHelp                  EQU  'H'
  pAddress               EQU  'A'
  pCommand               EQU  'C'
  pQuiet                 EQU  'Q'

; commands accelerators
; ---------------------------------------------------------------------------
  kACCEL_CMD_OTP         EQU  'O'
  kACCEL_CMD_OTP_ON      EQU  'A'
  kACCEL_CMD_OTP_OFF     EQU  'B'
  kACCEL_CMD_MODE        EQU  'M'
  kACCEL_CMD_MODE_ON     EQU  'C'
  kACCEL_CMD_MODE_OFF    EQU  'D'
  kACCEL_CMD_DOWN        EQU  '-'
  kACCEL_CMD_UP          EQU  '+'

; hardware abstraction layer commands
; ---------------------------------------------------------------------------
  kHAL_CMD_OTP           EQU  00h
  kHAL_CMD_OTP_ON        EQU  01h
  kHAL_CMD_OTP_OFF       EQU  02h
  kHAL_CMD_MODE          EQU  03h
  kHAL_CMD_MODE_ON       EQU  04h
  kHAL_CMD_MODE_OFF      EQU  05h
  kHAL_CMD_DOWN          EQU  06h
  kHAL_CMD_UP            EQU  07h

  kHALLowestCommandID    EQU  kHAL_CMD_OTP
  kHALHighestCommandID   EQU  kHAL_CMD_UP
