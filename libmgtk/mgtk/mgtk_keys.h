/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the generic mlisp bridge to the mgtk widget system.
 *
 *           It provides a 2-way event system w/o requiring direct bindings.
 *           ( eg You can make this a dlopen'able interface facade. )
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.27:
 * Mongoose - Created, from old mlisp and mgtk source headers
 ==========================================================================*/

#ifndef GUARD__MGTK_MONGOOSE_MGTK_KEYS_H_
#define GUARD__MGTK_MONGOOSE_MGTK_KEYS_H_

typedef enum
{
	KEY_LSHIFT =   1,
	KEY_RSHIFT =   2,
	KEY_LCTRL  =   4,
	KEY_RCTRL  =   8,
	KEY_LALT   =  16,
	KEY_RALT   =  32

} mgtk_key_modifers_t;


typedef enum
{
	KEY_UP         = 256,
	KEY_DOWN       = 257,
	KEY_RIGHT      = 258,
	KEY_LEFT       = 259,
	KEY_PAGE_DOWN  = 260,
	KEY_PAGE_UP    = 261,

	KEY_F1         = 500,
	KEY_F2         = 501,
	KEY_F3         = 502,
	KEY_F4         = 503,
	KEY_F5         = 504,
	KEY_F6         = 505,
	KEY_F7         = 506,
	KEY_F8         = 507,
	KEY_F9         = 508,
	KEY_F10        = 509,
	KEY_F11        = 510,
	KEY_F12        = 511,

	KEY_SPACE      = 600

} mgtk_key_std_t;

typedef enum
{
	MOUSE_BTN_RIGHT    =   1,
	MOUSE_BTN_LEFT     =   2,
	MOUSE_BTN_MIDDLE   =   4,
	MOUSE_BTN_UP       =   8,
	MOUSE_BTN_DOWN     =  16

} mgtk_mouse_key_t;

typedef enum
{
	MOUSE_BTN_STATE_NONE      = 0,
	MOUSE_BTN_STATE_PRESSED   = 1

} mgtk_mouse_key_state_t;

#endif
