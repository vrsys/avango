// -*- Mode:C++ -*-

#include <avango/daemon/EventMapping.win.h>
#include <avango/daemon/LinuxInput.win.h>

#include "hidusage.h"

namespace
{
  static const av::daemon::WindowsEventMapping sWindowsEventMap;

  static const unsigned short sKeyboardEventPairs[][2] = { 
    {0,              0},   // KEY_RESERVED
    {VK_ESCAPE,      1},   // KEY_ESC
    {0x31,           2},   // KEY_1
    {0x32,           3},   // KEY_2
    {0x33,           4},   // KEY_3
    {0x34,           5},   // KEY_4
    {0x35,           6},   // KEY_5
    {0x36,           7},   // KEY_6
    {0x37,           8},   // KEY_7
    {0x38,           9},   // KEY_8
    {0x39,           10},  // KEY_9
    {0x30,           11},  // KEY_0
    {VK_OEM_MINUS,   12},  // KEY_MINUS
    {VK_OEM_PLUS,    13},  // KEY_EQUAL
    {VK_BACK,        14},  // KEY_BACKSPACE
    {VK_TAB,         15},  // KEY_TAB
    {0x51,           16},  // KEY_Q
    {0x57,           17},  // KEY_W
    {0x45,           18},  // KEY_E
    {0x52,           19},  // KEY_R
    {0x54,           20},  // KEY_T
    {0x59,           21},  // KEY_Y
    {0x55,           22},  // KEY_U
    {0x49,           23},  // KEY_I
    {0x4f,           24},  // KEY_O
    {0x50,           25},  // KEY_P
    {VK_OEM_4,       26},  // KEY_LEFTBRACE
    {VK_OEM_6,       27},  // KEY_RIGHTBRACE
    {VK_RETURN,      28},  // KEY_ENTER
    {VK_LCONTROL,    29},  // KEY_LEFTCTRL
    {0x41,           30},  // KEY_A
    {0x53,           31},  // KEY_S
    {0x44,           32},  // KEY_D
    {0x46,           33},  // KEY_F
    {0x47,           34},  // KEY_G
    {0x48,           35},  // KEY_H
    {0x4a,           36},  // KEY_J
    {0x4b,           37},  // KEY_K
    {0x4c,           38},  // KEY_L
    {VK_OEM_1,       39},  // KEY_SEMICOLON
    {VK_OEM_7,       40},  // KEY_APOSTROPHE
    {VK_OEM_3,       41},  // KEY_GRAVE
    {VK_LSHIFT,      42},  // KEY_LEFTSHIFT
    {VK_OEM_5,       43},  // KEY_BACKSLASH
    {0x5a,           44},  // KEY_Z
    {0x58,           45},  // KEY_X
    {0x43,           46},  // KEY_C
    {0x56,           47},  // KEY_V
    {0x42,           48},  // KEY_B
    {0x4e,           49},  // KEY_N
    {0x4d,           50},  // KEY_M
    {VK_OEM_COMMA,   51},  // KEY_COMMA
    {VK_OEM_PERIOD,  52},  // KEY_DOT
    {VK_OEM_2,       53},  // KEY_SLASH
    {VK_RSHIFT,      54},  // KEY_RIGHTSHIFT
    {VK_MULTIPLY,    55},  // KEY_KPASTERISK
    {VK_LMENU,       56},  // KEY_LEFTALT
    {VK_SPACE,       57},  // KEY_SPACE
    {VK_CAPITAL,     58},  // KEY_CAPSLOCK
    {VK_F1,          59},  // KEY_F1
    {VK_F2,          60},  // KEY_F2
    {VK_F3,          61},  // KEY_F3
    {VK_F4,          62},  // KEY_F4
    {VK_F5,          63},  // KEY_F5
    {VK_F6,          64},  // KEY_F6
    {VK_F7,          65},  // KEY_F7
    {VK_F8,          66},  // KEY_F8
    {VK_F9,          67},  // KEY_F9
    {VK_F10,         68},  // KEY_F10
    {VK_NUMLOCK,     69},  // KEY_NUMLOCK
    {VK_SCROLL,      70},  // KEY_SCROLLLOCK
    {VK_NUMPAD7,     71},  // KEY_KP7
    {VK_NUMPAD8,     72},  // KEY_KP8
    {VK_NUMPAD9,     73},  // KEY_KP9
    {VK_SUBTRACT,    74},  // KEY_KPMINUS
    {VK_NUMPAD4,     75},  // KEY_KP4
    {VK_NUMPAD5,     76},  // KEY_KP5
    {VK_NUMPAD6,     77},  // KEY_KP6
    {VK_ADD,         78},  // KEY_KPPLUS
    {VK_NUMPAD1,     79},  // KEY_KP1
    {VK_NUMPAD2,     80},  // KEY_KP2
    {VK_NUMPAD3,     81},  // KEY_KP3
    {VK_NUMPAD0,     82},  // KEY_KP0
    {VK_DECIMAL,     83},  // KEY_KPDOT
    {0,              85},  // KEY_ZENKAKUHANKAKU
    {VK_OEM_102,     86},  // KEY_102ND
    {VK_F11,         87},  // KEY_F11
    {VK_F12,         88},  // KEY_F12
    {0,              89},  // KEY_RO
    {VK_KANA,        90},  // KEY_KATAKANA
    {0,              91},  // KEY_HIRAGANA
    {VK_CONVERT,     92},  // KEY_HENKAN
    {0,              93},  // KEY_KATAKANAHIRAGANA
    {VK_NONCONVERT,  94},  // KEY_MUHENKAN
    {0,              95},  // KEY_KPJPCOMMA
    {VK_SEPARATOR,   96},  // KEY_KPENTER
    {VK_RCONTROL,    97},  // KEY_RIGHTCTRL
    {VK_DIVIDE,      98},  // KEY_KPSLASH
    {VK_SNAPSHOT,    99},  // KEY_SYSRQ
    {VK_RMENU,       100},  // KEY_RIGHTALT
    {0x0a,           101},  // KEY_LINEFEED
    {VK_HOME,        102},  // KEY_HOME
    {VK_UP,          103},  // KEY_UP
    {VK_PRIOR,       104},  // KEY_PAGEUP
    {VK_LEFT,        105},  // KEY_LEFT
    {VK_RIGHT,       106},  // KEY_RIGHT
    {VK_END,         107},  // KEY_END
    {VK_DOWN,        108},  // KEY_DOWN
    {VK_NEXT,        109},  // KEY_PAGEDOWN
    {VK_INSERT,      110},  // KEY_INSERT
    {VK_DELETE,      111},  // KEY_DELETE
    {0,              112},  // KEY_MACRO
    {VK_VOLUME_MUTE, 113},  // KEY_MUTE
    {VK_VOLUME_DOWN, 114},  // KEY_VOLUMEDOWN
    {VK_VOLUME_UP,   115},  // KEY_VOLUMEUP
    {0,              116},  // KEY_POWER
    {0,              117},  // KEY_KPEQUAL
    {0,              118},  // KEY_KPPLUSMINUS
    {VK_PAUSE,       119},  // KEY_PAUSE
    {0,              121},  // KEY_KPCOMMA
    {VK_HANGUL,      122},  // KEY_HANGEUL
    {VK_HANJA,       123},  // KEY_HANJA
    {0,              124},  // KEY_YEN
    {VK_LWIN,        125},  // KEY_LEFTMETA
    {VK_RWIN,        126},  // KEY_RIGHTMETA
    {VK_APPS,        127},  // KEY_COMPOSE
    {VK_BROWSER_STOP,128},  // KEY_STOP
    {0,              129},  // KEY_AGAIN
    {0,              130},  // KEY_PROPS
    {0,              131},  // KEY_UNDO
    {0,              132},  // KEY_FRONT
    {0,              133},  // KEY_COPY
    {0,              134},  // KEY_OPEN
    {0,              135},  // KEY_PASTE
    {0,              136},  // KEY_FIND
    {0,              137},  // KEY_CUT
    {VK_HELP,        138},  // KEY_HELP
    {VK_MENU,        139},  // KEY_MENU
    {0,              140},  // KEY_CALC
    {0,              141},  // KEY_SETUP
    {VK_SLEEP,       142},  // KEY_SLEEP
    {0,              143},  // KEY_WAKEUP
    {0,              144},  // KEY_FILE
    {0,              145},  // KEY_SENDFILE
    {0,              146},  // KEY_DELETEFILE
    {0,              147},  // KEY_XFER
    {0,              148},  // KEY_PROG1
    {0,              149},  // KEY_PROG2
    {0,              150},  // KEY_WWW
    {0,              151},  // KEY_MSDOS
    {0,              152},  // KEY_COFFEE
    {0,              153},  // KEY_DIRECTION
    {0,              154},  // KEY_CYCLEWINDOWS
    {VK_LAUNCH_MAIL, 155},  // KEY_MAIL
    {0,              156},  // KEY_BOOKMARKS
    {0,              157},  // KEY_COMPUTER
    {VK_BROWSER_BACK,158},  // KEY_BACK
    {VK_BROWSER_FORWARD,159},//KEY_FORWARD
    {0,              160},  // KEY_CLOSECD
    {0,              161},  // KEY_EJECTCD
    {0,              162},  // KEY_EJECTCLOSECD
    {VK_MEDIA_NEXT_TRACK, 163},  // KEY_NEXTSONG
    {VK_MEDIA_PLAY_PAUSE, 164},  // KEY_PLAYPAUSE
    {VK_MEDIA_PREV_TRACK, 165},  // KEY_PREVIOUSSONG
    {VK_MEDIA_STOP,       166},  // KEY_STOPCD
    {0,      167},  // KEY_RECORD
    {0,      168},  // KEY_REWIND
    {0,      169},  // KEY_PHONE
    {0,      170},  // KEY_ISO
    {0,      171},  // KEY_CONFIG
    {VK_BROWSER_HOME,    172},  // KEY_HOMEPAGE
    {VK_BROWSER_REFRESH, 173},  // KEY_REFRESH
    {0,      174},  // KEY_EXIT
    {0,      175},  // KEY_MOVE
    {0,      176},  // KEY_EDIT
    {0,      177},  // KEY_SCROLLUP
    {0,      178},  // KEY_SCROLLDOWN
    {0,      179},  // KEY_KPLEFTPAREN
    {0,      180},  // KEY_KPRIGHTPAREN
    {0,      181},  // KEY_NEW
    {0,      182},  // KEY_REDO
    {VK_F13, 183},  // KEY_F13
    {VK_F14, 184},  // KEY_F14
    {VK_F15, 185},  // KEY_F15
    {VK_F16, 186},  // KEY_F16
    {VK_F17, 187},  // KEY_F17
    {VK_F18, 188},  // KEY_F18
    {VK_F19, 189},  // KEY_F19
    {VK_F20, 190},  // KEY_F20
    {VK_F21, 191},  // KEY_F21
    {VK_F22, 192},  // KEY_F22
    {VK_F23, 193},  // KEY_F23
    {VK_F24, 194},  // KEY_F24
    {0,      200},  // KEY_PLAYCD
    {0,      201},  // KEY_PAUSECD
    {0,      202},  // KEY_PROG3
    {0,      203},  // KEY_PROG4
    {0,      205},  // KEY_SUSPEND
    {0,      206},  // KEY_CLOSE
    {VK_PLAY,207},  // KEY_PLAY
    {0,      208},  // KEY_FASTFORWARD
    {0,      209},  // KEY_BASSBOOST
    {0,      210},  // KEY_PRINT
    {0,      211},  // KEY_HP
    {0,      212},  // KEY_CAMERA
    {0,      213},  // KEY_SOUND
    {0,      214},  // KEY_QUESTION
    {0,      215},  // KEY_EMAIL
    {0,      216},  // KEY_CHAT
    {0,      217},  // KEY_SEARCH
    {0,      218},  // KEY_CONNECT
    {0,      219},  // KEY_FINANCE
    {0,      220},  // KEY_SPORT
    {0,      221},  // KEY_SHOP
    {0,      222},  // KEY_ALTERASE
    {0,      223},  // KEY_CANCEL
    {0,      224},  // KEY_BRIGHTNESSDOWN
    {0,      225},  // KEY_BRIGHTNESSUP
    {VK_LAUNCH_MEDIA_SELECT, 226},  // KEY_MEDIA
    {0,      227},  // KEY_SWITCHVIDEOMODE
    {0,      228},  // KEY_KBDILLUMTOGGLE
    {0,      229},  // KEY_KBDILLUMDOWN
    {0,      230},  // KEY_KBDILLUMUP
    {0,      231},  // KEY_SEND
    {0,      232},  // KEY_REPLY
    {0,      233},  // KEY_FORWARDMAIL
    {0,      234},  // KEY_SAVE
    {0,      235},  // KEY_DOCUMENTS
    {0,      236},  // KEY_BATTERY
    {0,      240}   // KEY_UNKNOWN
  };

  // Taken from linux/drivers/hid/hid-input.c
  static const unsigned short sHidMappingConsumer[][3] = {   
    {0x030,  EV_KEY,  KEY_POWER},
    {0x031,  EV_KEY,  KEY_RESTART},
    {0x032,  EV_KEY,  KEY_SLEEP},
    {0x034,  EV_KEY,  KEY_SLEEP},
    {0x035,  EV_KEY,  KEY_KBDILLUMTOGGLE},
    {0x036,  EV_KEY,  BTN_MISC},
    {0x040,  EV_KEY,  KEY_MENU},
    {0x041,  EV_KEY,  KEY_SELECT},
    {0x042,  EV_KEY,  KEY_UP},
    {0x043,  EV_KEY,  KEY_DOWN},
    {0x044,  EV_KEY,  KEY_LEFT},
    {0x045,  EV_KEY,  KEY_RIGHT},
    {0x046,  EV_KEY,  KEY_ESC},
    {0x047,  EV_KEY,  KEY_KPPLUS},
    {0x048,  EV_KEY,  KEY_KPMINUS},
    {0x060,  EV_KEY,  KEY_INFO},
    {0x061,  EV_KEY,  KEY_SUBTITLE},
    {0x063,  EV_KEY,  KEY_VCR},
    {0x065,  EV_KEY,  KEY_CAMERA},
    {0x069,  EV_KEY,  KEY_RED},
    {0x06a,  EV_KEY,  KEY_GREEN},
    {0x06b,  EV_KEY,  KEY_BLUE},
    {0x06c,  EV_KEY,  KEY_YELLOW},
    {0x06d,  EV_KEY,  KEY_ZOOM},
    {0x082,  EV_KEY,  KEY_VIDEO_NEXT},
    {0x083,  EV_KEY,  KEY_LAST},
    {0x084,  EV_KEY,  KEY_ENTER},
    {0x088,  EV_KEY,  KEY_PC},
    {0x089,  EV_KEY,  KEY_TV},
    {0x08a,  EV_KEY,  KEY_WWW},
    {0x08b,  EV_KEY,  KEY_DVD},
    {0x08c,  EV_KEY,  KEY_PHONE},
    {0x08d,  EV_KEY,  KEY_PROGRAM},
    {0x08e,  EV_KEY,  KEY_VIDEOPHONE},
    {0x08f,  EV_KEY,  KEY_GAMES},
    {0x090,  EV_KEY,  KEY_MEMO},
    {0x091,  EV_KEY,  KEY_CD},
    {0x092,  EV_KEY,  KEY_VCR},
    {0x093,  EV_KEY,  KEY_TUNER},
    {0x094,  EV_KEY,  KEY_EXIT},
    {0x095,  EV_KEY,  KEY_HELP},
    {0x096,  EV_KEY,  KEY_TAPE},
    {0x097,  EV_KEY,  KEY_TV2},
    {0x098,  EV_KEY,  KEY_SAT},
    {0x09a,  EV_KEY,  KEY_PVR},
    {0x09c,  EV_KEY,  KEY_CHANNELUP},
    {0x09d,  EV_KEY,  KEY_CHANNELDOWN},
    {0x0a0,  EV_KEY,  KEY_VCR2},
    {0x0b0,  EV_KEY,  KEY_PLAY},
    {0x0b1,  EV_KEY,  KEY_PAUSE},
    {0x0b2,  EV_KEY,  KEY_RECORD},
    {0x0b3,  EV_KEY,  KEY_FASTFORWARD},
    {0x0b4,  EV_KEY,  KEY_REWIND},
    {0x0b5,  EV_KEY,  KEY_NEXTSONG},
    {0x0b6,  EV_KEY,  KEY_PREVIOUSSONG},
    {0x0b7,  EV_KEY,  KEY_STOPCD},
    {0x0b8,  EV_KEY,  KEY_EJECTCD},
    {0x0bc,  EV_KEY,  KEY_MEDIA_REPEAT},
    {0x0b9,  EV_KEY,  KEY_SHUFFLE},
    {0x0bf,  EV_KEY,  KEY_SLOW},
    {0x0cd,  EV_KEY,  KEY_PLAYPAUSE},
    {0x0e0,  EV_ABS,  ABS_VOLUME},
    {0x0e2,  EV_KEY,  KEY_MUTE},
    {0x0e5,  EV_KEY,  KEY_BASSBOOST},
    {0x0e9,  EV_KEY,  KEY_VOLUMEUP},
    {0x0ea,  EV_KEY,  KEY_VOLUMEDOWN},
    {0x0f5,  EV_KEY,  KEY_SLOW},
    {0x182,  EV_KEY,  KEY_BOOKMARKS},
    {0x183,  EV_KEY,  KEY_CONFIG},
    {0x184,  EV_KEY,  KEY_WORDPROCESSOR},
    {0x185,  EV_KEY,  KEY_EDITOR},
    {0x186,  EV_KEY,  KEY_SPREADSHEET},
    {0x187,  EV_KEY,  KEY_GRAPHICSEDITOR},
    {0x188,  EV_KEY,  KEY_PRESENTATION},
    {0x189,  EV_KEY,  KEY_DATABASE},
    {0x18a,  EV_KEY,  KEY_MAIL},
    {0x18b,  EV_KEY,  KEY_NEWS},
    {0x18c,  EV_KEY,  KEY_VOICEMAIL},
    {0x18d,  EV_KEY,  KEY_ADDRESSBOOK},
    {0x18e,  EV_KEY,  KEY_CALENDAR},
    {0x191,  EV_KEY,  KEY_FINANCE},
    {0x192,  EV_KEY,  KEY_CALC},
    {0x193,  EV_KEY,  KEY_PLAYER},
    {0x194,  EV_KEY,  KEY_FILE},
    {0x196,  EV_KEY,  KEY_WWW},
    {0x199,  EV_KEY,  KEY_CHAT},
    {0x19c,  EV_KEY,  KEY_LOGOFF},
    {0x19e,  EV_KEY,  KEY_COFFEE},
    {0x1a6,  EV_KEY,  KEY_HELP},
    {0x1a7,  EV_KEY,  KEY_DOCUMENTS},
    {0x1ab,  EV_KEY,  KEY_SPELLCHECK},
    {0x1ae,  EV_KEY,  KEY_KEYBOARD},
    {0x1b6,  EV_KEY,  KEY_IMAGES},
    {0x1b7,  EV_KEY,  KEY_AUDIO},
    {0x1b8,  EV_KEY,  KEY_VIDEO},
    {0x1bc,  EV_KEY,  KEY_MESSENGER},
    {0x1bd,  EV_KEY,  KEY_INFO},
    {0x201,  EV_KEY,  KEY_NEW},
    {0x202,  EV_KEY,  KEY_OPEN},
    {0x203,  EV_KEY,  KEY_CLOSE},
    {0x204,  EV_KEY,  KEY_EXIT},
    {0x207,  EV_KEY,  KEY_SAVE},
    {0x208,  EV_KEY,  KEY_PRINT},
    {0x209,  EV_KEY,  KEY_PROPS},
    {0x21a,  EV_KEY,  KEY_UNDO},
    {0x21b,  EV_KEY,  KEY_COPY},
    {0x21c,  EV_KEY,  KEY_CUT},
    {0x21d,  EV_KEY,  KEY_PASTE},
    {0x21f,  EV_KEY,  KEY_FIND},
    {0x221,  EV_KEY,  KEY_SEARCH},
    {0x222,  EV_KEY,  KEY_GOTO},
    {0x223,  EV_KEY,  KEY_HOMEPAGE},
    {0x224,  EV_KEY,  KEY_BACK},
    {0x225,  EV_KEY,  KEY_FORWARD},
    {0x226,  EV_KEY,  KEY_STOP},
    {0x227,  EV_KEY,  KEY_REFRESH},
    {0x22a,  EV_KEY,  KEY_BOOKMARKS},
    {0x22d,  EV_KEY,  KEY_ZOOMIN},
    {0x22e,  EV_KEY,  KEY_ZOOMOUT},
    {0x22f,  EV_KEY,  KEY_ZOOMRESET},
    {0x233,  EV_KEY,  KEY_SCROLLUP},
    {0x234,  EV_KEY,  KEY_SCROLLDOWN},
    {0x238,  EV_REL, REL_HWHEEL},
    {0x23d,  EV_KEY,  KEY_EDIT},
    {0x25f,  EV_KEY,  KEY_CANCEL},
    {0x269,  EV_KEY,  KEY_INSERT},
    {0x26a,  EV_KEY,  KEY_DELETE},
    {0x279,  EV_KEY,  KEY_REDO},
    {0x289,  EV_KEY,  KEY_REPLY},
    {0x28b,  EV_KEY,  KEY_FORWARDMAIL},
    {0x28c,  EV_KEY,  KEY_SEND}
  };

  static const unsigned short sHidMappingValue[][4] = {   
    {0x01, 0x30,  EV_ABS,  ABS_X},
    {0x01, 0x31,  EV_ABS,  ABS_Y},
    {0x01, 0x32,  EV_ABS,  ABS_Z},
    {0x01, 0x33,  EV_ABS,  ABS_RX},
    {0x01, 0x34,  EV_ABS,  ABS_RY},
    {0x01, 0x35,  EV_ABS,  ABS_RZ}
  };

}

av::daemon::WindowsEventMapping::WindowsEventMapping()
{
  for (std::size_t i = 0; i < sizeof(sKeyboardEventPairs)/sizeof(*sKeyboardEventPairs); ++i)
    mKeyboardVkey.insert(std::make_pair(sKeyboardEventPairs[i][0], sKeyboardEventPairs[i][1]));

  for (std::size_t i = 0; i < sizeof(sHidMappingConsumer)/sizeof(*sHidMappingConsumer); ++i)
    mHidMappingConsumer.insert(std::make_pair(sHidMappingConsumer[i][0], 
                                              std::make_pair(sHidMappingConsumer[i][1], sHidMappingConsumer[i][2])));

  for (std::size_t i = 0; i < sizeof(sHidMappingValue)/sizeof(*sHidMappingValue); ++i)
    mHidMappingValue.insert(std::make_pair(std::make_pair(sHidMappingValue[i][0], sHidMappingValue[i][1]), 
                                           std::make_pair(sHidMappingValue[i][2], sHidMappingValue[i][3])));
}

unsigned long 
av::daemon::WindowsEventMapping::mapKeyboardEvent(USHORT VKey)
{
  ::std::map<USHORT, unsigned long>::const_iterator return_value = sWindowsEventMap.mKeyboardVkey.find(VKey);
  return (return_value != sWindowsEventMap.mKeyboardVkey.end()) ? return_value->second : 0;
}

bool 
av::daemon::WindowsEventMapping::mapHidButtonEvent(unsigned short usagePage, 
                                                   unsigned short usageId, 
                                                   unsigned short appUsagePage, 
                                                   unsigned short appUsageId, 
                                                   EventPair& ev)
{
  bool ret = false;
  switch (usagePage)
  {
  case HID_USAGE_PAGE_BUTTON: {
    unsigned short code = usageId - 1;
    if (appUsagePage == HID_USAGE_PAGE_GENERIC)
    {
      switch (appUsageId) 
      {
      case HID_USAGE_GENERIC_MOUSE:
      case HID_USAGE_GENERIC_POINTER:  code += BTN_MOUSE; break;
      case HID_USAGE_GENERIC_JOYSTICK: code += BTN_JOYSTICK; break;
      case HID_USAGE_GENERIC_GAMEPAD:  code += BTN_GAMEPAD; break;
      default: code += BTN_MISC;
      }
    } else code += BTN_MISC;

    ev = std::make_pair(EV_KEY, code);
    ret = true;
    break; }

  case HID_USAGE_PAGE_CONSUMER:
    ::std::map<unsigned short, EventPair >::const_iterator return_value = sWindowsEventMap.mHidMappingConsumer.find(usageId);
    if (return_value != sWindowsEventMap.mHidMappingConsumer.end()) 
    {
      ev = return_value->second;
      ret = true;
    }
    break;
  }

  return ret;
}

bool 
av::daemon::WindowsEventMapping::mapHidValueEvent(unsigned short usagePage, 
                                                  unsigned short usageId, 
                                                  unsigned short appUsagePage,
                                                  unsigned short appUsageId, 
                                                  EventPair& ev)
{
  typedef ::std::map<std::pair<unsigned short, unsigned short>, EventPair > ValMap;
  bool ret = false;
  ValMap::const_iterator return_value = sWindowsEventMap.mHidMappingValue.find(std::make_pair(usagePage, usageId));
  if (return_value != sWindowsEventMap.mHidMappingValue.end()) 
  {
    ev = return_value->second;
    ret = true;
  }
  return ret;
}
