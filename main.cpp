#include <iostream>
#include <thread>

#include "boundedBuffer.h"
#include "pipelineGrepStages.h"
#include <thread>
#include <functional> // For std::ref

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
  boundedBuffer buff4(buffsize); //
  boundedBuffer buff5(buffsize); //
  // boundedbuffer buff4

  thread t1(stage1, ref(buff1));
  thread t2(stage2, ref(buff1), ref(buff2), filesize, uid, gid);
  thread t3(stage3, ref(buff2), ref(buff3));
  thread t4(stage4, ref(buff3), ref(buff4), str);
  thread t5(stage5, ref(buff4));

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();

  cout << "All Stages completed." << endl;
  return 0;
}
