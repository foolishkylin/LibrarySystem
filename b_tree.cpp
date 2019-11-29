#include "b_tree.h"

int Search(BTree p, int k)
// fun: searching in a no-left node 
// ret: the postion of node which value <= k
// key: none
{
    int i = 1;
    while(i <= p->keynum && k > p->key[i]) i++;
    return i;
}

void SearchBTree(BTree t, int k, RES *result)
// fun: searching in a btree 
// ret: a result ptr including (pt, i, tag)
// key: if searching successfully, tag = 1, [*pt][i]=k
//      else tag = 0, return the postion which can be inserted.
{
    int i = 0, found = 0;
    BTree p = t, q = NULL;
    // p points to node needed to search and q points to its parent
    while(p && !found)
    {
        i = Search(p, k);
        if (i <= p->keynum && p->key[i] == k)
            found = 1;
        else
        {
            q = p;
            p = p->ptr[i-1];
        }
    }

    if (found)
    {
        result->pt = p;
        result->i = i;
        result->tag = 1;
    }
    else
    {
        result->pt = q;
        result->i = i;
        result->tag = 0;
    }
}




