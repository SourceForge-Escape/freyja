/* -*- Mode: C; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Camera
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: OpenGL camera class for Freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.05.18:
 * Mongoose - Created
 =================================================================*/

#include "Camera.h"
#include <math.h>

unsigned int __CAMERA_CAMERA_COUNTER = 0;
const double Pi      = 3.141592653589;
const double TwoPi   = 3.141592653589 * 2.0;
const double PiOver2 = 3.141592653589 / 2.0;


Camera::Camera()
{
	_id = __CAMERA_CAMERA_COUNTER++;
	_camera_model = -1;
	_view_model = -1;
	_option = 0;
	Reset();
}


Camera::~Camera()
{
}


void Camera::Reset()
{
	_view_distance = 14.0;
	_translate_delta = 16.0;
	_rotate_delta = (Pi / 18.0);
	_theta = 0.0;

	_pos[0] = 0.0;
	_pos[1] = 0.0;
	_pos[2] = 128.0;

	_at[0] = 0.0;
	_at[1] = 0.0;
	_at[2] = 0.0;
}


double Camera::Eye(unsigned int i)
{
	switch (i)
	{
	case 0:
		return _pos[0];
		break;
	case 1:
		return _pos[1];
		break;
	case 2:
		return _pos[2];
		break;
	default:
		return 0.0;
	}
}


double Camera::Target(unsigned int i)
{
	switch (i)
	{
	case 0:
		return _at[0];
		break;
	case 1:
		return _at[1];
		break;
	case 2:
		return _at[2];
		break;
	default:
		return 0.0;
	}
}


void Camera::Command(enum camera_command cmd)
{
  switch (cmd)
  {
  case CAMERA_MOVE_BACKWARD:
	  _pos[2] -= (_translate_delta * cos(_theta));
	  _pos[0] -= (_translate_delta * sin(_theta));
	  break;
  case CAMERA_MOVE_FORWARD:
	  _pos[2] += (_translate_delta * cos(_theta));
	  _pos[0] += (_translate_delta * sin(_theta));
	  break;
 case CAMERA_ROTATE_RIGHT:
	 _theta += _rotate_delta;

	 if (_theta > Pi)
		 _theta -= TwoPi;

	 _at[2] = (_view_distance * cos(_theta)) + _pos[2];
	 _at[0] = (_view_distance * sin(_theta)) + _pos[0];
	 break;
  case CAMERA_ROTATE_LEFT:
	  _theta -=  _rotate_delta;

	  if (_theta < -Pi)
		  _theta += TwoPi;

	  _at[2] = (_view_distance * cos(_theta)) + _pos[2];
	  _at[0] = (_view_distance * sin(_theta)) + _pos[0];
	  break;

  // Go shorter distance on vertical movements
  case CAMERA_MOVE_UP:
	  _pos[1] -= _translate_delta / 2.0;   
	  _at[1] -= _translate_delta / 2.0;
	  break;
  case CAMERA_MOVE_DOWN:
	  _pos[1] += _translate_delta / 2.0;
	  _at[1] += _translate_delta / 2.0;
	  break;
  case CAMERA_SPEED_UP:
    _translate_delta++;
    break;
  case CAMERA_SPEED_DOWN:
    _translate_delta--;
  
    if (_translate_delta < 0.0)
      _translate_delta = 1.0;
  default:
    ;
  }
}


void Camera::Update()
{
   _at[2] = (_view_distance * cos(_theta)) + _pos[2];
   _at[0] = (_view_distance * sin(_theta)) + _pos[0];
	_at[1] = _pos[1]; // + _offest; // Removed toggle 'quick views'
}


#ifdef _CAMERA_GLU_BUILD
void Camera::LookAt()
{
	Update();

	gluLookAt(Eye(0), Eye(1), Eye(2),
				 Target(0),  Target(1), Target(2),
				 0.0, -1.0, 0.0);
}
#endif

#ifdef OBSOLETE
   if (_view_model)
   {
		glPushMatrix();
 		  glTranslatef(_at[0], _at[1] - 256.0, _at[2]);
		  DrawModel(_view_model);
		glPopMatrix();
	}
#endif

  ////// PRIVATE METHODS //////////////////////////////////////////////

