#include <fstream>
#include <vector>
#include "iostream"
#include <ctime>
using namespace std;

class Files
{
private:
	vector<char> _data;
	fstream _read;
	fstream _write;
public:
	string	_path;
	Files(string &path)
		:_path(path)
	{

	}
	~Files()
	{

	}
	bool read()
	{
		_read.open(_path, ios::in  | ios::binary);//open as binary to read
		if (_read.is_open())
		{
			char temp;
			while (!_read.eof())
			{
				_read.get(temp);
				if (_read.eof()) break;
				_data.push_back(temp);			
			}
			_read.close();
			return false;
		}	
		else
			return true;
	}
	bool WriteData()
	{
		_write.open(_path, ios::binary | ios::out | ios::trunc);//open as binary to rewrite file
		char temp;
		if (_write.is_open())
		{
			for (unsigned int i = 0; i < _data.size(); i++)
			{
				temp = _data.at(i);
				_write << temp;
			}
			_write.close();
			return false;
		}
		else
			return true;
	}
	void KeyGen(int psize)// generate key
	{
		srand(time(NULL));
		for (int i = 0; i < psize; i++)
		{
			_data.push_back(0 + rand() % 255);
		}
	}
	vector<char> &GetData() { return _data; }
};
void error(string &path)//error message
{
		cout << "no such file or directory:" <<path.c_str() << endl;
		cin.get();
}
void encode(Files &file, Files &key, Files &cfile)//encode file with key
{
	cfile.GetData().clear();
	for (unsigned int i = 0; i < file.GetData().size(); i++)
		cfile.GetData().push_back(file.GetData().at(i) ^ key.GetData().at(i));
	if (cfile.WriteData())
	{
		error(cfile._path);
		cin.get();
		return;
	}
}
void decode(Files &cfile, Files &key, Files &pfile)
{
	return encode(cfile, key, pfile);
}
void main()
{
	string path = "c:/test/plain.docx";
	string path2 = "c:/test/keykey.docx";
	string path3 = "C:/test/ciphercipher.docx";
	string path4 = "C:/test/plainplain.docx";
	Files pfile(path)
		, keyfile(path2)
		, cfile(path3)
		, cfile2(path4);
	if (pfile.read())
	{
		error(pfile._path);
		return;
	}
	keyfile.KeyGen(pfile.GetData().size());
	keyfile.WriteData();
	keyfile.GetData().clear();
	if (keyfile.read())
	{
		error(keyfile._path);
		return;
	}
	encode(pfile, keyfile,cfile);
	decode(cfile, keyfile, cfile2);
	//FILE * file;
	// fopen_s(&file,path4.c_str(), "rb");
	// if (file == NULL)
	//	 cout << "whoopsie";
	//vector<char> da(100);
	//fread(da.data(), 1, 100, file);
	//for each ( char i in da )
	//{
	//	cout << i;
	//}
}