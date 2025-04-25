#ifndef PIPELINE_GREP_STAGES_H
#define PIPELINE_GREP_STAGES_H

#include "boundedBuffer.h"

using namespace std;


void stage1(boundedBuffer &buff1);
void stage2(boundedBuffer &buff1, boundedBuffer &buff2, int filesize, int uid, int gid);

#endif // PIPELINE_GREP_STAGES_H