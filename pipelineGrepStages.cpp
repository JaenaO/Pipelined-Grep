#include <iostream>
#include <fstream> // Include this header for std::ifstream

#include "dirent.h"
#include <sys/stat.h>
#include "boundedBuffer.h"
#include "pipelineGrepStages.h"

using namespace std;

// gets all files in the directory
void stage1(boundedBuffer &buff1)
{
  DIR *dir = opendir(".");
  struct dirent *entry;

  entry = readdir(dir);
  while (entry != NULL)
  {
    string filename = entry->d_name;
    buff1.add(filename);
    entry = readdir(dir);
  }
  closedir(dir);
  buff1.add("done");
}

// filters the files based on filesize, uid, and gid
void stage2(boundedBuffer &buff1, boundedBuffer &buff2, int filesize, int uid, int gid)
{
  string filename;
  struct stat stats;

  while (true)
  {
    filename = buff1.remove(); // Remove a filename from buff1

    if (filename == "done")
    {
      buff2.add("done");
      break;
    }

    bool addFile = true;
    if (stat(filename.c_str(), &stats) == 0)
    {
      if (filesize != -1 && stats.st_size > filesize)
      {
        addFile = false;
      }
      if (uid != -1 && stats.st_uid != uid)
      {
        addFile = false;
      }
      if (gid != -1 && stats.st_gid != gid)
      {
        addFile = false;
      }
    }
    if (addFile)
    {
      buff2.add(filename);
    }
  }
}

// pipes all the lines of the files to the next stage
void stage3(boundedBuffer &buff2, boundedBuffer &buff3, string str)
{
  string filename;
  ifstream file;

  while (true)
  {
    filename = buff2.remove(); // Remove a filename from buff2
    cout << filename << endl;

    if (filename == "done")
    {
      buff3.add("done");
      break;
    }

    file.open(filename);
    string line;
    while (getline(file, line))
    {
      if (line.find(str) != string::npos)
      {
        buff3.add(line);
        cout << "Added line to buff3: " << line << endl;
      }
    }
    file.close();
  }
}