#pragma once
#include <fstream>
#include <sstream>
using namespace std;

class FileInputRef
{
	ifstream fin;
	bool opened;
public:
	FileInputRef(const string& filename, bool storage = false);
	~FileInputRef();

	bool IsOpened();

	int ReadInt();
	float ReadFloat();
	double ReadDouble();
	string ReadString();
	string ReadSubstring(char delim = '\n');
	char ReadChar();
	string ReadToEnd();

	void Close();
};

class FileOutputRef
{
	ofstream fout;
	bool opened;
public:
	FileOutputRef(const string&  filename);
	~FileOutputRef();

	void WriteInt(int data);
	void WriteFloat(float data);
	void WriteDouble(double data);
	void WriteString(string data);
	void WriteChar(char data);

	void Close();
};



class SecureFileInputRef
{
	stringstream fin;
	bool opened;
public:
	SecureFileInputRef(string filename);
	~SecureFileInputRef();

	bool IsOpened();

	int ReadInt();
	float ReadFloat();
	double ReadDouble();
	char ReadChar();
	string ReadToEnd();

	void Close();
};

class SecureFileOutputRef
{
	ofstream fout;
	stringstream sout;
	string _filename;
	bool opened;
	bool _secure;

public:
	SecureFileOutputRef(string filename, bool secure = false);
	~SecureFileOutputRef();

	void WriteInt(int data);
	void WriteFloat(float data);
	void WriteDouble(double data);
	void WriteChar(char data);

	void Close();
};
