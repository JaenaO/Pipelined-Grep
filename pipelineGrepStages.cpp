#include <iostream>
#include <fstream> // Include this header for std::ifstream
#include <string>

#include "dirent.h"
#include <sys/stat.h>
#include "boundedBuffer.h"
#include "pipelineGrepStages.h"

using namespace std;

// The thread in this stage recurses through the current directory and adds filenames to the first buffer
//(buff1 ).
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
/*
In this stage, the thread will read filenames from buff1 and filter out files according to the values
provided on the command-line for ⟨filesize⟩, ⟨uid⟩, and ⟨gid⟩ as described above. Those files not
filtered out are added to buff2
*/
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
      if (filesize != -1 && stats.st_size <= filesize)
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
// The thread in this stage reads each filename from buff2 and adds the lines in this file to buff3.
void stage3(boundedBuffer &buff2, boundedBuffer &buff3)
{
  string filename;
  ifstream file;

  while (true)
  {
    filename = buff2.remove(); // Remove a filename from buff2
    // cout << filename << endl;

    if (filename == "done")
    {
      buff3.add("done");
      break;
    }

    file.open(filename);
    string line;
    while (getline(file, line))
    {
      buff3.add(line); // Add each line to buff3
    }
    file.close();
  }
}
// In this stage, the thread reads the lines from buff3 and determines if any given one contains ⟨string⟩
// in it. If it does, it adds the line to buff4.
void stage4(boundedBuffer &buff3, boundedBuffer &buff4, string str)
{
  string line;
  while (true)
  {
    line = buff3.remove(); // Remove a line from buff3

    if (line == "done")
    {
      buff4.add("done");
      break;
    }
    else
    {
      // Check if the line contains the given substring
      if (line.find(str) != string::npos)
      {
        // cout << "Keyword " << str << " found in line: " << line << endl;
        buff4.add(line); // Add the line to buff4 if it contains the substring
      }
    }
  }
}
/*
In the final stage, the thread simply removes lines from buff4 and prints them to stdout. Also, you
need to figure out when to exit the program. How do you know when you got the last line? Hint:
you can use a “done” token (would not work if you had multiple threads in a stage).
*/
void stage5(boundedBuffer &buff4)
{
  string line;
  while (true)
  {
    line = buff4.remove(); // Remove a line from buff4
    cout << "" << line << endl;
    if (line == "done")
    {
      break;
    }
    //  Print the line or process it further
  }
}
