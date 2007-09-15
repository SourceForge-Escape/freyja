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
QueryDialog* gLastQueryDialog = NULL;

namespace mgtk {

QueryDialog* FindQueryDialog(const char* name)
{
	if ( gLastQueryDialog && gLastQueryDialog->mName == name )
		return gLastQueryDialog;

	unsigned int i;
	foreach (gQueryDialogs, i)
	{
		if ( gQueryDialogs[i].mName == name )
		{
			// Cache last hit to speed up most common pattern.
			gLastQueryDialog = &(gQueryDialogs[i]);
			return &(gQueryDialogs[i]);
		}
	}

	return NULL;
}


bool ExecuteQueryDialog(const char *name)
{
	QueryDialog* query = mgtk::FindQueryDialog( name );
	return query ? query->Execute() : false;
}

} // namespace mgtk


int mgtk_execute_query_dialog(const char *symbol)
{
	return mgtk::ExecuteQueryDialog(symbol);
}


float mgtk_get_query_dialog_float(const char *dialog, const char *symbol)
{
	QueryDialog* query = mgtk::FindQueryDialog( dialog );
	return query ? query->GetFloat( symbol ) : 0.0f;
}


int mgtk_get_query_dialog_int(const char *dialog, const char *symbol)
{
	QueryDialog* query = mgtk::FindQueryDialog( dialog );
	return query ? query->GetInt( symbol ) : 0;
}


const char* mgtk_get_query_dialog_string(const char* dialog, const char* symbol)
{
	QueryDialog* query = mgtk::FindQueryDialog( dialog );
	return query ? query->GetString( symbol ) : NULL;
}


void mgtk_set_query_dialog_string(const char* dialog, const char* symbol, const char* s)
{
	QueryDialog* query = mgtk::FindQueryDialog( dialog );

	if ( query ) 
		query->SetString(symbol, s);
}


void mgtk_set_query_dialog_string_default(const char* dialog, const char* symbol, const char* s)
{
	QueryDialog* query = mgtk::FindQueryDialog( dialog );

	if ( query ) 
		query->SetDefaultString(symbol, s);
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

	/* Floats */
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

	/* Integers */
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

	/* Textareas */
	printf("*** %i text areas\n", mTextAreaStrings.size());
	GtkWidget* textareas[mTextAreaStrings.size()]; // yes, stack abuse
	foreach (mTextAreaStrings, i)
	{
		// Reset to default value 
		mTextAreaStrings[i].Set( mTextAreaStrings[i].GetDefault() );

		/* Setup text view widget */
		GtkWidget* view = gtk_text_view_new();
		textareas[i] = view;

		if ( mTextAreaStrings[i].Get().c_str() )
		{
			GtkTextBuffer* buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW(view) );
			gtk_text_buffer_set_text(buffer, mTextAreaStrings[i].Get().c_str(), -1);	
		}

		GtkWidget *label = gtk_label_new(NULL);
		gtk_label_set_markup(GTK_LABEL(label), mTextAreaStrings[i].GetQuestion());
		gtk_label_set_selectable(GTK_LABEL(label), FALSE);
		gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), label);
		gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), view);
		gtk_widget_show_all(dialog);
	}

	/* TextEntry */	
	GtkWidget* textentries[mTextEntryStrings.size()]; // yes, stack abuse
	foreach (mTextEntryStrings, i)
	{
		// Reset to default value 
		mTextEntryStrings[i].Set( mTextEntryStrings[i].GetDefault() );

		GtkWidget* entry = textentries[i] = gtk_entry_new();

		if ( mTextEntryStrings[i].Get().c_str() )
		{
			gtk_entry_set_text(GTK_ENTRY(entry), mTextEntryStrings[i].Get().c_str());	
		}

		GtkWidget *box = gtk_hbox_new(FALSE, 0);
		GtkWidget *label = gtk_label_new(NULL);
		gtk_label_set_markup(GTK_LABEL(label), mTextEntryStrings[i].GetQuestion());
		gtk_label_set_selectable(GTK_LABEL(label), FALSE);

		gtk_widget_ref(box);
		gtk_container_add(GTK_CONTAINER(box), label);
		gtk_container_add(GTK_CONTAINER(box), entry);
		gtk_widget_show_all(box);

		gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), box);
	}

	if (mCancelText.c_str() && mCancelText.c_str()[0])
	{
		cancel = gtk_dialog_add_button(GTK_DIALOG(dialog), mCancelText.c_str(), 0);
		if (cancel)
		{
			icon = mgtk_create_icon(mCancelIcon.c_str(), GTK_ICON_SIZE_LARGE_TOOLBAR);

			if (icon)
				gtk_button_set_image(GTK_BUTTON(cancel), icon);
		}
	}

	accept = gtk_dialog_add_button(GTK_DIALOG(dialog), mAcceptText.c_str(), 
								   GTK_RESPONSE_ACCEPT);
	// Add icons to buttons
	icon = mgtk_create_icon(mAcceptIcon.c_str(), GTK_ICON_SIZE_LARGE_TOOLBAR);

	if (icon)
		gtk_button_set_image(GTK_BUTTON(accept), icon);

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

	foreach (mTextAreaStrings, i)
	{
		/* Emit signal with text data copied via property... */
		GtkTextBuffer* buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW(textareas[i]) );
		gchar* text;
		g_object_get(GTK_OBJECT( buffer ), "text", &text, NULL);
		mTextAreaStrings[i].Set(text);
		g_free(text);
	}

	foreach (mTextEntryStrings, i)
	{
		/* Emit signal with text data copied via property... */
		const gchar* text = gtk_entry_get_text( GTK_ENTRY(textentries[i]) );
		mTextEntryStrings[i].Set(text);
		// text here must not be freed from entry!
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
	unsigned int i;
	foreach (mFloats, i)
	{
		if ( mFloats[i].GetSymbolString() == symbol )
		{
			return mFloats[i].Get();
		}
	}

	return 0.0f;
}


int QueryDialog::GetInt(const char *symbol)
{
	unsigned int i;
	foreach (mInts, i)
	{
		if ( mInts[i].GetSymbolString() == symbol )
		{
			return mInts[i].Get();
		}
	}

	return 0;
}


const char* QueryDialog::GetString(const char *symbol)
{
	const char* s = GetTextEntryString( symbol );

	if ( s == NULL )
	{
		s = GetTextAreaString( symbol );		
	}

	return s;
}


const char* QueryDialog::GetTextEntryString(const char* symbol)
{
	unsigned int i;
	foreach (mTextEntryStrings, i)
	{
		if ( mTextEntryStrings[i].GetSymbolString() == symbol )
		{
			return mTextEntryStrings[i].mValue.c_str();
		}
	}

	return NULL;
}


const char* QueryDialog::GetTextAreaString(const char* symbol)
{
	unsigned int i;
	foreach (mTextAreaStrings, i)
	{
		if ( mTextAreaStrings[i].GetSymbolString() == symbol )
		{
			return mTextAreaStrings[i].mValue.c_str();
		}
	}

	return NULL;
}


bool QueryDialog::SetDefaultString(const char* symbol, const char* value)
{
	bool ret = SetDefaultTextEntryString( symbol, value );

	if ( !ret )
	{
		ret = SetDefaultTextAreaString( symbol, value );		
	}

	return ret;
}


bool QueryDialog::SetDefaultTextEntryString(const char* symbol, const char* value)
{
	unsigned int i;
	foreach (mTextEntryStrings, i)
	{
		if ( mTextEntryStrings[i].GetSymbolString() == symbol )
		{
			mTextEntryStrings[i].SetDefault( value );
			return true;
		}
	}

	return false;
}


bool QueryDialog::SetDefaultTextAreaString(const char* symbol, const char* value)
{
	unsigned int i;
	foreach (mTextAreaStrings, i)
	{
		if ( mTextAreaStrings[i].GetSymbolString() == symbol )
		{
			mTextAreaStrings[i].SetDefault( value );
			return true; 
		}
	}

	return false;
}


bool QueryDialog::SetString(const char* symbol, const char* value)
{
	bool ret = SetTextEntryString( symbol, value );

	if ( !ret )
	{
		ret = SetTextAreaString( symbol, value );		
	}

	return ret;
}


bool QueryDialog::SetTextEntryString(const char* symbol, const char* value)
{
	unsigned int i;
	foreach (mTextEntryStrings, i)
	{
		if ( mTextEntryStrings[i].GetSymbolString() == symbol )
		{
			mTextEntryStrings[i].Set( value );
			return true;
		}
	}

	return false;
}


bool QueryDialog::SetTextAreaString(const char* symbol, const char* value)
{
	unsigned int i;
	foreach (mTextAreaStrings, i)
	{
		if ( mTextAreaStrings[i].GetSymbolString() == symbol )
		{
			mTextAreaStrings[i].Set( value );
			return true; 
		}
	}

	return false;
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
  while ( mlisp_peek_for_vargs() )
  {
	  sym = symbol();

	  if ( mlisp_is_cstring(sym) )
	  {
		  mstl::String s;
		  s = mlisp_get_string(sym); 

		  if (s == "float")
		  {
			  arg_list_t *args[5];

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
			  arg_list_t *args[5];

			  for (unsigned int i = 0; i < 5; ++i)
			  {
				  args[i] = symbol();
			  }

			  const char *symbol = mlisp_get_string(args[0]);
			  const char *question = mlisp_get_string(args[1]);
			  float value = mlisp_get_numeric(args[2]);

			  QueryDialogValue<int> v(symbol, question, (int)value);
			  d.mInts.push_back(v);

			  //MGTK_ASSERTMSG(0, "'%s' '%s' %i", symbol, question, value);

			  for (unsigned int i = 0; i < 5; ++i)
			  {
				  delete_arg(&(args[i]));
			  }
		  }
		  else if (s == "textarea")
		  {
				const unsigned int count = 3;
				arg_list_t* args[count];

				for (unsigned int i = 0; i < count; ++i)
				{
					args[i] = symbol();
				}

				const char* symbol = mlisp_get_string(args[0]);
				const char* question = mlisp_get_string(args[1]);
				const char* value = mlisp_get_string(args[2]);

				QueryDialogValue<mstl::String> v(symbol, question, mstl::String(value));
				d.mTextAreaStrings.push_back(v);

				//MGTK_ASSERTMSG(0, "'%s' '%s' '%s'", symbol, question, value);

				for (unsigned int i = 0; i < count; ++i)
				{
					delete_arg(&(args[i]));
				}
		  }
		  else if (s == "textentry")
		  {
				const unsigned int count = 3;
				arg_list_t* args[count];

				for (unsigned int i = 0; i < count; ++i)
				{
					args[i] = symbol();
				}

				const char* symbol = mlisp_get_string(args[0]);
				const char* question = mlisp_get_string(args[1]);
				const char* value = mlisp_get_string(args[2]);

				QueryDialogValue<mstl::String> v(symbol, question, mstl::String(value));
				d.mTextEntryStrings.push_back(v);

				//MGTK_ASSERTMSG(0, "%i - '%s' '%s' '%s'", d.mTextEntryStrings.size(), symbol, question, value);

				for (unsigned int i = 0; i < count; ++i)
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
