#include <iostream>
#include <thread>

#include "monitor.h"

using namespace std;

int main(int argc, char *argv[]){
  if (argc < 6){
    cout << "Usage: pipegrep ⟨buffsize⟩ ⟨filesize⟩ ⟨uid⟩ ⟨gid⟩ ⟨string⟩" << endl;
    return 1;
  }

  int buffsize = atoi(argv[1]);
  int filesize = atoi(argv[2]);
  int uid = atoi(argv[3]);
  int gid = atoi(argv[4]);
  string str = argv[5];

  return 0;

}
