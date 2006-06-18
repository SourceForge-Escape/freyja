//
// File: GrannyStream.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//


#include "GrannyStream.h"

#include <iostream>
#define Uint8 unsigned char
using namespace std;

cGrannyStream::cGrannyStream (std::fstream * file, std::string filename)
{
//   assert(file);
   file->seekg(0, ios::end);
   m_size = file->tellg ();
   if (m_size) {
      m_data = (char *) malloc (m_size);
//      assert(m_data);
      file->seekg(0, ios::beg);
      file->read(m_data, m_size);
   } else 
      m_data = NULL;
   m_pos = 0;
   m_filename = filename;
}


cGrannyStream::~cGrannyStream ()
{
    if (m_data)
        free(m_data);
    m_data = NULL;
}

bool cGrannyStream::seekg (Uint32 pos)
{
   if (pos < m_size) {
     m_pos = pos;
     return true;
   } else {
     cout << "GrannyStream::seekg: file corrupt? (" + m_filename + ")";
     return false;
   }
}

Uint32 cGrannyStream::tellg ()
{
   return m_pos;
}

bool cGrannyStream::read (char * data, Uint32 count)
{
//   assert (data);
   if (m_pos + count <= m_size) {
     memcpy(data, m_data + m_pos, count);
     m_pos += count;
     return true;
   } else {
     memset(data, 0, count);
     cout << "GrannyStream::read: file corrupt? (" + m_filename + ")";
     return false;
   }
}


Uint32 cGrannyStream::readDword ()
{
   
   if (m_pos + 4 <= m_size) {
     Uint8 * p = (Uint8 *) m_data + m_pos;
     m_pos += 4;
     return (*p | *(p+1) << 8 | *(p+2) << 16 | *(p+3) << 24);
     
   } 
   cout << "GrannyStream::readDword: file corrupt? (" + m_filename + ")";
   return 0;
}

void cGrannyStream::get(char & value)
{
   if (m_pos < m_size) {
     value = *(m_data + m_pos);
     m_pos ++;
   } else {
     cout << "GrannyStream::get: file corrupt? (" + m_filename + ")";
     value = 0;
   }
}

