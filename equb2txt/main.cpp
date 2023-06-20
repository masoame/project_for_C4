#include<fstream>
#include<iostream>
#include<windows.h>

using namespace std;

#define BUFFER_MAX (1024*2)

char buffer_in[BUFFER_MAX];
char buffer_out[BUFFER_MAX];
char path[260];

int main(int argc, char* argv[], char* argu[])
{
	int count, ptr;
	char temp;
	bool flag;

	WIN32_FIND_DATAA file;

	ifstream equb;
	ofstream txt;

	//文件处理
	sprintf_s(path, "%s\\*.*html", argv[1]);
	cout << path << endl;
	HANDLE files = FindFirstFileA(path, &file);

	if (files == INVALID_HANDLE_VALUE)
	{
		cout << "句柄返回失败" << GetLastError() << endl;
		return -1;
	}

	do
	{
		equb.open(argv[1] + string("\\") + file.cFileName, ios::binary | ios::in);

		if (!equb.is_open())
		{
			cout << "文件路径有误" << GetLastError() << endl;
			return -1;
		}

		txt.open(argv[1] + string("\\") + file.cFileName + string(".txt"), ios::binary | ios::out);

		if (!txt.is_open())
		{
			cout << "文件路径有误" << GetLastError() << endl;
			return -1;
		}

		flag = false;

		while (!equb.eof())
		{
			equb.getline(buffer_in, BUFFER_MAX, '\n');

			count = 0;
			ptr = 0;

			//while (buffer_in[count] == ' ') count++;
			//if (buffer_in[count] == '\r' || buffer_in[count] == '\n') continue;

			while (temp = buffer_in[count])
			{
				++count;

				if (temp == '<')flag = true;
				else if (temp == '>')
				{
					flag = false;
					continue;
				}

				//跳过空格子使得文本更加紧凑
				if (flag || temp == ' ')continue;

				buffer_out[ptr] = temp;
				++ptr;
			}

			if (ptr == 0 || (ptr == 1 && (buffer_out[0] == '\r' || buffer_out[0] == '\n')))continue;

			buffer_out[ptr++] = '\n';

			txt.write(buffer_out, ptr);
		}

		equb.close();
		txt.close();
	} while (FindNextFileA(files, &file));

	FindClose(files);

	cout << "文本提取成功" << endl;
	system("pause");
}