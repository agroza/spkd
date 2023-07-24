; ---------------------------------------------------------------------------
; - PC Speaker Driver - String Constants (strings.inc)                      -
; - Copyright (C) 1998-2021 Alexandru Groza of Microprogramming TECHNIQUES  -
; - All rights reserved.                                                    -
; ---------------------------------------------------------------------------
; - License: GNU General Public License v3.0                                -
; ---------------------------------------------------------------------------

; program stringtable
; ---------------------------------------------------------------------------
  sProgram           DB   'PC SPEAKER DRIVER  VER. 1.2 REV. A',0
  sCopyright         DB   'Copyright (C) 1998-2021 Microprogramming TECHNIQUES',0
  sAuthor            DB   'Programming/PC Code: Alexandru Groza',0
  sRights            DB   'All rights reserved.',0

  sParameterMissing  DB   'Parameter(s) invalid or missing.',0

  sHelpUsage         DB   'Usage is: spkd.com [-?|-h(elp)] [-a=300-31F] [-c=0-7] [-q]',0
  sHelpWhere         DB   'Where:',0
  sHelpParameters    DB   '  -?|-h   shows this screen',1,2,\
                          '  -a=XXX  hexadecimal ISA card I/O address',1,2,\
                          '  -c=X    decimal hardware command number',1,2,\
                          '  -q      reduces text verbosity',0

  sHelpCommandsRef   DB   'Commands reference:',0
  sHelpCommands      DB   '  0 = output relay flip',1,2,\
                          '  1 = output relay on',1,2,\
                          '  2 = output relay off',1,2,\
                          '  3 = mode relay flip',1,2,\
                          '  4 = mode relay on',1,2,\
                          '  5 = mode relay off',1,2,\
                          '  6 = volume down',1,2,\
                          '  7 = volume up',0

  sControls          DB   '[',3,'ESC',4,'] Exit ',\
                          '[',3,kACCEL_CMD_UP,4,'/',3,kACCEL_CMD_DOWN,4,'] Volume (up/down) ',\
                          '[',3,kACCEL_CMD_OTP,4,'] Output (on/off) ',\
                          '[',3,kACCEL_CMD_MODE,4,'] Mode (on/off)',1,2,\
                          '[',3,kACCEL_CMD_OTP_ON,4,'] Output On ',\
                          '[',3,kACCEL_CMD_OTP_OFF,4,'] Output Off ',\
                          '[',3,kACCEL_CMD_MODE_ON,4,'] Amplifier ',\
                          '[',3,kACCEL_CMD_MODE_OFF,4,'] PC Speaker',0
  sInputPrompt       DB   '> ',0

  sExit              DB   'Exit',0
  sVolumeUp          DB   'Volume Up',0
  sVolumeDown        DB   'Volume Down',0
  sOutputFlipped     DB   'Output Relay Flipped',0
  sOutputOn          DB   'Output Relay On',0
  sOutputOff         DB   'Output Relay Off',0
  sModeFlipped       DB   'Mode Relay Flipped',0
  sModeOn            DB   'Amplifier Mode',0
  sModeOff           DB   'PC Speaker Mode',0

  sAddressOutOfRange DB   'I/O address out of range.',0
  sCommandExecuted   DB   'Command executed.',0
  sCommandOutOfRange DB   'Command out of range.',0
