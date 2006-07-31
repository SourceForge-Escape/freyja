/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : SystemIO w/ endian support
 * License : No use w/o permission (C) 2001-2006 Mongoose
 * Comments: Lots of nice utils from libfreyja moved to mstl
 *           aids system abstraction and debugging.
 * 
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2006.07.23:
 * Mongoose - Barrowed from my libfreyja, and then made more generic
 ================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_SYSTEMIO_H_
#define GUARD__FREYJA_MONGOOSE_SYSTEMIO_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef WIN32
#   include "windows.h"
#   include "winbase.h" // Required for Sleep()
#endif

// Some very old byte swapping routines
#define SWAP_2(x) ( (((x) & 0xff) << 8) | ((unsigned short)(x) >> 8) )
#define SWAP_4(x) ( ((x) << 24) | \
         (((x) << 8) & 0x00ff0000) | \
         (((x) >> 8) & 0x0000ff00) | \
         ((x) >> 24) )
#define FIX_SHORT(x) (*(unsigned short *)&(x) = SWAP_2(*(unsigned short *)&(x)))
#define FIX_INT(x)   (*(unsigned int *)&(x)   = SWAP_4(*(unsigned int *)&(x)))
#define FIX_FLOAT(x) FIX_INT(x)


namespace mstl {

class SystemIO
{
 public:	


	class File
	{
	public:

		enum ByteOrder {
			
			BIG    = 1,
			LITTLE 
			
		};


		File() :
			mHostOrder(LITTLE),
			mFileHandle(NULL),
			mBuffer(NULL),
			mBufferSize(0),
			mDirectory(NULL),
			mDirectoryName(NULL),
			mDirectoryListing(NULL)
		{
		}


#if 0 // This doesn't make sense to copy -- if YOU want to start here
		File(const File &f) :
			mHostOrder(f.mHostOrder),
			mFileHandle(NULL),
			mBuffer(NULL),
			mBufferSize(0),
			mDirectory(NULL),
			mDirectoryName(NULL),
			mDirectoryListing(NULL)
		{
		}
#endif


		virtual ~File()
		{
			Close();
			CloseDir();

			if (mBuffer) delete [] mBuffer;

			if (mDirectoryName) delete [] mDirectoryName;

			if (mDirectoryListing) delete [] mDirectoryListing;
		}

		
		void Close()
		{
			if (mFileHandle)
			{
				fclose(mFileHandle);
				mFileHandle = NULL;
			}
		}


		void CloseDir()
		{
			if (mDirectoryListing)
			{
				delete [] mDirectoryListing;
				mDirectoryListing = NULL;
			}

			if (mDirectory)
			{
				closedir(mDirectory);
				mDirectory = NULL;
			}
		}


		static bool CreateDir(const char *path)
		{
		#ifdef WIN32
			return ( _mkdir(path) > 0 );
		#else
			return ( mkdir(path, S_IRWXU | S_IRWXG) > 0 );
		#endif		
		}


		static bool DoesFileExist(const char *filename)
		{
			if (!filename || !filename[0])
				return false;

			FILE *f = fopen(filename, "rb");

			if (f)
			{
				fclose(f);
				return true;
			}

			return false;
		}


		const char *GetNextDirectoryListing()
		{
			struct dirent *d_ptr;

			if (mDirectoryListing)
			{
				delete [] mDirectoryListing;
				mDirectoryListing = NULL;
			}

			if (mDirectory && (d_ptr = readdir(mDirectory)))
			{
				long length = strlen(mDirectoryName) + strlen(d_ptr->d_name) + 2;
				mDirectoryListing = new char[length+1];
				snprintf(mDirectoryListing, length, "%s/%s", mDirectoryName, d_ptr->d_name);
				mDirectoryListing[length] = '\0';
			}

			return mDirectoryListing;
		}


		long GetOffset()
		{
			return ftell(mFileHandle);
		}


		long GetSize()
		{
			long whence = GetOffset();
			fseek(mFileHandle, 0, SEEK_END);
			long size = GetOffset();
			SetOffset(whence);

			return size;
		}


		static bool IsDirectory(const char *filename)
		{
			struct stat status;
			stat(filename, &status);
			return ((S_ISDIR(status.st_mode)));
		}


		bool IsEndOfFile()
		{
			return (feof(mFileHandle) != 0);
		}


		bool OpenDir(const char *dirname)
		{
			long length;


			if (!dirname || !dirname[0])
				return false;

			mDirectory = opendir(dirname);
	
			if (!mDirectory)
			{
				perror(dirname);
				return false;
			}

			length = strlen(dirname);
			mDirectoryName = new char[length+1];
			strncpy(mDirectoryName, dirname, length);
			mDirectoryName[length] = 0;

			return true;
		}

		
		void SetBufferSize(const long size)
		{
			if (size < 1) return;

			mBufferSize = size;
			if (mBuffer) delete [] mBuffer;
			mBuffer = new char[size];
		}


		void SetByteOrder(ByteOrder order) { mHostOrder = order; }


		bool SetOffset(long offset)
		{
			return (fseek(mFileHandle, offset, SEEK_SET) == 0);
		}


		void SetOffsetToEnd()
		{
			fseek(mFileHandle, 0, SEEK_END);
		}


	protected:

		bool Open(const char *filename, const char *mode)
		{
			if (!filename || !filename[0])
				return false;

			mFileHandle = fopen(filename, mode);
	
			if (!mFileHandle)
			{
				perror(filename);
				return false;
			}

			return true;
		}


		ByteOrder mHostOrder;             /* Host machine byte order */

		FILE *mFileHandle;                /* File members */
		char *mBuffer;
		long mBufferSize;

		DIR *mDirectory;                  /* Directory members */
		char *mDirectoryName;
		char *mDirectoryListing;
	};


	class FileReader : public File
	{
	public:
		FileReader() : File() { }
		~FileReader() { }

		bool Open(const char *filename) { return File::Open(filename, "rb"); }
		
		bool ReadBuffer(unsigned long length, unsigned char *buffer)
		{
			return (fread(buffer, length, 1, mFileHandle) == 1);	
		}


		bool ReadString(unsigned long length, char *buffer)
		{
			return (fread(buffer, length, 1, mFileHandle) == 1);
		}

		void ReadInt32(int &i) { i = ReadInt32(); }

		void ReadInt32U(unsigned int &u) { u = ReadInt32U(); }

		void ReadFloat32(float &f) { f = ReadFloat32(); }


		void ReadInt32Array(long size, int array[])
		{	
			for (int i = 0; i < size; ++i)
			{
				ReadInt32(array[i]);
			}
		}


		void ReadFloat32Array(long size, float array[])
		{	
			for (int i = 0; i < size; ++i)
			{
				ReadFloat32(array[i]);
			}
		}


		float ReadFloat32()
		{	
			float r;
			void *ptr = &r;
			size_t sz = fread(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileReader: ERROR failed to read 32bit float\n");

		#if HAVE_BIG_ENDIAN
			FIX_FLOAT(*ptr)
		#endif

			return r;
		}


		char ReadInt8()
		{
			char c;

			fread(&c, 1, 1, mFileHandle);  // heh, yeah

			return c;
		}


		unsigned char ReadInt8U()
		{
			unsigned char u;

			fread(&u, 1, 1, mFileHandle);  // heh, yeah

			return u;
		}


		short ReadInt16()
		{
			short int i = 0;
			void *ptr = &i;
			size_t sz = fread(ptr, 2, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileReader: ERROR failed to read 16bit int\n");

		#ifdef HAVE_BIG_ENDIAN
			FIX_SHORT(*ptr)
		#endif
			return i;
		}

	
		unsigned short ReadInt16U()
		{
			unsigned short int u = 0;
			void *ptr = &u;
			size_t sz = fread(ptr, 2, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileReader: ERROR failed to read 16bit uint\n");

		#ifdef HAVE_BIG_ENDIAN
			FIX_SHORT(*ptr)
		#endif
			return u;
		}


		int ReadInt32()
		{
			int i = 0;
			void *ptr = &i;
			size_t sz = fread(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileReader: ERROR failed to read 32bit int\n");

		#ifdef HAVE_BIG_ENDIAN
			FIX_INT(*ptr)
		#endif
			return i;
		}


		unsigned int ReadInt32U()
		{
			unsigned int u = 0;
			void *ptr = &u;
			size_t sz = fread(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileReader: ERROR failed to read 32bit uint\n");

		#ifdef HAVE_BIG_ENDIAN
			FIX_INT(*ptr)
		#endif
			return u;
		}


		long ReadLong()
		{
			long i = 0;
			void *ptr = &i;
			size_t sz = fread(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileReader: ERROR failed to read 32bit int\n");

		#ifdef HAVE_BIG_ENDIAN
			if (mHostOrder == LITTLE)
				FIX_INT(*ptr);
		#else
			if (mHostOrder == BIG)
				FIX_INT(*(unsigned int*)ptr);//i = SWAP_4(i);
		#endif
			return i;
		}


		unsigned long ReadLongU()
		{
			unsigned long u = 0;
			void *ptr = &u;
			size_t sz = fread(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileReader: ERROR failed to read 32bit uint\n");

		#ifdef HAVE_BIG_ENDIAN
			if (!mHostOrder == LITTLE)
				FIX_INT(*ptr);
		#else
			if (mHostOrder == BIG)
				u = SWAP_4(u);
		#endif
			return u;
		}

	};


	class TextFileWriter : public File
	{
	public:
		TextFileWriter() : File() { }
		~TextFileWriter() { }

		bool Open(const char *filename) { return File::Open(filename, "wb"); }
	 

		void Print(const char *s, ...)
		{
			va_list args;

			va_start(args, s);
			vfprintf(mFileHandle, s, args);
			va_end(args);
		}


		void PrintFloat(double d)
		{
			fprintf(mFileHandle, "%f", d);
		}


		void PrintInteger(int i)
		{
			fprintf(mFileHandle, "%i", i);
		}


		void PrintString(const char *s)
		{
			fprintf(mFileHandle, "%s", s);
		}

	};


	class TextFileReader : public File
	{
	public:
		TextFileReader() : File() { }
		~TextFileReader() { }

		bool Open(const char *filename) { return File::Open(filename, "rb"); }

		void AllocateBuffer()
		{
			if (mBufferSize == 0)
				SetBufferSize(2048);

			mBuffer[0] = '\0';
		}


		// Stops on '\0' and EOF
		bool FindNextChar(char c)
		{
			bool found = false;
			char lex;

			while ((lex = NextChar()) && !IsEndOfFile())
			{
				if (c == lex)
				{
					found = true;
					break;
				}
			}

			return found;
		}


		char NextChar()
		{
			char c;
			fread(&c, 1, 1, mFileHandle);  // heh, yeah
			return c;
		}


		double ParseDouble()
		{
			double r;
			fscanf(mFileHandle, "%lf", &r);
			return r;
		}


		float ParseFloat()
		{
			float r;
			fscanf(mFileHandle, "%f", &r);
			return r;
		}


		int ParseInteger()
		{
			int i;
			fscanf(mFileHandle, "%i", &i);
			return i;
		}


		bool ParseMatchingSymbol(const char *symbol)
		{
			/* Assumes !(!symbol) && !(!symbol[0]) */
			long l = strlen(symbol);
			const char *sym = ParseSymbol();
			bool test = (!strncmp(symbol, sym, l));

			if (!test) printf("Not matched: '%s' != '%s'\n", symbol, sym);

			return test;
		}


		const char *ParseStringLiteral()
		{
			long l, i = 0, state = 0;
			char *s;
			char c, lc = 0;

			AllocateBuffer();	

			while (i < mBufferSize && fscanf(mFileHandle, "%c", &c) != EOF)
			{
				switch (state)
				{
				case 0:
					if (c == '"')
						state = 1;
					break;

				case 1:
					if (c == '"' && lc != '\\')  // Allow quote escapes?
					{
						i = mBufferSize;
					}
					else
					{
						mBuffer[i++] = c;
						mBuffer[i] = 0;
					}
					break;
				}

				lc = c;
			}
	
			l = strlen(mBuffer);
			s = new char[l+1];
			strncpy(s, mBuffer, l);
			s[l] = 0;

			return s;
		}


		const char *ParseSymbol()
		{
			long i = 0, state = 0;
			char c;

			AllocateBuffer();

			while (i < mBufferSize && fscanf(mFileHandle, "%c", &c) != EOF)
			{
				switch (state)
				{
				case 0:
					if (c == '/')
					{
						state = 1;
						mBuffer[i++] = c;
						mBuffer[i] = 0;	
					}
					else if (c == ' ' || c == '\r' || c == '\n' || c == '\t')
					{
						if (i > 0)
							i = mBufferSize;
					}
					else
					{
						mBuffer[i++] = c;
						mBuffer[i] = 0;
					}
					break;

				case 1:
					if (c == '/')
					{
						state = 2;
						--i;
						mBuffer[i] = 0;
					}
					else
					{
						state = 0;
						mBuffer[i++] = c;
						mBuffer[i] = 0;
					}
					break;

				case 2:
					if (c == '\n')
					{
						/* Only wrap lines when given a only comment line(s) */
						if (i > 0)
							i = mBufferSize;
						else
							state = 0;
					}
					break;
				}
			}

			return mBuffer;
		}
		
	};


	class FileWriter : public File
	{
	public:
		FileWriter() : File() { }
		~FileWriter() { }
		
		bool Open(const char *filename) { return File::Open(filename, "wb"); }


		void WriteBuffer(unsigned int length, unsigned char *buffer)
		{
			fwrite(buffer, length, 1, mFileHandle);  // heh, yeah
		}


		void WriteString(unsigned int length, char *buffer)
		{
			fwrite(buffer, length, 1, mFileHandle);  // heh, yeah
		}


		void WriteInt16Array(long size, short int array[])
		{	
			for (int i = 0; i < size; ++i)
			{
				WriteInt16(array[i]);
			}
		}


		void WriteInt32Array(long size, int array[])
		{	
			for (int i = 0; i < size; ++i)
			{
				WriteInt32(array[i]);
			}
		}


		void WriteFloat32Array(long size, float array[])
		{	
			for (int i = 0; i < size; ++i)
			{
				WriteFloat32(array[i]);
			}
		}


		void WriteFloat32(float r)
		{	
			void *ptr = &r;
		#if HAVE_BIG_ENDIAN
			FIX_FLOAT(*ptr)
		#endif
			size_t sz = fwrite(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileWriter: ERROR failed to write 32bit float\n");
		}


		void WriteInt8(char c)
		{
			fwrite(&c, 1, 1, mFileHandle);
		}


		void WriteInt8U(unsigned char u)
		{
			fwrite(&u, 1, 1, mFileHandle);  // heh, yeah
		}


		void WriteInt16(short i)
		{
			void *ptr = &i;
		#ifdef HAVE_BIG_ENDIAN
			FIX_SHORT(*ptr)
		#endif
			size_t sz = fwrite(ptr, 2, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileWriter: ERROR failed to write 16bit int\n");
		}

			
		void WriteInt16U(unsigned short u)
		{
			void *ptr = &u;
		#ifdef HAVE_BIG_ENDIAN
			FIX_SHORT(*ptr)
		#endif
			size_t sz = fwrite(ptr, 2, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileWriter: ERROR failed to read 16bit uint\n");
		}


		void WriteInt32(int i)
		{
			void *ptr = &i;
		#ifdef HAVE_BIG_ENDIAN
			FIX_INT(*ptr)
		#endif
			size_t sz = fwrite(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileWriter: ERROR failed to read 32bit int\n");
		}


		void WriteInt32U(unsigned int u)
		{
			void *ptr = &u;
		#ifdef HAVE_BIG_ENDIAN
			FIX_INT(*ptr)
		#endif
			size_t sz = fwrite(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileWriter: ERROR failed to read 32bit uint\n");
		}


		void WriteLong(long l)
		{
			void *ptr = &l;
			size_t sz;

		#ifdef HAVE_BIG_ENDIAN
			FIX_INT(*ptr)
		#endif

			sz = fwrite(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileWriter: ERROR failed to read 32bit int\n");
		}


		void WriteLongU(unsigned long ul)
		{
			void *ptr = &ul;
		#ifdef HAVE_BIG_ENDIAN
			FIX_INT(*ptr)
		#endif
			size_t sz = fwrite(ptr, 4, 1, mFileHandle);

			if (sz < 1)
				printf("FreyjaFileWriter: ERROR failed to read 32bit uint\n");
		}

	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	SystemIO();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of SystemIO
	 ------------------------------------------------------*/

	virtual ~SystemIO();
	/*------------------------------------------------------
	 * Pre  : SystemIO object is allocated
	 * Post : Deconstructs an object of SystemIO
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static bool AssertMsg(bool expr, const char *format, ...)
	{
		if (expr)
			return false;

		va_list args;
		va_start(args, format);	
		vfprintf(stdout, format, args);
		printf("\n");
		va_end(args);

		// TODO: Add attachable helper objects here to handle assert events
		// useful for making GUI MsgBox asserts, etc
		
	
		// Soft breakpoint insertion
		Assert(expr);

		return true;
	}


	static void Assert(bool expr)
	{
		if (expr)
			return;

		// Soft breakpoint insertion
#if defined(__x86_64__) || defined(__x86_32__)
		asm(
			 "int $3 \n" 
		);
#elif defined(__powerpc__)
		/* Trap on any condition */
		asm(
			 "tw 31, r1, r1 \n"
		);
#else
#   warning "No soft breakpoint can be inserted for this machine language"
#endif
	}


	static void Basename(const char *filename, char *basename, long size)
	{
		long len_f, len, i;
 

		/* Check for null or blank input strings */
		if (!filename || !filename[0] || !basename)
		{
			return;
		}

		basename[0] = 0;

		len = len_f = strlen(filename);
	
		while (len_f-1)
		{
			--len_f;

			if (filename[len_f] == '/')
			{
				break;
			}
		}

		len = len - len_f;

		if (size > len)
		{
			for (i = 0; i < len; ++i)
			{
				basename[i] = filename[len_f+i];
				basename[i+1] = 0;
			}
		}
	}


	// NOTE: This defaults to overwrite, so check with DoesFileExist()
	//       first if you want to avoid that.
	static bool CopyFileToPath(const char *filename, const char *path)
	{
		FILE *src, *dest;
		char copy[1024];
		char buffer[1024];
		long size, sz = 1024;
		long len;

		src = fopen(filename, "rb");

		if (!src)
			return false;

		fseek(src, 0L, SEEK_END);
		size = ftell(src);
		fseek(src, 0L, SEEK_SET);

		Basename(filename, buffer, 1024);
		len = strlen(path) + 2 + strlen(buffer);
		snprintf(copy, len, "%s/%s", path, buffer);
		
		//printf("copy := '%s'\n", copy);
		dest = fopen(copy, "wb");

		if (!dest)
		{
			fclose(src);
			return false;
		}

		for (; size > sz; size -= sz)
		{
			fread(buffer, sz, 1, src);
			fwrite(buffer, sz, 1, dest);
		}

		if (size > 0)
		{
			fread(buffer, size, 1, src);
			fwrite(buffer, size, 1, dest);
		}

		fclose(src);
		fclose(dest);

		return true;
	}


	static void Log(const char *filename, const char *format, ...)
	{
		FILE *f = fopen(filename, "a");
		va_list args;
		char buffer[1024];
		unsigned int l;
		
		if (!f)
		{
			perror( filename );
			return;
		}

		va_start(args, format);

		vsnprintf(buffer, 1024, format, args);
	
		l = strlen(buffer);
  
		if (!l || !buffer[0])
			return;

		if (buffer[l-1] == '\n')
			buffer[l-1] = 0;

		fprintf(f, "%s\n", buffer);

		va_end(args);
	}


	static void MicroSleep(long usec)
	{
#ifdef WIN32
		Sleep(usec);
#else
		usleep(usec); 
#endif
	}


	static void BufferedPrint(char *string, unsigned int len, char *s, ...)
	/*------------------------------------------------------
	 * Pre  : <String> is allocated to <Len> characters
	 *        the rest is just like a printf() call
	 *
	 * Post : Generates the string and buffers it in <String>
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.12.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/
	{
		va_list args;

		if (s && s[0])
		{
			va_start(args, s);
			vsnprintf(string, len-1, s, args);	
			string[len-1] = 0;
			va_end(args);
		}	
	}

	static void Print(const char *format, ...)
	{
		va_list args;
		char buffer[1024];
		unsigned int l;
		
		va_start(args, format);

		vsnprintf(buffer, 1024, format, args);
	
		l = strlen(buffer);
  
		if (!l || !buffer[0])
			return;

		if (buffer[l-1] == '\n')
			buffer[l-1] = 0;

		fprintf(stdout, "%s\n", buffer);

		va_end(args);
	}


	virtual void PrintArgs(const char *format, va_list *args)
	{
		char buffer[1024];
		unsigned int l;

		/* Strip message of an trailing carrage return 
		 *  and print to stdout and the status bar */
		vsnprintf(buffer, 1024, format, *args);
	
		l = strlen(buffer);
  
		if (!l || !buffer[0])
			return;

		if (buffer[l-1] == '\n')
			buffer[l-1] = 0;

		fprintf(stdout, "%s\n", buffer);
	}


	static void PrintError(const char *format, ...)
	{
		va_list args;
		char buffer[1024];
		unsigned int l;
		
		va_start(args, format);

		vsnprintf(buffer, 1024, format, args);
	
		l = strlen(buffer);
  
		if (!l || !buffer[0])
			return;

		if (buffer[l-1] == '\n')
			buffer[l-1] = 0;

		fprintf(stderr, "%s\n", buffer);

		va_end(args);
	}


	virtual void PrintErrorArgs(const char *format, va_list *args)
	{
		char buffer[1024];
		unsigned int l;

		/* Strip message of an trailing carrage return 
		 *  and print to stdout and the status bar */
		vsnprintf(buffer, 1024, format, *args);
	
		l = strlen(buffer);
  
		if (!l || !buffer[0])
			return;

		if (buffer[l-1] == '\n')
			buffer[l-1] = 0;

		fprintf(stderr, "%s\n", buffer);
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};

} // namespace mstl

#endif // GUARD__FREYJA_MONGOOSE_DEBUG_H_
