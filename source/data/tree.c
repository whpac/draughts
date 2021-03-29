#include<malloc.h>

#include "tree.h"
#include "list.h"

struct TreeNode {
    void* content;
    List* children;
};

/**
 * Creates a tree node with no children.
 * @param content The root node content
 */
TreeNode* treeCreate(void* content){
    TreeNode* node = malloc(sizeof(TreeNode));
    node->content = content;
    node->children = listCreate();
    return node;
}

/**
 * Destroys a tree node and all its children and frees the memory occupied by them.
 * @param t The tree node
 * @param free_content Whether to free the memory occupied by tree contents
 */
void treeDestroy(TreeNode* t, char free_content){
    if(t == NULL) return;

    while(listGetLength(t->children) > 0){
        TreeNode* child = listGet(t->children, 0);
        treeDestroy(child, free_content);
        listRemove(t->children, 0, 0);
    }

    listDestroy(t->children, 0);

    if(free_content) free(t->content);
    free(t);
}

/**
 * Returns the content of the given node.
 * @param t The tree node
 */
void* treeGetNodeContent(TreeNode* t){
    if(t == NULL) return NULL;
    return t->content;
}

/**
 * Returns a child node stored at the specified index.
 * @param t The tree node
 * @param index The index of the desired child
 */
TreeNode* treeGetChildNode(TreeNode* t, int index){
    if(t == NULL) return NULL;

    TreeNode* child = listGet(t->children, index);
    return child;
}

/**
 * Counts the children nodes of a given node.
 * @param t The tree node
 */
int treeGetChildNodesCount(TreeNode* t){
    if(t == NULL) return 0;

    int count = listGetLength(t->children);
    return count;
}

/**
 * Adds en existing tree node as a child to another node. 
 * Returns the index of a newly-inserted node.
 * @param parent The parent node
 * @param child The child node
 */
int treeAddChildNode(TreeNode* parent, TreeNode* child){
    if(parent == NULL) return -1;
    if(child == NULL) return -1;

    return listAdd(parent->children, child);
}

/**
 * Removes the passed child node from its parent.
 * @param parent The parent node
 * @param child The node to remove from its parent
 * @param free_memory Whether to free memory occupied by the removed child
 */
void treeRemoveChildNode(TreeNode* parent, TreeNode* child, char free_memory){
    if(parent == NULL) return;
    if(child == NULL) return;

    for(int i = 0; i < listGetLength(parent->children); i++){
        if(listGet(parent->children, i) != child) continue;
        listRemove(parent->children, i, 0);

        if(free_memory) treeDestroy(child, 1);
        break;
    }
}