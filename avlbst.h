#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{
}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
  //std::cout << " setting balance " << this->getKey() << " " << (int) balance << std::endl;
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
  //std::cout << " updating balance " << this->getKey() << " " << (int) diff << std::endl;
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    //void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* current);
    AVLNode<Key, Value>* rotateLeft(AVLNode<Key, Value>* curr);
    AVLNode<Key, Value>* rotateRight(AVLNode<Key, Value>* curr);
    int getHeight(AVLNode<Key, Value>* root);
    int setTrueBalance(AVLNode<Key, Value>* root);
    //void removeFix(AVLNode<Key, Value>* n, int diff);
    void fixBalance(AVLNode<Key, Value>* curr);


};
// getHeight
template<class Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key, Value>* root){
  if(root == nullptr) return 0;
  return 1+std::max(getHeight(root->getLeft()), getHeight(root->getRight()));
}

template<class Key, class Value>
int AVLTree<Key, Value>::setTrueBalance(AVLNode<Key, Value>* root){
  if(root == nullptr) return 0;
  int balance = getHeight(root->getRight()) - getHeight(root->getLeft());
  root->setBalance(balance);
  return balance;
}
/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{

    // TODO
    AVLNode<Key, Value>* toInsert = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
    if(!this->root_ ){
      this->root_ = toInsert;
      return;
  }

    AVLNode<Key, Value> *curr = (static_cast<AVLNode<Key, Value>*>(this->root_));
    AVLNode<Key, Value> *parent = nullptr;
    // finding the position
    while(curr != nullptr){
      parent = curr;
      
      if(new_item.first < curr->getKey()){
        curr = static_cast<AVLNode<Key, Value>*>(curr->getLeft());

      }
      else if(new_item.first > curr->getKey()){
        curr = static_cast<AVLNode<Key, Value>*>(curr->getRight());
      }
      else{
        curr->setValue(new_item.second);
        delete toInsert;
        return;
      }
    }
    toInsert->setParent(parent);
    
    if(toInsert->getKey()<parent->getKey()) 
      {parent->setLeft(toInsert);}
    else parent->setRight(toInsert);

    // update balances
    int pBalance = parent->getBalance();
    AVLNode<Key, Value>* bCheck= parent;
    
    while(bCheck){
      setTrueBalance(bCheck);
      fixBalance(bCheck);
      if(bCheck->getBalance() == 0) break;
      bCheck = bCheck->getParent();
    }

}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* curr){
    if (curr == nullptr || curr->getLeft() == nullptr) {
    return curr;}
  bool onLeft = false;
  AVLNode<Key, Value>* newCurr = curr->getLeft();
  AVLNode<Key, Value>* parent = curr->getParent();
  if(parent && parent->getLeft() == curr){
    onLeft = true;
    //std::cout << "on left " << std::endl;
  }
  curr->setLeft(newCurr->getRight());
  if(newCurr->getRight()) newCurr->getRight()->setParent(curr);
  newCurr->setRight(curr);
  curr->setParent(newCurr);
 // std::cout << "set the things " << std::endl;
  if(onLeft){
    parent->setLeft(newCurr);
  } else if(parent){
    parent->setRight(newCurr);
  } else{
    this->root_ = newCurr;
  }
  newCurr->setParent(parent);
  if (this->root_ == newCurr) {
    newCurr->setParent(nullptr);  // ensure root has no parent
}
  setTrueBalance(newCurr);
  setTrueBalance(curr);


  return newCurr;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* curr){
  if (curr == nullptr || curr->getRight() == nullptr) {
    return curr;}

  bool onLeft = false;
  AVLNode<Key, Value>* newCurr = curr->getRight();
  AVLNode<Key, Value>* parent = curr->getParent();
  if(parent && parent->getLeft() == curr){
    onLeft = true;
  }
  curr->setRight(newCurr->getLeft());
  if(newCurr->getLeft()) newCurr->getLeft()->setParent(curr);
  newCurr->setLeft(curr);
  curr->setParent(newCurr);
  //std::cout << " set the things " << std::endl;
  if(onLeft){
    parent->setLeft(newCurr);
  } else if(parent){
    parent->setRight(newCurr);
  } else{
    this->root_ = newCurr;
  }
  newCurr->setParent(parent);
  if (this->root_ == newCurr) {
    newCurr->setParent(nullptr);  // ensure root has no parent
}

  setTrueBalance(curr);
  setTrueBalance(newCurr);
  return newCurr;
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO

    // height:   bCheck->setBalance(getHeight(bCheck->getRight()) - getHeight(bCheck->getLeft()));
    // pt 1: remove by walking the tree.
      if(!this->root_ ){
            return;
      }

    int diff;
    bool onLeft = false;

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    AVLNode<Key, Value>* child = nullptr;
    AVLNode<Key, Value>* parent = nullptr;

    if(curr == nullptr){
      return;
    }
    // pt2 if n has two children, swap with predecessor
    if(curr->getRight() && curr->getLeft()){
      AVLNode<Key, Value> *pred = (static_cast<AVLNode<Key, Value>*>(this->predecessor(curr)));
      nodeSwap(curr, pred);
    }
    
    //std::cout << " just swapped " << std::endl; 
    if(curr->getRight() || curr->getLeft()){
      //std::cout << "one kid mcgee " << std::endl;
      child = (curr->getLeft()) ? 
      static_cast<AVLNode<Key, Value>*>(curr->getLeft()) : 
      static_cast<AVLNode<Key, Value>*>(curr->getRight());
      //std::cout << child->getKey() << std::endl;
    }

        // p = parent
    parent = curr->getParent();


    // if p update balance, if n is left, diff += 1, if right, -=1 -- 
    if(parent){
      if(parent->getLeft() == curr){
        diff = 1; 
        onLeft = true;
        }
      else if(parent->getRight() == curr) diff = -1;  }


        // Update parent pointers
    if (parent == nullptr) {
        // Removing the root
          this->root_ = child;
    } else {
        // Connect parent to child
        if (onLeft) {
            parent->setLeft(child);
            parent->updateBalance(-1);

        } else {
            parent->setRight(child);
            parent->updateBalance(1);

        }
    }
    if(child) child->setParent(parent);
    

    //removeFix(parent, diff); 
      while(parent){

        setTrueBalance(parent);

        if(parent->getBalance() > 1 || parent->getBalance() < -1){
          fixBalance(parent);
        }

        parent = parent->getParent();
      }

    

    //patch with removeFix (p, diff)

}
template<class Key, class Value>
void AVLTree<Key, Value>::fixBalance(AVLNode<Key, Value>* curr){
  if(!curr) return;
  if(curr->getBalance() > 1){
    if(curr->getRight()->getBalance() < 0){
      rotateRight(curr->getRight());
    }
    rotateLeft(curr);
  }
  else if (curr->getBalance() < -1){
    if(curr->getLeft()->getBalance() > 0){
      rotateLeft(curr->getLeft());
    }
    rotateRight(curr);
  
}}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

// template<class Key, class Value>
// void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff){
//   AVLNode<Key, Value>* parent;
//   AVLNode<Key, Value>* left;
//   AVLNode<Key, Value>* right;
//   int ndiff = 0;
//   int balance;
//   bool onleft;
//   int lbalance = 0;

//   if(n == nullptr) return;
//   parent = n->getParent();
//   if(parent){
//     onleft = parent->getLeft() == n ? true : false;
//     ndiff = onleft ? 1 : -1;
//   }
//   balance = n->getBalance();

//   if(diff == -1){
//     if(balance + diff == -2){
//       left = n->getLeft();
//       if(left) lbalance = left->getBalance();
//       if(lbalance == 0){
//         rotateRight(n);
//         n->setBalance(-1);
//         left->setBalance(1);
//       }
//       else if((int)left->getBalance() == -1){
//         rotateRight(n);
//         n->setBalance(0);
//         left->setBalance(0);
//         removeFix(parent, ndiff);
//       }
//       else if((int)left->getBalance() == 1){
//         right = left->getRight();
//         rotateLeft(left);
//         rotateRight(n);
//         if((int)right->getBalance() == 1){
//           n->setBalance(0);
//           left->setBalance(-1);
//           right->setBalance(0);
//         }
//         else if((int)right->getBalance() == 0){
//             n->setBalance(0);
//             right->setBalance(0);
//             left->setBalance(0);
//         }
//         else if((int)right->getBalance() == -1){
//           n->setBalance(1);
//           left->setBalance(0);
//           right->setBalance(0);
//         }
//         removeFix(parent, ndiff);
//       }
//     }
//     else if(balance + diff == -1) n->setBalance(-1);
//     else if(balance + diff == 0){
//         n->setBalance(0);
//         removeFix(parent, ndiff);}
//     }

//   else if(diff == 1){
//     if(balance + diff == 2){
//       right = n->getRight(); 
//       if((int)right->getBalance() == 1){
//         rotateLeft(n);
//         n->setBalance(0);
//         right->setBalance(0);
//         removeFix(parent, ndiff);
//       }
//       else if((int)right->getBalance() == 0){
//         rotateLeft(n);
//         n->setBalance(1);
//         right->setBalance(-1);
//       }
//       else if((int)right->getBalance() == -1){
//         left = right->getLeft();
//         rotateRight(right);
//         rotateLeft(n);
//         if((int)left->getBalance() == -1){
//           n->setBalance(0);
//           right->setBalance(1);
//           left->setBalance(0);
//         }
//         else if((int)left->getBalance() == 0){
//             n->setBalance(0);
//             right->setBalance(0);
//             left->setBalance(0);
//         }
//         else if((int)left->getBalance() == 1){
//           n->setBalance(-1);
//           left->setBalance(0);
//           right->setBalance(0);
//         }
//         removeFix(parent, ndiff);
//       }
//     }
//     else if(balance + diff == 1) n->setBalance(1);
//     else if(balance + diff == 0){
//         n->setBalance(0);
//         removeFix(parent, ndiff);}
//     }
// }






// template<class Key, class Value>
// void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff){
//   //if(!n) std::cout << "!N " <<std::endl;
//   if(!n){return;}
//   //std::cout << "remove fix " << n->getKey() << " " << diff << std::endl;

//   // setting next calls
//   AVLNode<Key, Value>* p = n->getParent();
//   AVLNode<Key, Value>* left;
//   AVLNode<Key, Value>* right;
//   int ndiff = 0;
//   int balance = n->getBalance();
//   if(!p) {
//     //std::cout << "!P " <<std::endl; 
//   }
//   else{
//     //std::cout << "parent " << p->getKey()<<std::endl;;

//     if(p->getLeft() == n) ndiff = 1;
//     else ndiff = -1;
//   }
//   //std::cout << "diff " << diff << " balance " << balance << std::endl;
//   // diff = -1
//   if(diff == -1){
//     left = n->getLeft();
//     if(balance + diff == -2){
//       // case 1: left case			
//       if(balance == -1){

//         rotateRight(n);


//         n->setBalance(0);

//         left->setBalance(0);

//         removeFix(p, ndiff);
//       }

//       if(balance == 0){
//        // std::cout << "case 1 " << std::endl;
//         rotateRight(n);
//         n->setBalance(-1);
//         left->setBalance(1);

//       }
//       if(balance ==  1){
//        // std::cout << "case 2 " << std::endl;
//         right = left->getRight();
//         rotateLeft(left); // zig zag
//         rotateRight(n);
//         if(right->getBalance()==1){
//          // std::cout << "case 2.0 " << std::endl;
//           n->setBalance(0);
//           left->setBalance(-1);
//           right->setBalance(0);
//           // setTrueBalance(n);
//           // setTrueBalance(left);
//           // setTrueBalance(right);
//         }
//         if(right->getBalance() == 0){
//           //std::cout << "case 2.1 " << std::endl;
//           n->setBalance(0);
//           right->setBalance(0);
//           left->setBalance(0);
//           // setTrueBalance(n);
//           // setTrueBalance(left);
//           // setTrueBalance(right);
//         }
//         if(right->getBalance()==-1){
//           //std::cout << "case 2.3 " << std::endl;
//           n->setBalance(1);
//           left->setBalance(0);
//           right->setBalance(0);
//           // setTrueBalance(n);
//           // setTrueBalance(left);
//           // setTrueBalance(right);
//         }
//         removeFix(p, ndiff);
//       }
//     }

//     if(balance + diff == -1){
//       //std::cout << "case 3 " << std::endl;
//       n->setBalance(-1);
//       //setTrueBalance(n);
//       return;
//     }
//     if(balance + diff == 0) {
//       //std::cout << "case 4 " << std::endl;
//       n->setBalance(0);
//       //setTrueBalance(n);
//       //std::cout <<"about to call removefix? " << std::endl;
//       removeFix(p, ndiff);
//     }
//   }

//   // diff = 1
//   if(diff == 1){
//     right = n->getRight();
//     if(balance + diff == 2){
//       // case 1
//       if(balance == 1){
//         //std::cout << "case 5 " << std::endl;
//         // need Zag Zig
//         // if(right->getLeft() && !right->getRight()){
//         //   rotateLeft(right);
//         //   right = p->getRight();
//         // }
//         rotateLeft(n);
//         // setTrueBalance(n);
//         // setTrueBalance(right);
//         //if(right->getRight()) setTrueBalance(right->getRight());

//         n->setBalance(0);
//         right->setBalance(0);
//         removeFix(p, ndiff);
//       }

//       if(balance == 0){
//         //std::cout << "case 6 " << std::endl;
//         rotateLeft(n);
//         n->setBalance(1);
//         right->setBalance(-1);
//         // setTrueBalance(n);
//         // setTrueBalance(right);
//       }
//       if(balance == -1){
        
        
//         //std::cout << "case 7" << std::endl;
//         left = right->getLeft();
//         rotateRight(right);
//         rotateLeft(n);
//         if(left->getBalance()==-1){
//           //std::cout << "case 7.0 " << std::endl;
//           // setTrueBalance(n);
//           // setTrueBalance(right);
//           // setTrueBalance(left);

//           n->setBalance(0);
//           right->setBalance(1);
//           left->setBalance(0);
//         }
//         if(left->getBalance() == 0){
//           //std::cout << "case 7.1 " << std::endl;
//           n->setBalance(0);
//           right->setBalance(0);
//           left->setBalance(0);
//           // setTrueBalance(n);
//           // setTrueBalance(right);
//           // setTrueBalance(left);
//         }
//         if(left->getBalance()==1){
//           // std::cout << "case 7.2 " << std::endl;
//           n->setBalance(-1);
//           left->setBalance(0);
//           right->setBalance(0);
//           // setTrueBalance(n);
//           // setTrueBalance(right);
//           // setTrueBalance(left);
//         }
//         removeFix(p, ndiff);
//       }
//     }

//     if(balance + diff == 1){
//       //std::cout << "case 8 " << std::endl;
//       n->setBalance(1);
//       //setTrueBalance(n);
//       return;
//     }
//     if(balance + diff == 0) {
//      // std::cout << "case 9" << std::endl;
//       n->setBalance(0);
//       //setTrueBalance(n);
//       removeFix(p, ndiff);
//     }

//   }

// }





// // TODO insertFix
// template<class Key, class Value>
// void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* current){

//   if(!parent){
//     return;
//   }
//   AVLNode<Key, Value>* g = parent->getParent();
//   if(g == nullptr) {
//     //std::cout << " FAKE GRAMMY " << std::endl;
//     if(getHeight(parent) == -2) rotateRight(parent);
//     if(getHeight(parent) == 2) rotateLeft(parent);
//     setTrueBalance(parent);
//     setTrueBalance(parent->getParent());
//     return;
//   }
//   //this->printRoot(g);
  
//   //std::cout << "g " << g->getKey() << " " << (int) g->getBalance() << std::endl;

  
//   int b = g->getBalance();
//   // b = 0
//   if(b == 0) return;  
//   if(std::abs(b) == 1) insertFix(g, parent);

//   if(std::abs(b) == 2){
//     // zig zag - rotateleft(p) rotateright(g)
//     if(g->getLeft() == parent && parent->getRight() == current){
//       //std::cout << " zag zig " << g->getKey() << " " << std::endl;

//       rotateLeft(parent);
//       rotateRight(g);
//       if(current->getBalance() == -1){
//         parent->setBalance(0);
//         g->updateBalance(1);
//         current->setBalance(0);
//       }
//       else if(current->getBalance() == 0){
//         parent->setBalance(0);
//         g->setBalance(0);
//         current->setBalance(0);
//       }
//       else if(current->getBalance() == 1){
//         parent->updateBalance(-1);
//         g->setBalance(0);
//         current->setBalance(0);
//       }
//       else{
//        // std::cout<<"insert, balances wrong" << std::endl;
//       }
//     }
//     // zig zig - rotateright(g) bp, bg = 0
//     else if(g->getLeft() == parent && parent->getLeft() == current){
//       //std::cout << " zig zig " << g->getKey() << " " <<(int) b << std::endl;

//       rotateRight(g);
//       parent->setBalance(0);
//       g->setBalance(0);
//       //std::cout << " new balance " << (int) current->getBalance() << std::endl;
//     }
//     // zag zig - rotateright (p), rotateleft(g)
//     else if(g->getRight() == parent && parent->getLeft() == current){
//         //std::cout << " zag zig " << g->getKey() << " " << g->getBalance() << std::endl;
//         //std::cout << " parent " << parent->getKey() << std::endl;
//       rotateRight(parent);
//       rotateLeft(g);
//       // THESE MIGHT BE REASON FOR ERROR
//       if(current->getBalance() == -1){
//         //std::cout << "current " << current->getKey() << " -1" << std::endl;
//         parent->setBalance(0);
//         g->updateBalance(1);
//         current->setBalance(0);
//       }
//       else if(current->getBalance() == 0){
//         //std::cout << "current " << current->getKey() << " 0" << std::endl;
//         parent->setBalance(0);
//         g->setBalance(0);
//         current->setBalance(0);
//       }
//       else if(current->getBalance() == 1){
//         //std::cout << "current " << current->getKey() << " 1" << std::endl;
//         parent->updateBalance(-1);
//         g->setBalance(0);
//         current->setBalance(0);
//       }
//       else{
//         std::cout<<"insert, balances wrong" << std::endl;
//       }
//     }
//     // zag zag - rotateleft (g), bp bg = 0
//       else if(g->getRight() == parent && parent->getRight() == current){
//         //std::cout << " zag zag " << g->getKey() << " " << std::endl;
//         rotateLeft(g);
//         parent->setBalance(0);
//         g->setBalance(0);
//       }
//       else{
//         // std::cout << "something wrong in insert rotations" << std::endl;
//         // std::cout << "current " << current->getKey() << " parent " << parent->getKey() << " gramps " << g->getKey() << std::endl;
//         // std::cout << "current " << (int)current->getBalance() << " parent " << (int)parent->getBalance() << " gramps " << (int)g->getBalance() << std::endl;
//         return;
//       }
//     //std::cout << " got here? " << std::endl;
//   }
// }

#endif
