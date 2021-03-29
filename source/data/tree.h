#ifndef TREE_H
#define TREE_H

typedef struct TreeNode TreeNode;

TreeNode* treeCreate(void* content);
void treeDestroy(TreeNode* t, char free_content);
void* treeGetNodeContent(TreeNode* t);

TreeNode* treeGetChildNode(TreeNode* t, int index);
int treeGetChildNodesCount(TreeNode* t);
int treeAddChildNode(TreeNode* parent, TreeNode* child);
void treeRemoveChildNode(TreeNode* parent, TreeNode* child, char free_memory);

#endif