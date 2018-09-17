#include "LinkedList.h"

void List::reorder(Node * n)
{
	Node * copy = head;
	while (copy != NULL && copy->freq > n->freq)
	{
		copy = copy->next;
	}
	if (copy != NULL && copy->freq != n->freq)
	{
		int temp_freq = n->freq;
		n->freq = copy->freq;
		copy->freq = temp_freq;
		string temp_word = n->word;
		n->word = copy->word;
		copy->word = temp_word;
	}
}

List::~List()
{
	Node * garbage = head;
	while (head != NULL)
	{
		head = head->next;
		delete garbage;
		garbage = head;
	}
}

void List::insert(const string & w)
{
	if (head == NULL)
	{
		head = tail = new Node(w);
	}
	else
	{
		tail->next = new Node(w);
		tail = tail->next;
	}
}

void List::remove(const string & w)
{
	Node * copy = head;
	Node * copy_pre = copy;
	if (copy != NULL && copy->word != w) copy = copy->next;
	while (copy != NULL && copy->word != w)
	{
		copy_pre = copy_pre->next;
		copy = copy->next;
	}

	if (copy != NULL)
	{
		Node * garbage = copy;
		if (copy == head)
		{
			head = head->next;
			if (copy == tail)
				tail = NULL;
		}
		else 
		{
			if (copy == tail)
				tail = copy_pre;
			copy_pre->next = copy->next;
		}
		delete garbage;
		copy = copy_pre = NULL;
	}
}

bool List::search(const string & w)
{
	Node * copy = head;
	while (copy != NULL && copy->word != w)
	{
		copy = copy->next;
	}
	if (copy == NULL)
	{
		return false;
	}
	else
	{
		++(copy->freq);
		reorder(copy);
		return true;
	}
}

void List::resetFrequency()
{
	Node * copy = head;
	while (copy != NULL)
	{
		copy->freq = 0;
		copy = copy->next;
	}
}
