/*==========================================================================
 * 
 * Project : MDDC 
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : Mdl
 * Comments: This is the MDL model class.
 *
 *           See file COPYING for license details.
 *
 *           Quake (c) id Software 1995 
 *
 *-- History ---------------------------------------------------------- 
 *
 * 2000-03-16
 * Mongoose: Endian support
 *           Created from old MDDC source
 ==========================================================================*/

#include <stdio.h>
#include <string.h>
#include "Mdl.h"
#include "mendian.h"


Mdl::Mdl()
{
  _sign = true;
  _triangle = NULL;
  _vertex = NULL;
  _skin = NULL;
  _frame = NULL;
}

Mdl::Mdl(char *filename)
{
  Load(filename);
}

//FIXME: Not finished
Mdl::~Mdl()
{
   int i, j;


   if (_triangle)
     delete [] _triangle;

   if (_vertex)
     delete [] _vertex;

   if (_skin)
   { 
     for (i = 0; i < _numskins; i++)
     {     
       if (_skin[i].pixmap)
	 delete [] _skin[i].pixmap;

       if (_skin[i].gskin_interval)
	 delete [] _skin[i].gskin_interval;
     }

     delete [] _skin;
   }

   if (_frame)
   {
      for (i = 0; i < _numframes; i++)
      {
	if (_frame[i].gframe.gsframe)
        {
	  for(j = 0; j < (int)_frame[i].gframe.num_gsframes; j++)
	  {
	    if (_frame[i].gframe.gsframe[j].tv)
	      delete [] _frame[i].gframe.gsframe[j].tv;
	  }
	    
	  delete _frame[i].gframe.gsframe;
	}

	 if (_frame[i].sframe.tv)
            delete [] _frame[i].sframe.tv;

	 if (_frame[i].gframe.gsframe_interval)
	    delete [] _frame[i].gframe.gsframe_interval;
      }

      delete _frame;
   }
}

void Mdl::Header(MDL_t *header)
{
   if (!header)
      return;

   _ident = header->ident;
   _version = header->version;
   _scale[0] = header->scale[0];
   _scale[1] = header->scale[1];
    _scale[2] = header->scale[2];
   _scale_origin[0] = header->scale_origin[0];
   _scale_origin[1] = header->scale_origin[1];
   _scale_origin[2] = header->scale_origin[2];
   _boundingradius = header->boundingradius;
   _eyeposition[0] = header->eyeposition[0];
   _eyeposition[1] = header->eyeposition[1];
   _eyeposition[2] = header->eyeposition[2];
   _numskins = header->numskins;   
   _skinwidth = header->skinwidth;                            
   _skinheight = header->skinheight;    
   _numverts = header->numverts;     
   _numtris = header->numtris;      
   _numframes = header->numframes;    
   _synctype = header->synctype;    
   _flags = header->flags;       
   _size = header->size;     
}

MDL_t *Mdl::Header()
{
   MDL_t *header = new MDL_t;

   header->ident = _ident;
   header->version = _version;
   header->scale[0] = _scale[0];
   header->scale[1] = _scale[1];
   header->scale[2] = _scale[2];
   header->scale_origin[0] = _scale_origin[0];
   header->scale_origin[1] = _scale_origin[1];
   header->scale_origin[2] = _scale_origin[2];
   header->boundingradius = _boundingradius;
   header->eyeposition[0] = _eyeposition[0];
   header->eyeposition[1] = _eyeposition[1];
   header->eyeposition[2] = _eyeposition[2];
   header->numskins = _numskins;   
   header->skinwidth = _skinwidth;                            
   header->skinheight = _skinheight;    
   header->numverts = _numverts;     
   header->numtris = _numtris;      
   header->numframes = _numframes;    
   header->synctype = _synctype;    
   header->flags = _flags;       
   header->size = _size;  

   return header;
}

stvert_t *Mdl::Vertex()
{
   return _vertex;
}

void Mdl::Vertex(stvert_t *vertex)
{
   _vertex = vertex;
}

dtriangle_t *Mdl::Triangle()
{
   return _triangle;
}

void Mdl::Triangle(dtriangle_t *triangle)
{
   _triangle = triangle;
}

unsigned char *Mdl::Skin(int i)
{
  if (i < 0 || i > _numskins)
    return NULL;

  return _skin[i].pixmap;
}

int Mdl::SkinWidth()
{
   return _skinwidth;
}

int Mdl::SkinHeight()
{
   return _skinheight;
}

void Mdl::Skin(MDL_Skin_t *skin)
{
   _skin = skin;
}

MDL_Frame_t *Mdl::Frame()
{
   return _frame;
}

void Mdl::Frame(MDL_Frame_t *frame)
{
   _frame = frame;
}

void Mdl::Sign(bool set)
{
  _sign = set;
}

int Mdl::Save(char *filename)
{
   int i, k, ii, jj, skin_size;
   unsigned int j;
   FILE *f;
   char tag[64];

   
   if (!(_vertex && _triangle && _skin && _frame))
   {
      printf("Mdl::Save> Invalid MDL was loaded in memory.\n");
      return -1;
   }

   if (_sign)
      sprintf(tag, "\n[MDDC by Mongoose<stu7440@westga.edu>]");

   f = fopen(filename, "wb");

   if (!f)
   {  
      printf("Mdl::Save> Couldn't write %s.\n", filename);
      return -2;
   }
   else
      printf("Mdl::Save> Exporting '%s' as MDL.\n", filename);

   /* Mongoose: Start header */

   fwrite(&_ident,4,1,f);
   fwrite(&_version,4,1,f);
   fwrite(&_scale[0],4,1,f);
   fwrite(&_scale[1],4,1,f); 
   fwrite(&_scale[2],4,1,f);
   fwrite(&_scale_origin[0],4,1,f);
   fwrite(&_scale_origin[1],4,1,f);
   fwrite(&_scale_origin[2],4,1,f);
   fwrite(&_boundingradius,4,1,f);
   fwrite(&_eyeposition[0],4,1,f);           
   fwrite(&_eyeposition[1],4,1,f);
   fwrite(&_eyeposition[2],4,1,f);
   fwrite(&_numskins,4,1,f);
   fwrite(&_skinwidth,4,1,f);
   fwrite(&_skinheight,4,1,f);
   fwrite(&_numverts,4,1,f);
   fwrite(&_numtris,4,1,f);
   fwrite(&_numframes,1,4,f);
   fwrite(&_synctype,1,4,f); 
   fwrite(&_flags,4,1,f);     
   fwrite(&_size,4,1,f);  
   
   /* Mongoose: End header */

   /* Mongoose: Skin info */
   for(k = 0; k < _numskins; k++)
   {
      fwrite(&_skin[k].type, 4, 1, f);

      if(_skin[k].type == 0)
      {
         skin_size = _skinwidth * _skinheight;

	 for(i = 0; i < skin_size; i++)
            fputc(_skin[k].pixmap[i], f);
      }
      else
      {
         fwrite(&_skin[k].num_gskins, 4, 1, f);

         for(j = 0; j < _skin[k].num_gskins; j++)
         {
            fwrite(&_skin[k].gskin_interval[j], 4, 1, f);
         }

         for(j = 0; j < _skin[k].num_gskins; j++)
         {
            for(jj = 0; jj < _skinheight; jj++)
            {
               for(ii = 0; ii < _skinwidth; ii++)
               {
                  fputc(_skin[k].gskin[j].pixmap[ii+_skinwidth*jj],
                        f);
               }
            }
         }
      }
   }
   printf("Mdl::Save> Wrote %i skins.\n", k);

   /* Mongoose: Skin 2d Vertices */
   for(i = 0; i < _numverts; i++)
   {
      fwrite(&_vertex[i].onseam, 4, 1, f);
      fwrite(&_vertex[i].s, 4, 1,f);
      fwrite(&_vertex[i].t, 4, 1, f);
   }
   printf("Mdl::Save> Wrote %i vertices.\n", i);

   /* Mongoose: Triangle definitions */
   for(i = 0; i < _numtris; i++)
   {
      fwrite(&_triangle[i].facesfront, 4, 1, f);
      fwrite(&_triangle[i].vertindex[0], 4, 1, f);
      fwrite(&_triangle[i].vertindex[1], 4, 1, f);
      fwrite(&_triangle[i].vertindex[2], 4, 1, f);
   }
   printf("Mdl::Save> Wrote %i triangles.\n", i);

   /* Mongoose: Frames */
   for(k = 0; k < _numframes;  k++)
   {
     //_frame[k].type = 1;  // FIXME: hack to force multi frames

      fwrite(&_frame[k].type,4,1,f); 

      if (_frame[k].type == 0)  
      {
         fwrite(&_frame[k].sframe.bboxmin,1,4,f);
         fwrite(&_frame[k].sframe.bboxmax,1,4,f);
         fwrite(_frame[k].sframe.name,16,1,f);

         for(i=0;i<_numverts;i++)
         {
            fwrite(_frame[k].sframe.tv+i,1,4,f);
         }
      }
      else
      {
         fwrite(&_frame[k].gframe.num_gsframes,1,4,f);
         fwrite(&_frame[k].gframe.bboxmin,1,4,f);
         fwrite(&_frame[k].gframe.bboxmax,1,4,f);
         for(j=0; j<_frame[k].gframe.num_gsframes; j++)
         {
            fwrite(&_frame[k].gframe.gsframe_interval[j],4,1,f);
         }

         for(j=0; j<_frame[k].gframe.num_gsframes; j++)
         {
            fwrite(&_frame[k].gframe.gsframe[j].bboxmin,1,4,f);
            fwrite(&_frame[k].gframe.gsframe[j].bboxmax,1,4,f);
            fwrite(_frame[k].gframe.gsframe[j].name,16,1,f);

            for(i=0;i< _numverts;i++)
            {
               fwrite(_frame[k].gframe.gsframe[j].tv+i,1,4,f);
            }
         }
      }
   }
   printf("Mdl::Save> Wrote %i frames.\n", k);

   if (_sign)
      fwrite(tag, 1, strlen(tag), f);

   fclose(f);

   return 0;
}

int Mdl::Load(char *filename)
{
   FILE *f;
   int i, skin_sz;
   unsigned j;

   f = fopen(filename, "rb");

   if (!f)
   {
      printf("Mdl::Load> File '%s' not found.\n", filename);
      return -1;
   }

   if (!fread_int_small(&_ident, f))
      return -2;

   if (!fread_int_small(&_version, f))
      return -3;

   if (_ident != (('O'<<24)+('P'<<16)+('D'<<8)+'I'))
   {
      printf("Mdl::Load> Expected MDL IDPO header.\n");
      return -4;
   }

   if (_version != 6)
   {
      printf("Mdl::Load> Expected MDL version 6.\n");
      return -5;
   }

   if (!fread_float_small(&_scale[0], f))
      return -6;

   if (!fread_float_small(&_scale[1], f))
      return -7;

   if (!fread_float_small(&_scale[2], f))
      return -8;

   if (!fread_float_small(&_scale_origin[0], f))
      return -9;

   if (!fread_float_small(&_scale_origin[1], f))
      return -10;

   if (!fread_float_small(&_scale_origin[2], f))
      return -11;

   if (!fread_float_small(&_boundingradius, f))
      return -12;

   if (!fread_float_small(&_eyeposition[0], f))
      return -13;

   if (!fread_float_small(&_eyeposition[1], f))
      return -14;

   if (!fread_float_small(&_eyeposition[2], f))
      return -15;

   if (!fread_int_small(&_numskins, f))
      return -16;

   if (!fread_int_small(&_skinwidth, f))
      return -17;

   if (!fread_int_small(&_skinheight, f))
      return -18;

   if (!fread_int_small(&_numverts, f))
      return -19;

   if (!fread_int_small(&_numtris, f))
      return -20;

   if (!fread_int_small(&_numframes, f))
      return -21;

   if (!fread_int_small(&_synctype, f))
      return -22;

   if (!fread_int_small(&_flags, f))
      return -23;

   if (!fread_float_small(&_size, f))
      return -24;


   /* Read in the skin(s) */

   _skin = new MDL_Skin_t[_numskins];

   skin_sz = _skinheight * _skinwidth;

   for(i = 0; i < _numskins; i++)
   {
      if (!fread_u_int_small(&_skin[i].type, f))
         return -25;

      if (_skin[i].type == 0)
      {
         _skin[i].pixmap = new unsigned char[skin_sz];

         if (fread(_skin[i].pixmap, skin_sz, 1, f) != 1)
	 {
            printf("Mdl::Load> Error in skin fread.\n");
            return -26;
	 }

	 _skin[i].num_gskins = 0;
	 _skin[i].gskin_interval = NULL;
	 _skin[i].gskin = NULL;
      }
      else
      {      
         if (!fread_u_int_small(&_skin[i].num_gskins, f))
            return -27;

         if (_skin[i].num_gskins)
	   _skin[i].gskin_interval = new float[_skin[i].num_gskins];
	 else
	   _skin[i].gskin_interval = NULL;

         for(j = 0; j < _skin[i].num_gskins; j++)
            if (!fread_float_small(&_skin[i].gskin_interval[j], f))
               return -28;

	 if (_skin[i].num_gskins)
	   _skin[i].gskin = new MDL_GSkin_t[_skin[i].num_gskins];
	 else
	   _skin[i].gskin = NULL;

         for(j = 0; j < _skin[i].num_gskins; j++)
         {
            _skin[i].gskin[j].pixmap = new unsigned char[skin_sz];

            if (fread(_skin[i].gskin[j].pixmap, skin_sz, 1, f) != 1)
	    {
               printf("Mdl::Load> Error in group skin fread.\n");
               return -29;
	    }
         }
      }
   }
   printf("Mdl::Load> Read %i skin(s).\n", _numskins);

   /* Read in the texture's 2d vertex list mapping */


   _vertex = new stvert_t[_numverts];

   for(i = 0; i < _numverts; i++)
   {
      if (!fread_int_small(&_vertex[i].onseam, f))
         return -30;

      if (!fread_int_small(&_vertex[i].s, f))
         return -31;

      if (!fread_int_small(&_vertex[i].t, f))
         return -32;
   }

   printf("Mdl::Load> Read %i texels.\n", _numverts);

   /* Read in the index triangle list */

   _triangle = new dtriangle_t[_numtris];

   for(i = 0; i < _numtris; i++)
   {
      if (!fread_int_small(&_triangle[i].facesfront, f))
         return -33;

      if (!fread_int_small(&_triangle[i].vertindex[0], f))
         return -34;

      if (!fread_int_small(&_triangle[i].vertindex[1], f))
         return -35;

      if (!fread_int_small(&_triangle[i].vertindex[2], f))
         return -36;
   }
   printf("Mdl::Load> Read %i triangles.\n", _numtris);

   /* Read in frame animation information */

   _frame = new MDL_Frame_t[_numframes];

   printf("Mdl::Load> Reading %i frames\n", _numframes);

   for (i = 0; i < _numframes; i++)
   {
      if (!fread_u_int_small(&_frame[i].type, f))
         return -37;

      _frame[i].gframe.num_gsframes = 0;
      _frame[i].gframe.gsframe_interval = NULL;       	
      _frame[i].gframe.gsframe = NULL;

      if (_frame[i].type == 0)
      {
         if (fread(&_frame[i].sframe.bboxmin, 4, 1, f) != 1)
	 {
            printf("Mdl::Load> frame bbox min fread failed.\n");
            return -38;
	 }

         if (fread(&_frame[i].sframe.bboxmax, 4, 1, f) != 1)
	 {
            printf("Mdl::Load> frame bbox max fread failed.\n");
            return -39;
	 }

         if (fread(_frame[i].sframe.name, 16, 1, f) != 1)
	 {
            printf("Mdl::Load> frame name fread failed.\n");
            return -40;
	 }
         else
            printf("Mdl::Load> Frame[%i] %s\n", i, _frame[i].sframe.name);

         _frame[i].sframe.tv = new trivertex_t[_numverts];

         if (fread(_frame[i].sframe.tv, 4 * _numverts, 1, f) != 1)
	 {
            printf("Mdl::Load> Frame vertex[%i] fread failed.\n", i);
	    return -41;
	 }
      }
      else
      {
         if (!fread_u_int_small(&_frame[i].gframe.num_gsframes, f))
            return -42;

         if (fread(&_frame[i].gframe.bboxmin, 4, 1, f) != 1)
            return -43;

         if (fread(&_frame[i].gframe.bboxmax, 4, 1, f) != 1)
            return -44;

         _frame[i].gframe.gsframe_interval = 
                new float[_frame[i].gframe.num_gsframes];

         _frame[i].gframe.gsframe =
                new MDL_MFrame_t[_frame[i].gframe.num_gsframes];

         for (j = 0; j < _frame[i].gframe.num_gsframes; j++)
         {
            if (!fread_float_small(&_frame[i].gframe.gsframe_interval[j], f))
               return -45;
         }

         for (j = 0; j < _frame[i].gframe.num_gsframes; j++)
         {
            if (fread(&_frame[i].gframe.gsframe[j].bboxmin, 4, 1, f) != 1)
               return -46;

            if (fread(&_frame[i].gframe.gsframe[j].bboxmax, 4, 1, f) != 1)
               return -47;

            if (fread(&_frame[i].gframe.gsframe[j].name, 16, 1, f) != 1)
               return -48;
            else
               printf("Mdl::Load> GFrame[%i][%i] %s\n", 
                      i, j, _frame[i].gframe.gsframe[j].name);

            _frame[i].gframe.gsframe[j].tv = new trivertex_t[_numverts];

            if (fread(&_frame[i].gframe.gsframe[j].tv,1,4*_numverts,f) != 1)
	       return -49;
         }
      }
   }

   fclose(f);

   return 0;
}


#ifdef TEST_MDL
int main(int argc, char *argv[])
{
  Mdl mdl;


  printf("[MDL class test]\n");

  if (argc > 2)
  {
    if (strcmp(argv[1], "load") == 0)
    {
      if (!mdl.Load(argv[2]))
	printf("main: Load reports success.\n");
    }
    else if (strcmp(argv[1], "save") == 0)
    {
      // FIXME: Add code here to generate a small mdl

      if (!mdl.Save(argv[2]))
	printf("main: Save reports success.\n");
    }
    else if (strcmp(argv[1], "test") == 0 && argc > 3)
    {
      if (!mdl.Load(argv[2]))
	printf("main: Load reports success.\n");
      if (!mdl.Save(argv[3]))
	printf("main: Save reports success.\n");
    }
    else
      printf("\n\n%s [save | load | test] filename.mdl [testout.mdl]\n", 
	     argv[0]);
  }
  else
  {
    printf("\n\n%s [save | load | test] filename.mdl [testout.mdl]\n", 
	   argv[0]);
  }
}
#endif

