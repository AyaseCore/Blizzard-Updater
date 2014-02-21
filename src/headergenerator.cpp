#include <fstream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <Fi

#include "file.hpp"

unsigned long crc32_table[256];
unsigned long ulPolynomial = 0x04c11db7;

struct UpdateFileHeader
{
  unsigned short HdrSize; // always 0x0018
  unsigned char version; // always 0x04
  unsigned char override; // 0x01 => override with new file; 0x04 => update file (bsdiff)
  unsigned int crc32; // of the file before update
  unsigned int OldFileSize; // file size before the update (uncompressed)
  unsigned int NewFileSize; // size of the entire file after the update is applied (uncompressed)
  unsigned int unknown6; // always 0
  unsigned int unknown7; // always 0
};

unsigned long Reflect(unsigned long ref, char ch)
{                                 // Used only by Init_CRC32_Table()

  unsigned long value(0);

  // Swap bit 0 for bit 7
  // bit 1 for bit 6, etc.
  for(int i = 1; i < (ch + 1); i++)
  {
    if(ref & 1)
      value |= 1 << (ch - i);
    ref >>= 1;
  }
  return value;
}

void InitCrcTable()
{

  // 256 values representing ASCII character codes.
  for(int i = 0; i <= 0xFF; i++)
  {
    crc32_table[i]=Reflect(i, 8) << 24;
    for (int j = 0; j < 8; j++)
      crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
    crc32_table[i] = Reflect(crc32_table[i], 32);
  }

}

int Get_CRC(unsigned char* buffer, unsigned long bufsize)
{

  unsigned long  crc(0xffffffff);
  int len;
  len = bufsize;
  // Save the text in the buffer.

  // Perform the algorithm on each character
  // in the string, using the lookup table values.

  for(int i = 0; i < len; i++)
    crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ buffer[i]];


  // Exclusive OR the result with the beginning value.
  return crc^0xffffffff;

}

long FileSize(FILE *input)
{

  long fileSizeBytes;
  fseek(input, 0, SEEK_END);
  fileSizeBytes = ftell(input);
  fseek(input, 0, SEEK_SET);

  return fileSizeBytes;

}


unsigned int getCRCFromFile(string path)
{ 

  FILE *fs = fopen(path.c_str(), "rb");   //open file for reading

  unsigned int crc;
  long bufsize = FileSize(fs), result;
  unsigned char *buffer = new unsigned char[bufsize];

  if(buffer == NULL)
  {
    printf("\nError out of memory\n");
    return 0;

  }
  
  // copy the file into the buffer:
  result = fread (buffer,1,bufsize,fs);
  fclose(fs);

  if(result != bufsize)
  {
    printf("\nError reading file %s\n", path.c_str());
    return 0;
  }


  InitCrcTable();
  crc = Get_CRC(buffer, bufsize);
  delete [] buffer;

  return crc;
}

int main(int argc, char **argv)
{
  std::tr2::sys

  /*
  for (int i = 0; i != sfa.count(); ++i)
  {
    unsigned int crc = getCRCFromFile(sfa[i].c_str());

    string path(sfa[i].c_str());
    string filename;

    size_t pos = path.find_last_of("\\");
    if(pos != string::npos)
      filename.assign(path.begin() + pos + 1, path.end());
    else
      filename = path;

    ofstream OutFile;

    UpdateFileHeader test;
    test.HdrSize = 0x0018;
    test.NewFileSize = 0;
    test.OldFileSize = 0;
    test.override = 0x01;
    test.crc32 = crc;
    test.unknown6 = 0;
    test.unknown7 = 0;
    test.version = 0x04;

    ifstream file(sfa[i].c_str(), ios::in | ios::binary | ios::ate);
    ifstream::pos_type fileSize;
    char* fileContents;
    if(file.is_open())
    {
      fileSize = file.tellg();
      test.NewFileSize = fileSize;
      test.OldFileSize = fileSize;
      fileContents = new char[fileSize];
      file.seekg(0, ios::beg);
      if(!file.read(fileContents, fileSize))
      {
        printf("Failed to read.\n");
      }
      file.close();
      OutFile.open(sfa[i].c_str(), ios::binary);

      OutFile.write(reinterpret_cast<const char*>(&test), sizeof(UpdateFileHeader));

      //const char* output = reinterpret_cast<const char*>(data);

      //OutFile.write(output, 24);
      OutFile.write(fileContents, fileSize);

      delete[] fileContents;
    }

    OutFile.close();
  }
  */
  return 0;
}
