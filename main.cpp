#include <stdio.h>
#include "b_tree.h"

int main()
{
	RES res = (RES)malloc(sizeof(RESNode));
	BTree t = (BTree)malloc(sizeof(BTNode));
	if (t && res)
	{
		int test[10] = { 60, 20, 30, 70, 72, 50, 52, 68, 71, 100 };
		t->parent = NULL;
		for (int i = 0; i <= M; i++)
			t->ptr[i] = NULL;
		t->keynum = 0;
		for (int i = 0; i < 10; i++)
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
		SearchBTree(t, 50, res);
		DeleteBTree(res->ptr, res->pos, t);
		ShowBTree(t, 0);

	}


	return 0;
}