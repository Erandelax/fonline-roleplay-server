#ifndef __KEYBOARD__
#define __KEYBOARD__

// Keyboard input flags
#define KIF_NO_SPEC_SYMBOLS    ( 1 )  // Ignore \n \r \t
#define KIF_ONLY_NUMBERS       ( 2 )  // Only 0..9
#define KIF_FILE_NAME          ( 4 )  // Ignore \/:*?\"<>|

namespace Keyb
{
    extern bool ShiftDwn;
    extern bool CtrlDwn;
    extern bool AltDwn;
    extern bool KeyPressed[ 0x100 ];

    void   Init();
    void   Lost();
    void   GetChar( uchar dik, const char* dik_text, string& str, uint* position, uint max, int flags );
    void   GetChar( uchar dik, const char* dik_text, char* str, uint str_size, uint* position, uint max, int flags );
    void   EraseInvalidChars( char* str, int flags );
    uchar  MapKey( ushort code );
    ushort UnmapKey( uchar key );
}

// Key codes
#ifndef MAKE_KEY_CODE
# define MAKE_KEY_CODE( name, index, code )    const uchar name = index
#endif
MAKE_KEY_CODE( DIK_ESCAPE, 0x01, SDL_SCANCODE_ESCAPE );
MAKE_KEY_CODE( DIK_1, 0x02, SDL_SCANCODE_1 );
MAKE_KEY_CODE( DIK_2, 0x03, SDL_SCANCODE_2 );
MAKE_KEY_CODE( DIK_3, 0x04, SDL_SCANCODE_3 );
MAKE_KEY_CODE( DIK_4, 0x05, SDL_SCANCODE_4 );
MAKE_KEY_CODE( DIK_5, 0x06, SDL_SCANCODE_5 );
MAKE_KEY_CODE( DIK_6, 0x07, SDL_SCANCODE_6 );
MAKE_KEY_CODE( DIK_7, 0x08, SDL_SCANCODE_7 );
MAKE_KEY_CODE( DIK_8, 0x09, SDL_SCANCODE_8 );
MAKE_KEY_CODE( DIK_9, 0x0A, SDL_SCANCODE_9 );
MAKE_KEY_CODE( DIK_0, 0x0B, SDL_SCANCODE_0 );
MAKE_KEY_CODE( DIK_MINUS, 0x0C, SDL_SCANCODE_MINUS );              /* - on main keyboard */
MAKE_KEY_CODE( DIK_EQUALS, 0x0D, SDL_SCANCODE_EQUALS );
MAKE_KEY_CODE( DIK_BACK, 0x0E, SDL_SCANCODE_BACKSPACE );           /* backspace */
MAKE_KEY_CODE( DIK_TAB, 0x0F, SDL_SCANCODE_TAB );
MAKE_KEY_CODE( DIK_Q, 0x10, SDL_SCANCODE_Q );
MAKE_KEY_CODE( DIK_W, 0x11, SDL_SCANCODE_W );
MAKE_KEY_CODE( DIK_E, 0x12, SDL_SCANCODE_E );
MAKE_KEY_CODE( DIK_R, 0x13, SDL_SCANCODE_R );
MAKE_KEY_CODE( DIK_T, 0x14, SDL_SCANCODE_T );
MAKE_KEY_CODE( DIK_Y, 0x15, SDL_SCANCODE_Y );
MAKE_KEY_CODE( DIK_U, 0x16, SDL_SCANCODE_U );
MAKE_KEY_CODE( DIK_I, 0x17, SDL_SCANCODE_I );
MAKE_KEY_CODE( DIK_O, 0x18, SDL_SCANCODE_O );
MAKE_KEY_CODE( DIK_P, 0x19, SDL_SCANCODE_P );
MAKE_KEY_CODE( DIK_LBRACKET, 0x1A, SDL_SCANCODE_LEFTBRACKET );
MAKE_KEY_CODE( DIK_RBRACKET, 0x1B, SDL_SCANCODE_RIGHTBRACKET );
MAKE_KEY_CODE( DIK_RETURN, 0x1C, SDL_SCANCODE_RETURN );             /* Enter on main keyboard */
MAKE_KEY_CODE( DIK_LCONTROL, 0x1D, SDL_SCANCODE_LCTRL );
MAKE_KEY_CODE( DIK_A, 0x1E, SDL_SCANCODE_A );
MAKE_KEY_CODE( DIK_S, 0x1F, SDL_SCANCODE_S );
MAKE_KEY_CODE( DIK_D, 0x20, SDL_SCANCODE_D );
MAKE_KEY_CODE( DIK_F, 0x21, SDL_SCANCODE_F );
MAKE_KEY_CODE( DIK_G, 0x22, SDL_SCANCODE_G );
MAKE_KEY_CODE( DIK_H, 0x23, SDL_SCANCODE_H );
MAKE_KEY_CODE( DIK_J, 0x24, SDL_SCANCODE_J );
MAKE_KEY_CODE( DIK_K, 0x25, SDL_SCANCODE_K );
MAKE_KEY_CODE( DIK_L, 0x26, SDL_SCANCODE_L );
MAKE_KEY_CODE( DIK_SEMICOLON, 0x27, SDL_SCANCODE_SEMICOLON );
MAKE_KEY_CODE( DIK_APOSTROPHE, 0x28, SDL_SCANCODE_APOSTROPHE );
MAKE_KEY_CODE( DIK_GRAVE, 0x29, SDL_SCANCODE_GRAVE );              /* accent grave */
MAKE_KEY_CODE( DIK_LSHIFT, 0x2A, SDL_SCANCODE_LSHIFT );
MAKE_KEY_CODE( DIK_BACKSLASH, 0x2B, SDL_SCANCODE_BACKSLASH );
MAKE_KEY_CODE( DIK_Z, 0x2C, SDL_SCANCODE_Z );
MAKE_KEY_CODE( DIK_X, 0x2D, SDL_SCANCODE_X );
MAKE_KEY_CODE( DIK_C, 0x2E, SDL_SCANCODE_C );
MAKE_KEY_CODE( DIK_V, 0x2F, SDL_SCANCODE_V );
MAKE_KEY_CODE( DIK_B, 0x30, SDL_SCANCODE_B );
MAKE_KEY_CODE( DIK_N, 0x31, SDL_SCANCODE_N );
MAKE_KEY_CODE( DIK_M, 0x32, SDL_SCANCODE_M );
MAKE_KEY_CODE( DIK_COMMA, 0x33, SDL_SCANCODE_COMMA );
MAKE_KEY_CODE( DIK_PERIOD, 0x34, SDL_SCANCODE_PERIOD );        /* . on main keyboard */
MAKE_KEY_CODE( DIK_SLASH, 0x35, SDL_SCANCODE_SLASH );          /* / on main keyboard */
MAKE_KEY_CODE( DIK_RSHIFT, 0x36, SDL_SCANCODE_RSHIFT );
MAKE_KEY_CODE( DIK_MULTIPLY, 0x37, SDL_SCANCODE_KP_MULTIPLY ); /* * on numeric keypad */
MAKE_KEY_CODE( DIK_LMENU, 0x38, SDL_SCANCODE_LALT );           /* left Alt */
MAKE_KEY_CODE( DIK_SPACE, 0x39, SDL_SCANCODE_SPACE );
MAKE_KEY_CODE( DIK_CAPITAL, 0x3A, SDL_SCANCODE_CAPSLOCK );     // ?
MAKE_KEY_CODE( DIK_F1, 0x3B, SDL_SCANCODE_F1 );
MAKE_KEY_CODE( DIK_F2, 0x3C, SDL_SCANCODE_F2 );
MAKE_KEY_CODE( DIK_F3, 0x3D, SDL_SCANCODE_F3 );
MAKE_KEY_CODE( DIK_F4, 0x3E, SDL_SCANCODE_F4 );
MAKE_KEY_CODE( DIK_F5, 0x3F, SDL_SCANCODE_F5 );
MAKE_KEY_CODE( DIK_F6, 0x40, SDL_SCANCODE_F6 );
MAKE_KEY_CODE( DIK_F7, 0x41, SDL_SCANCODE_F7 );
MAKE_KEY_CODE( DIK_F8, 0x42, SDL_SCANCODE_F8 );
MAKE_KEY_CODE( DIK_F9, 0x43, SDL_SCANCODE_F9 );
MAKE_KEY_CODE( DIK_F10, 0x44, SDL_SCANCODE_F10 );
MAKE_KEY_CODE( DIK_NUMLOCK, 0x45, SDL_SCANCODE_NUMLOCKCLEAR );
MAKE_KEY_CODE( DIK_SCROLL, 0x46, SDL_SCANCODE_SCROLLLOCK );             /* Scroll Lock */
MAKE_KEY_CODE( DIK_NUMPAD7, 0x47, SDL_SCANCODE_KP_7 );
MAKE_KEY_CODE( DIK_NUMPAD8, 0x48, SDL_SCANCODE_KP_8 );
MAKE_KEY_CODE( DIK_NUMPAD9, 0x49, SDL_SCANCODE_KP_9 );
MAKE_KEY_CODE( DIK_SUBTRACT, 0x4A, SDL_SCANCODE_KP_MINUS );           /* - on numeric keypad */
MAKE_KEY_CODE( DIK_NUMPAD4, 0x4B, SDL_SCANCODE_KP_4 );
MAKE_KEY_CODE( DIK_NUMPAD5, 0x4C, SDL_SCANCODE_KP_5 );
MAKE_KEY_CODE( DIK_NUMPAD6, 0x4D, SDL_SCANCODE_KP_6 );
MAKE_KEY_CODE( DIK_ADD, 0x4E, SDL_SCANCODE_KP_PLUS );                /* + on numeric keypad */
MAKE_KEY_CODE( DIK_NUMPAD1, 0x4F, SDL_SCANCODE_KP_1 );
MAKE_KEY_CODE( DIK_NUMPAD2, 0x50, SDL_SCANCODE_KP_2 );
MAKE_KEY_CODE( DIK_NUMPAD3, 0x51, SDL_SCANCODE_KP_3 );
MAKE_KEY_CODE( DIK_NUMPAD0, 0x52, SDL_SCANCODE_KP_0 );
MAKE_KEY_CODE( DIK_DECIMAL, 0x53, SDL_SCANCODE_KP_PERIOD );    /* . on numeric keypad */
MAKE_KEY_CODE( DIK_F11, 0x57, SDL_SCANCODE_F11 );
MAKE_KEY_CODE( DIK_F12, 0x58, SDL_SCANCODE_F12 );
MAKE_KEY_CODE( DIK_NUMPADENTER, 0x9C, SDL_SCANCODE_KP_ENTER ); /* Enter on numeric keypad */
MAKE_KEY_CODE( DIK_RCONTROL, 0x9D, SDL_SCANCODE_RCTRL );
MAKE_KEY_CODE( DIK_DIVIDE, 0xB5, SDL_SCANCODE_KP_DIVIDE );     /* / on numeric keypad */
MAKE_KEY_CODE( DIK_SYSRQ, 0xB7, SDL_SCANCODE_SYSREQ );
MAKE_KEY_CODE( DIK_RMENU, 0xB8, SDL_SCANCODE_RALT );           /* right Alt */
MAKE_KEY_CODE( DIK_PAUSE, 0xC5, SDL_SCANCODE_PAUSE );          /* Pause */
MAKE_KEY_CODE( DIK_HOME, 0xC7, SDL_SCANCODE_HOME );            /* Home on arrow keypad */
MAKE_KEY_CODE( DIK_UP, 0xC8, SDL_SCANCODE_UP );                /* UpArrow on arrow keypad */
MAKE_KEY_CODE( DIK_PRIOR, 0xC9, SDL_SCANCODE_PAGEUP );         /* PgUp on arrow keypad */
MAKE_KEY_CODE( DIK_LEFT, 0xCB, SDL_SCANCODE_LEFT );            /* LeftArrow on arrow keypad */
MAKE_KEY_CODE( DIK_RIGHT, 0xCD, SDL_SCANCODE_RIGHT );          /* RightArrow on arrow keypad */
MAKE_KEY_CODE( DIK_END, 0xCF, SDL_SCANCODE_END );              /* End on arrow keypad */
MAKE_KEY_CODE( DIK_DOWN, 0xD0, SDL_SCANCODE_DOWN );            /* DownArrow on arrow keypad */
MAKE_KEY_CODE( DIK_NEXT, 0xD1, SDL_SCANCODE_PAGEDOWN );        /* PgDn on arrow keypad */
MAKE_KEY_CODE( DIK_INSERT, 0xD2, SDL_SCANCODE_INSERT );        /* Insert on arrow keypad */
MAKE_KEY_CODE( DIK_DELETE, 0xD3, SDL_SCANCODE_DELETE );        /* Delete on arrow keypad */
MAKE_KEY_CODE( DIK_LWIN, 0xDB, SDL_SCANCODE_LGUI );            // ?               /* Left Windows key */
MAKE_KEY_CODE( DIK_RWIN, 0xDC, SDL_SCANCODE_RGUI );            // ?               /* Right Windows key */
MAKE_KEY_CODE( DIK_TEXT, 0xFE, 510 );
MAKE_KEY_CODE( DIK_CLIPBOARD_PASTE, 0xFF, 511 );

#endif // __KEYBOARD__
