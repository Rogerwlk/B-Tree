#include "BTree.h"

BtreeNode::~BtreeNode()
{
	delete[] keys;
	delete[] frequency;
	delete[] children;
	keys = NULL;
	frequency = NULL;
	children = NULL;
}

/*ostream & operator<<(ostream & os, const BtreeNode & t)
{
	for (unsigned i = 0; i < t.num_key; i++)
	{
		cout << "j: " << i << "   ";
		os << t.keys[i] << "   ";
	}
	cout << std::endl;
	return os;
}*/  //Only for debugging

Btree::~Btree()
{
	clear(this->root);
}

void Btree::clear(BtreeNode * &p)
{
	//Delete BtreeNode recursively.
	if (!(p->is_leaf))
	{
		for (unsigned i = 0; i < p->num_key + 1; i++)
		{
			clear(p->children[i]);
		}
	}
	delete p;
	p = NULL;
}

/*ostream & operator<<(ostream & os, const Btree & t)
{
	queue<BtreeNode*> q;
	q.push(t.root);

	while (!q.empty())
	{
		unsigned size = q.size();
		for (unsigned i = 0; i < size; i++)
		{
			BtreeNode * n = q.front();
			os << "i: " << i << "  " << *n;
			for (unsigned j = 0; j <= n->num_key; j++)
			{
				if (n->children[j] != NULL)
					q.push(n->children[j]);
			}
			q.pop();
		}
		os << "\n\n";
	}
	return os;
}*/ //Only for debugging

bool Btree::search(const string & s)
{
	return search(this->root, s);
}

bool Btree::search(const BtreeNode * n, const string & s)
{
	//Find the maximum index by comparing key.
	unsigned i = 0;
	while (i < n->num_key && s > n->keys[i])
	{
		i++;
	}
	//It is found in the key.
	if (i < n->num_key && s == n->keys[i])
	{
		n->frequency[i]++;
		return true;
	}
	
	if (n->is_leaf) //It is not in the internal key and leaf key.
	{
		return false;
	}
	else //It is not in the internal key. Keep searching in its child.
	{
		return search(n->children[i], s);
	}
}

void Btree::insert(const string & s)
{
	if (root->num_key == (m - 1)) 
		//Root's key is full. Need to create a new root and increment the height.
	{
		BtreeNode * copy = root;
		root = new BtreeNode(m);
		root->is_leaf = false;
		root->children[0] = copy;
		splitChild(root, copy, 0);
		insertNonFullBtree(root, s);
	}
	else //The whole b-tree is not full.
	{
		insertNonFullBtree(root, s);
	}
}

void Btree::splitChild(BtreeNode * &parent, BtreeNode * &child, const unsigned & i)
{
	//Create a new child.
	BtreeNode * newNode = new BtreeNode(m);
	newNode->is_leaf = child->is_leaf;
	newNode->num_key = (m >> 1) - 1;
	//copy right half of child to newNode
	for (unsigned j = 0; j < newNode->num_key; j++)
	{
		//Copy frequency and keys.
		newNode->frequency[j] = child->frequency[j + ((m + 1) >> 1)];
		newNode->keys[j] = child->keys[j + ((m + 1) >> 1)];
		child->keys[j + ((m + 1) >> 1)] = "";
	}
	if (!child->is_leaf)
	{
		for (unsigned j = 0; j < (m >> 1); j++)
		{
			//Copy children.
			newNode->children[j] = child->children[j + ((m + 1) >> 1)];
			child->children[j + ((m + 1) >> 1)] = NULL;
		}
	}
	child->num_key = ((m - 1) >> 1);

	//Move parent's children to insert newNode as a new child in the parent
	for (unsigned j = parent->num_key; j > i; j--)
	{
		parent->children[j + 1] = parent->children[j];
	}
	parent->children[i + 1] = newNode;

	//Move parent's keys to make room for insertion of a new key.
	//Because j is unsigned, j<(j+1) means j hasn't underflowed.
	for (unsigned j = parent->num_key - 1; j < (j + 1) && j >= i; j--)
	{
		parent->frequency[j + 1] = parent->frequency[j];
		parent->keys[j + 1] = parent->keys[j];
	}
	//Insert the middle key of full node as a new key in the parent.
	parent->frequency[i] = child->frequency[(m - 1) >> 1];
	parent->keys[i] = child->keys[(m - 1) >> 1];
	parent->num_key++;
}

void Btree::insertNonFullBtree(BtreeNode * &n, const string & s)
{
	unsigned i;
	//Finding repetitive insertions.
	bool repeat = false;
	for (i = 0; i < n->num_key && s <= n->keys[i]; i++)
	{
		if (s == n->keys[i])
		{
			repeat = true;
			break;
		}
	}
	if (!repeat)
	{
		i = n->num_key - 1;
		if (n->is_leaf)
		{
			//Moving keys to make room. Only inserting at leaf.
			//Because i is unsigned, i<(i+1) means i hasn't underflowed.
			while (i < (i + 1) && s < n->keys[i])
			{
				n->frequency[i + 1] = n->frequency[i];
				n->keys[i + 1] = n->keys[i];
				i--;
			}
			i++;
			n->keys[i] = s;
			n->num_key++;
		}
		else
		{
			//Because i is unsigned, i<(i+1) means i hasn't underflowed.
			//Going down the tree to insert at leaf.
			while (i < (i + 1) && s < n->keys[i])
			{
				i--;
			}
			i++;
			//The child is full and can't be inserted.
			if (n->children[i]->num_key == (m - 1))
			{
				splitChild(n, n->children[i], i); //Split the child to make room.
				if (s > n->keys[i]) i++; //Decide going down the old child or the new child.
			}
			insertNonFullBtree(n->children[i], s);
		}
	}
}

void Btree::remove(const string & s)
{
	if (!root)
	{
		throw string("The tree is empty!\n");
	}
	else
	{
		//Remove a key from the b-tree.
		removeFullBtree(root, s);
		//If root's key is deleted, decrement the height of b-tree.
		if (root->num_key == 0 && !root->is_leaf)
		{
			BtreeNode * garbage = root;
			root = root->children[0];
			delete garbage;
			garbage = NULL;
		}
	}
}

void Btree::getMostFrequent()
{
	getMostFrequent(root);
}

void Btree::resetFrequency()
{
	max_frequency = 0;
	most_frequent_word = "";
	resetFrequency(root);
}

void Btree::getMostFrequent(const BtreeNode * n)
{
	unsigned i;
	for (i = 0; i < n->num_key; i++)
	{
		//Comparing each stored frequency with max_frequency.
		if (n->frequency[i] > max_frequency) 
		{
			most_frequent_word = n->keys[i];
			max_frequency = n->frequency[i];
		}
	}
	if (!n->is_leaf)
	{
		for (i = 0; i <= n->num_key; i++)
		{
			getMostFrequent(n->children[i]);
			//Going down each child.
		}
	}
}

void Btree::resetFrequency(BtreeNode *& n)
{
	unsigned i;
	for (i = 0; i < n->num_key; i++)
	{
		//Resetting all frequncy values to 0.
		n->frequency[i] = 0;
	}
	if (!n->is_leaf)
	{
		for (i = 0; i <= n->num_key; i++)
		{
			resetFrequency(n->children[i]);
			//Going down each child.
		}
	}
}

void Btree::mergeChildren(BtreeNode * &parent, const unsigned & i)
{
	BtreeNode * leftChild = parent->children[i];
	BtreeNode * rightChild = parent->children[i + 1];
	unsigned j;

	//parent key denotes to the end of left child
	leftChild->frequency[leftChild->num_key] = parent->frequency[i];
	leftChild->keys[leftChild->num_key] = parent->keys[i];
	leftChild->num_key++;

	//append right child to the end of left child
	if (!leftChild->is_leaf)
	{
		for (j = 0; j < rightChild->num_key; j++)
		{
			leftChild->frequency[leftChild->num_key] = rightChild->frequency[j];
			leftChild->keys[leftChild->num_key] = rightChild->keys[j];
			leftChild->children[leftChild->num_key] = rightChild->children[j];
			leftChild->num_key++;
		}
		leftChild->children[leftChild->num_key] = rightChild->children[j];
	}
	else
	{
		//No need to move children if two children are leaf.
		for (j = 0; j < rightChild->num_key; j++)
		{
			leftChild->frequency[leftChild->num_key] = rightChild->frequency[j];
			leftChild->keys[leftChild->num_key] = rightChild->keys[j];
			leftChild->num_key++;
		}
	}
	delete rightChild;
	rightChild = NULL;
	//move parent to delete the denoted key
	for (j = i; j < parent->num_key - 1; j++)
	{
		parent->frequency[j] = parent->frequency[j + 1];
		parent->keys[j] = parent->keys[j + 1];
	}
	parent->frequency[j] = 0;
	parent->keys[j] = "";

	//move parent to delete the denoted key's right child
	for (j = i + 1; j < parent->num_key; j++)
	{
		parent->children[j] = parent->children[j + 1];
	}
	parent->children[j] = NULL;

	parent->num_key--;
}

void Btree::borrowFromLeft(BtreeNode *& parent, const unsigned & i)
{
	BtreeNode * leftChild = parent->children[i];
	BtreeNode * rightChild = parent->children[i + 1];
	unsigned j;
	j = rightChild->num_key;
	//Move every child and key in the right child to the right,
	//so the index 0 can store a new child and key.
	rightChild->children[j + 1] = rightChild->children[j];
	for (j = rightChild->num_key - 1; j < (j + 1); j--)
	{
		rightChild->keys[j + 1] = rightChild->keys[j];
		rightChild->frequency[j + 1] = rightChild->frequency[j];
		rightChild->children[j + 1] = rightChild->children[j];
	}
	//fill the space at index 0
	rightChild->keys[0] = parent->keys[i];
	rightChild->frequency[0] = parent->frequency[i];
	rightChild->children[0] = leftChild->children[leftChild->num_key];
	leftChild->children[leftChild->num_key] = NULL;
	rightChild->num_key++;
	//renew parent's key and frequency
	parent->keys[i] = leftChild->keys[leftChild->num_key - 1];
	parent->frequency[i] = leftChild->frequency[leftChild->num_key - 1];
	leftChild->keys[leftChild->num_key - 1] = "";
	leftChild->frequency[leftChild->num_key - 1] = 0;
	leftChild->num_key--;
}

void Btree::borrowFromRight(BtreeNode *& parent, const unsigned & i)
{
	BtreeNode * leftChild = parent->children[i];
	BtreeNode * rightChild = parent->children[i + 1];
	unsigned j;
	j = leftChild->num_key;
	//fill out leftChild's last children
	leftChild->children[j + 1] = rightChild->children[0];
	leftChild->keys[j] = parent->keys[i];
	leftChild->frequency[j] = parent->frequency[i];
	leftChild->num_key++;
	//fill out parent's key and frequency
	parent->keys[i] = rightChild->keys[0];
	parent->frequency[i] = rightChild->frequency[0];
	//delete rightChild[0] by moving every child and key to the left.
	for (j = 0; j < (rightChild->num_key - 1); j++)
	{
		rightChild->keys[j] = rightChild->keys[j + 1];
		rightChild->frequency[j] = rightChild->frequency[j + 1];
		rightChild->children[j] = rightChild->children[j + 1];
	}
	rightChild->children[j] = rightChild->children[j + 1];
	rightChild->num_key--;
}

string Btree::getPred(BtreeNode *& parent, const unsigned & i)
{
	BtreeNode * node = parent->children[i];
	while (!node->is_leaf)
	{
		//The largest key is in the last children.
		node = node->children[node->num_key];
	}
	return node->keys[node->num_key - 1];
}

string Btree::getSucc(BtreeNode *& parent, const unsigned & i)
{
	BtreeNode * node = parent->children[i+1];
	while (!node->is_leaf)
	{
		//The smallest key is in the first children.
		node = node->children[0];
	}
	return node->keys[0];
}

void Btree::removeFullBtree(BtreeNode *& n, const string & s)
{
	//Find the index to remove by comparing with keys.
	unsigned i;
	for (i = 0; i < n->num_key && s > n->keys[i]; i++)
	{}
	
	if (i < n->num_key && s == n->keys[i]) //The item to remove is found.
	{
		if (n->is_leaf) //The item is in the leaf.
		{
			unsigned j;
			for (j = i; j < (n->num_key - 1); j++)
			{
				n->keys[j] = n->keys[j + 1];
				n->frequency[j] = n->frequency[j + 1];
			}
			n->keys[j] = "";
			n->frequency[j] = 0;
			n->num_key--;
		}
		else //The item is in the internal node.
		{
			//Left child satisfies the minimum requirement.
			if (n->children[i]->num_key >= (m >> 1)) 
			{
				string newS = getPred(n, i);
				n->keys[i] = newS;
				removeFullBtree(n->children[i], newS);
			}
			//Right child satisfies the minimum requirement.
			else if (n->children[i + 1]->num_key >= (m >> 1))
			{
				string newS = getSucc(n, i);
				n->keys[i] = newS;
				removeFullBtree(n->children[i + 1], newS);
			}
			else //Neither of two children satisfies the minimum requirement. Have to merge.
			{
				mergeChildren(n, i);
				removeFullBtree(n->children[i], s);
			}
		}
	}
	else //The item is not found.
	{
		if (n->is_leaf) //Reach the bottom and not find the item. Item is not in the btree.
		{
			//cout << "Key " << s << " is not in the btree!\n";
			return;
		}
		bool flag = false; //Marking if the index needs to be changed due to merging.
		if (n->children[i]->num_key < (m >> 1)) //The children doesn't satisfy minimum requirement.
		{
			//Its left child satisfies the minimum requirement, so borrow one from it.
			if (i > 0 && n->children[i - 1]->num_key >= (m >> 1))
			{
				borrowFromLeft(n, i - 1);
			}
			//Its right child satisfies the minimum requirement, so borrow one from it.
			else if (i < n->num_key && n->children[i + 1]->num_key >= (m >> 1))
			{
				borrowFromRight(n, i);
			}
			//Neither two children satisfies the minimum requirement. Need to merge.
			else
			{
				if (i < n->num_key) //Not the last child. Merge with its next child.
				{
					mergeChildren(n, i);
				}
				else //The last child. Merge with its previous child.
				{
					flag = true; //The index going down needs to be changed.
					mergeChildren(n, i - 1);
				}
			}
		}
		if (flag)
		{
			removeFullBtree(n->children[i - 1], s);
		}
		else
		{
			removeFullBtree(n->children[i], s);
		}
	}
}