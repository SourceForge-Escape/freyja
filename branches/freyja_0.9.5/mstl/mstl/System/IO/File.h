
#ifndef GUARD__MSTL_SYSTEMIO_FILE_H_
#define GUARD__MSTL_SYSTEMIO_FILE_H_

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
namespace SystemIO {

class File
{
public:

	typedef enum {

		BIG    = 1,
		LITTLE 
		
	} ByteOrder;


	////////////////////////////////////////////////////////////
	// Public Methods.
	////////////////////////////////////////////////////////////

	File();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~File();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Destructor.
	 *
	 ------------------------------------------------------*/

	ByteOrder GetByteOrder();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Get the actual host byte order.
	 *
	 ------------------------------------------------------*/

	void Close();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Close any file handles open.
	 *
	 ------------------------------------------------------*/

	void CloseDir();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Close any directory handles open.
	 *
	 ------------------------------------------------------*/

	const char *GetNextDirectoryListing();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets the next listing in an open dir handle.
	 *
	 ------------------------------------------------------*/

	virtual void Flush();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Flushes handle to disk.
	 *
	 ------------------------------------------------------*/

	virtual long GetOffset();
	virtual bool SetOffset(long offset);
	virtual void SetOffsetToEnd();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : File offset property.
	 *
	 ------------------------------------------------------*/

	virtual long GetSize();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the size of the file.
	 *
	 ------------------------------------------------------*/

	bool IsEndOfFile();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if offset is at end of file.
	 *
	 ------------------------------------------------------*/

	bool OpenDir(const char *dirname);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if directory was opened.
	 *
	 ------------------------------------------------------*/

	void SetBufferSize(const long size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set the reserve of the 'scratch buffer'.
	 *
	 ------------------------------------------------------*/

	void SetByteOrder(ByteOrder order);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set the byte order to use for disk I/O.
	 *
	 ------------------------------------------------------*/

	virtual bool Skip(long offset);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Move the file offset forward by <offset> bytes.
	 *
	 ------------------------------------------------------*/

	
	////////////////////////////////////////////////////////////
	// Public Static Methods.
	////////////////////////////////////////////////////////////

	static int CompareFilenameExtention(const char *filename, const char *ext);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : str*() like comparsion of <filename>'s extension and <ext>.
	 *
	 ------------------------------------------------------*/

	static bool CreateDir(const char *path);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if a new directory was created.
	 *
	 ------------------------------------------------------*/

	static bool DoesFileExist(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if file name is on disk.
	 *
	 ------------------------------------------------------*/

	static bool IsDirectory(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if <filename> is a directory.
	 *
	 ------------------------------------------------------*/

	static void GetCurrentWorkingDir(char *buffer, long size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the PWD of the program inside an allocated <buffer>.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Protected Methods.
	////////////////////////////////////////////////////////////

 protected:

	bool Open(const char *filename, const char *mode);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Basic fopen() wrapper.
	 *
	 ------------------------------------------------------*/

	ByteOrder mHostOrder;             /* Host machine byte order */

	FILE *mFileHandle;                /* File members */
	char *mBuffer;
	long mBufferSize;

	DIR *mDirectory;                  /* Directory members */
	char *mDirectoryName;
	char *mDirectoryListing;

 private:

	File(const File &f) { }
	/*------------------------------------------------------
	 * Pre  : You don't want do this, so I'm making sure you can't.
	 * Post : 
	 *
	 ------------------------------------------------------*/

	File &operator =(const File &f) { return *this; }
	/*------------------------------------------------------
	 * Pre  : You don't want do this, so I'm making sure you can't.
	 * Post : 
	 *
	 ------------------------------------------------------*/
};


////////////////////////////////////////////////////////////
// Public Inline Methods
////////////////////////////////////////////////////////////

inline
File::File() :
	mHostOrder(LITTLE),  // Host order ( writes/reads in LITTLE endian. )
	mFileHandle(NULL),
	mBuffer(NULL),
	mBufferSize(0),
	mDirectory(NULL),
	mDirectoryName(NULL),
	mDirectoryListing(NULL)
{
}


inline
File::~File()
{
	Close();
	CloseDir();

	if (mBuffer) delete [] mBuffer;

	if (mDirectoryName) delete [] mDirectoryName;

	if (mDirectoryListing) delete [] mDirectoryListing;
}


inline
File::ByteOrder File::GetByteOrder()
{
#ifdef __BIG_ENDIAN__
	return BIG;
#else
	return LITTLE;
#endif
}


inline
void File::Close()
{
	if (mFileHandle)
	{
		fclose(mFileHandle);
		mFileHandle = NULL;
	}
}


inline
void File::CloseDir()
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


inline
const char *File::GetNextDirectoryListing()
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


inline
void File::Flush()
{
	fflush(mFileHandle);
}


inline
long File::GetOffset()
{
	return ftell(mFileHandle);
}


inline
long File::GetSize()
{
	long whence = GetOffset();
	fseek(mFileHandle, 0, SEEK_END);
	long size = GetOffset();
	SetOffset(whence);

	return size;
}


inline
bool File::IsEndOfFile()
{
	return (feof(mFileHandle) != 0);
}


inline
bool File::OpenDir(const char *dirname)
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


inline
void File::SetBufferSize(const long size)
{
	if (size < 1) return;

	mBufferSize = size;
	if (mBuffer) delete [] mBuffer;
	mBuffer = new char[size];
}


inline
void File::SetByteOrder(ByteOrder order) 
{ 
	mHostOrder = order; 
}


inline
bool File::SetOffset(long offset)
{
	return (fseek(mFileHandle, offset, SEEK_SET) == 0);
}


inline
bool File::Skip(long offset)
{
	return (fseek(mFileHandle, GetOffset() + offset, SEEK_SET) == 0);
}


inline
void File::SetOffsetToEnd()
{
	fseek(mFileHandle, 0, SEEK_END);
}



////////////////////////////////////////////////////////////
// Public Inline Static Methods
////////////////////////////////////////////////////////////

inline
int File::CompareFilenameExtention(const char *filename, const char *ext)
{
	// This could greatly improve, but for now perserve the old freyja func

	/* Check for null or blank input strings */
	if (!filename || !filename[0] || !ext || !ext[0])
	{
		return -1;
	}

	unsigned int len_f = strlen(filename) - 1;
	unsigned int len_e = strlen(ext) - 1;

	while (len_f+1 && len_e+1)
	{
		//printf("extcheck: %c %c\n", filename[len_f], ext[len_e]);

		if (filename[len_f] == ext[len_e])
		{
			if (ext[len_e] == '.')
			{
				//printf("extcheck: match found\n");
				return 0;
			}
		}
		else if (filename[len_f] < ext[len_e])
		{
			return -1;
		}
		else if (filename[len_f] > ext[len_e])
		{
			return 1;
		}

		--len_f;
		--len_e;
	}
	
	return 1;
}


inline
bool File::CreateDir(const char *path)
{
#ifdef WIN32
	return ( _mkdir(path) > 0 );
#else
	return ( mkdir(path, S_IRWXU | S_IRWXG) > 0 );
#endif		
}


inline
bool File::DoesFileExist(const char *filename)
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


inline
bool File::IsDirectory(const char *filename)
{
	struct stat status;
	stat(filename, &status);
	return ((S_ISDIR(status.st_mode)));
}


inline
void File::GetCurrentWorkingDir(char *buf, long size)
{
#ifdef WIN32
#   if defined(_MSC_VER)
#      define getcwd _getcwd
#   endif
	getcwd(buf, size);
#else
	getcwd(buf, size);
#endif
}


////////////////////////////////////////////////////////////
// Protected Inline Methods
////////////////////////////////////////////////////////////

inline
bool File::Open(const char *filename, const char *mode)
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

} // namespace SystemIO
} // namespace mstl


#endif // GUARD__MSTL_SYSTEMIO_FILE_H_

