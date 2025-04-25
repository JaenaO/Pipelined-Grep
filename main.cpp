#include <iostream>
#include <thread>

#include "boundedBuffer.h"
#include "pipelineGrepStages.h"

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

  boundedBuffer buff1(buffsize);
  boundedBuffer buff2(buffsize);
  
  thread t1(stage1, ref(buff1));
  thread t2(stage2, ref(buff1), ref(buff2), filesize, uid, gid);

  // Main acting as a consumer for stage 2
  string filename;
  while (true) {
    filename = buff2.remove();
    if (filename == "done") {
      break;
    }
  }

  t1.join();
  t2.join();

  cout << "Stage 1 and Stage 2 completed." << endl;
  return 0;
}
