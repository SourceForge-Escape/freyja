/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000, 2001 Mongoose
 * Comments: This is the main client entry.
 *
 *-- TODO ---------------------------------------------------
 *
 * Merge this into unified Freyja class then
 * move gtk dependent code out into FreyjaGnome
 *
 *-- History ------------------------------------------------- 
 *
 * 2004.04.11:
 * Mongoose - Removed GNOME support, lots of little clean ups
 *
 * 2000.09.16:
 * Mongoose - Created
 ==========================================================================*/

#include <stdio.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <mstl/Map.h>

#include "freyja_events.h"
#include "FreyjaModel.h"
#include "FreyjaControl.h"
#include "MaterialManager.h"

#include "gnome_callbacks.h"
#include "gnome_interface.h"
#include "gnome_resource.h"


typedef struct gtk_anim_tree_s
{
	Map<int, struct gtk_anim_tree_s *> sibling;
	Map<int, struct gtk_anim_tree_s *> child;

	char *label;

} gtk_anim_tree_t;

void spinbutton_value_set(int event, float val);
float spinbutton_value_get_float(int event, bool *error);

// Mongoose 2002.02.01, Gobals used from other files, try to 
//   reduce these as much as you can with gtk+ use
extern FreyjaModel *gFreyjaModel;
extern FreyjaControl *gFreyjaControl;

// Mongoose 2002.02.01, Gobals only accessed in this file
Map<int, GtkWidget *> WIDGET_MAP;
Map<int, int> NOTEBOOK_EVENT;

//////////////////////////////////////////////////////////////////////////

Map<int, Vector<GtkWidget *> *> gObserverGtkWigets;


void freyja_event_fullscreen()
{
	application_window_fullscreen();
}


void freyja_event_unfullscreen()
{
	application_window_unfullscreen();
}


void freyja_event_subscribe_gtk_widget(int index, GtkWidget *widget)
{
	Vector<GtkWidget *> *widgets = gObserverGtkWigets[index];


	if (!widgets)
	{
		widgets = new Vector<GtkWidget *>;
		gObserverGtkWigets.Add(index, widgets);
	}

	widgets->pushBack(widget);

	event_print("(freyja_event_subscribe_gtk_widget %d %p)", 
				//gObserverGtkWigets.NumItems(), index, widget, widgets->size()
				index, widget);
}


void freyja_event_notify_observer1f(event_subject_id id, float r)
{
	Vector<GtkWidget *> *widgets;
	GtkWidget *widget;
	unsigned int i;

	widgets = gObserverGtkWigets[id];

	if (!widgets)
	{
		event_print("freyja_event_notify_observer1f> ERROR, unknown id %d\n",
					id);
		return;
	}

	event_print("(freyja_event_notify_observer1f %d %f)", id, r);

	for (i = widgets->begin(); i < widgets->end(); ++i)
	{
		widget = (*widgets)[i];

		if (!widget)
			continue;

		if (GTK_IS_SPIN_BUTTON(widget))
		{
			GtkAdjustment *adj = gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(widget));
			
			// gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), r);
			
			if (adj)
			{
				gtk_adjustment_set_value(adj, r);
				event_print("(freyja_event_notify_gtk_widget %d %p)", 
							id, widget);
			}
		}
		else
		{
			event_print("freyja_event_notify_observer1f> ERROR, unknown widget\n");
			//	return;
		}
	}
}


void freyja_event_file_dialog_pattern(char *pattern)
{
	GtkWidget *file = getGtkFileSelectionWidget();

	gtk_file_selection_complete(GTK_FILE_SELECTION(file), pattern);
}

void freyja_event_file_dialog(char *title)
{
	GtkWidget *file = getGtkFileSelectionWidget();
	
	gtk_window_set_title(GTK_WINDOW(file), title);
	gtk_widget_show(file);
}

/* Shutdown / exit handlers */
void mgtk_guard_shutdown()
{
	event_print("@Gtk+ shuting down...");
	gtk_exit(0);
}


void mgtk_destroy_window(GtkWidget *widget)
{
	mgtk_guard_shutdown();
}


void freyja_event_exit()
{
	mgtk_guard_shutdown();
}


void freyja_event_info_dialog(char *message)
{
	mgtk_create_info_dialog(message);
}


float freyja_event_get_float(int event)
{
	float value;
	bool error;

	value = spinbutton_value_get_float(event, &error);
	return value;
}


void freyja_event_set_float(int event, float value)
{
	spinbutton_value_set(event, value);
}



//////////////////////////////////////////////////////////////////////////


void event_swap_buffers()
{
	/* Hhhmm...  nothing really needed here, swapping is handled in 
	 * the gtk glarea interfacing code after draw requests */
	usleep(500); // Hhmm...
}


void event_refresh()
{
	if (getGtkGLAreaWidget())
	{
		mgtk_refresh_glarea(getGtkGLAreaWidget());
	}
}

// FIXME: factor out key mapping from GDK and mgtk!

void mgtk_event_button_press(GtkWidget *widget, GdkEventButton *event)
{
	GdkModifierType state;
	glarea_window_state_t *gl_state;
	int x, y, mod, btn_state, button;


	gl_state = (glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(widget),
																			 "gl_window_state");
	
	/* Beginning of drag, reset mouse position */
	gdk_window_get_pointer(event->window, &x, &y, &state);
  
	gl_state->mouse_x = x;
	gl_state->mouse_y = y;

	btn_state = MOUSE_BTN_STATE_PRESSED;
	button = 0;
	mod = 0;

	if (event->state & GDK_CONTROL_MASK)
		mod |= KEY_RCTRL;

	if (state & GDK_BUTTON1_MASK)
	{
		button |= MOUSE_BTN_LEFT;
	}

	if (state & GDK_BUTTON2_MASK)
	{
		button |= MOUSE_BTN_MIDDLE;
	}

	if (state & GDK_BUTTON3_MASK)
	{
		button |= MOUSE_BTN_RIGHT;
	}

	if (state & GDK_BUTTON4_MASK)
	{
		button |= MOUSE_BTN_UP;
	}

	if (state & GDK_BUTTON5_MASK)
	{
		button |= MOUSE_BTN_DOWN;
	}

#ifdef DEBUG_GTK_MOUSE
	char foo[32];
	sprintf(foo, "MOUSE_BTN_%i", button);
	event_print("Sending mouse event to Freya; x: %i y: %i, %s, %s\n", 
					x, y, 
					(button == MOUSE_BTN_LEFT) ? "MOUSE_BTN_LEFT" :
					(button == MOUSE_BTN_RIGHT) ? "MOUSE_BTN_RIGHT" :
					(button == MOUSE_BTN_MIDDLE) ? "MOUSE_BTN_MIDDLE" :
					(button == MOUSE_BTN_4) ? "MOUSE_BTN_4" :
					(button == MOUSE_BTN_5) ? "MOUSE_BTN_5" :
					foo, 
					(btn_state==MOUSE_BTN_STATE_PRESSED)?"MOUSE_BTN_STATE_PRESSED":
					"MOUSE_BTN_STATE_NONE");
#endif

	event_mouse(button, btn_state, mod, x, y);
	mgtk_refresh_glarea(widget); // Seems a little much
}


void mgtk_event_button_release(GtkWidget *widget, GdkEventButton *event)
{
	//mgtk_callback_mouse_button_release(); /* assumes no chording */
	event_mouse(0, 0, 0, 0, 0);
}


void mgtk_event_key_press(GtkWidget *widget, GdkEventKey *event)
{
	unsigned int mod = 0;
	int key = -1;


	//gtk_widget_add_accelerator(item, "activate_item", accel, 
	//                           tmp_key, GDK_MOD1_MASK, (GtkAccelFlags)0);

	if (event->state & GDK_CONTROL_MASK)
	{
		mod |= KEY_RCTRL;
	}

	if (event->state & GDK_SHIFT_MASK)
	{
		mod |= KEY_RSHIFT;
	}

	switch (event->keyval)
	{
	case GDK_Left:
		key = KEY_LEFT;
		break;
	case GDK_Right:
		key = KEY_RIGHT;
		break;
	case GDK_Up:
		key = KEY_UP;
		break;
	case GDK_space:
		key = KEY_SPACE;
		break;
	case GDK_Down:
		key = KEY_DOWN;
		break;
	case GDK_F1:
		key = KEY_F1;
		break;		
	case GDK_F2:
		key = KEY_F2;
		break;		
	case GDK_F3:
		key = KEY_F3;
		break;		
	case GDK_F4:
		key = KEY_F4;
		break;
	case GDK_F5:
		key = KEY_F5;
		break;		
	case GDK_F6:
		key = KEY_F6;
		break;		
	case GDK_F7:
		key = KEY_F7;
		break;
	case GDK_F8:
		key = KEY_F8;
		break;		
	case GDK_F9:
		key = KEY_F9;
		break;
	case GDK_F10:
		key = KEY_F10;
		break;		
	case GDK_F11:
		key = KEY_F11;
		break;		
	case GDK_F12:
		key = KEY_F12;
		break;
	default:
		if ((unsigned char)event->keyval < 128)
		{
			key = (unsigned char)event->keyval;
		}
	}

	/* Prevent the default handler from being run */
	//gtk_signal_emit_stop_by_name(GTK_OBJECT(widget), "key_press_event");

	if (key < 0)
	{
		return;
	}

#ifdef DEBUG_GTK_KEYS
	event_print("*** key_press_event> %i (%c) : %i\n", key, key, mod);
#endif

	freyja_event_key_press(key, mod);
}


void mgtk_event_key_release(GtkWidget *widget, GdkEventKey *event)
{
#ifdef DEBUG_GTK_KEYS
	event_print("*** key_release_event> %i : %i\n", key, mod);
#endif

	//freyja_event_key_release(key, mod);
}


void mgtk_event_mouse_motion(GtkWidget *widget, GdkEventMotion *event)
{
	int x, y;
	GdkModifierType state;
	glarea_window_state_t *gl_state;


	gl_state = (glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(widget),
																			 "gl_window_state");

	if (event->is_hint)
	{
		gdk_window_get_pointer(event->window, &x, &y, &state);
	} 
	else 
	{
		x = (int)event->x;
		y = (int)event->y;
		state = (GdkModifierType)event->state;
	}

	gl_state->mouse_x = x;
	gl_state->mouse_y = y;
	event_motion(x, y);
	mgtk_refresh_glarea(widget);
}


void callback_update_skeleton_ui_gtk(callback_bone_t *bone, 
									 GtkTreeStore *store, GtkTreeIter root)
{
	GtkTreeIter child;
	unsigned int i;


	if (!bone)
		return;

#ifdef DEBUG_BONE_LOAD
	printf("C_BONE %d ::\n", bone->id);
#endif

	if (bone->id == 0) // Root bone
	{
		store = gtk_tree_store_new(N_COLUMNS,       /* Total number of cols */
								   G_TYPE_STRING,   /* Bone Name */
    	                           G_TYPE_INT);     /* Bone Id */

		gtk_tree_store_append(store, &root, NULL);
		gtk_tree_store_set(store, &root,
								 NAME_COLUMN, bone->name,
								 ID_COLUMN, bone->id,
								 -1);

		for (i = 0; i < bone->numChildren; ++i)
		{
			callback_update_skeleton_ui_gtk(&bone->children[i], store, root);
		}

		resource_rebuild_skeleton_treeview(GTK_TREE_MODEL(store));
	}
	else if (bone->id > 256)
	{
		printf("callback_update_skeleton_ui_gtk> ERROR: Invalid bone\n");
	}
	else if (!store)
	{
		printf("callback_update_skeleton_ui_gtk> ERROR: Invalid skeleton\n");
	}
	else
	{
		gtk_tree_store_append(store, &child, &root);
		gtk_tree_store_set(store, &child,
								 NAME_COLUMN, bone->name,
								 ID_COLUMN, bone->id,
								 -1);

		for (i = 0; i < bone->numChildren; ++i)
		{
			callback_update_skeleton_ui_gtk(&bone->children[i], store, child);
		}
	}
}


void callback_update_skeleton(callback_bone_t *bone)
{
	GtkTreeIter root;

	callback_update_skeleton_ui_gtk(bone, NULL, root);
}


void index_add_notebook_event(int page, int event)
{
	NOTEBOOK_EVENT.Add(page, event);
}


void index_add_gtk_widget(int event, GtkWidget *widget)
{
	WIDGET_MAP.Add(event, widget);
}


void map_print_int(int i)
{
	printf("%i", i);
}


void map_print_gtk(GtkWidget *widget)
{
	if (widget)
	{
		if (GTK_IS_SPIN_BUTTON(widget))
			printf("spinbutton");
		else if (GTK_IS_BUTTON(widget))
			printf("button");
		else
			printf("widget?");
	}
}


void option_menu_value_set(int event, int index)
{
	GtkWidget *test;

	test = WIDGET_MAP[event];

	if (test && GTK_IS_OPTION_MENU(test))
	{
		gtk_option_menu_set_history(GTK_OPTION_MENU(test), index);
	}
	else
	{
		event_print("ERROR: gtk+ widget value setting failed for event %i",
						event);
		WIDGET_MAP.Print(map_print_int, map_print_gtk);
	}	
}


void textentry_value_set(int event, const char *s)
{
	GtkWidget *test;

	test = WIDGET_MAP[event];

	if (test)// && GTK_IS_ENTRY(test))
	{
		gtk_entry_set_text(GTK_ENTRY(test), s);
	}
	else
	{
		event_print("ERROR: gtk+ widget value setting failed for event %i",
						event);
		WIDGET_MAP.Print(map_print_int, map_print_gtk);
	}	
}


void togglebutton_value_set(int event, bool val)
{
	GtkWidget *test;

	test = WIDGET_MAP[event];

	if (test && GTK_IS_TOGGLE_BUTTON(test))
	{
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(test)) != val ? TRUE : FALSE)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(test),  
												  val ? TRUE : FALSE);
		}
	}
	else
	{
		event_print("ERROR: gtk+ widget value setting failed for event %i",
						event);
		WIDGET_MAP.Print(map_print_int, map_print_gtk);
	}
}


// Mongoose 2002.01.31, Hack for adjusting widget values by event id
void spinbutton_value_set(int event, float val)
{
	GtkWidget *test;

	test = WIDGET_MAP[event];

	if (test)// && GTK_IS_SPIN_BUTTON(test))
	{
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(test), val);
	}
	else
	{
		event_print("ERROR: gtk+ widget value setting failed for event %i",
						event);
		WIDGET_MAP.Print(map_print_int, map_print_gtk);
	}
}


// Mongoose 2002.01.31, Hack for adjusting widget values by event id
float spinbutton_value_get_float(int event, bool *error)
{
	GtkWidget *test;


	test = WIDGET_MAP[event];
	*error = true;

	if (test)// && GTK_IS_SPIN_BUTTON(test))
	{
		*error = false;
		return gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(test));
	}
	else
	{
		event_print("ERROR: gtk+ widget value setting failed for event %i",
						event);
		WIDGET_MAP.Print(map_print_int, map_print_gtk);
	}

	return 0.0;
}


int spinbutton_value_get_int(int event, bool *error)
{
	GtkWidget *test;


	test = WIDGET_MAP[event];
	*error = true;

	if (test)// && GTK_IS_SPIN_BUTTON(test))
	{
		*error = false;
		return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(test));
	}
	else
	{
		event_print("ERROR: gtk+ widget value setting failed for event %i",
						event);
		WIDGET_MAP.Print(map_print_int, map_print_gtk);
	}

	return 0;
}


void rc_textbox_event(GtkWidget *widget, gpointer user_data)
{
	char *text;


	if (!widget)
	{
		return;
	}

	text = (char *)gtk_entry_get_text(GTK_ENTRY(widget));

	switch (GPOINTER_TO_INT(user_data))
	{
	case 799:
		gMaterialManager->setName(text);
		break;
	default:
		;
	}
}


gint popup_menu_handler_event(GtkWidget *widget, GdkEventButton *event)
{
	if (event->button == 3) 
	{
		gtk_menu_popup(GTK_MENU(widget), NULL, NULL, NULL, NULL,
							event->button, event->time);
		return TRUE;
	}
	
	return FALSE;
}


void refresh_material_interface()
{
	vec4_t ambient, diffuse, specular, emissive;
	unsigned int src, dest, flags;
	vec_t shininess;


	gMaterialManager->getColor(MaterialManager::eAmbient, ambient);
	gMaterialManager->getColor(MaterialManager::eDiffuse, diffuse);
	gMaterialManager->getColor(MaterialManager::eSpecular, specular);
	gMaterialManager->getColor(MaterialManager::eEmissive, emissive);
	shininess = gMaterialManager->getShininess();
	src = gMaterialManager->getBlendSourceIndex();
	dest = gMaterialManager->getBlendDestIndex();
	flags = gMaterialManager->getFlags();

	spinbutton_value_set(700, ambient[0]);	
	spinbutton_value_set(701, ambient[1]);
	spinbutton_value_set(702, ambient[2]);
	spinbutton_value_set(703, ambient[3]);
	spinbutton_value_set(704, diffuse[0]);
	spinbutton_value_set(705, diffuse[1]);
	spinbutton_value_set(706, diffuse[2]);
	spinbutton_value_set(707, diffuse[3]);
	spinbutton_value_set(708, specular[0]);
	spinbutton_value_set(709, specular[1]);
	spinbutton_value_set(710, specular[2]);
	spinbutton_value_set(711, specular[3]);
	spinbutton_value_set(712, emissive[0]);
	spinbutton_value_set(713, emissive[1]);
	spinbutton_value_set(714, emissive[2]);
	spinbutton_value_set(715, emissive[3]);
	spinbutton_value_set(716, shininess);
	option_menu_value_set(780, src);
	option_menu_value_set(781, dest);


	if (flags & Material::fEnable_Blending)
	{
		togglebutton_value_set(790, true);
	}
	else
	{
		togglebutton_value_set(790, false);
	}

	textentry_value_set(799, gMaterialManager->getName());

	event_print("refresh_material_interface> FIXME, Texture widgets not syncable yet, %s:%i", __FILE__, __LINE__);
}


void popup_menu_detacher_event(GtkWidget *attach_widget,GtkMenu *menu)
{
}


void animation_tree_item_selected(GtkWidget *item, gpointer event_id)
{
	event_print("animation_tree_item_selected> got id %i",
				GPOINTER_TO_INT(event_id));
}


gint aframe_adj_event(GtkWidget *widget, GdkEventButton *event)
{
  GtkAdjustment *adj;


  if (widget)
  {
    adj = GTK_ADJUSTMENT(widget);

	if (adj->value != gFreyjaModel->getCurrentAnimationFrame())
	{
		gFreyjaModel->setCurrentAnimationFrame((unsigned int)adj->value);
		adj->value = gFreyjaModel->getCurrentAnimationFrame();
		event_refresh();
	}
  }
  
  return TRUE;
}


void event_send_color(GtkWidget *colorbutton, gpointer id)
{
	GdkColor color;
	guint16 alpha;
	float r, g, b, a;


	gtk_color_button_get_color(GTK_COLOR_BUTTON(colorbutton), &color);
	alpha = gtk_color_button_get_alpha(GTK_COLOR_BUTTON(colorbutton));


	r = color.red / 65535.0;
	g = color.green / 65535.0;
	b = color.blue / 65535.0;
	a = alpha / 65535.0;

	freyja_handle_color(GPOINTER_TO_INT(id), r, g, b, a);
}



gint spinbutton_int_event(GtkSpinButton *spin, gpointer event_id)
{
	//	int old_value;
	int new_value;


	if (spin)
	{
		new_value = gtk_spin_button_get_value_as_int(spin);

		switch (GPOINTER_TO_INT(event_id))
		{
		default:
			event_print("spinbutton_int_event> event %i not handled, email bug", 
							GPOINTER_TO_INT(event_id));
		}
	}
	else
	{
		event_print("spinbutton_int_event> ERROR: Assertion '%s' failed",
						"spin != NULL");
	}

	return TRUE;
}


int spinbutton_uint_set_range(GtkSpinButton *spin, 
							  unsigned int value,
							  unsigned int min, unsigned int max)
{
	gtk_spin_button_set_range(spin, -1, 100);

	if (value > max)
	{
		gtk_spin_button_set_range(spin, min, max);
		return 1;
	}	

	return 0;
}


gint spinbutton_uint_event(GtkSpinButton *spin, gpointer event_id)
{
	unsigned int event;
	unsigned int value;
	float x, y, z;


	if (!spin)
	{
		event_print("ERROR: Assertion '%s' failed %s:%d",
					"spin != NULL", __FILE__, __LINE__);
		return TRUE;
	}

	value = gtk_spin_button_get_value_as_int(spin);
	event = GPOINTER_TO_INT(event_id);

	switch (event)
	{
	case 500:
		if (!spinbutton_uint_set_range(spin, value, 0, eggGetNum(FREYJA_MESH)))
		{
			gFreyjaModel->setCurrentMesh(value);			
			event_refresh();
		}
		break;
	case 501:
		if (!spinbutton_uint_set_range(spin, value, 0, eggGetNum(FREYJA_GROUP)))
		{
			gFreyjaModel->setCurrentGroup(value);
			event_refresh();
		}
		break;
	case 503:
		if (value != gFreyjaModel->getCurrentTextureIndex())
		{
			gFreyjaModel->setCurrentTextureIndex(value);
			//value = gFreyjaModel->getCurrentTextureIndex();
			//spinbutton_value_set(event, value);
			event_refresh();
		}
		break;
	case 504:
		if (!spinbutton_uint_set_range(spin, 
									  value, 0, eggGetNum(FREYJA_BONE)))
		{
			gFreyjaModel->setCurrentBone(value);
			//value = gFreyjaModel->getCurrentBone();
			//spinbutton_value_set(event, value);

			/* Mongoose 2002.08.31, Update spin buttons dependent 
			 * on this one */
			gFreyjaModel->getBoneRotation(&x, &y, &z);
			spinbutton_value_set(520, x);
			spinbutton_value_set(521, y);
			spinbutton_value_set(522, z);
			
			gFreyjaModel->getBoneTranslation(&x, &y, &z);
			spinbutton_value_set(510, x);
			spinbutton_value_set(511, y);
			spinbutton_value_set(512, z);
			event_refresh();
		}
		break;
	case 717:
	case 718:
		event -= 717;

		gMaterialManager->setTexture(event, value);
		event_print("Material[%i].texture[%d] = %i",
					gMaterialManager->getCurrent(), 
					event,
					gMaterialManager->getTexture(event));
		event_refresh();
		break;
	default:
		event_print("spinbutton_uint_event> event %i not handled, email bug", 
					GPOINTER_TO_INT(event_id));
	}

	return TRUE;
}



void freyja_handle_color(int id, float r, float g, float b, float a)
{
	vec4_t color;

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	switch (id)
	{
	case 9000:
		gMaterialManager->setColor(MaterialManager::eAmbient, color);
		event_refresh();
		break;
	case 9001:
		gMaterialManager->setColor(MaterialManager::eDiffuse, color);
		event_refresh();
		break;
	case 9002:
		gMaterialManager->setColor(MaterialManager::eSpecular, color);
		event_refresh();
		break;
	case 9003:
		gMaterialManager->setColor(MaterialManager::eEmissive, color);
		event_refresh();
		break;
	}
}


gint spinbutton_float_event(GtkSpinButton *spin, gpointer event_id)
{
	static float old_value[3];
	static float old_value2[3];
	float new_value;
	float *test;
	float x, y, z;
	vec4_t color;


	if (spin)
	{
		new_value = gtk_spin_button_get_value_as_float(spin);
	}
	else
	{
		event_print("spinbutton_float_event> ERROR: Assertion '%s' failed",
						"spin != NULL");

		return TRUE;
	}

	switch (GPOINTER_TO_INT(event_id))
	{
	case 510:
	case 511:
	case 512:
		new_value = gtk_spin_button_get_value_as_float(spin);
		
		gFreyjaModel->getBoneTranslation(&x, &y, &z);
		
		switch (GPOINTER_TO_INT(event_id)-510)
		{
		case 0:
			gFreyjaModel->setBoneTranslation(new_value, y, z);
			break;
		case 1:
			gFreyjaModel->setBoneTranslation(x, new_value, z);
			break;
		case 2:
			gFreyjaModel->setBoneTranslation(x, y, new_value);
			break;
		}
		
		old_value2[GPOINTER_TO_INT(event_id)-510] = new_value;
		event_refresh();
		break;
	case 520:
	case 521:
	case 522:
		new_value = gtk_spin_button_get_value_as_float(spin);

		gFreyjaModel->getBoneRotation(&x, &y, &z);

		switch (GPOINTER_TO_INT(event_id)-520)
		{
		case 0:
			gFreyjaModel->setBoneRotation(new_value, y, z);
			break;
		case 1:
			gFreyjaModel->setBoneRotation(x, new_value, z);
			break;
		case 2:
			gFreyjaModel->setBoneRotation(x, y, new_value);
			break;
		}

		old_value[GPOINTER_TO_INT(event_id)-520] = new_value;
		event_refresh();
		break;
	case 700:
	case 701:
	case 702:
	case 703:
		gMaterialManager->getColor(MaterialManager::eAmbient, 
														  color);
		color[GPOINTER_TO_INT(event_id)-700] = new_value;
		gMaterialManager->setColor(MaterialManager::eAmbient,
														  color);
		event_refresh();
		break;
	case 704:
	case 705:
	case 706:
	case 707:
		gMaterialManager->getColor(MaterialManager::eDiffuse,
														  color);
		color[GPOINTER_TO_INT(event_id)-704] = new_value;
		gMaterialManager->setColor(MaterialManager::eDiffuse,
														  color);
		event_refresh();
		break;
	case 708:
	case 709:
	case 710:
	case 711:
		gMaterialManager->getColor(MaterialManager::eSpecular,
														  color);
		color[GPOINTER_TO_INT(event_id)-708] = new_value;
		gMaterialManager->setColor(MaterialManager::eSpecular,
														  color);
		event_refresh();
		break;
	case 712:
	case 713:
	case 714:
	case 715:
		gMaterialManager->getColor(MaterialManager::eEmissive,
														  color);
		color[GPOINTER_TO_INT(event_id)-712] = new_value;
		gMaterialManager->setColor(MaterialManager::eEmissive,
														  color);
		event_refresh();
		break;
	case 716:
		gMaterialManager->setShininess(new_value);
		event_refresh();
		break;
	case 800:
	case 801:
	case 802:
		test = gFreyjaModel->GetLight0Pos();
		test[GPOINTER_TO_INT(event_id)-800] = new_value;
		event_refresh();
		break;
	case eZOOM:
		gFreyjaControl->setZoom(new_value);
		event_refresh();
		break;
	default:
		event_print("spinbutton_float_event> event %i not handled, email bug", 
						GPOINTER_TO_INT(event_id));
	}

	return TRUE;
}


// Mongoose 2002.02.23, OBSOLETE this with new scripting ability 
//   to generate event pairs from tabs
// Mongoose 2002.01.21, FIXME to handle dynamic ordered tabs/plugin tabs
void notebook_switch_page_event(GtkNotebook *notebook, GtkNotebookPage *page,
								gint page_num, gpointer user_data)
{
	// event_print("notebook_switch_page_event> %i", page_num);

	switch (page_num)
	{
	case 0:
		freyja_event2i(EVENT_FREYJA_MODE, FREYJA_MODE_MODEL_EDIT);
		break;
	case 1:
		freyja_event2i(EVENT_FREYJA_MODE, FREYJA_MODE_TEXTURE_EDIT);
		break;
	case 2:
		freyja_event2i(EVENT_FREYJA_MODE, FREYJA_MODE_MODEL_VIEW);
		break;
	case 3:
		freyja_event2i(EVENT_FREYJA_MODE, FREYJA_MODE_MATERIAL_EDIT);
		break;
	default:
		event_print("Unknown mode?  Email mongoose@users.sourceforge.net\n");
		return;
	}
}



////////////////////////////////////////////////////////////////
// Gtk+ widget func wrappers for event pair EVENT
////////////////////////////////////////////////////////////////

void material_event(GtkWidget *widget, gpointer user_data)
{
	unsigned int flags;


	switch (GPOINTER_TO_INT(user_data))
	{
	case 60:
		flags = gMaterialManager->getFlags();
		
		if (flags & Material::fEnable_Texture)
			gMaterialManager->clearFlag(Material::fEnable_Texture);
		else
			gMaterialManager->setFlag(Material::fEnable_Texture);
		
		event_print("OpenGL texturing is [%s]", 
						(flags & Material::fEnable_Texture) ? "ON" : "OFF");
		event_refresh();

	case 70:
		flags = gMaterialManager->getFlags();

		if (flags & Material::fEnable_DetailTexture)
			gMaterialManager->clearFlag(Material::fEnable_DetailTexture);
		else
			gMaterialManager->setFlag(Material::fEnable_DetailTexture);

		event_print("OpenGL detail texturing is [%s]", 
						(flags & Material::fEnable_DetailTexture) ? "ON" : "OFF");
		event_refresh();

	case 80:
		flags = gMaterialManager->getFlags();

		if (flags & Material::fEnable_Normalize)
			gMaterialManager->clearFlag(Material::fEnable_Normalize);
		else
			gMaterialManager->setFlag(Material::fEnable_Normalize);

		event_print("OpenGL normalization of normals is [%s]", 
						(flags & Material::fEnable_Normalize) ? "ON" : "OFF");
		event_refresh();
		break;

	case 90:
		flags = gMaterialManager->getFlags();
		
		if (flags & Material::fEnable_Blending)
			gMaterialManager->clearFlag(Material::fEnable_Blending);
		else
			gMaterialManager->setFlag(Material::fEnable_Blending);
		
		event_print("OpenGL blending [%s]", 
						(flags & Material::fEnable_Blending) ? "ON" : "OFF");
		event_refresh();
		break;

	case 100:
		gMaterialManager->setBlendSource(GL_ZERO);
		break;
	case 101:
		gMaterialManager->setBlendSource(GL_ONE);
		break;
	case 102:
		gMaterialManager->setBlendSource(GL_SRC_COLOR);
		break;
	case 103:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_SRC_COLOR);
		break;
	case 104:
		gMaterialManager->setBlendSource(GL_DST_COLOR);
		break;
	case 105:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_DST_COLOR);
		break;
	case 106:
		gMaterialManager->setBlendSource(GL_SRC_ALPHA);
		break;
	case 107:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_SRC_ALPHA);
		break;
	case 108:
		gMaterialManager->setBlendSource(GL_DST_ALPHA);
		break;
	case 109:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_DST_ALPHA);
		break;
	case 110:
		gMaterialManager->setBlendSource(GL_SRC_ALPHA_SATURATE);
		break;
	case 111:
		gMaterialManager->setBlendSource(GL_CONSTANT_COLOR);
		break;
	case 112:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_CONSTANT_COLOR);
		break;
	case 113:
		gMaterialManager->setBlendSource(GL_CONSTANT_ALPHA);
		break;
	case 114:
		gMaterialManager->setBlendSource(GL_ONE_MINUS_CONSTANT_ALPHA);
		break;


	case 200:
		gMaterialManager->setBlendDest(GL_ZERO);
		break;
	case 201:
		gMaterialManager->setBlendDest(GL_ONE);
		break;
	case 202:
		gMaterialManager->setBlendDest(GL_SRC_COLOR);
		break;
	case 203:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_SRC_COLOR);
		break;
	case 204:
		gMaterialManager->setBlendDest(GL_DST_COLOR);
		break;
	case 205:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_DST_COLOR);
		break;
	case 206:
		gMaterialManager->setBlendDest(GL_SRC_ALPHA);
		break;
	case 207:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_SRC_ALPHA);
		break;
	case 208:
		gMaterialManager->setBlendDest(GL_DST_ALPHA);
		break;
	case 209:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_DST_ALPHA);
		break;
	case 210:
		gMaterialManager->setBlendDest(GL_SRC_ALPHA_SATURATE);
		break;
	case 211:
		gMaterialManager->setBlendDest(GL_CONSTANT_COLOR);
		break;
	case 212:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_CONSTANT_COLOR);
		break;
	case 213:
		gMaterialManager->setBlendDest(GL_CONSTANT_ALPHA);
		break;
	case 214:
		gMaterialManager->setBlendDest(GL_ONE_MINUS_CONSTANT_ALPHA);
		break;

	default:
		freyja_event2i(EVENT_MATERIAL, GPOINTER_TO_INT(user_data));
	}
}


void main_event(GtkWidget *widget, gpointer user_data)
{
	freyja_event2i(EVENT_MAIN, GPOINTER_TO_INT(user_data));
}


void mode_event(GtkWidget *widget, gpointer user_data)
{
	freyja_event2i(EVENT_FREYJA_MODE, GPOINTER_TO_INT(user_data));
}


void misc_event(GtkWidget *widget, gpointer user_data)
{
	freyja_event2i(EVENT_MISC, GPOINTER_TO_INT(user_data));
}


void tag_event(GtkWidget *widget, gpointer user_data)
{
	freyja_event2i(EVENT_BONE, GPOINTER_TO_INT(user_data));
}


void mesh_event(GtkWidget *widget, gpointer user_data)
{
	freyja_event2i(EVENT_MESH, GPOINTER_TO_INT(user_data));
}


void point_event(GtkWidget *widget, gpointer user_data)
{
	freyja_event2i(EVENT_POINT, GPOINTER_TO_INT(user_data));
}


void polygon_event(GtkWidget *widget, gpointer user_data)
{
	freyja_event2i(EVENT_POLYGON, GPOINTER_TO_INT(user_data));
}


void animation_event(GtkWidget *widget, gpointer user_data)
{
	freyja_event2i(EVENT_ANIMATION, GPOINTER_TO_INT(user_data));
	event_refresh();
}




/* Mongoose 2004.05.24, 
 * All this below here goes into gtk_freyja.cpp post split */
void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
#ifdef unix
	snprintf(dest, size, "%s/.freyja/icons/%s",
			 (char *)getenv("HOME"), icon_name);
#else
	strcpy(dest, "data/icons/%s", icon_name);
#endif
}


void freyja_event_notify_view_log(const char *message)
{
	GtkWidget *widget = getGtkStatusBarWidget();


	if (widget)
	{
		if (GTK_IS_STATUSBAR(widget))
		{
			gtk_statusbar_push(GTK_STATUSBAR(widget), 0, message);
		}
		else if (GTK_IS_LABEL(widget))
		{
			gtk_label_set_text(GTK_LABEL(widget), message);
		}
	}
}


int main(int argc, char *argv[])
{
#ifdef ENABLE_NLS
	bindtextdomain(PACKAGE, PACKAGE_LOCALE_DIR);
	textdomain(PACKAGE);
#endif

	gtk_init(&argc, &argv);

	event_print("@GTK+ interface started...");	
	event_print("Email bug reports to %s", EMAIL_ADDRESS);

	/* Mongoose 2002.02.23, 
	 * Start Freyja which builds the widgets from a script */
	freyja_event_start();

	/* Mongoose 2002.02.23, 
	 * Load file passed by command line args */
	if (argc > 1)
	{
		fileselection_dir_set_event(argv[1]);
		freyja_event_file_dialog_notify(argv[1]);
	}

	//gdk_threads_enter();
	gtk_main();
	//gdk_threads_leave();

	return 0;
}
