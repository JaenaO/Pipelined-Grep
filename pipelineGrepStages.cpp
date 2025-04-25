#include <iostream>

#include "dirent.h"
#include <sys/stat.h>
#include "boundedBuffer.h"
#include "pipelineGrepStages.h"

using namespace std;

// gets all files in the directory
void stage1(boundedBuffer &buff1){
  DIR *dir = opendir(".");
  struct dirent *entry;
  
  entry = readdir(dir);
  while(entry != NULL){
    string filename = entry->d_name;
    buff1.add(filename);
    cout << "Added file to buff1: " << filename << endl;
    entry = readdir(dir);
  }
  closedir(dir);
  buff1.add("done"); 
}

// filters the files based on filesize, uid, and gid
void stage2(boundedBuffer &buff1, boundedBuffer &buff2, int filesize, int uid, int gid){
  string filename;
  struct stat stats;

  while (true) {
    filename = buff1.remove(); // Remove a filename from buff1
    cout << "Removed file from buff1: " << filename << endl;

    if (filename == "done"){
      buff2.add("done");
      break;
    }

    bool addFile = true;
    if (stat(filename.c_str(), &stats) == 0){
      if (filesize != -1 && stats.st_size > filesize){
        addFile = false;
      }
      if (uid != -1 && stats.st_uid != uid){
        addFile = false;
      }
      if (gid != -1 && stats.st_gid != gid){
        addFile = false;
      }
    } 
    if (addFile) {
      buff2.add(filename);
      cout << "Added file to buff2: " << filename << endl;
    }
  }
}