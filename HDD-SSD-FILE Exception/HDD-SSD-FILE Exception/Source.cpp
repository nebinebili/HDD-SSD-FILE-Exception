#include <iostream>
#include<string>
#include<ctime>
#include <windows.h>
using namespace std;

class Disc {
protected:
	class File {
	private:
		string create = "";
	public:
		string filename;
		string content;

		File(string filename, string content = "", string create = __TIME__) {
			this->filename = filename;
			this->content = content;
			SetCreate(create);

		}
		string GetCreate()const {
			return create;
		}
		void SetCreate(const string create) {
			if (create.empty()) return;

			time_t currently = time(0);
			char dt[30]{};
			ctime_s(dt, sizeof dt, &currently);
			string time = dt;

			this->create = dt;
		}
	};

	File** files = NULL;
	int file_count = 0;
	double current_size = 0;
public:
	const static double max_size;

	File& getFile(string filename) {
		cout << "----- Get File  ------" << endl;
		for (int i = 0; i < file_count; i++)
		{
			if (files[i]->filename == filename) {

				return *files[i];
			}
		}
		throw "File is not found";
	}

	bool isExistsFile(string filename) {
		for (size_t i = 0; i < file_count; i++)
		{
			if (filename == files[i]->filename) return true;
		}
		return false;
	}

	virtual void createFile(string filename, string content = "", string create = __TIME__) {
		if (isExistsFile(filename)) throw "File already exist";

		auto newFiles = new File * [file_count + 1];

		for (size_t i = 0; i < file_count; i++)
		{
			newFiles[i] = files[i];

		}
		if (current_size += content.length() > max_size) {
			throw "Content size more Max Size";
		}

		File* file = new File(filename, content);

		newFiles[file_count] = file;

		delete[] files;
		files = newFiles;
		newFiles = nullptr;
		file_count++;
	}

	virtual void setFileContent(string filename, string content = "") {
		cout << "----- Change File Content ------" << endl;
		for (int i = 0; i < file_count; i++)
		{
			if (files[i]->filename == filename) {
				current_size -= files[i]->content.length();
				if (current_size + content.length() > max_size) throw "This content size more Max Size";
				files[i]->content = content;
				return;
			}
		}
		throw "Set File content is not possible->File is not found";

	}

	void deleteFileContent(string filename) {
		cout << "Delete File Content..." << endl;
		for (int i = 0; i < file_count; i++)
		{
			if (files[i]->filename == filename) {
				files[i]->content = " ";
				return;
			}
		}
		throw "Delete File content is not possible->File is not found";
	}
	void updateFilename(string oldfilename, string newfilename) {
		cout << "Update File Name..." << endl;
		for (int i = 0; i < file_count; i++)
		{
			if (files[i]->filename == oldfilename) {
				files[i]->filename = newfilename;
				return;
			}
		}
		throw "Update File is not possible->File is not found";
	}
	void deleteFileByFilename(string filename) {
		for (int i = 0; i < file_count; i++)
		{
			if (files[i]->filename == filename) {
				auto newFiles = new File * [file_count - 1];
				current_size -= files[i]->content.length();

				for (int k = 0, j = 0; j < file_count; j++)
				{
					if (j != i)
						newFiles[k++] = files[j];
				}
				delete[] files;
				files = newFiles;
				newFiles = nullptr;
				file_count--;

				return;
			}
		}
		throw "Delete File filename is not possible->File is not found";


	}
	size_t hashcode(string& data) {
		size_t h(0);
		for (int i = 0; i < data.length(); i++)
			h = (h << 6) ^ (h >> 26) ^ data[i];
		return h;
	}

	void hashFile(string filename) {

		for (int i = 0; i < file_count; i++)
		{
			if (files[i]->filename == filename) {
				size_t hash = hashcode(files[i]->content);
				files[i]->content = to_string(hash);
				return;
			}
		}
	   throw "Hashing is not possible->File is not found";
	}
	void showfile() {

		cout << "----- Info Files ------" << endl;
		for (int i = 0; i < file_count; i++)
		{
			cout << "File Name->" << files[i]->filename << endl;
			cout << "File Content->" << files[i]->content << endl;
			cout << "File Create time->" << files[i]->GetCreate() << endl << endl;

		}
	}

    virtual ~Disc()=0
	{

	}
};

class SSD :public Disc {
public:
	void  createFile(string filename, string content = "", string create = __TIME__)override{
		double temp = content.length() / 10;
		cout << "File is creating..." << endl;
		Sleep(temp * 100);
		Disc::createFile(filename, content);
	}
	void setFileContent(string filename, string content = "")override {
		for (int i = 0; i < file_count; i++)
		{
			if (files[i]->filename == filename) {
				current_size -= files[i]->content.length();
				if (current_size + content.length() > max_size) throw "This content size more Max Size";
				double temp = files[i]->content.length() / 10;
				cout << "File content is chancing" << endl;
				Sleep(temp * 100);
				files[i]->content = content;
				return;
			}
		}
		throw "Set File content is not possible->File is not found";
	}

};

class HDD :public Disc {
public:
	void  createFile(string filename, string content = "", string create = __TIME__)override{
		double temp = content.length() / 10;
		temp *= 1000;
		cout << "File is creating..." << endl;
		Sleep(temp);
		Disc::createFile(filename, content);
	}
	void setFileContent(string filename, string content = "")override{
		for (int i = 0; i < file_count; i++)
		{
			if (files[i]->filename == filename) {
				current_size -= files[i]->content.length();
				if (current_size + content.length() > max_size) throw "This content size more Max Size";
				double temp = files[i]->content.length() / 10;
				cout << "File content is chancing..." << endl;
				Sleep(temp * 1000);
				files[i]->content = content;
				return;
			}
		}
		throw "Set File content is not possible->File is not found";
	}
};

const double HDD::max_size = 1024;

class OperationSystem {
	OperationSystem() {}
public:
	static void controlPC();
};

void OperationSystem::controlPC() {

	HDD hdd;
	SSD ssd;
	try
	{
		hdd.createFile("newtext1.txt", "hello world");
		hdd.createFile("newtext2.txt", "Salam necesen");
		hdd.createFile("newtext3.txt", "axsamin xeyr");
		ssd.createFile("newtext4.txt", "sabahin xeyr");
		ssd.createFile("newtext5.txt", "gecen xeyr");

		/*ssd.deleteFileByFilename("newtext5.txt");
		hdd.deleteFileByFilename("newtext1.txt");
		cout << "------- AFTER Delete File by Filename ---------" << endl;*/

		/*hdd.deleteFileContent("newtext1.txt");
		ssd.deleteFileContent("newtext4.txt");
		cout << "------- AFTER Delete File Content ---------" << endl;*/

		/*hdd.setFileContent("newtext.txt", "Hi");
		ssd.setFileContent("newtext5.txt", "Sumqayit");
		cout << "------- AFTER Set File Content ---------" << endl;*/

		/*hdd.updateFilename("newtext2.txt", "updatehdd.txt");
		ssd.updateFilename("newtext5.txt", "updatessd,text");
		cout << "------- AFTER Update File ---------" << endl;*/

		hdd.hashFile("newtext.txt");
		ssd.hashFile("newtext5.txt");
		cout << "------- AFTER Hash File ---------" << endl;
	}
	catch (const char* ex)
	{
		cout << "Exception : " << ex << endl;
	}
	cout << "----- HDD INFO-----" << endl;
	hdd.showfile();
	cout << "---- SSD INFO----" << endl;
	ssd.showfile();
}

void main() {
	OperationSystem::controlPC();
}