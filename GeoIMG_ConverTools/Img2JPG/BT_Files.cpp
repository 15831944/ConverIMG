#include "BT_Files.h"
#include "stdafx.h"

void GetAllFiles(string path, vector<string>& files, string format)
{

	long   hFile = 0;
	//�ļ���Ϣ      
	struct _finddata_t fileinfo;//�����洢�ļ���Ϣ�Ľṹ��     
	int len = format.length();
	string p, temp;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  //��һ�β���    
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))  //������ҵ������ļ���    
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //�����ļ��в���    
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );    
					GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
				}
			}
			else //������ҵ��Ĳ������ļ���     
			{
				//files.push_back(p.assign(fileinfo.name) );  //���ļ�·�����棬Ҳ����ֻ�����ļ���:    p.assign(path).append("\\").append(fileinfo.name)    
				temp = fileinfo.name;
				//�ж��ַ����Ƿ���format��ʽ��β  
				if (temp.length() > len && temp.compare(temp.length() - len, len, format) == 0)
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile); //��������    
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
