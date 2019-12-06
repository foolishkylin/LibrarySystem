#include <stdio.h>
#include "b_tree.h"

int main()
{
	RES res = (RES)malloc(sizeof(RESNode));
	BTree t = (BTree)malloc(sizeof(BTNode));
	if (t && res)
	{
		int test[18] = { 35, 16, 18, 70, 5, 50, 22, 60, 13, 17, 12 , 45, 25, 42, 15, 90, 30, 7 };
		t->parent = NULL;
		for (int i = 0; i <= M; i++)
			t->ptr[i] = NULL;
		t->keynum = 0;
		for (int i = 0; i < 18; i++)
		{
			SearchBTree(t, test[i], res);
			if (res->tag == SEARCH_FAIL)
				InsertBTree(t, test[i], res->ptr, res->pos);
			if (i % 1 == 0)
			{
				printf("\n--the %dth test, insert %d--\n\n", i + 1, test[i]);
				ShowBTree(t, 0);
			}
		}
		SearchBTree(t, 45, res);
		DeleteBTree(res->ptr, res->pos, t);
		ShowBTree(t, 0);
		SearchBTree(t, 90, res);
		DeleteBTree(res->ptr, res->pos, t);
		ShowBTree(t, 0);
		// 这里的42结点的父关系错误
		// 42过继到50下面的时候没用改变其父属性
		SearchBTree(t, 50, res);
		DeleteBTree(res->ptr, res->pos, t);
		ShowBTree(t, 0);
		SearchBTree(t, 22, res);
		DeleteBTree(res->ptr, res->pos, t);
		ShowBTree(t, 0);
		SearchBTree(t, 42, res);
		DeleteBTree(res->ptr, res->pos, t);
		ShowBTree(t, 0);

	}


	return 0;
}