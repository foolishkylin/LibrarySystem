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


void SearchBTree(BTree t, int k, RES &result)
// fun: searching in a btree 
// ret: a result ptr including (pt, i, tag)
// key: if searching successfully, tag = 1, [*pt][i]=k
//      else tag = 0, return the postion which can be inserted.
{
    int i = 0, found = 0;
    BTree p = t, q = NULL;
    // p points to node needed to being searched and q points to its parent
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


void split(BTree &q, int s, BTree &ap)
{
	int i, j, n = q->keynum;
	ap = (BTree)malloc(sizeof(BTNode));
	if (ap)
	{
		ap->ptr[0] = q->ptr[s];
		for (i = s + 1, j = 1; i <= n; i++, j++)
		{
			ap->key[j] = q->key[i];
			ap->ptr[j] = q->ptr[i];
		}
		ap->keynum = n - s;
		ap->parent = q->parent;
		for (i = 0; i <= n - s; i++)
			if (ap->ptr[i])
				ap->ptr[i]->parent = ap;
		q->keynum = s - 1;
	}
	else
		printf("error in split: memory insufficient.");
}


void newRoot(BTree &t, BTree p, int x, BTree ap)
{
	t = (BTree)malloc(sizeof(BTNode));
	if (t)
	{
		t->keynum = 1;
		t->ptr[0] = p;
		t->ptr[1] = ap;
		t->key[1] = x;
		if (p)
			p->parent = t;
		if (ap)
			ap->parent = t;
		t->parent = NULL;
	}
	else
		printf("error in newRoot: memory insufficient.");
}


void Insert(BTree &q, int i, int x, BTree ap)
{
	int j, n = q->keynum;
	for (j = n; j >= i; j--)
	{
		q->key[j + 1] = q->key[j];
		q->ptr[j + 1] = q->ptr[j];
	}
	q->key[i] = x;
	q->ptr[i] = ap;
	if (ap)
		ap->parent = q;
	q->keynum++;
}


void InsertBTree(BTree &t, int k, BTree q, int i)
{
	int x, s, finished = 0, needNewRoot = 0;
	BTree ap;
	if (!q)
		newRoot(t, NULL, k, NULL);
	else
	{
		x = k, ap = NULL;
		while (!needNewRoot && !finished)
		{
			Insert(q, i, x, ap);
			if (q->keynum < M)
				finished = 1;
			else
			{
				s = (M + 1) / 2;
				split(q, s, ap);
				if (q->parent)
				{
					q = q->parent;
					i = Search(q, x);
				}
				else
					needNewRoot = 1;
			}
		}
		if (needNewRoot)
			newRoot(t, q, x, ap);
	}

}

void DeleteBTree(BTree &p, int i)
{
	//if (p->ptr[i])
	//{
	//	successor(p, i);
	//	deletebtree(p, 1);
	//}
	//else
	//{
	//	remove(p, i);
	//	if (p->keynum < (m - 1) / 2)
	//		restore(p, i);
	//}
}


void ShowBTree(BTree root)
{
	if (root)
	{
		ShowBTree(root->ptr[1]);
		for (int i = 1; i <= root->keynum; i++)
		{
			printf("%d\n", root->key[i]);
			if (i > 1)
				ShowBTree(root->ptr[i]);
		}
	}
}


