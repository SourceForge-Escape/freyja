#include <stdio.h>
#include <string.h>
#include <stdlib.h>



char *posix_filename(char *old_filename)
{
  char c;
  int i = 0, j = 0, length;
  char *filename;
  
  
  if (!old_filename || !old_filename[0])
    return NULL;
  
  length = strlen(old_filename);
  
  filename = new char[length];
  
  for (i = 0; i < length; i++)
  {
    c = old_filename[i];
    
    // Trim off unwanted leading chars, which are otherwise valid
    if (j == 0)
    {
      switch (c)
      {
      case '-':
	//case '.':
	i++;
	break;
      }
      
      /* Remove special characters and try to make name follow:
       * Word-Word_Word.ext style
       */
      switch (c)
      {
      case '=':  case ',':  case '|':
      case '*':  case '/':  case '\\':
      case '$':  case '@':  case '!':
      case '^':  case '+':  case '~':
      case '`':  case ';':  case '"':
      case '(':  case '?':  case ':':
      case '&':  case '[':  case ']':
      case ')':  case '{':  case '}':
	break;
      case ' ':
	// Replace blanks

	filename[j++] = '_'; 
	break;
      case '-':
	// Condense [-]+ patterns
	
	if (j > 0 && (filename[j - 1] == '_' || filename[j - 1] == '-'))
	  filename[j - 1] = '-';
	else
	  filename[j++] = '-';
	break;
      case '%':
	// Try some common uuencode repacements here
	
	if (i + 1 < length && old_filename[i + 1] == '2')
	{
	  i++;
	  
	  if (i + 1 < length && old_filename[i + 1] == '0')
	  {
	    i++;
	    
	    if (j > 0 && filename[j -1] != '_' && filename[j - 1] != '-') 
	      filename[j++] = '_';
	  }
	}
	
	break;
      default:
	filename[j++] = c;
      }
    }

    filename[j] = 0;
  }
 
  return filename;
}


#ifndef strdup
char *strdup(const char *src)
{
   char *dest = NULL;
   int len;


   if (!src || !src[0])
      return NULL;

   len = strlen(src);
   dest = new char[len + 1];
   strncpy(dest, src, len);
   dest[len] = 0;

   return dest;
}
#endif


int extcheck(const char *filename, const char *ext)
{
  unsigned int len_f, len_e;
 
 
  len_f = strlen(filename) - 1;
  len_e = strlen(ext) - 1;

  while (len_f+1 && len_e+1)
  {
    if (filename[len_f] == ext[len_e])
    {
      if (ext[len_e] == '.')
      {
	return 0;
      }
    }
    else if (filename[len_f] < ext[len_e])
      return -1;
    else if (filename[len_f] > ext[len_e])
      return 1;

    len_f--;
    len_e--;
  }

  return 1;
}
