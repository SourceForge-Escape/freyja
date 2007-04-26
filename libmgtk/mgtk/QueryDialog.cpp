/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Mgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : QueryDialog
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: Query Dialog class 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_CONFIRMATIONDIALOG - Builds QueryDialog class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.04.04:
 * Mongoose - Created, for improved locale support and UI design
 ==========================================================================*/

#include <mstl/String.h>
#include <mstl/Vector.h>

// What a mess
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "mgtk_interface.h"
#include "Resource.h"
#include "QueryDialog.h"

mstl::Vector<QueryDialog> gQueryDialogs;

namespace mgtk {

bool ExecuteQueryDialog(const char *name)
{
	mstl::String n(name);
	unsigned int i;

	foreach (gQueryDialogs, i)
	{
		if (n == gQueryDialogs[i].mName)
		{
			return gQueryDialogs[i].Execute();
		}
	}

	return false;
}

} // namespace mgtk


int mgtk_execute_query_dialog(const char *symbol)
{
	return mgtk::ExecuteQueryDialog(symbol);
}


float mgtk_get_query_dialog_float(const char *dialog, const char *symbol)
{
	mstl::String n(dialog);
	unsigned int i;

	foreach (gQueryDialogs, i)
	{
		if (n == gQueryDialogs[i].mName)
		{
			return gQueryDialogs[i].GetFloat(symbol);
		}
	}

	return 0.0f;
}


int mgtk_get_query_dialog_int(const char *dialog, const char *symbol)
{
	mstl::String n(dialog);
	unsigned int i;

	foreach (gQueryDialogs, i)
	{
		if (n == gQueryDialogs[i].mName)
		{
			return gQueryDialogs[i].GetInt(symbol);
		}
	}

	return 0;
}

bool QueryDialog::Execute()
{
	GtkWidget *dialog, *icon, *info, *cancel, *accept, *hbox;
	int ret = 0;

	dialog = gtk_dialog_new();

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_widget_ref(hbox);
	gtk_object_set_data_full(GTK_OBJECT(GTK_DIALOG(dialog)->vbox), 
							 "hbox1", hbox, (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(hbox);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), 
					   hbox, FALSE, TRUE, 0);

	icon = mgtk_create_icon((char*)mDialogIcon.c_str(), GTK_ICON_SIZE_DIALOG);

	info = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(info), mInformationMessage.c_str());
	gtk_label_set_selectable(GTK_LABEL(info), TRUE);

	gtk_container_add(GTK_CONTAINER(hbox), icon);
	gtk_container_add(GTK_CONTAINER(hbox), info);

	unsigned int i;
	GtkWidget *spinbuttons[mFloats.size()]; // yes, stack abuse
	foreach (mFloats, i)
	{
		// FIXME: Needs per mFloat attribs for these
		float min = 0.0f, max = 64.0f, step = 0.1f;
		int digits = 1;

		// Reset to default value
		mFloats[i].Set(mFloats[i].GetDefault());

		GtkWidget *box = gtk_hbox_new(FALSE, 0);

		spinbuttons[i] = mgtk_create_spinbutton2(box, "spin", 
												mFloats[i].GetDefault(), 
												min, max, 
												step, step*10, step*10, digits);

		GtkWidget *label = gtk_label_new(NULL);
		gtk_label_set_markup(GTK_LABEL(label), mFloats[i].GetQuestion());
		gtk_label_set_selectable(GTK_LABEL(label), FALSE);

		gtk_widget_ref(box);
		gtk_container_add(GTK_CONTAINER(box), label);
		gtk_container_add(GTK_CONTAINER(box), spinbuttons[i]);
		gtk_widget_show_all(box);

		gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), box);
	}

	GtkWidget *spinbuttons2[mInts.size()]; // yes, stack abuse
	foreach (mInts, i)
	{
		// FIXME: Needs per mFloat attribs for these
		float min = 0.0f, max = 64.0f;

		// Reset to default value
		mInts[i].Set(mInts[i].GetDefault());

		GtkWidget *box = gtk_hbox_new(FALSE, 0);

		spinbuttons2[i] = mgtk_create_spinbutton(box, "spin", 
												 mInts[i].GetDefault(), 
												 min, max);

		GtkWidget *label = gtk_label_new(NULL);
		gtk_label_set_markup(GTK_LABEL(label), mInts[i].GetQuestion());
		gtk_label_set_selectable(GTK_LABEL(label), FALSE);

		gtk_widget_ref(box);
		gtk_container_add(GTK_CONTAINER(box), label);
		gtk_container_add(GTK_CONTAINER(box), spinbuttons2[i]);
		gtk_widget_show_all(box);

		gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), box);
	}

	cancel = gtk_dialog_add_button(GTK_DIALOG(dialog), mCancelText.c_str(), 0);
	accept = gtk_dialog_add_button(GTK_DIALOG(dialog), mAcceptText.c_str(), 
								   GTK_RESPONSE_ACCEPT);
	// FIXME: Add icons to buttons here, or roll own dialog button code
	gtk_widget_show_all(dialog);


	/* Force user to close this dialog by stopping other events */
	switch (gtk_dialog_run(GTK_DIALOG(dialog)))
	{      
	case GTK_RESPONSE_ACCEPT:
		ret = 1;
		break;
	default:
		ret = 0;
	}

	foreach (mFloats, i)
	{
		float r = 
		gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbuttons[i]));
		mFloats[i].Set(r);
	}

	foreach (mInts, i)
	{
		int v = (int)
		gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbuttons2[i]));
		mInts[i].Set(v);
	}

	gtk_widget_destroy(dialog);

	return ret;
}


float QueryDialog::GetFloat(const char *symbol)
{
	mstl::String n(symbol);
	unsigned int i;

	foreach (mFloats, i)
	{
		if (n == mFloats[i].GetSymbol())
		{
			return mFloats[i].Get();
		}
	}

	return 0.0f;
}


int QueryDialog::GetInt(const char *symbol)
{
	mstl::String n(symbol);
	unsigned int i;

	foreach (mInts, i)
	{
		if (n == mInts[i].GetSymbol())
		{
			return mInts[i].Get();
		}
	}

	return 0;
}


////////////////////////////////////////////////////////////
// MLISP RC implementations
////////////////////////////////////////////////////////////

arg_list_t *mgtk_rc_query_dialog(arg_list_t *box)
{
  QueryDialog d;
  arg_list_t *a;

  symbol_enforce_type_assert(&a, CSTRING);
  d.mName = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  d.mDialogIcon = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  d.mInformationMessage = mlisp_get_string(a); 
  delete_arg(&a);

  // ARGS
  arg_list_t *sym = NULL;
  while (mlisp_peek_for_vargs())
  {
	  sym = symbol();

	  if (mlisp_is_cstring(sym))
	  {
		  mstl::String s;
		  s = mlisp_get_string(sym); 

		  if (s == "float")
		  {
			  arg_list_t *args[6];

			  for (unsigned int i = 0; i < 5; ++i)
			  {
				  args[i] = symbol();
			  }

			  const char *symbol = mlisp_get_string(args[0]);
			  const char *question = mlisp_get_string(args[1]);
			  float value = mlisp_get_numeric(args[2]);

			  QueryDialogValue<float> v(symbol, question, value);
			  d.mFloats.push_back(v);

			  for (unsigned int i = 0; i < 5; ++i)
			  {
				  delete_arg(&(args[i]));
			  }
		  }
		  else if (s == "int")
		  {
			  arg_list_t *args[6];

			  for (unsigned int i = 0; i < 5; ++i)
			  {
				  args[i] = symbol();
			  }

			  const char *symbol = mlisp_get_string(args[0]);
			  const char *question = mlisp_get_string(args[1]);
			  float value = mlisp_get_numeric(args[2]);

			  QueryDialogValue<int> v(symbol, question, (int)value);
			  d.mInts.push_back(v);

			  //MGTK_ASSERTMSG(0, "'%s' '%s' %i", symbol, question, (int)value);

			  for (unsigned int i = 0; i < 5; ++i)
			  {
				  delete_arg(&(args[i]));
			  }
		  }
		  else
		  {
			  break;
		  }
	  }
	  else
	  {
		  break;
	  }

	  delete_arg(&sym);
  }

  a = sym;
  arg_enforce_type_assert(&a, CSTRING);
  //symbol_enforce_type_assert(&a, CSTRING);  
  d.mCancelIcon = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  d.mCancelText = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  d.mAcceptIcon = mlisp_get_string(a); 
  delete_arg(&a);

  symbol_enforce_type_assert(&a, CSTRING);
  d.mAcceptText = mlisp_get_string(a); 
  delete_arg(&a);

  gQueryDialogs.push_back(d);

  return NULL;
}
