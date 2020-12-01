// Types.h
//
//	Common used types for VIGASOCO
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _VIGASOCO_TYPES_H_
#define _VIGASOCO_TYPES_H_


static const int MIN_INT = - 2147483647 - 1;
static const int MAX_INT = 2147483647;

typedef unsigned char UINT8;
typedef signed char INT8;
typedef unsigned short UINT16;
typedef signed short INT16;
typedef unsigned int UINT32;
typedef signed int INT32;

//TODO: otra opcion seria usar los tipos de SDL...
#ifdef __GNUC__
typedef signed long long INT64;
typedef unsigned long long UINT64;
#else
typedef signed __int64 INT64;
typedef unsigned __int64 UINT64;
#endif


struct Rect {
	int left;
	int top;
	int right;
	int bottom;

	Rect() { left = top = right = bottom = 0; }
	Rect(int l, int t, int r, int b) { left = l; top = t; right = r; bottom = b; }
	Rect(int width, int height) { left = top = 0; right = width - 1;  bottom = height - 1; }
};

struct VideoInfo {
	int width, height;
	Rect visibleArea;
	int colors;
	int refreshRate;

	VideoInfo() { width = height = colors = refreshRate = 0; }
};


enum Inputs {
	UNMAPPED = -1,
	// game driver inputs
	GAME_DRIVER_INPUTS = 0,
	P1_UP = 0,
	P1_LEFT,
	P1_DOWN,
	P1_RIGHT,
	P1_BUTTON1,
	P1_BUTTON2,
	P2_UP,
	P2_LEFT,
	P2_DOWN,
	P2_RIGHT,
	P2_BUTTON1,
	P2_BUTTON2,
	START_1,
	START_2,
	COIN_1,
	COIN_2,
	SERVICE_1,
	SERVICE_2,

	// keyboard inputs
	KEYBOARD_INPUTS,
	KEYBOARD_A = KEYBOARD_INPUTS,
	KEYBOARD_B,
	KEYBOARD_C,
	KEYBOARD_D,
	KEYBOARD_E,
	KEYBOARD_F,
	KEYBOARD_G,
	KEYBOARD_H,
	KEYBOARD_I,
	KEYBOARD_J,
	KEYBOARD_K,
	KEYBOARD_L,
	KEYBOARD_M,
	KEYBOARD_N,
	KEYBOARD_O,
	KEYBOARD_P,
	KEYBOARD_Q,
	KEYBOARD_R,
	KEYBOARD_S,
	KEYBOARD_T,
	KEYBOARD_U,
	KEYBOARD_V,
	KEYBOARD_W,
	KEYBOARD_X,
	KEYBOARD_Y,
	KEYBOARD_Z,
	KEYBOARD_0,
	KEYBOARD_1,
	KEYBOARD_2,
	KEYBOARD_3,
	KEYBOARD_4,
	KEYBOARD_5,
	KEYBOARD_6,
	KEYBOARD_7,
	KEYBOARD_8,
	KEYBOARD_9,
	KEYBOARD_SPACE,
	KEYBOARD_INTRO,
	KEYBOARD_SUPR,

	// core inputs
	CORE_INPUTS,
	FUNCTION_1 = CORE_INPUTS,
	FUNCTION_2,
	FUNCTION_3,
	FUNCTION_4,
	FUNCTION_5,
	FUNCTION_6,
	FUNCTION_7,
	FUNCTION_8,
	FUNCTION_9,
	FUNCTION_10,
	FUNCTION_11,
	FUNCTION_12,

	END_OF_INPUTS
};

enum ParameterType {
	PARAM_BOOLEAN = 0x00001000,
	PARAM_INTEGER = 0x00002000,
	PARAM_INPUT = 0x00003000,
	PARAM_ARRAY = 0x80000000
};

enum PluginType {
	VIDEO_PLUGIN = 0x1000,
	INPUT_PLUGIN = 0x2000,
	AUDIO_PLUGIN = 0x3000,
	LOADER_PLUGIN = 0x4000
};

#endif	// _VIGASOCO_TYPES_H_
