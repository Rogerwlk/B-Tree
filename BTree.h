#ifndef BTREE_H
#define BTREE_H

#include <string>

using std::string;

/* //ONLY FOR DEBUGGING
//#include <queue>
//#include <iostream>

using std::ostream;
using std::operator<<;
using std::queue; 
using std::cout;
*/

class BtreeNode
{
public:
	string * keys;
	unsigned * frequency;
	BtreeNode ** children;
	unsigned num_key;
	bool is_leaf;
	BtreeNode(unsigned m) :
		keys(new string[m - 1]),
		frequency(new unsigned[m - 1]()),
		children(new BtreeNode*[m]()),
		num_key(0),
		is_leaf(true) 
	{
		if (m < 4) throw string("ERROR B-TREE ORDER LESS THAN 4!");
	}
	~BtreeNode();
	//friend ostream & operator<<(ostream & os, const BtreeNode & t); //ONLY FOR DEBUGGING
};

class Btree
{
private:
	BtreeNode * root;
	unsigned m;
	unsigned max_frequency;
	string most_frequent_word;

	void clear(BtreeNode * &p); //used by destructor only

	//Traverse the b-tree and increment the frequency.
	bool search(const BtreeNode * n, const string & s);

	//Insert after checking if root's children is full.
	void insertNonFullBtree(BtreeNode * &n, const string & s);
	//Split child into two children and increment the parent's key.
	void splitChild(BtreeNode * &parent, BtreeNode * &child, const unsigned & i);

	//Remove a node from b-tree.
	void removeFullBtree(BtreeNode * &n, const string & s);
	//Denote the parent's key and merge two children into one.
	void mergeChildren(BtreeNode * &parent, const unsigned & i);

	//Parent's left child borrow one key and one child from right child through parent's key. 
	void borrowFromLeft(BtreeNode * &parent, const unsigned & i);
	//Parent's right child borrow one key and one child from left child through parent's key. 
	void borrowFromRight(BtreeNode * &parent, const unsigned & i);

	//Return the largest key in the parent's children i.
	string getPred(BtreeNode *& parent, const unsigned & i);
	//Return the smallest key in the parent's children i.
	string getSucc(BtreeNode *& parent, const unsigned & i);

	//Update values for most frequent word and its frequency.
	void getMostFrequent(const BtreeNode * n);

	//Reset all frequency values to 0.
	void resetFrequency(BtreeNode *& n);

public:
	Btree(unsigned m = 4) :
		root(new BtreeNode(m)),
		m(m),
		max_frequency(0),
		most_frequent_word("") 
	{
		if (m < 4) throw string("ERROR B-TREE ORDER LESS THAN 4!");
	}

	~Btree();

	//Accessors.
	unsigned get_max_frequency() const { return max_frequency; }
	string get_most_frequent_word() const { return most_frequent_word; }

	//friend ostream & operator<<(ostream & os, const Btree & t); //ONLY FOR DEBUGGING

	//Use the private member function search(...) to implement recursively.
	bool search(const string & s);

	//Use the private member function insert(...) to implement recursively.
	void insert(const string & s);
	
	//Use the private member function remove(...) to implement recursively.
	void remove(const string & s);

	//Use the private member function getMostFrequent(...) to implement recursively.
	void getMostFrequent();

	//Use the private member function resetFrequency(...) to implement recursively.
	void resetFrequency();
};

#endif // !BTREE_H
