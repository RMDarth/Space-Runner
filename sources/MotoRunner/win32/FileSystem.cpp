#define _CRT_SECURE_NO_WARNINGS
#include "FileSystem.h"
#include <fstream>
#include <sstream>

FileInputRef::FileInputRef(string filename, bool storage)
{
	fin.open(filename, ios::in);

	opened = false;
	if (fin)
		opened = true;
}
FileInputRef::~FileInputRef()
{
	if (opened)
		fin.close();
}

int FileInputRef::ReadInt()
{
	int data;
	fin >> data;
	
	return data;
}

float FileInputRef::ReadFloat()
{
	float data;
	fin >> data;

	return data;
}

double FileInputRef::ReadDouble()
{
	double data;
	fin >> data;
	
	return data;
}

string FileInputRef::ReadString()
{
	int length;
	fin >> length;
	char *str = new char[length + 2];
	fin.getline(str, length + 2);
	char tmp[256];
	int curlength;

	strcpy(str, "");
	do {
		fin.getline(tmp, length + 2);
		strcat(str, tmp);
		curlength = strlen(str);
		if (curlength < length)
			strcat(str, "\n");
	} while (curlength < length && !fin.eof());

	string data = str;
	delete[] str;

	return data;
}

char FileInputRef::ReadChar()
{
	char data;
	fin >> data;

	return data;
}

string FileInputRef::ReadToEnd()
{
	char ch;
	stringstream stream;
	while( fin.get(ch) )
	{
       stream << ch;
	}
	string data = stream.str();

	return data;
}

bool FileInputRef::IsOpened()
{
	return opened;
}

void FileInputRef::Close()
{
	fin.close();
	opened = false;
}



FileOutputRef::FileOutputRef(string filename)
{
	fout.open(filename, ios::out);
	opened = true;

	stringstream str;
	
}

FileOutputRef::~FileOutputRef()
{
	if (opened)
		fout.close();
}


void FileOutputRef::WriteInt(int data)
{
	fout << data << ' ';
}

void FileOutputRef::WriteFloat(float data)
{
	fout << data << ' ';  
}

void FileOutputRef::WriteDouble(double data)
{
	fout << data << ' ';
}

void FileOutputRef::WriteString(string data)
{
	fout << data.size() << endl;
	fout << data << endl;
}

void FileOutputRef::WriteChar(char data)
{
	fout << data << ' ';
}


void FileOutputRef::Close()
{
	fout.close();
	opened = false;
}


/// SECURE

const char* cypherDecypther(const char* data, int len)
{
	char key[5] = { 'P', 'S', 'q', 'R', '2' }; //Any chars will work
	char* finish = new char[len];

	for (int i = 0; i < len; i++)
		finish[i] = data[i] ^ key[i % (sizeof(key) / sizeof(char))] ^ i;

	return finish;
}

const char* encryptDecryptFile(string file, long &size)
{
	ifstream fin(file, ios::in | ios::binary);

	if (!fin)
	{
		size = -1;
		return nullptr;
	}

	// get size of file
	fin.seekg(0, fin.end);
	size = fin.tellg();
	fin.seekg(0);

	char* buffer = new char[size];
	fin.read(buffer, size);

	// write to outfile
	const char* result = cypherDecypther(buffer, size);
	delete[] buffer;
	
	return result;
}


SecureFileInputRef::SecureFileInputRef(string filename)
{
	opened = false;
	
	long size;
	const char* decrypt = encryptDecryptFile(filename, size);
	if (size < 0)
	{
		return;
	}


	fin.write(decrypt, size);
	delete[] decrypt;
	
	opened = true;
}

SecureFileInputRef::~SecureFileInputRef()
{
	if (opened)
		Close();
}

int SecureFileInputRef::ReadInt()
{
	int data;
	fin.read((char*)&data, sizeof(data));

	return data;
}

float SecureFileInputRef::ReadFloat()
{
	float data;
	fin.read((char*)&data, sizeof(data));

	return data;
}

double SecureFileInputRef::ReadDouble()
{
	double data;
	fin.read((char*)&data, sizeof(data));

	return data;
}

char SecureFileInputRef::ReadChar()
{
	char data;
	fin.read((char*)&data, sizeof(data));

	return data;
}

string SecureFileInputRef::ReadToEnd()
{
	return fin.str();
}

bool SecureFileInputRef::IsOpened()
{
	return opened;
}

void SecureFileInputRef::Close()
{
	opened = false;
}



SecureFileOutputRef::SecureFileOutputRef(string filename, bool secure)
{
	fout.open(filename, ios::out | ios::binary);
	opened = true;
	_filename = filename;
}

SecureFileOutputRef::~SecureFileOutputRef()
{
	if (opened)
		fout.close();
}


void SecureFileOutputRef::WriteInt(int data)
{
	fout.write((char*)&data, sizeof(data));
}

void SecureFileOutputRef::WriteFloat(float data)
{
	fout.write((char*)&data, sizeof(data));
}

void SecureFileOutputRef::WriteDouble(double data)
{
	fout.write((char*)&data, sizeof(data));
}

void SecureFileOutputRef::WriteChar(char data)
{
	fout.write((char*)&data, sizeof(data));
}


void SecureFileOutputRef::Close()
{
	fout.close();
	long size;
	const char* encrypt = encryptDecryptFile(_filename, size);
	
	fout.open(_filename, ios::out | ios::binary);
	fout.write(encrypt, size);
	fout.close();

	delete[] encrypt;
	opened = false;
}
