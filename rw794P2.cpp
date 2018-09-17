#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <ctime>
#include <iomanip>
#include "LinkedList.h"
#include "BTree.h"

using namespace std;

const unsigned B_TREE_ORDER = 8; //B_TREE_ORDER must be no less than 4.

double totalTime(const clock_t & timeBegin, const clock_t & timeEnd)
{
	return static_cast<double>(timeEnd - timeBegin) / CLOCKS_PER_SEC;
}

template<typename T>
void loadDict(T &structure, const string &dictionary)
{
	ifstream dictionary_f;
	string word;
	
	dictionary_f.open(dictionary.c_str(), ios::in);
	if (dictionary_f.is_open())
	{
		while (dictionary_f >> word)
		{
			structure.insert(word);
		}
		dictionary_f.close();
	}
	else
	{
		cout << "Fail to open dictionary file!\n";
	}
}

template<typename T>
void remove(T &structure, const string &remove)
{
	string word;
	ifstream removal_f;

	removal_f.open(remove.c_str(), ios::in);
	if (removal_f.is_open())
	{
		while (removal_f >> word)
		{
			structure.remove(word);
		}
		removal_f.close();
	}
	else
	{
		cout << "Fail to open remove file!\n";
	}
}

template<typename T>
void spellCheck(T &structure, const string &input, unsigned &misspelled)
{
	clock_t timeBegin, timeEnd;
	char ch;
	string word = "";
	ifstream input_f;
	misspelled = 0;

	input_f.open(input.c_str(), ios::in);
	if (input_f.is_open())
	{
		structure.resetFrequency();
		timeBegin = clock();
		while ((ch = input_f.get()) != EOF)
		{
			if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z')
			{
				word += tolower(ch);
			}
			else if (word != "")
			{
				if (!structure.search(word))
				{
					misspelled++;
				}
				word = "";
			}
		}
		timeEnd = clock();
		cout << "Spellchecking time: " << totalTime(timeBegin, timeEnd) << endl;
		input_f.close();
	}
	else
	{
		cout << "Fail to open input file!\n";
	}
}

int main(int argc, char * argv[])
{
	try
	{
		if (argc < 3) throw string("Need three files for input, dictionary and removal.");
		string inputFile = argv[1],
			dictionaryFile = argv[2],
			removalFile = argv[3];
		List dict_list;
		Btree dict_btree(B_TREE_ORDER), dict_btree_order_4(4);
		unsigned misspelled = 0;

		cout << fixed << showpoint << setprecision(2);
		
		cout << "-----Linked List Output Before Removal-----\n";
		loadDict(dict_list, dictionaryFile);
		spellCheck(dict_list, inputFile, misspelled);
		cout << "Total Misspelled Words: " << misspelled << endl
			<< "Most Frequent Word: ";
		if (dict_list.getHead() != NULL)
			cout << dict_list.getHead()->word;
		cout << endl;
		cout << "Its Frequency: ";
		if (dict_list.getHead() != NULL)
			cout << dict_list.getHead()->freq << endl << endl;
		else
			cout << 0 << endl << endl;
		
		cout << "-----Btree of Order 4 Output Before Removal-----\n";
		loadDict(dict_btree_order_4, dictionaryFile);
		spellCheck(dict_btree_order_4, inputFile, misspelled);
		dict_btree_order_4.getMostFrequent();
		cout << "Total Misspelled Words: " << misspelled << endl
			<< "Most Frequent Word: " << dict_btree_order_4.get_most_frequent_word() << endl
			<< "Its Frequency: " << dict_btree_order_4.get_max_frequency() << endl << endl;

		cout << "-----Btree of Order " << B_TREE_ORDER << " Output Before Removal-----\n";
		loadDict(dict_btree, dictionaryFile);
		spellCheck(dict_btree, inputFile, misspelled);
		dict_btree.getMostFrequent();
		cout << "Total Misspelled Words: " << misspelled << endl
			<< "Most Frequent Word: " << dict_btree.get_most_frequent_word() << endl
			<< "Its Frequency: " << dict_btree.get_max_frequency() << endl << endl;
		
		cout << "-----Linked List Output After Removal-----\n";
		remove(dict_list, removalFile);
		spellCheck(dict_list, inputFile, misspelled);
		cout << "Total Misspelled Words: " << misspelled << endl
			<< "Most Frequent Word: "; 
		if (dict_list.getHead() != NULL)
			cout << dict_list.getHead()->word;
		cout << endl;
		cout << "Its Frequency: ";
		if (dict_list.getHead() != NULL) 
			cout << dict_list.getHead()->freq << endl << endl;
		else
			cout << 0 << endl << endl;
		
		cout << "-----Btree of Order " << B_TREE_ORDER << " Output After Removal-----\n";
		remove(dict_btree, removalFile);
		spellCheck(dict_btree, inputFile, misspelled);
		dict_btree.getMostFrequent();
		cout << "Total Misspelled Words: " << misspelled << endl
			<< "Most Frequent Word: " << dict_btree.get_most_frequent_word() << endl
			<< "Its Frequency: " << dict_btree.get_max_frequency() << endl << endl;

		cout << "-----Btree of Order 4 Output After Removal-----\n";
		remove(dict_btree_order_4, removalFile);
		spellCheck(dict_btree_order_4, inputFile, misspelled);
		dict_btree_order_4.getMostFrequent();
		cout << "Total Misspelled Words: " << misspelled << endl
			<< "Most Frequent Word: " << dict_btree_order_4.get_most_frequent_word() << endl
			<< "Its Frequency: " << dict_btree_order_4.get_max_frequency() << endl << endl;
	}
	catch (const string & err)
	{
		cerr << err << endl;
	}
	catch (...)
	{
		cerr << "Unknown error!\n";
	}
	
	return 0;
}