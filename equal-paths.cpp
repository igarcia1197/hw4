#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
bool checkPaths(Node* node, int curDepth, int &leafDepth) {
    // if node is a leaf check its depth
    if (!node->left && !node->right) {
        if (leafDepth == -1) { // first leaf set its depth
            leafDepth = curDepth;
            return true;
        }
        return curDepth == leafDepth;
    }
    
    bool leftOk = true;
    bool rightOk = true;
    if (node->left)
        leftOk = checkPaths(node->left, curDepth + 1, leafDepth);
    if (node->right)
        rightOk = checkPaths(node->right, curDepth + 1, leafDepth);
    
    return leftOk && rightOk;
}

bool equalPaths(Node * root)
{

  if (root == nullptr)
        return true;
    
    int leafDepth = -1; // start with -1 to mark no leaf yet
    return checkPaths(root, 0, leafDepth);  

}

