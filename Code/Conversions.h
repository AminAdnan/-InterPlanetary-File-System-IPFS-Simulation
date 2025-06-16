#pragma once
#ifndef CONVERSIONS_H_
#define CONVERSIONS_H_

#include<iostream>
using namespace std;




template<typename T>
class node {
public:
	node* next;
	T data;
	node() {
		next = nullptr;
		data = 0;
	}
};

template<typename T>
class Stack {
public:
	node<T>* top;
	Stack() {
		top = nullptr;
	}
	void push(T d) {
		node<T>* temp = new node<T>;
		temp->data = d;
		temp->next = nullptr;
		if (top == nullptr) {
			top = temp;
		}
		else {
			temp->next = top;
			top = temp;
		}
	}
	void pop() {
		if (top != nullptr) {
			node<T>* temp = top;
			top = top->next;
			delete temp;
			temp = nullptr;
		}
	}
	T peek() {
		if (top != nullptr) {
			return top->data;
		}
		else {
			return -1;
		}
	}
	void clear() {
		while (top != nullptr) {
			node<T>* temp = top;
			top = top->next;
			delete temp;
			temp = nullptr;
		}
	}
	bool empty() {
		if (top == nullptr) {
			return true;
		}
		return false;
	}

};

void Addition(string str1, string str2, int& carry, Stack<int>& s);

string hex_binary(string hex, int identifier)
{
	//max hex in 40 digit is FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF (40 f's) == 160 bits 
	if (identifier <= 0)
		return " ";

	char* binary = new char[160];//10 array is 0-9

	int i = hex.length() - 1; //hex string index
	int j = 160; //binary array index
	int temp = 0;
	while (j >= 0 && i >= 0)
	{
		if (hex[i] >= '0' && hex[i] <= '9')
		{
			temp = hex[i] - '0';//int 
			//9 1001
			binary[--j] = temp % 2 + '0';
			temp /= 2;
			binary[--j] = temp % 2 + '0';
			temp /= 2;
			binary[--j] = temp % 2 + '0';
			binary[--j] = (temp /= 2) + '0';
		}
		else if (hex[i] == 'A' || hex[i] == 'a')
		{
			//1010
			binary[--j] = '0';
			binary[--j] = '1';
			binary[--j] = '0';
			binary[--j] = '1';
		}
		else if (hex[i] == 'B' || hex[i] == 'b')
		{
			//1011
			binary[--j] = '1';
			binary[--j] = '1';
			binary[--j] = '0';
			binary[--j] = '1';
		}
		else if (hex[i] == 'C' || hex[i] == 'c')
		{
			//1100
			binary[--j] = '0';
			binary[--j] = '0';
			binary[--j] = '1';
			binary[--j] = '1';
		}
		else if (hex[i] == 'D' || hex[i] == 'd')
		{
			//1101
			binary[--j] = '1';
			binary[--j] = '0';
			binary[--j] = '1';
			binary[--j] = '1';
		}
		else if (hex[i] == 'E' || hex[i] == 'e')
		{
			//1110
			binary[--j] = '0';
			binary[--j] = '1';
			binary[--j] = '1';
			binary[--j] = '1';
		}
		else if (hex[i] == 'F' || hex[i] == 'f')
		{
			binary[--j] = '1';
			binary[--j] = '1';
			binary[--j] = '1';
			binary[--j] = '1';
		}
		i--;
	}

	//binary string
	string bin = "";
	i = 0;
	//i = 160 - identifier;//160 - 25 = 135  
	//i--;//pick bits from 134 to 159

	while (i <= 159)
	{
		bin += binary[i];
		i++;
	}
	delete binary;
	return bin;
}

string bin_dec(string bin, int identifier_space)
{
	int bits = bin.length(); //no of bits to convert
	int i = bin.length() - 1;//binary index

	string answer = "0";
	string tempstr = "1";

	int carry = 0;
	Stack <int>s;

	for (int j = 0; j < bits; j++)//traversing backwards
	{
		if (j != 0)//first iteration has been done
		{
			Addition(tempstr, tempstr, carry, s);//2^9 + 2^10
			tempstr = "";
			while (!s.empty()) {
				tempstr += (s.peek() + '0');
				// cout << "tempstr: " << temp << endl;
				s.pop();
			}
		}
		// i = bin.length()-1
		if (bin[i] == '0')
		{
			i--; //skip if binary has a zero
			continue;
		}
		else
		{
			Addition(answer, tempstr, carry, s);//2^9 + 2^10
			answer = "";
			while (!s.empty()) {
				answer += (s.peek() + '0');
				// cout << "ans: " << temp << endl;
				s.pop();
			}
			i--;
		}

	}

	return answer;
}

string power(int identifierspace)
{
	//2^4  ->  add 2, 8 times == 16
	string tempstr = "2";
	int carry = 0;
	Stack <int>s;

	for (int i = 1; i < identifierspace; i++)
	{
		Addition(tempstr, tempstr, carry, s);//2^9 + 2^10
		tempstr = "";
		while (!s.empty()) {
			tempstr += (s.peek() + '0');
			// cout << "tempstr: " << temp << endl;
			s.pop();
		}
	}
	return tempstr;
}

void Addition(string str1, string str2, int& carry, Stack<int>& s)
{
	int aSize = str1.length() - 1;
	int bSize = str2.length() - 1;

	char* a = new char[aSize + 1];
	for (int j = 0; j < aSize + 1; j++)
		a[j] = '0';

	int i = 0;
	while (str1[i] != '\0')
	{
		a[i] = str1[i];
		i++;
	}


	char* b = new char[bSize + 1];
	for (int j = 0; j < bSize + 1; j++)
		b[j] = '0';

	i = 0;
	while (str2[i] != '\0')
	{
		b[i] = str2[i];
		i++;
	}


	i = 0;
	int temp = 0;
	int greater = 0;

	if (aSize >= bSize)
	{
		greater = aSize;
		i = bSize;
		while (i >= 0)
		{
			temp = (a[greater] - '0') + (b[i] - '0') + carry;
			carry = temp / 10;
			temp %= 10;

			s.push(temp);
			i--;
			greater--;
		}

		while (greater >= 0)
		{
			temp = (a[greater] - '0') + carry;
			carry = temp / 10;
			temp %= 10;

			s.push(temp);
			greater--;
		}
	}
	else
	{
		greater = bSize;
		i = aSize;
		while (i >= 0)
		{
			temp = (a[i] - '0') + (b[greater] - '0') + carry;
			carry = temp / 10;
			temp %= 10;

			s.push(temp);
			i--;
			greater--;
		}

		while (greater >= 0)
		{
			temp = (b[greater] - '0') + carry;
			carry = temp / 10;
			temp %= 10;

			s.push(temp);
			greater--;
		}
	}

	//Remaining carry
	while (carry > 0)
	{
		temp = carry % 10;
		s.push(temp);
		carry /= 10;
	}

	delete[] a;
	delete[] b;
}

bool greaterthanequalto(string a, string b)
{
	int aSize = a.length();
	int bSize = b.length();

	if (aSize < bSize)
		return 0;

	if (aSize > bSize)
		return 1;

	for (int i = 0; i < aSize; i++)
	{
		if (a[i] < b[i])
			return 0;

		else if (a[i] > b[i])
			return 1;
	}
	return 1;
}

bool equaltoequal(string a, string b)
{
	int aSize = a.length();
	int bSize = b.length();

	if (aSize < bSize)
		return 0;

	if (aSize > bSize)
		return 0;

	for (int i = 0; i < aSize; i++)
	{
		if (a[i] < b[i])
			return 0;

		else if (a[i] > b[i])
			return 0;
	}
	return 1;
}

bool lessthanequalto(string& a, string& b)
{
	int aSize = a.length();
	int bSize = b.length();

	if (aSize > bSize)
		return false;

	if (aSize < bSize)
		return 1;

	for (int i = 0; i < aSize; i++)
	{
		if (a[i] < b[i])
			return true;

		else if (a[i] > b[i])
			return false;
	}
	return true;
}

bool lessthan(string& a, string& b)
{
	int aSize = a.length();
	int bSize = b.length();

	if (aSize > bSize)
		return false;

	if (aSize < bSize)
		return 1;

	for (int i = 0; i < aSize; i++)
	{
		if (a[i] < b[i])
			return true;

		else if (a[i] > b[i])
			return false;
	}
}
	
void Subtract(string& str1, string& str2, int& borrow, Stack<int>& s)
{
	int aSize = str1.length();
	int bSize = str2.length();

	if (aSize < bSize)
	{
		cout << "Negative answer\n";
		return;
	}

	if (aSize == bSize && str1 == str2)
	{
		s.push(0);
		return;
	}

	int i = 0;
	int temp = 0;
	int greater = aSize - 1;

	// Create arrays to hold the digits of the strings
	int* a = new int[aSize] {0};
	int* b = new int[bSize] {0};

	// Convert characters to integers and store in arrays
	for (i = 0; i < aSize; ++i)
	{
		a[i] = str1[i] - '0';
	}

	for (i = 0; i < bSize; ++i)
	{
		b[i] = str2[i] - '0';
	}

	i = bSize - 1;


	while (i >= 0)
	{
		temp = a[greater] - b[i] - borrow;
		if (temp < 0)
		{
			borrow = 1;
			temp = (temp + 10) % 10;
		}
		else
			borrow = 0;

		s.push(temp);
		i--;
		greater--;
	}

	while (greater >= 0)
	{
		temp = a[greater] - borrow;
		if (temp < 0)
		{
			borrow = 1;
			temp = (temp + 10) % 10;
		}
		else
			borrow = 0;

		s.push(temp);
		greater--;
	}

	while (s.peek() == 0)
		s.pop();

	// Remaining borrow
	while (borrow > 0)
	{
		temp = borrow % 10;
		s.push(temp);
		borrow /= 10;
	}

	delete[] a;
	delete[] b;
}

string Multiply(string str1, string str2)
{
	if (str1 == "0" || str2 == "0")
	{
		return "0";
	}

	int aSize = str1.length() - 1;
	int bSize = str2.length() - 1;

	char* a = new char[aSize + 1];
	for (int j = 0; j < aSize + 1; j++)
		a[j] = '0';

	int i = 0;
	while (str1[i] != '\0')
	{
		a[i] = str1[i];
		i++;
	}

	char* b = new char[bSize + 1];
	for (int j = 0; j < bSize + 1; j++)
		b[j] = '0';

	i = 0;
	while (str2[i] != '\0')
	{
		b[i] = str2[i];
		i++;
	}

	i = bSize;
	int temp = 0;

	Stack<int> s;
	int carry = 0;

	string t1 = "";
	string t2 = "";
	string ans = "";
	int c = 0;

	while (i >= 0) //lower number 7
	{
		for (int j = aSize; j >= 0; j--) //upper number 123
		{
			temp = (a[j] - '0') * (b[i] - '0') + carry;
			carry = temp / 10;
			temp %= 10;

			s.push(temp);
		}
		if (carry != 0)
			s.push(carry);

		t1 = "";//           240
		while (!s.empty())
		{
			t1 += (s.peek() + '0');
			s.pop();
		}
		for (int k = i; k < bSize; k++)
		{
			//no of zeros asize - i 
			t1 += "0";
		}
		t2 = "";
		Addition(ans, t1, c, s);
		while (!s.empty())
		{
			t2 += (s.peek() + '0');//240
			s.pop();
		}
		ans = t2;
		t1 = "";
		i--;
		carry = 0;
	}

	delete[] a;
	delete[] b;

	return ans;
}

string decrement(string str)
{
	if (str == "0")
		return "0";

	int borrow = 0;
	Stack<int> s;
	string t = "1";
	Subtract(str, t, borrow, s);
	string answer = "";
	while (!s.empty()) {
		answer += (s.peek() + '0');
		// cout << "ans: " << temp << endl;
		s.pop();
	}
	return answer;
}

string Modulus(string str1, string str2)
{
	if (str1 == "0")
		return "0";

	if (str2 == "0")
		return "Math Error";

	int aSize = str1.length() - 1;
	int bSize = str2.length() - 1;

	int borrow = 0;
	Stack<int> s;
	int temp = 0;
	string temp1 = "";
	string t1 = "";
	string multiple = "";

	int i = 0;
	while (i <= aSize)
	{
		//str1 12345  str2 71
		for (; lessthanequalto(t1, str2) && i <= aSize; i++)
		{
			//finding a's digits less than b's size
			t1 += str1[i];// t1 = 123  ,  524  ,  275
		}
		multiple = "9";
		do
		{
			temp1 = Multiply(multiple, str2); //temp1 = 71  ,  497  ,  213
			multiple = decrement(multiple);

		} while (lessthanequalto(t1, temp1));

		borrow = 0;

		if (t1.length() < temp1.length())
			cout << i << endl;

		Subtract(t1, temp1, borrow, s);
		t1 = "";

		while (!s.empty()) {
			t1 += (s.peek() + '0');
			s.pop();
		}
		//t1 = 52  ,  27  ,  62
	}
	return t1;
}

string increment(string str)
{
	int carry = 0;
	Stack<int> s;
	string t = "1";
	Subtract(str, t, carry, s);
	string answer = "";
	while (!s.empty()) {
		answer += (s.peek() + '0');
		// cout << "ans: " << temp << endl;
		s.pop();
	}
	return answer;
}

string GenerateKey(string sha, int identifierspace)
{
	string binary = hex_binary(sha, identifierspace);

	string decimal = bin_dec(binary, identifierspace);

	string divisor = power(identifierspace);

	string key = Modulus(decimal, divisor);

	return key;
}

string GenerateFileKey(string sha, int identifierspace)
{
	string binary = hex_binary(sha, identifierspace);

	string decimal = bin_dec(binary, identifierspace);

	return decimal;
}

#endif