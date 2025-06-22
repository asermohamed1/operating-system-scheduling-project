#ifndef MemoryTree_h
#define MemoryTree_h


#include "Structs.h"
#include <stdlib.h>
#include <stdbool.h>
#include "../clk.h"
#include <stdio.h>


Tree* createTree();
void MakeMemoryTree(MemoryNode** root,int size,int start,int end);
MemoryNode* createNode(int id, int size, int start, int end);
void SearchnDelete(MemoryNode* root,int id,bool* isFreed);
bool insertProcess(MemoryNode* root,int id,int size);
void logAllocatedMemory(int start,int end,int id);
void logFreedMemory(int start,int end,int id);
void MakeMemoryLog();

#endif
