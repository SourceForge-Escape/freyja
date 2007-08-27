--------------------------------------------------------------------------
-- Project: Freyja
-- Author : Terry 'Mongoose' Hendrix II
-- Website: http://icculus.org/freyja
-- Email  : mongooseichiban@gmail.com
-- Comment: Interface plugin for perlin noise and texture generator.
--------------------------------------------------------------------------

-- Create a cube mesh as a load test.
--cube1 = freyjaMeshCreateCube(0.0, 8.0, 0.0, 8.0)

function ePerlinSeed(i)
	gPerlinNoiseSeed = i
end


function ePerlinNoiseW(i)
	gPerlinNoiseW = i
end


function ePerlinNoiseH(i)
	gPerlinNoiseH = i
end


function ePerlinNoiseClamp(i)
	gPerlinNoiseClamp = i;
end


function ePerlinNoiseIA(v)
	gPerlinNoiseIA = v
end


function ePerlinNoiseIB(v)
	gPerlinNoiseIB = v
end


function ePerlinNoiseD(v)
	gPerlinNoiseD = v
end

function texture_plugin_init_gui()

	window = mgtk_window("Lua plugin test", "icon.png")

	--embedded_frame = mgtk_summonbox( "FirstPartyPluginSlot" )

	--handlebox = mgtk_handlebox( 1 )
	--mgtk_box_pack( embedded_frame, handlebox )

	--vbox = mgtk_vbox( true, 1 )
	--mgtk_box_pack( handlebox, vbox )

	--expander = mgtk_expander( "TextureGen [Lua]", false )
	--mgtk_box_pack( vbox, expander )

	vbox = mgtk_vbox( true, 1 )
	mgtk_box_pack( window, vbox )

	menubar = mgtk_menubar()
	mgtk_box_pack( vbox, menubar )
	mgtk_append_menu( menubar, mgtk_submenu(ePerlinNoiseMenu, "_Tools") )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( vbox, hbox ) 
	mgtk_box_pack( hbox, mgtk_label("Perlin Noise", 0.0, 0.5) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( vbox, hbox) 
	mgtk_box_pack( hbox, mgtk_label("Seed", 0.0, 0.5) )
	mgtk_box_pack( hbox, mgtk_spinbutton_unit(257, 0, 1000, ePerlinNoiseSeed) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( hbox, mgtk_label("Width", 0.0, 0.5) )
	mgtk_box_pack( hbox, mgtk_spinbutton_unit(256, 8, 1024, ePerlinNoiseW) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( hbox, mgtk_label("Height", 0.0, 0.5) )
	mgtk_box_pack( hbox, mgtk_spinbutton_unit(256, 8, 1024, ePerlinNoiseH) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( hbox, mgtk_label("Clamping on?", 0.0, 0.5) )
	mgtk_box_pack( hbox, mgtk_spinbutton_unit(1, 0, 1, ePerlinNoiseClamp) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( hbox, mgtk_label("iA", 0.0, 0.5) ) 
	mgtk_box_pack( hbox, mgtk_spinbutton_float(1.0, 0.0, 99999.0, 1.0, 1.0, 1.0, 1, ePerlinNoiseIA) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( hbox, mgtk_label("iB", 0.0, 0.5) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(2.0, 0.0, 99999.0, 1.0, 1.0, 1.0, 1, ePerlinNoiseIB) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( hbox, mgtk_label("d", 0.0, 0.5) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(20.0, 0.0, 100.0, 1.0, 1.0, 1.0, 3, ePerlinNoiseD) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( hbox, mgtk_label("Modulate color", 0.0, 0.5) )
	mgtk_box_pack( hbox, mgtk_colorbutton(1, eColorPerlinMult) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( hbox, mgtk_label("Add color", 0.0, 0.5) )
	mgtk_box_pack( hbox, mgtk_colorbutton(1, eColorPerlinAdd) )

	hbox = mgtk_hbox( true, 0 )
	mgtk_box_pack( hbox, mgtk_button("Generate texture", eMode, ePerlinNoiseGen) )

end

texture_plugin_init_gui()



function TexturePlugin:Spawn()
	self:Spawn()
	texture_plugin_init_gui()
end


function TexturePlugin:Draw()
	if self.draw_func then
		self:draw_func()
	end
end


function TexturePlugin:New(id)
    -- Create a new, empty table as in instance of the object
    local instance = { }

    -- Initialize any members
    instance.id = plugin_create(id)
	instance.timer = 0.0
	instance.task = nil
	instance.client = nil
	instance.signalhandler_func = nil
	instance.uiregister_func = nil
	instance.draw_func = nil

	instance.seed = 257
	instance.width = 256
	instance.height = 256
	instance.clamp = 1
	instance.ia = 1.0
	instance.ib = 2.0
	instance.d = 20.0
	instance.add_color = { 0.51,	0.51,	0.29,	1.0 }
	instance.mult_color = { 0.61,	0.51,	0.35,	1.0 }

    -- Set the object as the metatable and
    -- __index table of the instance. This way
    -- the object is searched for any member
    -- (typically methods) the instance doesn't have.
    setmetatable(instance,self)
    self.__index = self
    -- Return the instance
    return instance
end





function eColorPerlinMult(c, count)
	for i = 0, 4 do
		gColorPerlinMult[i] = c[i]
	end

--	mgtk_event_set_color(Resource::mInstance->getIntByName("eColorPerlinMult"), c[0], c[1], c[2], c[3]);
--	mgtk_event_gl_refresh();
end


function eColorPerlinAdd(c, count)
	for i = 0, 4 do
		gColorPerlinAdd[i] = c[i];
	end

--	mgtk_event_set_color(Resource::mInstance->getIntByName("eColorPerlinAdd"), c[0], c[1], c[2], c[3]);
--	mgtk_event_gl_refresh();
end



--[[



void ePerlinNoiseGen()
{
	PerlinNoise perlin;
	uint32 seed = 257, w = 256, h = 256, clamp = 1;
	vec_t iA = 1.0f, iB = 2.0f, d = 20.0f;

	seed = gPerlinNoiseSeed;
	w = gPerlinNoiseW;
	h = gPerlinNoiseH;
	clamp = gPerlinNoiseClamp;
	iA = gPerlinNoiseIA;
	iB = gPerlinNoiseIB; 
	d = gPerlinNoiseD; 

	byte *image = perlin.generateBuffer(w, h, seed);

	if (!image)
	{
		mgtk_print("ePerlinNoiseGen: Invalid image generation");
		return;
	}

	if (clamp)
		perlin.clampBufferIntensity(image, w, h, iA, iB, d);

	/* Modulate by a color and add a base half intensity */
	byte *rgb = new byte[w*h*3];
	
	// Greyscale -> RGB 
	for (uint32 i = 0, n = w*h, idx; i < n; ++i)
	{
		byte c = image[i];
		idx = i*3;
		rgb[idx] = rgb[idx+1] = rgb[idx+2] = c; 
	}

	// hahaha it's 0600 no sleep -- can't wait to clean this prototype!
	for (uint32 i = 0, n = w * h * 3; i < n; ++i)
	{
		/* NOTE: No clamping or scaling of colors, however there is a 
		         weakened 50 / 50 add in the sense that ADD can only contrib
				 _up_to_ 50% of full intensity ( 255 ).

				The reason for this is to allow bleeding for plasma, etc.
		*/

		// Modulate and adjust intensity per pixel
		rgb[i] = (byte)(rgb[i] * gColorPerlinMult[0]) + 
				(byte)(128 * gColorPerlinAdd[0]);
		++i;
		rgb[i] = (byte)(rgb[i] * gColorPerlinMult[1]) +
				(byte)(128 * gColorPerlinAdd[1]);
		++i;
		rgb[i] = (byte)(rgb[i] * gColorPerlinMult[2]) + 
				(byte)(128 * gColorPerlinAdd[2]);
	}

#ifdef BUILT_INTO_APPLICATION
	freyja_load_texture_buffer(rgb, w, h, 24);
#else
	unsigned int tid = freyjaTextureCreateBuffer(rgb, 3, w, h, RGB_24);
	unsigned long id = ResourceEvent::GetResourceIdBySymbol("eTextureUpload");

	mgtk_print("!Uploading texture %i, using function #%i...", tid, id);

	if (ResourceEvent::listen(id - 10000, tid))
		mgtk_print("!ePerlinNoiseGen generated texture successfully.");
	else
		mgtk_print("!ePerlinNoiseGen failed to upload texture to application.");

	freyjaTextureDelete(tid);
#endif

	mgtk_event_gl_refresh();
}


void eGenerateTextureCheckerboard()
{
	byte bg[4] = { (byte)(255 * gColorPerlinAdd[0]), (byte)(255 * gColorPerlinAdd[1]), (byte)(255 * gColorPerlinAdd[2]), (byte)(255 * gColorPerlinAdd[3]) };

	byte fg[4] = { (byte)(255 * gColorPerlinMult[0]), (byte)(255 * gColorPerlinMult[1]), (byte)(255 * gColorPerlinMult[2]), (byte)(255 * gColorPerlinMult[3]) };
					
	uint32 width = 256;
	uint32 height = 256;
	uint32 runlen = 16;
	byte *image = new unsigned char[height*width*3];
	bool swap = true;

	for (uint32 i = 0, l = 0; i < width; ++i, ++l)
	{
		for (uint32 j = 0; j < height; ) // 2px black border
		{
			byte *rgba = (swap) ? bg : fg;			

			for (uint32 k = 0; k < runlen; ++k, ++j)
			{
				uint32 idx = ( height * i + j ) * 3;

				/* RBGA */
				image[idx]   = rgba[0];
				image[idx+1] = rgba[1];
				image[idx+2] = rgba[2];
				//image[idx+3] = rgba[3];
			}

			swap = !swap;
		}

		if (l >= runlen)
		{
			l = 0;
			swap = !swap;	
		}
	}

	unsigned int tid = freyjaTextureCreateBuffer(image, 3, width, height, RGB_24);
	unsigned long id = ResourceEvent::GetResourceIdBySymbol("eTextureUpload");

	mgtk_print("!Uploading texture %i, using function #%i...", tid, id);

	if (ResourceEvent::listen(id - 10000, tid))
		mgtk_print("!ePerlinNoiseGen generated texture successfully.");
	else
		mgtk_print("!ePerlinNoiseGen failed to upload texture to application.");

	freyjaTextureDelete(tid);
	//delete [] image;
}

void eGenerateTextureGradiant()
{
	uint32 width = 256;
	uint32 height = 256;
	byte *image = new unsigned char[height*width*3];
	float iw = 1.0f / (float)width;
	//float ih = 1.0f / (float)height;

	for (uint32 i = 0; i < height; ++i)
	{
		for (uint32 j = 0; j < width; ++j) 
		{
			uint32 idx = ( width * i + j ) * 3;
			float u = j * iw;
			//float v = i * ih;

			const float gr = (1.0f - u);
			const float gr2 = u;

			// Diagonal
			//bg = (byte)(255 * (1.0f - ( (0.5f * u) + (0.5f * v) ) ) );
			//fg = (byte)(255 * (( (0.5f * u) + (0.5f * v) ) ) );

			for (uint32 k = 0; k < 3; ++k)
			{
				float c = gColorPerlinAdd[k] * gr + gColorPerlinMult[k] * gr2;
				image[idx+k]   = (byte)(255 * c);
			}
		}
	}

	unsigned int tid = freyjaTextureCreateBuffer(image, 3, width, height, RGB_24);
	unsigned long id = ResourceEvent::GetResourceIdBySymbol("eTextureUpload");

	mgtk_print("!Uploading texture %i, using function #%i...", tid, id);

	if (ResourceEvent::listen(id - 10000, tid))
		mgtk_print("!%s() generated texture successfully.", __func__);
	else
		mgtk_print("!%s() failed to upload texture to application.", __func__);

	freyjaTextureDelete(tid);
	//delete [] image;
}


void eGenerateTextureColorcube()
{
	uint32 width = 256;
	uint32 height = 256;
	byte *image = new unsigned char[height*width*3];
	float iw = 1.0f / (float)width;
	float ih = 1.0f / (float)height;

	for (uint32 i = 0; i < height; ++i)
	{
		for (uint32 j = 0; j < width; ++j) 
		{
			uint32 idx = ( width * i + j ) * 3;
			float u = j * iw;
			float v = i * ih;

			image[idx]   = (byte)(255 *v);
			image[idx+1] = (byte)(255 * (0.5f * u + 0.5f * v) );
			image[idx+2] = (byte)(255 * ( 0.5f - 0.25f * u + 0.25f * v ) );
		}
	}

	unsigned int tid = freyjaTextureCreateBuffer(image, 3, width, height, RGB_24);
	unsigned long id = ResourceEvent::GetResourceIdBySymbol("eTextureUpload");

	mgtk_print("!Uploading texture %i, using function #%i...", tid, id);

	if (ResourceEvent::listen(id - 10000, tid))
		mgtk_print("!%s() generated texture successfully.", __func__);
	else
		mgtk_print("!%s() failed to upload texture to application.", __func__);

	freyjaTextureDelete(tid);
	//delete [] image;
}


void ePerlinNoiseNop()
{
}


void PerlinNoiseEventsAttach()
{
	ResourceEventCallback::add("ePerlinNoisePluginsMenu", &ePerlinNoiseNop);

	ResourceEventCallback::add("ePerlinNoiseMenu", &ePerlinNoiseNop);

	ResourceEventCallback::add("ePerlinNoiseGen", &ePerlinNoiseGen);

	ResourceEventCallback::add("ePerlinNoiseCheckerboard", &eGenerateTextureCheckerboard);
	ResourceEventCallback::add("ePerlinNoiseColorcube", &eGenerateTextureColorcube);
	ResourceEventCallback::add("eGenerateTextureGradiant", &eGenerateTextureGradiant);

	


	// FIXME: Add limits and a GUI generator wrapper for this
	//        the GUI generator wrapper will have to wait until 
	//        interface is done to call itself back to generate
	// FIXME: Also find a way to make these data members of the other event
	//        if possible ( remember callbacks might need functions )
	ResourceEventCallback::add("eDialogPerlinNoise", &eDialogPerlinNoise);

	ResourceEventCallbackUInt::add("ePerlinNoiseSeed", &ePerlinNoiseSeed);
	ResourceEventCallbackUInt::add("ePerlinNoiseW", &ePerlinNoiseW);
	ResourceEventCallbackUInt::add("ePerlinNoiseH", &ePerlinNoiseH);
	ResourceEventCallbackUInt::add("ePerlinNoiseClamp", &ePerlinNoiseClamp);

	ResourceEventCallbackVec::add("ePerlinNoiseIA", &ePerlinNoiseIA);
	ResourceEventCallbackVec::add("ePerlinNoiseIB", &ePerlinNoiseIB);
	ResourceEventCallbackVec::add("ePerlinNoiseD", &ePerlinNoiseD);

	ResourceEventCallbackVecArray::add("eColorPerlinMult", &eColorPerlinMult);
	ResourceEventCallbackVecArray::add("eColorPerlinAdd", &eColorPerlinAdd);
}


void PerlinNoiseGUIAttach()
{
	char *basename = "plugins/perlinnoise.mlisp";
	char *filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;

	int menuId = Resource::mInstance->getIntByName("ePerlinNoiseMenu");
	int mainMenuId = Resource::mInstance->getIntByName("ePluginMenu");

	int pluginMenuId = Resource::mInstance->getIntByName("ePerlinNoisePluginsMenu");
	mgtk_append_menu_to_menu(mainMenuId, "Texture Generator", pluginMenuId);

	int id = Resource::mInstance->getIntByName("ePerlinNoiseCheckerboard");
	mgtk_append_item_to_menu(menuId, "Checkerboard Texture", id);
	mgtk_append_item_to_menu(pluginMenuId, "Checkerboard", id);

	id = Resource::mInstance->getIntByName("ePerlinNoiseColorcube");
	mgtk_append_item_to_menu(menuId, "Colorcube Texture", id);
	mgtk_append_item_to_menu(pluginMenuId, "Colorcube", id);

	id = Resource::mInstance->getIntByName("eGenerateTextureGradiant");
	mgtk_append_item_to_menu(menuId, "Gradiant Texture", id);
	mgtk_append_item_to_menu(pluginMenuId, "Gradiant", id);

	id = Resource::mInstance->getIntByName("ePerlinNoiseGen");
	mgtk_append_item_to_menu(pluginMenuId, "Perlin Noise", id);

	int add = Resource::mInstance->getIntByName("eColorPerlinAdd");
	mgtk_event_set_color(add, 0.51f, 0.51f, 0.29f, 1.0f);
	
	int multi = Resource::mInstance->getIntByName("eColorPerlinMult");
	mgtk_event_set_color(multi, 0.61f, 0.51f, 0.35f, 1.0f);
}


void perlinnoise_draw()
{
	// FIXME: You need some kind of token or setting to control this draw.

	mgtk_draw_color3f(1.0f, 1.0f, 0.0f);
	mgtk_draw_line6f(-10.0f, 3.0f, 0.0f,
					 10.0f, 3.0f, 0.0f);

	mgtk_draw_point_size1f(5.0f);
	mgtk_draw_color3f(1.0f, 0.0f, 0.0f);
	mgtk_draw_point3f(-10.0f, 3.0f, 0.0f);
	mgtk_draw_point3f(10.0f, 3.0f, 0.0f);
}

]]

