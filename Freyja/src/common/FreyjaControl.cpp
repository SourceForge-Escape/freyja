/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaControl
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2000-09-10:
 * Mongoose - Created
 =================================================================*/

#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <stdarg.h> 
#include <string.h>

#include "FreyjaControl.h"


void event_register_render(FreyjaRender *r);
void event_register_model(FreyjaModel *m);
void event_register_control(FreyjaControl *c);



FreyjaEvent::FreyjaEvent()
{
	_func_event = NULL;

	_pair_active = false;
	_pair_event[0] = 0;
	_pair_event[1] = 0;
}


FreyjaEvent::~FreyjaEvent()
{
}


void FreyjaEvent::AddEvent(int e, int c)
{
	_pair_event[0] = e;
	_pair_event[1] = c;
	_pair_active = true;
}

	
void FreyjaEvent::Emit()
{
	if (_func_event)
	{
		//(*_func_event)();
	}

	if (_pair_active)
	{
		freyja_event2i(_pair_event[0], _pair_event[1]);
	}
}


////////////////////////////////////////////////////////////////


FreyjaControl::FreyjaControl()
{
	_model = new FreyjaModel();
	_render = new FreyjaRender();
	_render->Register(_model);

	if (EGG_LIB_VERSION != 0x32312E38)
	{
		printf("FreyjaControl::FreyjaControl> Assertion failure\n");
		printf("*** You must upgrade libfreyja_egg to v8.12\n");
		exit(-1);
	}

	if (!_model || !_render)
	{
		printf("FreyjaControl::FreyjaControl> Assertion failure\n");
		exit(-1);
	}
	
	_minor_mode = NO_MODE;
	_major_mode = MODEL_EDIT_MODE;
	
	event_register_render(_render);
	event_register_model(_model);
	event_register_control(this);

	ReadRC();
}


FreyjaControl::~FreyjaControl()
{
}


void FreyjaControl::Event(int mode, int cmd)
{
	static int last_event = EVENT_MESH;
	static int last_cmd = CMD_MESH_SELECT;
	
	
	if (mode == EVENT_REDO_LAST)
	{
		mode = last_event;
		cmd = last_cmd;
	}

	// Mongoose 2002.01.12, Bcast event
	_model->handleEvent(mode, cmd);
	
	switch (mode)
	{
// 	case EVENT_MAIN:
// 		switch (cmd)
// 		{
// 		case CMD_TOGGLE_FULLSCREEN: 
// 			application_window_fullscreen();
// 			break;
// 		}
// 		break;

	case EVENT_FREYJA_MODE:
		switch (cmd)
		{
		case FREYJA_MODE_RENDER_BONETAG:
			_render->Flags(FreyjaRender::RENDER_BONES, 
								!(_render->Flags() & FreyjaRender::RENDER_BONES));
			event_print("Bone Rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_BONES) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_POINTS:
			_render->Flags(FreyjaRender::RENDER_POINTS, 
								!(_render->Flags() & FreyjaRender::RENDER_POINTS));
			event_print("Point Rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_POINTS) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_WIREFRAME:
			_render->Flags(FreyjaRender::RENDER_WIREFRAME, 
								!(_render->Flags() & FreyjaRender::RENDER_WIREFRAME));
			event_print("Wireframe Rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_WIREFRAME) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_FACE:
			_render->Flags(FreyjaRender::RENDER_FACE, 
								!(_render->Flags() & FreyjaRender::RENDER_FACE));
			event_print("Face Rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_FACE) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_LIGHTING:
			_render->Flags(FreyjaRender::RENDER_NORMAL, 
								!(_render->Flags() & FreyjaRender::RENDER_NORMAL));
			_render->Flags(FreyjaRender::RENDER_LIGHTING, 
								!(_render->Flags() & FreyjaRender::RENDER_LIGHTING));
			event_print("GL Lighting is [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_LIGHTING) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_TEXTURE:
			_render->Flags(FreyjaRender::RENDER_TEXTURE, 
								!(_render->Flags() & FreyjaRender::RENDER_TEXTURE));
			event_print("Texture rendering is [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_TEXTURE) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case FREYJA_MODE_RENDER_MATERIAL:
			_render->Flags(FreyjaRender::RENDER_MATERIAL, 
								!(_render->Flags() & FreyjaRender::RENDER_MATERIAL));
			event_print("Material rendering is [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_MATERIAL) ? 
							"ON" : "OFF");
			event_refresh();
			break;

		case FREYJA_MODE_TEXTURE_EDIT:
			_render->ViewMode(VIEWMODE_TEXTURE_EDIT);
			event_refresh();
			event_print("Texture Edit");
			_major_mode = TEXTURE_EDIT_MODE;
			event_set_mouse_active(true);
			break;
		case FREYJA_MODE_MODEL_EDIT:
			_render->ViewMode(VIEWMODE_MODEL_EDIT);
			event_refresh();
			event_print("Model Edit");
			_major_mode = MODEL_EDIT_MODE;
			event_set_mouse_active(true);
			break;
		case FREYJA_MODE_MODEL_VIEW:
			_render->ViewMode(VIEWMODE_MODEL_VIEW);
			event_refresh();
			event_print("Animation View");
			_major_mode = MODEL_VIEW_MODE;
			event_set_mouse_active(true);
			break;
		case FREYJA_MODE_MATERIAL_EDIT:
			_render->ViewMode(VIEWMODE_MATERIAL_EDIT);
			event_refresh();
			event_print("Material Edit");
			_major_mode = MATERIAL_EDIT_MODE;
			event_set_mouse_active(false);
			break;
		case FREYJA_MODE_PLANE_XY:
			event_print("Plane XY");      
			_model->CurrentPlane(Egg::PLANE_XY);
			event_refresh();
			break;
		case FREYJA_MODE_PLANE_XZ:
			event_print("Plane XZ");      
			_model->CurrentPlane(Egg::PLANE_XZ);
			event_refresh();
			break;
		case FREYJA_MODE_PLANE_YZ:
			event_print("Plane YZ");      
			_model->CurrentPlane(Egg::PLANE_YZ);
			event_refresh();
			break;
		}
		break;

	case EVENT_BONE:
		switch (cmd)
		{
		case CMD_BONE_SELECT:
			event_print("Select bone by pivot");
			_minor_mode = BONE_SELECT_MODE;
			break;
		case CMD_BONE_CONNECT:
			event_print("Select bone to connect to current");
			_minor_mode = BONE_CONNECT_MODE;
			break;
		case CMD_BONE_DISCONNECT:
			event_print("Select bone to break from current");
			_minor_mode = BONE_DISCONNECT_MODE;
			break;
		case CMD_BONE_MOVE_PIVOT:
			event_print("Moving bone pivot mode");
			_minor_mode = TAG_MOVE;
			break;
		}
		break;

	case EVENT_MESH:
		switch (cmd)
		{
		case CMD_MESH_SCALE:
			_minor_mode = MODEL_SCALE;
			event_print("Mesh scale mode");
			break;
		case CMD_MESH_ROTATE:
			_minor_mode = MODEL_ROTATE;
			event_print("Mesh rotate mode");
			break;
		case CMD_MESH_SELECT:
			_minor_mode = MESH_SELECT_MODE;
			event_print("Select mesh by center point");
			break;
		case CMD_MESH_MOVE:
			_minor_mode = MESH_MOVE;
			event_print("Mesh move mode");
			break;
		case CMD_MESH_MOVE_CENTER:
			_minor_mode = MESH_MOVE_CENTER;
			event_print("Reposition mesh center point");
			break;
		case CMD_MESH_GEN_FROM_FRAME:
			event_print("FreyjaControl::Event> %s, %s:%i",
							"CMD_MESH_GEN_FROM_FRAME no longer implemented", 
							__FILE__, __LINE__);
			break;
		case CMD_MESH_FULLCOPY:
			_minor_mode = MESH_WHOLE_COPY_MODE;      
			event_print("FreyjaControl::Event> %s, %s:%i",
							"CMD_MESH_FULLCOPY no longer implemented", 
							__FILE__, __LINE__);
			break;
		case CMD_MESH_COPY:
			_minor_mode = MESH_COPY_MODE;      
			event_print("FreyjaControl::Event> %s, %s:%i",
							"CMD_MESH_COPY no longer implemented", 
							__FILE__, __LINE__);
			break;
		case CMD_MESH_SPLIT:
			_minor_mode = MESH_CUT_MODE;      
			event_print("FreyjaControl::Event> %s, %s:%i",
							"CMD_MESH_SPLIT no longer implemented", 
							__FILE__, __LINE__);
			break;
		case CMD_MESH_MERGE:
			_minor_mode = MESH_PASTE_MODE;      
			event_print("FreyjaControl::Event> %s, %s:%i",
							"CMD_MESH_MERGE no longer implemented", 
							__FILE__, __LINE__);
			break;
		}
		break;

	case EVENT_POLYGON:
		switch (cmd)
		{
		case CMD_POLYGON_DELETE:
			_minor_mode = POLYGON_DEL_MODE;
			event_print("Select polygon by vertices to delete");
			break;
		case CMD_POLYGON_ADD:
			_minor_mode = POLYGON_ADD_MODE;
			event_print("Select vertices to create a polygon");
			break;
		case CMD_POLYGON_SELECT:
			_minor_mode = POLYGON_SELECT_MODE;
			event_print("Select polygon by vertices to highlight it");
			break;
		}
		break;

	case EVENT_POINT:
		switch (cmd)
		{
		case CMD_POINT_DELETE:
			_minor_mode = POINT_DEL_MODE;
			event_print("Select vertex to delete");
			break;
		case CMD_POINT_MOVE:
			_minor_mode = VERTEX_MOVE;
			event_print("Select vertex to move");
			break;
		case CMD_POINT_ADD:
			_minor_mode = POINT_ADD_MODE;
			event_print("Select point in space to create a vertex");
			break;    
		case CMD_POINT_COMBINE:
			if (_minor_mode != VERTEX_COMBINE)
			{
				_minor_mode = VERTEX_COMBINE;
			}
			else
			{	
				_minor_mode = NO_MODE;
			}

			event_print("Vertex combine [%s]", 
							(_minor_mode == VERTEX_COMBINE) ? "on" : "off");
			break;
		}
		break;

	case EVENT_MISC:
		switch (cmd)
		{    
		case CMD_MISC_RENDER_BBOX:
			_render->Flags(FreyjaRender::RENDER_BBOX, 
								!(_render->Flags() & FreyjaRender::RENDER_BBOX));
			event_print("BoundingBox rendering [%s]", 
							(_render->Flags() & FreyjaRender::RENDER_BBOX) ? 
							"ON" : "OFF");
			break;
		case CMD_MISC_RENDER_GRID:
			_render->Flags(FreyjaRender::RENDER_EDIT_GRID, 
								!(_render->Flags() & FreyjaRender::RENDER_EDIT_GRID));
			event_print("Edit Grid rendering [%s]",
						(_render->Flags() & FreyjaRender::RENDER_EDIT_GRID) ? 
							"ON" : "OFF");
			event_refresh();
			break;
		case CMD_MISC_RENDER_ROT_Z_M:
			_render->Rotate(Z_F, -_render->RotateAmount());
			break;
		case CMD_MISC_RENDER_ROT_Z_P:
			_render->Rotate(Z_F, _render->RotateAmount());
			break;
		case CMD_MISC_RENDER_ROT_Y_M:
			_render->Rotate(Y_F, -_render->RotateAmount());
			break;
		case CMD_MISC_RENDER_ROT_Y_P:
			_render->Rotate(Y_F, _render->RotateAmount());
			break;
		case CMD_MISC_RENDER_ROT_X_M:
			_render->Rotate(X_F, -_render->RotateAmount());
			break;
		case CMD_MISC_RENDER_ROT_X_P:
			_render->Rotate(X_F, _render->RotateAmount());
			break;
		case CMD_MISC_CAMMODE:
			_render->Flags(FreyjaRender::RENDER_CAMERA, 
								!(_render->Flags() & FreyjaRender::RENDER_CAMERA));
			break;  
		case CMD_MISC_SCREENSHOT:
			_render->ScreenShot();
			break;
		case CMD_MISC_TEX_SLOT_LOAD:
			event_set_load_texture_to_slot(!query_load_texture_to_slot());
			event_print("Texture loading to current slot [%s]",
							query_load_texture_to_slot() ? "on" : "off");
			break;
		case CMD_MISC_SCENE_ROTATE:
			_minor_mode = MODEL_ROTATE;
			event_print("Scene rotate mode");
			break;
		case CMD_MISC_SELECT:
			switch (last_event)
			{
			case EVENT_BONE:
				event_print("Select tag by center point");
				_minor_mode = BONE_SELECT_MODE;
				break;
			case EVENT_MESH:
				event_print("Select mesh by center point");
				_minor_mode = MESH_SELECT_MODE;
				break;
			}
			break;
		case CMD_MISC_BBOX_SELECT:
			if (_minor_mode == VERTEX_BBOX_SELECT_MODE)
			{
				_minor_mode = NO_MODE;
				_render->Flags(FreyjaRender::RENDER_BBOX_SEL, 0);
				_model->BBoxListBuild();
				event_print("Vertex box select: Vertex list built");
			}
			else
			{
				_minor_mode = VERTEX_BBOX_SELECT_MODE;
				_render->Flags(FreyjaRender::RENDER_BBOX_SEL, 1);
				event_print("Vertex box select: Press agian to end selection");
			}
			break;
		case CMD_MISC_TEXEL_COMBINE:
			if (_minor_mode != TEXEL_COMBINE)
			{
				_minor_mode = TEXEL_COMBINE;
			}
			else
			{	
				_minor_mode = NO_MODE;
			}

			event_print("Texel combine [%s]", 
							(_minor_mode == TEXEL_COMBINE) ? "on" : "off");
			break;
		}   
		break;

	}

	last_event = mode;
	last_cmd = cmd;
}


void FreyjaControl::ReadRC()
{
	int i;
	float f;
	char *s;
	char *rc[3];
	char *rc0 = "freyja.mlisp";
	char *rc1 = "freyja.el";
	char *rc2 = "freyja.lisp";
	bool failed = true;


	rc[0] = rc0;
	rc[1] = rc1;
	rc[2] = rc2;

	for (i = 0; i < 3 && failed; ++i)
	{
		s = freyja_rc_map(rc[i]);

		if (!s)
		{
			continue;
		}

		if (_resource.Load(s))
		{
			continue;
		}
		else
		{
			failed = false;
		}

		delete [] s;
	}

	if (failed)
	{
		printf("FreyjaControl::ReadRC> ERROR: Couldn't find '%s'\n", s);
		event_shutdown();
	}

	if (_resource.Lookup("DUMP_SYMTAB", &i) && i)
	{
		_resource.Print();
	}

	if (_resource.Lookup("GRID_ON", &i) && i)
	{
		_render->Flags(FreyjaRender::RENDER_EDIT_GRID, 
							!(_render->Flags() & FreyjaRender::RENDER_EDIT_GRID));
	}

	if (_resource.Lookup("ROTATE_AMOUNT", &f))
	{
		_render->RotateAmount(f);
	}

	if (_resource.Lookup("FLUSH_RESOURCE", &i) && i)
	{
		_resource.Flush();
	}
}


void FreyjaControl::AddKeyCommand(int key, int event, int command)
{
	FreyjaEvent *fevent;

	
	fevent = new FreyjaEvent;
	fevent->AddEvent(event, command);

	//printf("*** %i, %i:%i", key, event, command);

	_key.Add(key, fevent);
}


bool FreyjaControl::Key(unsigned int key, int x, int y, int modifer_keys)
{
	FreyjaEvent *fevent;


	if (!(modifer_keys & KEY_RCTRL))
	{
		return false;
	}

#ifdef DEBUG_KEY
	static unsigned int last_key = 0;
	static unsigned int count = 0;
	
	if (last_key != key)
	{
		if (count)
		{
			printf("FreyjaControl::Key> %i:%i repeated %i times\n", 
					 key, modifer_keys, count);
		}
		
		printf("FreyjaControl::Key> recieved %i:%i\n", key, modifer_keys);
		count = 0;
	}
	else
	{
		count++;
	}
	
	last_key = key;
#endif
	
	fevent = _key[key];
	
	if (fevent)
	{
#ifdef DEBUG_KEY
		printf("FreyjaControl::Key> Found event for key %i\n", key);
#endif
		fevent->Emit();
		return true;
	}
#ifdef DEBUG_KEY
	else
	{
		printf("FreyjaControl::Key> Found event for key %i\n", key);
	}
#endif

	return false;
}


void FreyjaControl::ScreenToWorld(float *x, float *y)
{
	float width, height, invz, fs;
	float scroll[3];
 

	width = _render->Width();
	height = _render->Height();
	_model->getSceneTranslation(scroll);

#ifdef DEBUG_SCREEN_TO_WORLD
	printf("Screen (%.3f, %.3f); aspect_ratio = %.3f; zoom = %.3f\n", 
			 *x, *y, width/height, _model->getZoom());
	printf("Scroll (%.3f, %.3f, %.3f)\n", 
			 scroll[0], scroll[1], scroll[2]);
#endif

	invz = (1.0 / _model->getZoom());
	fs = (40.0 * invz) / height;  // fov 40?

	*x = (*x - width / 2.0) * fs;
	*y = -(*y - height / 2.0) * fs;
	
	switch (_model->CurrentPlane())
	{
	case Egg::PLANE_XY:
		*x -= scroll[0] * invz;
		*y -= scroll[1] * invz;

#ifdef DEBUG_SCREEN_TO_WORLD
		printf("World (%.3f, %.3f, 0.000)\n", *x, *y);
#endif
		break;
	case Egg::PLANE_XZ:
		*x -= scroll[0] * invz;
		*y -= scroll[2] * invz;

#ifdef DEBUG_SCREEN_TO_WORLD
		printf("World (%.3f, 0.000, %.3f)\n", *x, *y);
#endif
		break;
	case Egg::PLANE_YZ:
		*x -= scroll[1] * invz;
		*y -= scroll[2] * invz;

#ifdef DEBUG_SCREEN_TO_WORLD
		printf("World (0.000, %.3f, %.3f)\n", *x, *y);
#endif
		break;
	}
}

int BTN_HACK;
bool FreyjaControl::Motion(int x, int y)
{
	static int old_y = 0, old_x = 0;

	
	switch (_major_mode)
	{
	case MODEL_VIEW_MODE:
		if (BTN_HACK & MOUSE_BTN_RIGHT)
		{
			if (x > old_x)
				freyja_event2i(EVENT_MISC, CMD_MISC_RENDER_ROT_Y_M);
			else if (x < old_x)
				freyja_event2i(EVENT_MISC, CMD_MISC_RENDER_ROT_Y_P);
			
			if (y > old_y)
				freyja_event2i(EVENT_MISC, CMD_MISC_RENDER_ROT_X_M);
			else if (y < old_y)
				freyja_event2i(EVENT_MISC, CMD_MISC_RENDER_ROT_X_P);

			old_x = x;
			old_y = y;
		}
		else if (0) // don't edit mesh in animation view 
		{
			MotionEdit(x, y, _model->CurrentPlane());
		}
    break;
  case TEXTURE_EDIT_MODE:
	  if (_tex_state)
	  {
		  float s;
		  float t;

		  s = (float)x / (float)_render->Width();
		  t = (float)y / (float)_render->Height();
		  
		  if (s > 1.0) s = 1.0;
		  if (s < 0.0) s = 0.0;
		  if (t > 1.0) t = 1.0;
		  if (t < 0.0) t = 0.0;
		  
		  _model->TexelMove(s, t);
		  return true;
	  }
    break;
  case MODEL_EDIT_MODE:
    MotionEdit(x, y, _model->CurrentPlane());
    break;
  default:
    ;
  }

  return true;
}


bool FreyjaControl::Mouse(int btn, int state, int mod,  int x, int y)
{
	freyja_control_mode_t mode = _minor_mode;

	BTN_HACK = btn;
	// Mongoose 2002.01.12, Allow temp mode override
	if (mod & KEY_RCTRL)
	{
		Event(EVENT_MISC, CMD_MISC_SELECT);
	}

	switch (_major_mode)
	{
	case MODEL_VIEW_MODE:
		if (btn == MOUSE_BTN_4 && state == MOUSE_BTN_STATE_PRESSED)
		{
			freyja_event2i(EVENT_MISC, CMD_MISC_ZOOM_IN);
			event_print("Zoom in");
		}
		
		if (btn == MOUSE_BTN_5 && state == MOUSE_BTN_STATE_PRESSED)
		{
			freyja_event2i(EVENT_MISC, CMD_MISC_ZOOM_OUT);
			event_print("Zoom out");
		}
		break;
	case TEXTURE_EDIT_MODE:
		if (_tex_state)
		{
			_tex_state = false;
			return true;
		}
		else if (btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED)
		{
			float s, t;
			
			
			s = (float)x / (float)_render->Width();
			t = (float)y / (float)_render->Height();
			
			// Mongoose: Clamp texels to be bound by min and max
			if (s > 1.0) s = 1.0;
			if (s < 0.0) s = 0.0;
			if (t > 1.0) t = 1.0;
			if (t < 0.0) t = 0.0;
			
			if (_minor_mode == TEXEL_COMBINE)
			{
				_model->TexelCombine(s, t);
			}
			else
			{
				_model->TexelSelect(s, t);
			}
			
			_tex_state = true;
			return true;
		}
		break;
	case MODEL_EDIT_MODE:
		MouseEdit(btn, state, mod, x, y, _model->CurrentPlane());
		break;
	default:
		;
	}

	// Mongoose 2002.01.12, Reset mode
	_minor_mode = mode;

	return true;
}


//////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////


void FreyjaControl::MotionEdit(int x, int y, Egg::egg_plane plane)
{
  static int old_y = 0, old_x = 0;
  int px, mx, py, my;
  float xx = x, yy = y;
  

  // Mongoose: Convert screen to world system
  ScreenToWorld(&xx, &yy);

  // Mongoose: No click and release
  if (_mouse_state != 1)
  {
    return;
  }

  switch (_minor_mode)
  {
  case MODEL_ROTATE:
    switch (plane)
    {
    case Egg::PLANE_XY:
      px = CMD_TRANSFORM_ROT_UP_Y;
      mx = CMD_TRANSFORM_ROT_DOWN_Y;
      py = CMD_TRANSFORM_ROT_UP_X;
      my = CMD_TRANSFORM_ROT_DOWN_X;
      break;
    case Egg::PLANE_XZ:
      px = CMD_TRANSFORM_ROT_UP_Z;
      mx = CMD_TRANSFORM_ROT_DOWN_Z;
      py = CMD_TRANSFORM_ROT_UP_X;
      my = CMD_TRANSFORM_ROT_DOWN_X;
      break;
    case Egg::PLANE_YZ:
      px = CMD_TRANSFORM_ROT_UP_Z;
      mx = CMD_TRANSFORM_ROT_DOWN_Z;
      py = CMD_TRANSFORM_ROT_UP_Y;
      my = CMD_TRANSFORM_ROT_DOWN_Y;
      break;
	 default:
		 return;
    }
    

    if (x < old_x)
      freyja_event2i(EVENT_TRANSFORM, mx);
    else if (x > old_x)
      freyja_event2i(EVENT_TRANSFORM, px);
    
    if (y < old_y)
      freyja_event2i(EVENT_TRANSFORM, my);
    else if (y > old_y)
      freyja_event2i(EVENT_TRANSFORM, py);

    old_x = x;
    old_y = y;
    break; 
  case MODEL_SCALE:
    switch (plane)
    {
    case Egg::PLANE_XY:
      my = CMD_TRANSFORM_SCALE_UP_Y;
      py = CMD_TRANSFORM_SCALE_DOWN_Y;
      px = CMD_TRANSFORM_SCALE_UP_X;
      mx = CMD_TRANSFORM_SCALE_DOWN_X;
      break;
    case Egg::PLANE_XZ:
      my = CMD_TRANSFORM_SCALE_UP_Z;
      py = CMD_TRANSFORM_SCALE_DOWN_Z;
      px = CMD_TRANSFORM_SCALE_UP_X;
      mx = CMD_TRANSFORM_SCALE_DOWN_X;
      break;
    case Egg::PLANE_YZ:
      my = CMD_TRANSFORM_SCALE_UP_Z;
      py = CMD_TRANSFORM_SCALE_DOWN_Z;
      px = CMD_TRANSFORM_SCALE_UP_Y;
      mx = CMD_TRANSFORM_SCALE_DOWN_Y;
      break;
	 default:
		 return;
    }
    
    if (x < old_x)
      freyja_event2i(EVENT_TRANSFORM, mx);
    else if (x > old_x)
      freyja_event2i(EVENT_TRANSFORM, px);
    
    if (y < old_y)
      freyja_event2i(EVENT_TRANSFORM, my);
    else if (y > old_y)
      freyja_event2i(EVENT_TRANSFORM, py);

    old_x = x;
    old_y = y;
    break;
  case VERTEX_BBOX_SELECT_MODE:
    _model->BBoxMove(xx, yy);
    break;
  case TAG_MOVE:
    _model->TagMove(xx, yy);
    break;
  case TAG_MOVE_CENTER:
    _model->TagMoveCenter(xx, yy);
    break;
  case MESH_MOVE:
    _model->MeshMove(xx, yy);
    break;
  case MESH_MOVE_CENTER: 
    _model->MeshMoveCenter(xx, yy);
    break;
  case VERTEX_MOVE:
    _model->VertexMove(xx, yy);
    break;
  default:
    break;
  }
}


void FreyjaControl::MouseEdit(int btn, int state, int mod, int x, int y, 
										Egg::egg_plane plane) 
{ 
	float xx = x, yy = y;
	unsigned int master_tag;
	
	
	ScreenToWorld(&xx, &yy);
	
	if (!(btn == MOUSE_BTN_LEFT && state == MOUSE_BTN_STATE_PRESSED))
	{
		return;
	}

	switch(_minor_mode)
	{
	case VERTEX_COMBINE:
		if (_mouse_state == 0)
		{
			_model->VertexCombine(xx, yy);
			 _mouse_state = 1;
		}
		else
		{
			_mouse_state = 0;
		}
		break;
	case VERTEX_MOVE:
	case MESH_MOVE:
		if (_mouse_state == 0)
		{
			_model->VertexSelect(xx, yy);
			_mouse_state = 1;
		}
		else
		{
			_mouse_state = 0;
		}
		break;
	case VERTEX_BBOX_SELECT_MODE:
		if (_mouse_state == 0)
		{
			_model->BBoxSelect(xx, yy);
			_mouse_state = 1;
		}
		else
			_mouse_state = 0;
      break;
	case MESH_SELECT_MODE:
		_model->MeshSelect(xx, yy);
		break;
	case POINT_DEL_MODE:
		_model->VertexSelect(xx, yy);
		_model->VertexDelete();
		break;
	case BONE_SELECT_MODE:
		_model->TagSelect(xx, yy);
		break;
	case BONE_CONNECT_MODE:
		master_tag = _model->getCurrentBone();
		_model->TagSelect(xx, yy);
		_model->TagConnect(master_tag, _model->getCurrentBone());
		_model->setCurrentBone(master_tag);
		break;
	case BONE_DISCONNECT_MODE:
		master_tag = _model->getCurrentBone();
		_model->TagSelect(xx, yy);
		_model->TagDisconnect(master_tag, _model->getCurrentBone());
		_model->setCurrentBone(master_tag);
		break;
	case TAG_MOVE:
		if (_mouse_state == 0)
			_mouse_state = 1;
		else
			_mouse_state = 0;
		break;
	case MESH_MOVE_CENTER:
		if (_mouse_state == 0)
			_mouse_state = 1;
		else
			_mouse_state = 0;
		break;
	case POINT_ADD_MODE: 
		_model->VertexNew(xx, yy);
		break;
	case POLYGON_ADD_MODE:
		_model->PolygonAddVertex(xx, yy);
		break;
	case POLYGON_DEL_MODE:
		_model->PolygonDelVertex(xx, yy);
		break;
	case POLYGON_SELECT_MODE:
		_model->PolygonSelectVertex(xx, yy);
		break;
	case MESH_WHOLE_COPY_MODE:
		break;
	case MESH_PASTE_MODE:
		event_print("Mesh paste not impelmented\n");
		break;
	case MESH_COPY_MODE:
	case MESH_CUT_MODE:
		_model->VertexBuffer(xx, yy);
	default:
		;
	}
}

