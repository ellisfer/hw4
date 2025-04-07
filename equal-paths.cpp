#ifndef RECCHECK
#include <iostream>
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
    // Add your code below

    // if root null - it's a leaf, balanced
    if(!root){
        return true;
    }
    int height = leftestheight(root, 0);
    return equalPathsrecursion(root, 0, height);

}

int leftestheight(Node* root, int depth) {
    if(!root){
        return depth;
    }
    if(root->left){
        return leftestheight(root->left, depth+1);
    }
    else if(root->right){
        return leftestheight(root->right, depth+1);
    }
    else{
        return depth;
    }
}

bool equalPathsrecursion(Node* root, int depth, int goalHeight){
    if(!root){
        return false;
    }

    // leaves
    if(!(root->left) && !(root->right)){
        if(depth == goalHeight){
            return true;
        }
        else{
            return false;
        }
    }

    if(root->left){
        if(!equalPathsrecursion(root->left, depth + 1, goalHeight)){
            return false;
        }
    }
    if(root->right){
        if(!equalPathsrecursion(root->right, depth + 1, goalHeight)){
            return false;
        }
    }
    return true;
}

