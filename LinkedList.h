#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>

using std::string;

class Node
{
public:
	string word;
	int freq; //frequency
	Node * next;

	Node(string w = "", int f = 0, Node * n = NULL) :
		word(w), freq(f), next(n) {};
};

class List
{
private:
	Node * head;
	Node * tail;
	void reorder(Node * n);
public:
	List(Node * h = NULL, Node * t = NULL) :head(h), tail(t) {};
	~List();

	void insert(const string & w);
	void remove(const string & w);
	bool search(const string & w);
	Node * getHead() const { return head; }
	void resetFrequency();
};

#endif // !LINKEDLIST_H
