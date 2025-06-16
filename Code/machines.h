#pragma once
#ifndef MACHINES_H_
#define MACHINES_H_


#include<iostream>
#include "Btrees.h"
using namespace std;

class CLLNode;
class CircularLinkedList;
class fingertablenode;
class fingertable;
class Record;



class Record
{
public:

	char* fileName;
	string shastring;

	Record() {
		shastring = "";
	}

	Record(char* str)
	{
		int len = 0;
		while (str[len] != '\0')
			len++;
		fileName = new char[len + 1];

		for (int i = 0; i < len; i++)
		{
			fileName[i] = str[i];
		}
		fileName[len] = '\0';

		shastring = "";
	}

	void record_readfile()    //read file
	{
		ifstream inputfile(fileName);
		if (inputfile.is_open())
		{
			char ch;
			while (inputfile.get(ch))
			{
				shastring += ch;
			}
		}
		else {
			cout << "\n\tfile cound not open\n";
		}
		inputfile.close();
	}

	void createfile(string content, string filepath) {  //write into the newly created file

		bool checkexistance = false;
		char ch = ' ';
		ifstream inputfile(filepath);
		if (inputfile.is_open())
		{
			while (inputfile.get(ch))
			{
				//shastring += ch;
				checkexistance = true;
				cout << "\n\tThe given file already exists.\n";
				break;
			}
		}
		inputfile.close();


		if (!checkexistance)   //if file is newly created, add content to it

		{
			ofstream file(filepath, ios::app);
			if (file.is_open()) {
				file << content;
			}
			else {
				cout << "\n\tfile cound not be created\n";
			}
			file.close();
		}
	}


	void deleteFile(const string& filepath) {
		// Convert narrow string to wide string
		wstring wide_filepath(filepath.begin(), filepath.end());

		if (DeleteFile(wide_filepath.c_str())) {
			cout << "\n\tFile deleted successfully\n";
		}
		else {
			cout << "\n\tError deleting file\n";
		}
	}

	~Record()
	{
		delete[] fileName;
		fileName = 0;
	}

};






class fingertablenode {
public:

	int id;   //no of nodes in finger table
	fingertablenode* next;
	fingertablenode* prev;
	CLLNode* machine;

	fingertablenode() {
		id = 0;
		next = nullptr;
		prev = nullptr;
		machine = nullptr;
	}

};

class fingertable
{

public:
	fingertablenode* head;
	fingertablenode* tail;

	fingertable() {
		head = nullptr;
		tail = nullptr;
	}

	void InsertNodes(int ID, CLLNode* address)
	{
		fingertablenode* temp = new fingertablenode;
		temp->next = nullptr;
		temp->prev = nullptr;
		temp->id = ID;
		temp->machine = address;


		if (head == nullptr && tail == nullptr) {
			head = temp;
			tail = temp;
		}
		else {
			tail->next = temp;
			temp->prev = tail;
			tail = temp;
		}
	}

	void Delete_FingerTable()
	{
		cout << "\tfingertable Destructor\n";
		fingertablenode* current = head;
		fingertablenode* garbage = current;
		while (current != NULL)
		{
			garbage = current;
			if (current != nullptr)
				current = current->next;
			//cout << "destructor\n";
			delete garbage;
		}
		garbage = nullptr;
		current = nullptr;
	}

};


class CLLNode {
public:
	//int data;
	string machinekey;
	string machinename;
	string machinepath;
	string machinehash;
	fingertable* ftable;
	CLLNode* next;
	static int counter;
	BTree* tree;
	CLLNode() {
		//machinepath = "C:/Users/Admin/Desktop/";
		machinepath = "C:/Users/Asus/OneDrive/Desktop/";
		machinename = "";
		machinekey = "";
		ftable = nullptr;
		next = nullptr;
		tree = nullptr;
	}

};
int CLLNode::counter = 0;

class CircularLinkedList
{

public:
	CLLNode* tail;
	CLLNode* head;

	CircularLinkedList() {
		tail = nullptr;
		head = nullptr;
	}

	void setfingertable(int size, int identifierspace)
	{
		//if a new machine is added, delete the previous finger table for all themachines and generate the new ones

		CLLNode* ptr = head;
		while (ptr->next != head) {
			ptr->ftable = new fingertable;

			for (int i = 1; i <= size; i++) {

				//search for the address of the machine
				int carry = 0;
				Stack<int>s;
				Addition(ptr->machinekey, power(i - 1), carry, s);
				string x = "";
				while (!s.empty()) {
					x += (s.peek() + '0');
					s.pop();
				}

				if (x > to_string(identifierspace)) {
					string temp = power(identifierspace);
					x = Modulus(x, temp);
				}
				CLLNode* address = findaddress(x);//returns the address for the successor
				ptr->ftable->InsertNodes(i, address);
			}
			ptr = ptr->next;
		}
		//for thr last machine
		ptr->ftable = new fingertable;
		for (int i = 1; i <= size; i++) {

			//search for the address of the machine
			int carry = 0;
			Stack<int>s;
			Addition(ptr->machinekey, power(i - 1), carry, s);
			string x = "";
			while (!s.empty()) {
				x += (s.peek() + '0');
				s.pop();
			}

			if (x > to_string(identifierspace)) {
				string temp = power(identifierspace);
				x = Modulus(x, temp);
			}

			CLLNode* address = findaddress(x);
			ptr->ftable->InsertNodes(i, address);
		}
		cout << "\tMachine FingerTable has been created successfully.\n";
	}

	CLLNode* findaddress(string required) {
		CLLNode* ptr = head;

		while (ptr->next != head && ptr->next != nullptr && lessthan(ptr->next->machinekey, required)) {
			ptr = ptr->next;
		}
		return ptr->next;
	}

	void set_btree(int order) {


		CLLNode* ptr = head;
		while (ptr->next != head) {
			ptr->tree = new BTree(order);
			ptr = ptr->next;
		}
		ptr->tree = new BTree(order);
	}

	void delete_file_btree(string key, string decimal_sha, string requestedmac, int identifierspace, string &temp) {

		string path = "";
		string t = power(identifierspace);
		//if request arrives on the first machine
		CLLNode* ptr = head;

		if (ptr->machinekey != requestedmac) {
			//first go to the machine where the request arrives
			do {
				ptr = ptr->next;
			} while (ptr->machinekey != requestedmac && ptr->next != head);
		}

		if (ptr->machinekey != requestedmac) {
			cout << "\n\tYour entered machine does not exist\n";
			return;
		}
		//search through to all machines until an appropriate machine is found for deletion
		while (lessthan(ptr->machinekey, key) && ptr->next != head) {
			path += ptr->machinename + "-> ";
			ptr = ptr->next;
		}

		//delete file into the btree of the found machine
		if (greaterthanequalto(ptr->machinekey,key)) {

			path += ptr->machinename;
			cout << "\n\tPath to reach the appropriate Machine is: \n";
			cout << "\n\t"<<path << endl;

			//stores the path of the file to be deleted
			temp = *ptr->tree->search(decimal_sha);

			//for deletion fo a file
			ptr->tree->deletion(decimal_sha);

			//print the updated b tree
			cout << "\n\tThe updated B-Tree is: \n";
			ptr->tree->traverse();
			//cout << "here1\n";

			return;
		}
		else if (lessthan(tail->machinekey,key) && greaterthanequalto(t, key))
		{
			ptr = head;
			path += head->machinename;

			cout << "\n\tPath to reach the appropriate Machine is: \n";
			cout << "\n\t" << path << endl;


			//stores the path of the file to be deleted
		    temp = *ptr->tree->search(decimal_sha);
			ptr->tree->deletion(decimal_sha);

			//print the updated b tree
			cout << "\n\tThe updated B-Tree is: \n";
			ptr->tree->traverse();
			//cout << "here2\n";

			return;
		}
		else {
			cout << "\n\tno such machine exists\n";
			return;
		}

	}

	string returnhead() {
		return head->machinekey;
	}

	CLLNode* returnhead_ADD() {
		return head;
	}

	void insert(string val, string namemachine) {

		CLLNode* temp = new CLLNode;
		temp->machinekey = val;
		temp->next = nullptr;
		temp->machinename = namemachine;
		temp->machinepath += namemachine;

		if (tail == nullptr) {
			tail = temp;
			tail->next = temp;
			head = tail;
		}
		else {

			if (lessthan(temp->machinekey,head->machinekey)) {
				tail->next = temp;
				temp->next = head;
				head = temp;
				return;

			}

			if (head == tail && lessthan(tail->machinekey, temp->machinekey)) {
				temp->next = head;
				head->next = temp;
				tail = temp;
			}


			else {

				CLLNode* ptr = head;
				CLLNode* qtr = nullptr;

				while (ptr != tail && ptr->next != nullptr && lessthan(ptr->next->machinekey,temp->machinekey)) {
					ptr = ptr->next;
				}
				qtr = ptr->next;

				if (ptr == tail) {
					tail = temp;
				}

				temp->next = qtr;
				ptr->next = temp;


			}

		}

	}

	void setfolder() {

		CLLNode* ptr = head;
		while (ptr->next != head) {

			//wstring converts the given string into a wide string and  c_str obtains 
			//pointer to the data string that is compatible with the Create Directory(Windows function)
			if (CreateDirectory(std::wstring(ptr->machinepath.begin(), ptr->machinepath.end()).c_str(), NULL)) {

				cout << "\tMachine Folder created successfully." << endl;
			}
			else {
				cout << "\tFailed to create folder." << endl;
			}
			ptr = ptr->next;
		}
		//for the last machine
		if (CreateDirectory(std::wstring(ptr->machinepath.begin(), ptr->machinepath.end()).c_str(), NULL)) {

			cout << "\tMachine Folder created successfully." << endl;
		}
		else {
			cout << "\tFailed to create folder." << endl;
		}
	}

	bool isEmpty() {
		if (tail == nullptr) {
			return true;
		}
		return false;
	}

	void insertAtIndex(string val, int index) {
		CLLNode* temp = new CLLNode;
		temp->machinekey = val;
		temp->next = nullptr;

		if (tail == nullptr) {
			return;
		}
		else {
			CLLNode* ptr = tail->next;
			CLLNode* qtr = tail->next;
			int index = 0;
			do {
				index++;
				qtr = ptr;
				ptr = ptr->next;
			} while (index != index && ptr->next != tail->next);

			if (index == index) {
				temp->next = ptr;
				qtr->next = temp;
			}
		}
	}

	void deleteData(string val) {
		CLLNode* ptr = tail->next;
		CLLNode* qtr = tail->next;
		CLLNode* t = nullptr;

		if (!isEmpty()) {
			do {
				qtr = ptr;
				ptr = ptr->next;
			} while (ptr->machinekey != val && ptr->next != tail->next);

			if (ptr != nullptr && ptr->machinekey == val) {
				ptr = ptr->next;
				t = qtr->next;
				qtr->next = ptr;
				delete t;
			}
		}
	}

	void print() {
		CLLNode* ptr = head;

		cout << "\n\n";
		while (ptr != tail && ptr != nullptr)
		{
			cout << "\t" << ptr->machinename << "\t Key: " << ptr->machinekey << endl;
			ptr = ptr->next;
		}
		if (ptr != nullptr)
		{
			cout << "\t" << ptr->machinename << "\t Key: " << ptr->machinekey << endl;
		}
	}

	bool FileExist(string key)
	{
		CLLNode* ptr = head;

		while (ptr != tail && ptr != nullptr) {

			if (ptr->tree->doesKeyExist(key))
				return 1;

			ptr = ptr->next;
		}
		if (ptr != nullptr && ptr->tree != nullptr)
		{
			if (ptr->tree->doesKeyExist(key))
				return 1;
		}

		return 0;
	}

	void printroutingtable(string machinenum) {

		CLLNode* ptr = head;
		do {
			ptr = ptr->next;
		} while (ptr->next != head && ptr->machinekey != machinenum);
		cout << endl;
		if (ptr->machinekey != machinenum) {
			cout << "\tNo such machine exists";
			return;
		}
		fingertablenode* qtr = ptr->ftable->head;
		while (qtr != nullptr) {
			cout << "\tNode number: " << qtr->id << "   Points to: " << qtr->machine->machinename << endl;
			qtr = qtr->next;
		}

	}

	void PrintAllRoutingTable()
	{
		CLLNode* ptr = head;
		fingertablenode* qtr = ptr->ftable->head;
		while (ptr->next != head)
		{
			cout << "\n\tMachine: " << ptr->machinekey << endl;
			qtr = ptr->ftable->head;
			while (qtr != nullptr) {
				if (qtr->machine != nullptr)
					cout << "\t\tNode number: " << qtr->id << "   Points to: " << qtr->machine->machinekey << endl;
				qtr = qtr->next;
			}
			ptr = ptr->next;
		}

		cout << "\n\tMachine: " << ptr->machinekey << endl;
		qtr = ptr->ftable->head;
		while (qtr != nullptr) {
			if (qtr->machine != nullptr)
				cout << "\t\tNode number: " << qtr->id << "   Points to: " << qtr->machine->machinekey << endl;
			qtr = qtr->next;
		}
	}

	void printBTree(string machinenum) {

		CLLNode* ptr = head;
		do {
			ptr = ptr->next;
		} while (ptr->next != head && ptr->machinekey != machinenum);

		if (ptr->machinekey != machinenum) {
			ptr = ptr->next;
		}

		if (ptr->machinekey != machinenum) {
			cout << "\tNo such machine exists";
			return;
		}

		ptr->tree->traverse();   //prints the entire b tree for the specified machine
	}
	//not used
	bool search(string val, string machinenum) {

		CLLNode* ptr = head;
		while (ptr->machinekey != machinenum && ptr != tail) {
			ptr = ptr->next;           //find the required machine for request
		}
		//case 1
		if (ptr->machinekey == val) {
			//find value in the btree
			cout << ptr->machinekey << endl;

			return true;
		}
		//case 2
		else if (lessthan(ptr->machinekey, val) &&  greaterthanequalto(ptr->ftable->head->machine->machinekey, val)) {
			ptr = ptr->ftable->head->machine;

			//find value in the btree
			cout << ptr->machinekey << endl;

			return true;
		}
		//case 3
		else {
			fingertablenode* qtr = ptr->ftable->head;

			while (qtr->next != nullptr) {
				if (lessthan(qtr->machine->machinekey, val) && greaterthanequalto(qtr->next->machine->machinekey, val)) {
					ptr = qtr->machine; //jump to next machine
					cout << ptr->machinekey << endl;
					return search(val, ptr->machinekey);
					break;
				}
				qtr = qtr->next;
			}
			cout << qtr->machine->machinekey << endl;
			return search(val, qtr->machine->machinekey);   //for search on the last value of ft

		}


		return false;
	}

	void SearchFileByFT(string machinenamee, string filemodkey, string filedeckey, int identifierspace)
	{
		CLLNode* ptr = head;

		while (ptr->machinename != machinenamee && ptr != tail) {
			ptr = ptr->next;           //find the required machine for request
		}
		if ( greaterthanequalto(filemodkey,tail->machinekey) || lessthanequalto(filemodkey,head->machinekey) )
		{
			bool find = head->tree->doesKeyExist(filedeckey);
			if (find) {

				cout << "\n\tFile exists on " << head->machinename << endl;
			}
			else {
				cout << "\n\tFile does not exist\n";
			}
			return;
		}
		//case 1
		if (equaltoequal(ptr->machinekey, filemodkey)) {
			//	//find value in the btree
			//	cout << "File exists on " << ptr->machinename << endl;


			bool find = ptr->tree->doesKeyExist(filedeckey);
			if (find) {

				cout << "\n\tFile exists on " << ptr->machinename << endl;
			}
			else {
				cout << "\n\tFile does not exist\n";
			}
			//call btree- with filedeckey----------------------------------------------------------------------------------------------------------------------------------------------------
			return;
		}
		//case 2
		else if (lessthan(ptr->machinekey,filemodkey) &&  greaterthanequalto(ptr->ftable->head->machine->machinekey, filemodkey))
		{
			ptr = ptr->ftable->head->machine;
			//	cout << "File exists on " << ptr->machinename << endl;

				//find value in the btree
			bool find = ptr->tree->doesKeyExist(filedeckey);
			if (find) {

				cout << "\n\tFile exists on " << ptr->machinename << endl;
			}
			else {
				cout << "\n\tFile does not exist\n";
			}
			return;
		}
		else if (lessthan(tail->machinekey, filemodkey) &&  greaterthanequalto(power(identifierspace), filemodkey))
		{
			//cout << "File exists on " << ptr->machinename << endl;


			//find value in the btree
			bool find = ptr->tree->doesKeyExist(filedeckey);
			if (find) {

				cout << "\n\tFile exists on " << ptr->machinename << endl;
			}
			else {
				cout << "\n\tFile does not exist\n";
			}
			//call btree- with filedeckey----------------------------------------------------------------------------------------------------------------------------------------------------
			return;
		}
		//case 3
		else
		{
			fingertablenode* qtr = ptr->ftable->head;

			while (qtr->next != nullptr) {
				if (lessthan(qtr->machine->machinekey,filemodkey) && greaterthanequalto(qtr->next->machine->machinekey, filemodkey))
				{
					ptr = qtr->machine; //jump to next machine
					cout << ptr->machinename << " -> ";

					return SearchFileByFT(ptr->machinekey, filemodkey, filedeckey, identifierspace);
					break;
				}
				qtr = qtr->next;
			}
			cout << qtr->machine->machinename << " -> ";

			return SearchFileByFT(qtr->machine->machinekey, filemodkey, filedeckey, identifierspace);   //for search on the last value of ftn
		}
	}

	void ClearFingerTables()
	{
		CLLNode* ptr = head;
		while (ptr->next != head)
		{
			ptr->ftable->Delete_FingerTable();
			ptr->ftable = nullptr;
			ptr = ptr->next;
		}
		tail->ftable->Delete_FingerTable();
		tail->ftable = nullptr;
	}

	void DeleteMachine()
	{

	}
	
	string insert_file_routingtable(string key, string filename, string sha, string machine_number, int identifierspace, string& path) 
	{  
		//using search of routing table
		//key = file key moded to be inserted machinenumber machine which thas been requested
		//sha dec key of file (not mod)
		//temp is the file path
		//filename is name of file

		string t = power(identifierspace); 
		//if request arrives on the first machine 
		CLLNode* ptr = head;

		if (ptr->machinekey != machine_number) {
			//first go to the machine where the request arrives
			do {
	    		ptr = ptr->next;
			} while (ptr->machinekey != machine_number && ptr->next != head);
		}

		//case 1
		if (equaltoequal(ptr->machinekey, key)) {   //if key number matches that of the machine
			path += ptr->machinename;
			cout << "\n\tPath to reach the appropriate Machine is: \n";
			cout << "\n\t" << path << endl;

			string temp = ptr->machinepath + "/"; 
			temp += filename; //complete path of the file
			ptr->tree->insert(sha, temp, filename); 
			return temp; 
		  
		}
		//case 2
		else if (lessthan(ptr->machinekey, key)&& greaterthanequalto(ptr->ftable->head->machine->machinekey, key))
		{
		    path += ptr->machinename;
			path += " -> ";
			ptr = ptr->ftable->head->machine;
			//	cout << "File exists on " << ptr->machinename << endl;

				//find value in the btree
	        	path += ptr->machinename;
				cout << "\n\tPath to reach the appropriate Machine is: \n";
				cout << "\n\t" << path << endl;

				string temp = ptr->machinepath + "/";
				temp += filename; //complete path of the file
				ptr->tree->insert(sha, temp, filename);
				return temp;
	
		}
		//case 3
		else
		{
			fingertablenode* qtr = ptr->ftable->head;
			path += ptr->machinename;
			while (qtr->next != nullptr) {
				if (lessthan(qtr->machine->machinekey, key) && greaterthanequalto(qtr->next->machine->machinekey, key))
				{
				if(!equaltoequal(qtr->next->machine->machinekey, key))
				{
					ptr = qtr->machine; //jump to next machine
					path += " -> ";
					path += ptr->machinename;
					path += " -> ";
				}
				else {
					ptr = qtr->next->machine;
					path += " -> ";
					
				}
					return insert_file_routingtable(key,filename, sha, ptr->machinekey, identifierspace, path);
					break;
				}
				qtr = qtr->next;
			}
	
			//if the key is greater than the largest machine, it is inserted on the first machine
			if (lessthan(tail->machinekey, key) || lessthan(key, head->machinekey)) {
				cout << "\n\tPath to reach the appropriate Machine is: \n";
				cout << "\n\t" << path << endl;

				string temp = ptr->machinepath + "/";
				temp += filename; //complete path of the file
				ptr->tree->insert(sha, temp, filename);
				return temp;
			}


			path += " -> ";
		//	ptr = qtr->machine; //jump to next machine
			//path += ptr->machinename;
			//path += " -> ";
			return insert_file_routingtable(key,filename, sha, qtr->machine->machinekey, identifierspace, path);   //for search on the last value of ftn
		}

	}

	string insert_file_btree(string key, string filename, string sha, string machine_number, int identifierspace) 
	{

		//key = file key moded to be inserted machinenumber machine which thas been requested
		//sha dec key of file

		string path = "";
		string t = power(identifierspace);
		//if request arrives on the first machine
		CLLNode* ptr = head;

		if (ptr->machinekey != machine_number) {
			//first go to the machine where the request arrives
			do {
				ptr = ptr->next;
			} while (ptr->machinekey != machine_number && ptr->next != head);
		}

		if (ptr->machinekey != machine_number) {
			cout << "\n\tYour entered machine does not exist\n";
			return "";
		}
		//search through to all machines until an appropriate machine is found for insertion
		while (lessthan(ptr->machinekey,key) && ptr->next != head) {
			path += ptr->machinename + "-> ";
			ptr = ptr->next;
		}

		//insert file into the btree of the found machine
		if (greaterthanequalto(ptr->machinekey, key)) {

			path += ptr->machinename;
			cout << "\n\tPath to reach the appropriate Machine is: \n";
			cout <<"\n\t" << path << endl;

			string temp = ptr->machinepath + "/" + filename; //complete path of the file
			ptr->tree->insert(sha, temp, filename);
			return temp;
		}
		else if (lessthan(tail->machinekey, key) && greaterthanequalto(t, key))
		{
			ptr = head;
			path += head->machinename;

			cout << "\n\tPath to reach the appropriate Machine is: \n";
			cout <<"\n\t" << path << endl;

			string temp = ptr->machinepath + "/" + filename; //complete path of the file
			ptr->tree->insert(sha, temp, filename);
			return temp;
		}
		else {
			cout << "\n\tno such machine exists\n";
			return "";
		}

	}

	CLLNode* AddMachine(string key, string machineName, int order, int identifierspace)
	{
		ClearFingerTables();//clears all Routing tables
		insert(key, machineName);
		setfingertable(identifierspace, identifierspace);

		CLLNode* ptr = head;
		while (ptr->next != head)
		{
			if (ptr->machinekey == key)
				break;

			ptr = ptr->next;
		}
		if (tail->machinekey == key)
			ptr = tail;

		if (ptr->machinekey == key)
		{
			//sets btree 
			ptr->tree = new BTree(order);
			//create a folder
			if (CreateDirectory(std::wstring(ptr->machinepath.begin(), ptr->machinepath.end()).c_str(), NULL)) {

				cout << "\n\tFolder created successfully." << endl;
			}
			else {
				cout << "\n\tFailed to create folder." << endl;
			}

			CLLNode* qtr = ptr->next;
			deleteBTree(qtr, identifierspace,order);//deletes all files from the succ node

			return ptr;
		}
		else
		{
			cout << "Machine not added\n";
		}
	}
	void InsertDisplacedFiles(string** files, int size, int identifierspace)
	{
		Record r;
		while (size >= 0)
		{
			string modfilekey = Modulus(files[0][size], power(identifierspace));
	
			r.deleteFile(files[1][size]);
			string path = insert_file_btree(modfilekey, files[2][size], files[0][size], head->machinekey, identifierspace);
			r.createfile(files[1][size], path);
			// Convert narrow string to wide string
			
			size--;
		}

	}

	void deleteBTree(CLLNode *qtr, int identifierspace,int order)
	{
		BTree tree = *qtr->tree;

		string* K = new string[100];
		string* V = new string[100];
		string* N = new string[100];
		string temp = " ";
		int i = 0;
		int j = 0;
		string key = "";
		string value = "";
		string name = "";
		while (tree.root != nullptr)
		{
			key = tree.root->keys[0];
			value = tree.root->values[0];
			name = tree.root->names[0];
			K[i] = key;
			V[i] = value;
			N[i] = name;
			temp = key;
			i++;
			tree.deletionSingle(key);
		}

		string** result = new string * [3];
		result[0] = new string[i];
		result[1] = new string[i];
		result[2] = new string[i];



		// Copy keys and values to the result array
		for (int j = 0; j < i; ++j) {
			result[0][j] = K[j];
			result[1][j] = V[j];
			result[2][j] = N[j];
		}

		qtr->tree = new BTree(order);

		//to insert all the displaced files
		InsertDisplacedFiles(result, i-1, identifierspace);

		delete[] K;
		delete[] V;
		delete[] N;
		for (int j = 0; j < i; ++j)
			delete[] result[j];
		delete[] result;
	}

	~CircularLinkedList()
	{

		if (tail != nullptr)
		{
			CLLNode* current = tail->next;
			CLLNode* garbage;

			while (current != tail) {
					cout << "CircularLinkedList Destructor\n";
				garbage = current;
				current = current->next;

				garbage->ftable->Delete_FingerTable();
				delete garbage->ftable;
				garbage->ftable = nullptr;

				garbage->tree->DeleteBTree();
				delete garbage->tree;
				garbage->tree = nullptr;

				delete garbage;
			}
			tail->ftable->Delete_FingerTable();
			delete tail->ftable;
			tail->ftable = nullptr;

			tail->tree->DeleteBTree();
			delete tail->tree;
			tail->tree = nullptr;

			delete tail;

			tail = nullptr;
			garbage = nullptr;
			current = nullptr;
		}
	}
};


string calculateSHA1(const string& data)
{
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;

	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
		if (CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) {
			if (CryptHashData(hHash, reinterpret_cast<const BYTE*>(data.c_str()), data.length(), 0)) {
				DWORD hashSize = 0;
				DWORD dataSize = sizeof(DWORD);
				if (CryptGetHashParam(hHash, HP_HASHSIZE, reinterpret_cast<BYTE*>(&hashSize), &dataSize, 0)) {
					vector<BYTE> hashBuffer(hashSize, 0);
					if (CryptGetHashParam(hHash, HP_HASHVAL, hashBuffer.data(), &hashSize, 0)) {
						string hashString;
						for (BYTE byte : hashBuffer) {
							char hexChar[3];
							snprintf(hexChar, sizeof(hexChar), "%02x", byte);
							hashString += hexChar;
						}
						CryptDestroyHash(hHash);
						CryptReleaseContext(hProv, 0);
						return hashString;
					}
				}
			}
			CryptDestroyHash(hHash);
		}
		CryptReleaseContext(hProv, 0);
	}

	return ""; // Error occurred
}




#endif