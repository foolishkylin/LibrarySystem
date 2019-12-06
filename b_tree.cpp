#include "b_tree.h"

int Search(BTree search_node, int search_value)
// fun: searching in a no-left node 
// ret: the postion of node which value <= search_value
// key: none
{
    int i = 1;
    while(i <= search_node->keynum && search_value > search_node->key[i]) i++;
    return i;
}


void SearchBTree(BTree tree, int search_value, RES &search_result)
// fun: searching in a btree 
// ret: a result ptr including (pt, i, tag)
// key: if searching successfully, tag = 1, [*pt][i]=k
//      else tag = 0, return the postion which can be inserted.
{
    int search_postion = 0, position_found = 0;
    BTree current_node = tree, parent_node = NULL;
    // p points to node needed to being searched and q points to its parent
    while(current_node && !position_found)
    {
        search_postion = Search(current_node, search_value);
        if (search_postion <= current_node->keynum && current_node->key[search_postion] == search_value)
            position_found = 1;
        else
        {
            parent_node = current_node;
            current_node = current_node->ptr[search_postion-1];
        }
    }

    if (position_found)
    {
        search_result->ptr = current_node;
        search_result->pos = search_postion;
        search_result->tag = SEARCH_SUCCESS;
    }
    else
    {
        search_result->ptr = parent_node;
        search_result->pos = search_postion;
        search_result->tag = SEARCH_FAIL;
    }
}


void Split(BTree& current_node, int split_postion, BTree &splited_node)
// fun: split the current_node
// ret: none
// key: for the position split_postio, cut the current_node->key, the front ones are belong to
//		current_node and the behind are belong to splited_node 
{
	int i, j, current_keynum = current_node->keynum;
	splited_node = (BTree)malloc(sizeof(BTNode));
	if (splited_node)
	{
		// !!!!!!!!!!!!!!!!!!!!!!!!
		for (int i = 0; i <= M; i++)
			splited_node->ptr[i] = NULL;
		// !!!!!!!!!!!!!!!!!!!!!!!!

		splited_node->ptr[0] = current_node->ptr[split_postion];
		for (i = split_postion + 1, j = 1; i <= current_keynum; i++, j++)
		{
			splited_node->key[j] = current_node->key[i];
			splited_node->ptr[j] = current_node->ptr[i];
		}
		splited_node->keynum = current_keynum - split_postion;
		splited_node->parent = current_node->parent;
		for (i = 0; i <= current_keynum - split_postion; i++)
			if (splited_node->ptr[i])
				splited_node->ptr[i]->parent = splited_node;
		current_node->keynum = split_postion - 1;
	}
	else
		printf("error in split: memory insufficient.");
}


void NewRoot(BTree& new_root, BTree left_son, int root_key, BTree right_son)
// fun: create a new root for the nodes left_son and right_son
// ret: none
// key: if sons == 0, then this func is to create a true root
{
	new_root = (BTree)malloc(sizeof(BTNode));
	if (new_root)
	{
		// !!!!!!!!!!!!!!!!!!!!!!!!
		for (int i = 0; i <= M; i++)
			new_root->ptr[i] = NULL;
		// !!!!!!!!!!!!!!!!!!!!!!!!

		new_root->keynum = 1;
		new_root->ptr[0] = left_son;
		new_root->ptr[1] = right_son;
		new_root->key[1] = root_key;
		if (left_son)
			left_son->parent = new_root;
		if (right_son)
			right_son->parent = new_root;
		new_root->parent = NULL;
	}
	else
		printf("error in newRoot: memory insufficient.");
}


void Insert(BTree& current_node, int inserted_postion, int inserted_value, BTree inserted_node)
// fun: insert a new node
// ret: none
// key: none
{
	int j, current_keynum = current_node->keynum;
	for (j = current_keynum; j >= inserted_postion; j--)
	{
		current_node->key[j + 1] = current_node->key[j];
		current_node->ptr[j + 1] = current_node->ptr[j];
	}
	current_node->key[inserted_postion] = inserted_value;
	current_node->ptr[inserted_postion] = inserted_node;
	if (inserted_node)
		inserted_node->parent = current_node;
	current_node->keynum++;
}


void InsertBTree(BTree& tree, int inserted_value, BTree inserted_node, int insert_position)
// fun: insert a new key to tree
// ret: none
// key: none
{
	int current_inserted_value, splited_position, finished = FALSE, need_new_root = FALSE;
	BTree splited_node;
	if (!inserted_node)
		// if the tree is empty
		// create a root node
		NewRoot(tree, NULL, inserted_value, NULL);
	else
	{
		current_inserted_value = inserted_value, splited_node = NULL;
		while (!need_new_root && !finished)
		{
			Insert(inserted_node, insert_position, current_inserted_value, splited_node);
			if (inserted_node->keynum < M)
				finished = 1;
			else
			// the node is full
			{
				splited_position = (M + 1) / 2;
				Split(inserted_node, splited_position, splited_node);
				current_inserted_value = inserted_node->key[splited_position];
				// the current_inserted_value now will change, 
				// maybe not equal to the inserted_value
				if (inserted_node->parent)
				{
					inserted_node = inserted_node->parent;
					insert_position = Search(inserted_node, current_inserted_value);
					// find the position of key 
					// which needed to insert to the parent
				}
				else
				// the tree is only a root node
					need_new_root = TRUE;
			}
		}
		if (need_new_root)
			NewRoot(tree, inserted_node, current_inserted_value, splited_node);
			// create a new node for the inserted node and splited node
	}

}

void DeleteBTree(BTree& deleted_node, int deleted_postion, BTree &t)
{
	if (deleted_node->ptr[deleted_postion-1])
	{
		BTree te = t;
		Successor(deleted_node, deleted_postion);
		printf("\n--now Successor!--\n");
		ShowBTree(t, 0);
		DeleteBTree(deleted_node, deleted_node->keynum, t);
		// printf("\n--now Delete the %d!--\n", deleted_node->key[deleted_node->keynum - 1]);
	}
	else
	{
		Remove(deleted_node, deleted_postion);
		printf("now the deleted node was removed\n");
		printf("deleted_node->keynum (%d) : (M - 1) / 2 (%d)\n", deleted_node->keynum, (M - 1) / 2);
		if (deleted_node->keynum < (M - 1) / 2)
			Restore(deleted_node, deleted_postion, t);
	}
}

void Remove(BTree& deleted_node, int deleted_postion)
{
	for (int i = deleted_postion; i < deleted_node->keynum; i++)
	{
		deleted_node->key[i] = deleted_node->key[i + 1];
		deleted_node->ptr[i] = deleted_node->ptr[i + 1];
	}
	// deleted_node->ptr[deleted_node->keynum - 1] = deleted_node->ptr[deleted_node->keynum];
	deleted_node->keynum -= 1;
}


void Successor(BTree& deleted_node, int deleted_postion)
{
	// 把当前非叶结点的值与其左子交换，直到左子是叶子
	KeyType temp = deleted_node->key[deleted_postion];
	if (deleted_node->keynum < deleted_postion - 1)
	{
		printf("there is leaf!");
		return ;
	}
	BTree min_son = deleted_node->ptr[deleted_postion - 1];
	deleted_node->key[deleted_postion] = min_son->key[min_son->keynum];
	min_son->key[min_son->keynum] = temp;
	deleted_node = min_son;
	if (min_son->ptr[min_son->keynum - 1])
		Successor(deleted_node, deleted_node->keynum);
}


void Restore(BTree& deleted_node, int deleted_postion, BTree &t)
{
	if (!deleted_node->parent && !deleted_node->keynum)
	{
		t = deleted_node->ptr[0];
		deleted_node->ptr[0]->parent = NULL;
		free(deleted_node);
		printf("the root was updated!\n\n");
		return;
	}
	else if (!deleted_node->parent)
		return ;
	printf("--now should restore--\n\n");
	BTree parent = deleted_node->parent, left_brother = NULL, right_brother = NULL;
	KeyType parent_key = 0, parent_postion = 1;
	int deleted_node_position = 0, left_found = FALSE, right_found = FALSE, parent_found = FALSE, i;
	for (; deleted_node_position <= parent->keynum && parent->ptr[deleted_node_position] != deleted_node;
		deleted_node_position++);
	printf("deleted postion: %d\n\n", deleted_node_position);
	if (deleted_node->keynum + 1 == (M - 1) / 2)
	{
		if (parent->ptr[deleted_node_position] != deleted_node)
			printf("error in Restore: cannot find the deleted_node_position!\n");
		if (deleted_node_position)
		{
			left_brother = parent->ptr[deleted_node_position - 1];
			left_found = TRUE;
		}
		if (deleted_node_position < parent->keynum)
		{
			right_brother = parent->ptr[deleted_node_position + 1];
			right_found = TRUE;
		}
		if (left_found && left_brother->keynum >= (M + 1) / 2)
		{
			printf("--now should adjust the left part--\n\n");
			KeyType max_brother_key = left_brother->key[left_brother->keynum];
			for (i = 1; i <= parent->keynum && ! parent_found; i++)
			{
				if (parent->key[i] > max_brother_key)
				{
					parent_key = parent->key[i];
					printf("left rich: parent node than bigger than left-max-node was find\n\n");
					parent_postion = i;
					parent_found = TRUE;
				}
			}

			// 为保证思路连贯，此处用中文写出
			// 在deleted_node处需要向left_brother借key来调整的时候，需要注意以下几点：
			// 1. 流程大致是先借来left_brother的key，再借来parent的key，把parent的key插入被删结点后，
			//    再把left_brother的key替换掉parent那个借来的key，最后把left_brother的key删掉
			// 2. 借来的left_brother的key必须是其最大的key，即left_brother->key[left_brother->keynum]
			// 3. 借来的parent的key必须是恰好比借来的left_brother的key大的那一个key
			// 4. parent的key必须插入在deleted_node中最小的位置，即1
			// 5. 调整后需让兄弟的最大子过继给被调整结点
			// 在向right_brother借key的时候，与如上相反。

			deleted_node->keynum += 1;
			// because the parent key is smaller than the deleted key
			// so the key should be the min position of deleted node
			for (i = deleted_node->keynum; i > 1; i--)
				deleted_node->key[i] = deleted_node->key[i - 1];
			deleted_node->key[1] = parent_key;
			parent->key[parent_postion] = max_brother_key;

			for (i = deleted_node->keynum; i > 0; i--)
				deleted_node->ptr[i] = deleted_node->ptr[i - 1];
			deleted_node->ptr[0] = left_brother->ptr[left_brother->keynum];

			// 调整父属性
			if (deleted_node->ptr[0])
				deleted_node->ptr[0]->parent = deleted_node;
			Remove(left_brother, left_brother->keynum);
			// here the left->brother->ptr[left_brother->keynum] != NULL
			// but the left->brother->keynum--. so it is ok.
		}
		else if (right_found && right_brother->keynum >= (M + 1) / 2)
		{
			printf("--now should adjust the right part--\n\n");
			KeyType min_brother_key = right_brother->key[1];
			for (i = parent->keynum; i && !parent_found; i--)
				if (parent->key[i] < min_brother_key)
				{
					parent_key = parent->key[i];
					printf("right rich: parent node than smaller than right-min-node was found\n\n");
					parent_postion = i;
					parent_found = TRUE;
				}
			deleted_node->keynum += 1;
			// because the parent key is bigger than the deleted key
			// so the key should be the max position of deleted node
			deleted_node->key[deleted_node->keynum] = parent_key;
			parent->key[parent_postion] = min_brother_key;

			deleted_node->ptr[deleted_node->keynum] = right_brother->ptr[0];
			// 调整父属性
			if (deleted_node->ptr[deleted_node->keynum])
				deleted_node->ptr[deleted_node->keynum]->parent = deleted_node;

			for (int i = 1; i < right_brother->keynum; i++)
			{
				right_brother->key[i] = right_brother->key[i + 1];
				right_brother->ptr[i-1] = right_brother->ptr[i];
				// here need to delete right_brother->ptr[0]
			}
			right_brother->ptr[right_brother->keynum - 1] = right_brother->ptr[right_brother->keynum];
			right_brother->keynum -= 1;
		}
		else
		{
			// 非叶结点的合并也需要注意子的过继问题
			// 由于合并后父节点可能不空，所以合并结点可能在父节点的任意存在子位置
			// 这导致了对于父节点的合并，其子的过继比较困难
			// 故这里需要确保被合并结点必须被删除
			// 然后过继所有子
			BTree parent = deleted_node->parent;
			KeyType parent_key = 0;
			printf("now merge\n\n");
			if (left_found)
			{
				printf("now merge the left node\n\n");
				// 并非(parent->keynum + 1) / 2]， 应是分割left_brother与deleted_node的位置
				parent_key = parent->key[deleted_node_position];
				// parent->ptr[deleted_node_position] = NULL;
				Remove(parent, deleted_node_position);
				// 这里父节点由于合并的key与指向被合并子的指针被删除
				// extract the middle key of parent
				
				left_brother->keynum += 1;
				left_brother->key[left_brother->keynum] = parent_key;
				// 合并父节点后，最后一个指针是空
				// 如果被合并结点非空，继承其所有子
				if (deleted_node->keynum)
				{
					left_brother->ptr[left_brother->keynum] = deleted_node->ptr[0];
					if (left_brother->ptr[left_brother->keynum])
						left_brother->ptr[left_brother->keynum]->parent = left_brother;
					while (deleted_node->keynum)
					{
						// left_brother->keynum++;
						if (left_brother->keynum > 4)
						{
							printf("error in merge: node->keynum overflow!");
							break;
						}
						left_brother->key[++left_brother->keynum] = deleted_node->key[1];
						if (deleted_node->ptr[1])
						{
							left_brother->ptr[left_brother->keynum] = deleted_node->ptr[1];
							left_brother->ptr[left_brother->keynum]->parent = left_brother;
						}
						Remove(deleted_node, 1);
					}
				}
				else
					// 如果被合并结点空，则继承其0/1号指针上非空的那个
				{
					if (deleted_node->ptr[0])
						left_brother->ptr[left_brother->keynum] = deleted_node->ptr[0];
					else if (deleted_node->ptr[1])
						left_brother->ptr[left_brother->keynum] = deleted_node->ptr[1];

					if (left_brother->ptr[left_brother->keynum])
						left_brother->ptr[left_brother->keynum]->parent = left_brother;
				}
				printf("parent_key_num: %d\n", parent->keynum);
				free(deleted_node);
			}
			else
			{
				if (!right_found)
				{
					printf("error in merge: the left/right nodes were not found!\n\n");
					return ;
				}
				printf("now merge the right node\n\n");
				// 并非(parent->keynum + 1) / 2]， 应是分割left_brother与deleted_node的位置
				parent_key = parent->key[deleted_node_position + 1];
				// parent->ptr[deleted_node_position] = NULL;
				// ERROR: Remove(parent, deleted_node_position+1); 这里删除了指向合并子的指针，正确应如下

				// 此处删除父节点由于合并的key
				for (int i = deleted_node_position; i < parent->keynum; i++)
					parent->key[i] = parent->key[i + 1];
				// 此处删除父结点指向被合并子的指针
				if (parent->keynum)
					for (i = deleted_node_position; i < parent->keynum; i++)
						parent->ptr[i] = parent->ptr[i + 1];
				else
					parent->ptr[deleted_node_position] = NULL;
				parent->keynum -= 1;
				// 若此，之后父节点时，若父空，则直接过继左右子非空的那一部分

				// extract the middle key of parent

				right_brother->keynum += 1;
				right_brother->ptr[right_brother->keynum] = right_brother->ptr[right_brother->keynum - 1];
				for (i = right_brother->keynum; i > 1; i--)
				{
					right_brother->key[i] = right_brother->key[i - 1];
					right_brother->ptr[i - 1] = right_brother->ptr[i - 2];
				}
				right_brother->key[1] = parent_key;


				// 此处右兄弟需要继承被合并结点的子
				// 若被合并结点非空，应按照父节点，被合并结点的较大子，较小子的顺序插入到右兄弟的0号指针
				if (deleted_node->keynum)
				{
					right_brother->ptr[0] = deleted_node->ptr[deleted_node->keynum];
					if (right_brother->ptr[0])
						right_brother->ptr[0]->parent = right_brother;
					while (deleted_node->keynum)
					{
						for (i = right_brother->keynum; i > 1; i--)
						{
							right_brother->key[i] = right_brother->key[i - 1];
							right_brother->ptr[i - 1] = right_brother->ptr[i - 2];
						}
						right_brother->key[1] = deleted_node->key[deleted_node->keynum];
						Remove(deleted_node, deleted_node->keynum);
						if (deleted_node->ptr[deleted_node->keynum])
						{
							right_brother->ptr[0] = deleted_node->ptr[deleted_node->keynum];
							right_brother->ptr[0]->parent = right_brother;
						}
					}
				}
				else
				// 如果被合并结点空，则继承其0/1号指针上非空的那个
				{
					if (deleted_node->ptr[0])
						right_brother->ptr[0] = deleted_node->ptr[0];
					else if (deleted_node->ptr[1])
						right_brother->ptr[0] = deleted_node->ptr[1];
					
					if (right_brother->ptr[0])
						right_brother->ptr[0]->parent = right_brother;
				}
				free(deleted_node);
			}
		}
	}
	if (parent->keynum < (M + 1) / 2)
	{
		printf("parent node is not required!\n\n");
		// ShowBTree(t, 0);
		Restore(parent, 1, t);
	}
}

void ShowBTree(BTree root, int layer)
// fun: show the whole btree
// ret: none
// key: print the tree according the layer of tree
{
	if (root)
	{
		for (int j = 0; j < layer; j++)
			printf("	  ");
		for (int i = 1; i <= root->keynum; i++)
		{	
			printf("%d", root->key[i]);
			if (i != root->keynum)
				printf(",");
			else
				printf("\n");
		}
		for (int i = 0; i <= root->keynum; i++)
			ShowBTree(root->ptr[i], layer + 1);
		//if (root->ptr[0] != null)
		//	showbtree(root->ptr[0], layer + 1);
	}
}


