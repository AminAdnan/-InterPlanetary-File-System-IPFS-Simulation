#pragma once
#ifndef BTREES_H_
#define BTREES_H_

#include <iostream>
using namespace std;

class BTree;
class TreeNode;


class TreeNode
{
public:
    string* keys;
    string* values;
    string* names;

    int t; //minimum degree, to get range for keys
    TreeNode** children;
    int size; // no. of keys stored in node
    bool leaf; //check if its leaf node or not

public:
    TreeNode(int temp, bool bool_leaf);

    void deletion(string k, bool flag);
    void deletionSingle(string k, bool kf);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    //for names
    string getPredName(int idx);
    string getSuccName(int idx);

    //for values
    string getPred(int idx);
    string getSucc(int idx);
    //for keys
    string getPredecessor(int idx);
    string getSuccessor(int idx);

    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
    void insertNonFull(string k, string value, string name);
    void splitChild(int i, TreeNode* y);
    void traverse();

    bool keyFound(string k);
    int findKey(string k);
    string* search(string k);
    void PrintKey(string k, bool flag);
    ~TreeNode() 
    {
        cout << "Node Deleted\n";
        delete[] keys;
        keys = nullptr;

        delete[] values;
        values = nullptr;

        delete[] names;
        names = nullptr;

        for (int i = 0; i < 2 * t - 1; ++i) 
        {
            if(children[i] != nullptr)
            delete children[i];
            children[i] = nullptr;
        }
    }
    friend class BTree;
};

class BTree
{
public:
    TreeNode* root;
    int t; //min degree

public:

    BTree(int temp)
    {
        root = nullptr;
        t = temp;
    }

    void traverse()
    {
        if (root != nullptr)
        {
            root->traverse();
        }
        else
        {
            cout << "\n\tThe Tree doesnot exist\n";
        }
        cout << endl;
    }

    bool doesKeyExist(string k)
    {
        return root ? root->search(k) : false;
    }

    string* search(string k)
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        else
        {
            return root->search(k);
        }
    }

    void PrintKey(string k, bool flag = false) // prints the key and its value
    {
        if (root == nullptr)
        {
            cout << "tree is empty" << endl;
        }
        else
        {
            root->PrintKey(k, flag);
        }
    }

    void insert(string k, string value, string name);

    void deletion(string k, bool kf = false);

    void deletionSingle(string k, bool kf = false);
    
    void clearTree(TreeNode* node)
    {
        if (!node)
        {
            return;
        }

        // Recursively delete children of the current node
        for (int i = 0; i < node->size + 1; ++i)
        {
            if (node->children[i] != nullptr)
            {
                clearTree(node->children[i]);
                delete node;
            }
        }
    }

    void DeleteBTree() {
        cout << "BTree Destructor\n";
      clearTree(root);
    }
};

TreeNode::TreeNode(int t1, bool leaf1) {
    t = t1;
    leaf = leaf1;

    keys = new string[2 * t1 - 1];
    values = new string[2 * t1 - 1];
    names = new string[2 * t1 - 1];
    children = new TreeNode * [2 * t1];
    for (int i = 0; i < 2 * t1; ++i)
    {
        children[i] = nullptr;
    }

    size = 0;
}

void TreeNode::traverse() {
    int i;
    for (i = 0; i < size; i++)
    {
        if (leaf == false)
        {
            children[i]->traverse();
        }
        cout << "\n\tKEY:" << keys[i] << "\tVALUE:" << values[i] <<endl;
        
    }

    if (leaf == false)
    {
        children[i]->traverse();
    }

}

int TreeNode::findKey(string k) // returns index of key that is greater or equal to k
{

    int idx = 0;

    while (keys && idx < size && keys[idx] < k)
    {
        ++idx;
    }
    return idx;
}

void TreeNode::PrintKey(string k, bool flag) //searches and prints the key and its values
{
    //bool flag = false;
    int duplicates = 0;
    for (int i = 0; i < size; ++i)
    {
        if (keys[i] == k)
        {
            duplicates++;
        }
    }
    int i = 0;
    while (i < size && k > keys[i])
    {
        i++;
    }
    if (keys[i] == k)
    {
        flag = true;
        cout << "The key: " << k << " contains the Value: ";
        for (int j = 0; j < duplicates; ++j)
        {
            cout << values[i];
            if (j < duplicates - 1)
            {
                cout << ", ";
            }
            i++;
        }

        cout << endl;
        //return;
    }

    if (leaf == true)
    {
        if (!flag)
        {
            cout << "The Key: " << k << " not found" << endl;
        }
        return;
    }

    return children[i]->PrintKey(k, flag);
}

bool TreeNode::keyFound(string k)
{
    int idx = 0;
    while (idx < size && k > keys[idx])
    {
        ++idx;
    }

    if (idx < size && keys[idx] == k)
    {
        return true; // Key found in this node
    }
    else if (leaf)
    {
        return false; // Key not found in a leaf node
    }
    else if (children[idx])
    {
        return children[idx]->search(k); // Recursively search in the appropriate child node
    }
    else
    {
        return false; // Key not found in an empty child node
    }
}


string* TreeNode::search(string k) //returns values of key
{

    int i = 0;

    while (i < size && k > keys[i])
    {
        i++;
    }

    if (keys[i] == k)
    {
        /*
        for (int j = 0; j < duplicates; ++j)
        {

            arr[j] = values[i];
            i++;
        }*/

        // return arr;
        return &values[i];

    }

    if (leaf == true)
    {
        return nullptr;
    }

    return children[i]->search(k);
}

//bool keyfound = false;
void TreeNode::deletion(string k, bool kf)
{
    int count = 0;
    int idx = findKey(k);
    //keyfound = false;
    if (idx < size && keys[idx] == k) //if  // while for handling duplicates
    {
        //keyfound = true;
        bool foundKey = keyFound(k);
        while (foundKey && keys[idx] == k)
        {

            if (count > 0)
            {
                cout << "\n\tKey: " << k << " has duplicates" << endl;
            }
            kf = true;
            if (leaf)
            {
                // cout << "Removing: " << k << endl;
                removeFromLeaf(idx);
            }
            else
            {

                removeFromNonLeaf(idx);

            }

            idx = findKey(k);

            foundKey = keyFound(k);



            count++;

        }

    }

    /*
    else
    {

        if (leaf && !kf)
        {
            cout << "The key " << k << " does not exist in tree" << endl;
            return;
        }

    }*/

    bool flag = ((idx == size) ? true : false);
    if (children[idx])
    {

        if (children[idx] && children[idx]->size < t)
        {
            fill(idx);
        }
        if (children[idx - 1] && flag && idx > size)
        {
            children[idx - 1]->deletion(k, kf);
        }
        else
        {
            children[idx]->deletion(k, kf);
        }
        return;
    }


}


void TreeNode::removeFromLeaf(int idx)
{
    for (int i = idx + 1; i < size; ++i)
    {
        keys[i - 1] = keys[i];
        values[i - 1] = values[i]; // check
        names[i - 1] = names[i]; // check

    }
    size--;
    return;


}

void TreeNode::removeFromNonLeaf(int idx)
{
    string k = keys[idx];
    if (children[idx]->size >= t)
    {
        string pred = getPredecessor(idx);
        string predVal = getPred(idx); // check
        string predName = getPredName(idx); // check

        keys[idx] = pred;
        values[idx] = predVal; // check
        names[idx] = predName;

        children[idx]->deletion(pred, true);
    }

    else if (children[idx + 1]->size >= t)
    {
        string succ = getSuccessor(idx);
        string succVal = getSucc(idx); // check
        string succName = getSuccName(idx); // check

        keys[idx] = succ;
        values[idx] = succVal;
        names[idx] = succName;

        children[idx + 1]->deletion(succ, true);
    }

    else
    {
        merge(idx);
        children[idx]->deletion(k, true);
    }
    return;


}
//for names

string TreeNode::getPredName(int idx)
{
    TreeNode* cur = children[idx];
    while (!cur->leaf)
    {
        cur = cur->children[cur->size];
    }
    return cur->names[cur->size - 1];
}

string TreeNode::getSuccName(int idx)
{
    TreeNode* cur = children[idx + 1];
    while (!cur->leaf)
        cur = cur->children[0];

    return cur->names[0];
}


//for values

string TreeNode::getPred(int idx)
{
    TreeNode* cur = children[idx];
    while (!cur->leaf)
    {
        cur = cur->children[cur->size];
    }
    return cur->values[cur->size - 1];
}

string TreeNode::getSucc(int idx)
{
    TreeNode* cur = children[idx + 1];
    while (!cur->leaf)
        cur = cur->children[0];

    return cur->values[0];
}

//for keys

string TreeNode::getPredecessor(int idx)
{
    TreeNode* cur = children[idx];
    while (!cur->leaf)
    {
        cur = cur->children[cur->size];
    }
    return cur->keys[cur->size - 1];
}

string TreeNode::getSuccessor(int idx)
{
    TreeNode* cur = children[idx + 1];
    while (!cur->leaf)
        cur = cur->children[0];

    return cur->keys[0];
}


void TreeNode::fill(int idx)
{
    if (idx != 0 && children[idx - 1]->size >= t)
        borrowFromPrev(idx);

    else if (idx != size && children[idx + 1]->size >= t)
        borrowFromNext(idx);

    else {
        if (idx != size)
            merge(idx);
        else
            merge(idx - 1);
    }
    return;
}

void TreeNode::borrowFromPrev(int idx)
{
    TreeNode* child = children[idx];
    TreeNode* sibling = children[idx - 1];

    for (int i = child->size - 1; i >= 0; --i)
    {
        child->keys[i + 1] = child->keys[i];
        child->values[i + 1] = child->values[i]; // check
        child->names[i + 1] = child->names[i]; // check

    }

    if (!child->leaf)
    {
        for (int i = child->size; i >= 0; --i)
        {
            child->children[i + 1] = child->children[i];
        }
    }

    child->keys[0] = keys[idx - 1];
    child->values[0] = values[idx - 1]; // check
    child->names[0] = names[idx - 1]; // check


    if (!child->leaf)
    {
        child->children[0] = sibling->children[sibling->size];
    }

    keys[idx - 1] = sibling->keys[sibling->size - 1];
    values[idx - 1] = sibling->values[sibling->size - 1]; // check
    names[idx - 1] = sibling->names[sibling->size - 1]; // check

    child->size += 1;
    sibling->size -= 1;

    return;
}


void TreeNode::borrowFromNext(int idx)
{
    TreeNode* child = children[idx];
    TreeNode* sibling = children[idx + 1];

    child->keys[(child->size)] = keys[idx];
    child->values[(child->size)] = values[idx]; // check
    child->names[(child->size)] = names[idx]; // check

    if (!(child->leaf))
    {
        child->children[(child->size) + 1] = sibling->children[0];
    }

    keys[idx] = sibling->keys[0];
    values[idx] = sibling->values[0]; // check
    names[idx] = sibling->names[0]; // check

    for (int i = 1; i < sibling->size; ++i)
    {
        sibling->keys[i - 1] = sibling->keys[i];
        sibling->values[i - 1] = sibling->values[i]; // check
        sibling->names[i - 1] = sibling->names[i]; // check

    }

    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->size; ++i)
        {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->size += 1;
    sibling->size -= 1;

    return;
}


void TreeNode::merge(int idx) //how tf is this working
{
    TreeNode* child = children[idx];
    TreeNode* sibling = children[idx + 1];

    child->keys[t - 1] = keys[idx];
    child->values[t - 1] = values[idx]; // check
    child->names[t - 1] = names[idx]; // check

    for (int i = 0; i < sibling->size; ++i)
    {
        child->keys[i + t] = sibling->keys[i];
        child->values[i + t] = sibling->values[i]; // check
        child->names[i + t] = sibling->names[i]; // check

    }

    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->size; ++i)
        {
            child->children[i + t] = sibling->children[i];
        }
    }

    for (int i = idx + 1; i < size; ++i)
    {
        keys[i - 1] = keys[i];
        values[i - 1] = values[i]; // check
        names[i - 1] = names[i]; // check

    }

    for (int i = idx + 2; i <= size; ++i)
    {
        children[i - 1] = children[i];
    }

    child->size += sibling->size + 1;
    size--;

    delete (sibling);
    return;
}


void BTree::deletionSingle(string k, bool kf)
{
    //keyfound = false;
    if (!root)
    {
        cout << "tree is empty" << endl;
        return;
    }

    string* temp1 = search(k);
    if (!temp1)
    {
        cout << "key does not exist yyeeeet" << endl;
        return;
    }


    root->deletionSingle(k, kf);


    if (root && root->size == 0)
    {
        TreeNode* temp = root;
        if (root->leaf)
        {
            root = nullptr;

        }
        else
        {
            root = root->children[0];
        }

       delete temp;
    }

    return;

}
//**

void BTree::deletion(string k, bool kf)
{
    //keyfound = false;
    if (!root)
    {
        cout << "\n\ttree is empty" << endl;
        return;
    }

    string* temp1 = search(k);
    if (!temp1)
    {
        cout << "\n\tkey does not exist yyeeeet" << endl;
        return;
    }


    root->deletion(k, kf);


    if (root && root->size == 0)
    {
        TreeNode* temp = root;
        if (root->leaf)
        {
           //root = nullptr;
            /////not needed
        }
        else
        {
            root = root->children[0];
        }
        //delete temp;
    }

    return;

}


void BTree::insert(string k, string value, string name)
{
    if (root == nullptr)
    {
        root = new TreeNode(t, true);
        root->keys[0] = k;
        root->values[0] = value;
        root->names[0] = name;

        root->size = 1;
    }
    else
    {
        if (root->size == 2 * t - 1) // when node is full i.e cant have any more keys
        {
            TreeNode* s = new TreeNode(t, false);

            s->children[0] = root;

            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < k)
            {
                i++;
            }
            s->children[i]->insertNonFull(k, value, name);

            root = s;
        }
        else
        {
            root->insertNonFull(k, value, name); // if node is not full
        }
    }
}

void TreeNode::insertNonFull(string k, string value, string name) //appends the key and value
{
    int i = size - 1;

    if (leaf == true) {
        while (i >= 0 && keys[i] > k) // sort in ascending order, basically
        {

            keys[i + 1] = keys[i];
            values[i + 1] = values[i];
            names[i + 1] = names[i];

            i--;
        }

        keys[i + 1] = k;
        values[i + 1] = value;
        names[i + 1] = name;

        size = size + 1;
    }
    else
    {
        while (i >= 0 && keys[i] > k)
        {
            i--;
        }
        if (children[i + 1]->size == 2 * t - 1)
        {
            splitChild(i + 1, children[i + 1]);

            if (keys[i + 1] < k)
            {
                i++;
            }
        }
        children[i + 1]->insertNonFull(k, value, name);
    }
}

void TreeNode::splitChild(int i, TreeNode* y)
{
    TreeNode* z = new TreeNode(y->t, y->leaf);
    z->size = t - 1;

    for (int j = 0; j < t - 1; j++)
    {
        z->keys[j] = y->keys[j + t];
        z->values[j] = y->values[j + t];
        z->names[j] = y->names[j + t];

    }

    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
        {
            z->children[j] = y->children[j + t];
        }
    }

    y->size = t - 1;
    for (int j = size; j >= i + 1; j--)
    {
        children[j + 1] = children[j];
    }

    children[i + 1] = z;

    for (int j = size - 1; j >= i; j--)
    {
        keys[j + 1] = keys[j];
        values[j + 1] = values[j];
        names[j + 1] = names[j];

    }

    keys[i] = y->keys[t - 1];
    values[i] = y->values[t - 1];
    names[i] = y->names[t - 1];

    size = size + 1;
}

void TreeNode::deletionSingle(string k, bool kf)
{
    int count = 0;
    int idx = findKey(k);
    //keyfound = false;
    if (idx < size && keys[idx] == k) //if  // while for handling duplicates
    {
        //keyfound = true;
        bool foundKey = keyFound(k);
        //   while (foundKey && keys[idx] == k)
       //    {

        if (count > 0)
        {
            cout << "Key: " << k << " has duplicates" << endl;
        }
        kf = true;
        if (leaf)
        {
            // cout << "Removing: " << k << endl;
            removeFromLeaf(idx);
        }
        else
        {

            removeFromNonLeaf(idx);

        }

        idx = findKey(k);

        foundKey = keyFound(k);



        count++;


    }

    bool flag = ((idx == size) ? true : false);
    if (children[idx])
    {

        if (children[idx] && children[idx]->size < t)
        {
            fill(idx);
        }
        if (children[idx - 1] && flag && idx > size)
        {
            children[idx - 1]->deletion(k, kf);
        }
        else
        {
            children[idx]->deletion(k, kf);
        }
        return;
    }


}


#endif