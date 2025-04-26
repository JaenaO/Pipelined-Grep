#include <iostream>
#include <thread>

#include "boundedBuffer.h"
#include "pipelineGrepStages.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc < 6)
  {
    cout << "Usage: pipegrep ⟨buffsize⟩ ⟨filesize⟩ ⟨uid⟩ ⟨gid⟩ ⟨string⟩" << endl;
    return 1;
  }

  int buffsize = atoi(argv[1]);
  int filesize = atoi(argv[2]);
  int uid = atoi(argv[3]);
  int gid = atoi(argv[4]);
  string str = argv[5];

  boundedBuffer buff1(buffsize); // getting file names from directory
  boundedBuffer buff2(buffsize);
  boundedBuffer buff3(buffsize);
  // boundedbuffer buff4

  thread t1(stage1, ref(buff1));
  thread t2(stage2, ref(buff1), ref(buff2), filesize, uid, gid);
  thread t3(stage3, ref(buff2), ref(buff3), str); // stage 3 is not implemented
  // thread t4
  // thread t5

  // Main acting as a consumer for stage 2
  string filename;
  while (true) // clean up buffer
  {
    filename = buff3.remove(); // future- buff4 and print&remove
    if (filename == "done")
    {
      break;
    }
  }

  t1.join();
  t2.join();

  cout << "Stage 1 and Stage 2 completed." << endl;
  return 0;
}
