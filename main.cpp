#include <stdio.h>
#include "b_tree.h"

int main()
{
	RES res = (RES)malloc(sizeof(RESNode));
	BTree t = (BTree)malloc(sizeof(BTNode));
	if (t && res)
	{
		int test[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		t->parent = NULL;
		for (int i = 0; i <= M; i++)
			t->ptr[i] = NULL;
		t->keynum = 0;
		for (int i = 0; i < 10; i++)
		{
			SearchBTree(t, test[i], res);
			if (!res->tag)
				InsertBTree(t, test[i], res->pt, res->i);
			printf("\n--the %dth test, insert %d--\n", i+1, test[i]);
			printf("\n--version 0--\n");
			ShowBTree(t);
			printf("\n--version 1--\n");
			for (int j = 1; j <= t->keynum; j++)
				printf("%d", t->key[j]);
		}

	}


	return 0;
}