#include "Tree.h"


Tree* createTree(int size) {
    Tree* tree = (Tree*)malloc(sizeof(Tree));
    tree->root = NULL;
    return tree;
}

void MakeMemoryTree(MemoryNode** root,int size,int start,int end)
{
    if (size == 0) return;
    *root = createNode(-1, size, start, end);
    MakeMemoryTree(&(*root)->left, size/2, start, start + size/2 - 1);
    MakeMemoryTree(&(*root)->right, size/2, start + size/2, end);
}


MemoryNode* createNode(int id, int size, int start, int end) {
    MemoryNode* node = (MemoryNode*)malloc(sizeof(MemoryNode));
    node->id = id;
    node->size = size;
    node->start = start;
    node->end = end;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void SearchnDelete(MemoryNode* root,int id,bool* isFreed)
{
    if (root == NULL) return;
    SearchnDelete(root->left, id,isFreed);
    SearchnDelete(root->right, id,isFreed);
    if (root->id == id)
    {
        if (!*isFreed) logFreedMemory(root->start,root->end,id);
        *isFreed = true;
        root->id = -1;
    }
}

bool insertProcess(MemoryNode* root,int id,int size) {
    if (!root) return false;
    if (size > root->size) return false;
    if (root->id != -1 && root->left && root->left->id == -1 && root->right && root->right->id == -1) 
        return false;
    if (root->id != -1 && !root->left  && !root->right) return false;
    if (size == root->size && root->id == -1) {
        root->id = id;
        logAllocatedMemory(root->start,root->end,id);
        return true;
    }
    else if (insertProcess(root->left, id, size)) {
        root->id = id;
        return true;
    } 
    else if (insertProcess(root->right, id, size)) {
        root->id = id;
        return true;
    }   
    return false;
}

void logAllocatedMemory(int start,int end,int id)
{
    FILE *file = fopen("memory.log", "a+");  
    if (file != NULL) {
        fprintf(file, "At time %d allocated %d bytes for process %d from %d to %d\n",get_clk(),end - start + 1,id,start,end);  
        fclose(file);  
    }
}

void logFreedMemory(int start,int end,int id)
{   
    FILE *file = fopen("memory.log", "a+");  
    if (file != NULL) {
        fprintf(file, "At time %d freed %d bytes for process %d from %d to %d\n",get_clk(),end - start + 1,id,start,end);  
        fclose(file);  
    }   
}