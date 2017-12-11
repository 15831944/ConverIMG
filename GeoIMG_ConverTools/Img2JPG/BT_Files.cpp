#include "BT_Files.h"
#include "stdafx.h"

void GetAllFiles(string path, vector<string>& files, string format)
{

	long   hFile = 0;
	//文件信息      
	struct _finddata_t fileinfo;//用来存储文件信息的结构体     
	int len = format.length();
	string p, temp;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  //第一次查找    
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))  //如果查找到的是文件夹    
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //进入文件夹查找    
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );    
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
				}
			}
			else //如果查找到的不是是文件夹     
			{
				//files.push_back(p.assign(fileinfo.name) );  //将文件路径保存，也可以只保存文件名:    p.assign(path).append("\\").append(fileinfo.name)    
				temp = fileinfo.name;
				//判断字符串是否以format格式结尾  
				if (temp.length() > len && temp.compare(temp.length() - len, len, format) == 0)
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile); //结束查找    
	}
}

bool file_exsit(string file)
{
	fstream _file;
	bool s = true;
	_file.open(file, ios::in);
	if (!_file)
		s = false;
	_file.close();
	return s;
}
