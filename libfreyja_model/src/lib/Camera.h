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
 *           TODO: Move to quaterions ASAP!
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2001.05.18:
 * Mongoose - Created, based on my old GL camera code
 *            that has been used in GooseEgg since alpha
 *            and algorithms from Yuri Zhivago's trview
 ================================================================*/


#ifndef __FREYJA_MONGOOSE_CAMERA_H_
#define __FREYJA_MONGOOSE_CAMERA_H_


enum camera_command                /* Interactive camera control */
{
  CAMERA_MOVE_FORWARD = 1,
  CAMERA_MOVE_BACKWARD,
  CAMERA_MOVE_UP,
  CAMERA_MOVE_DOWN,
  CAMERA_ROTATE_RIGHT,
  CAMERA_ROTATE_LEFT,
  CAMERA_SPEED_UP,
  CAMERA_SPEED_DOWN,
};


class Camera
{
 public:

  Camera();
  /*------------------------------------------------------
   * Pre  : 
   * Post : Constructs an object of Camera
   *
   *-- History ------------------------------------------
   *
   * 2001.05.18: 
   * Mongoose - Created
   ------------------------------------------------------*/


  ~Camera();
  /*------------------------------------------------------
   * Pre  : Camera object is allocated
   * Post : Deconstructs an object of Camera
   *
   *-- History ------------------------------------------
   *
   * 2001.05.18: 
   * Mongoose - Created
   ------------------------------------------------------*/


  void Reset();
  /*------------------------------------------------------
   * Pre  : 
   * Post : Camera is set to inital state
   *
   *-- History ------------------------------------------
   *
   * 2001.05.18: 
   * Mongoose - Created
   ------------------------------------------------------*/


  double Eye(unsigned int i);
  /*------------------------------------------------------
   * Pre  : i = {(0, X), (1, Y), (2, Z)}
   * Post : Returns various data values for axis
   *
   *-- History ------------------------------------------
   *
   * 2001.05.18: 
   * Mongoose - Created
   ------------------------------------------------------*/


  double Target(unsigned int i);
  /*------------------------------------------------------
   * Pre  : i = {(0, X), (1, Y), (2, Z)}
   * Post : Returns various data values for axis
   *
   *-- History ------------------------------------------
   *
   * 2001.05.18: 
   * Mongoose - Created
   ------------------------------------------------------*/


  void Command(enum camera_command cmd);
  /*------------------------------------------------------
   * Pre  : Command is valid camera command
   * Post : Sends interactive command to camera
   *
   *-- History ------------------------------------------
   *
   * 2001.05.18: 
   * Mongoose - Created
   ------------------------------------------------------*/


  void Update();
  /*------------------------------------------------------
   * Pre  : 
   * Post : Updates viewing target before camera is called
	*        to draw
   *
   *-- History ------------------------------------------
   *
   * 2001.05.18: 
   * Mongoose - Created
   ------------------------------------------------------*/


 private:

  unsigned int _id;           /* Unquie id */

  int _camera_model;          /* Model id to render for camera */

  int _view_model;            /* Model id to render for 3rd person view */

  double _pos[3];             /* Location in 3 space (aka eye) */

  double _at[3];              /* Postition we're looking at  */

  bool _update;               /* Check to see if view needs updating */      

  unsigned int _option;       /* For testing with flags  */

  double _view_distance;      /* Distance from target */

  double _translate_delta;    /* Step size to move */

  double _rotate_delta;       /* Radians to rotate */

  double _theta;              /* View angle */
};

#endif
