#include<Windows.h>
#include<iostream>
using namespace std;
#include<fstream>
#include<string>
#include<conio.h>
#include <filesystem>

//基础路径
string wj_lj = "temp\\data\\target_path.txt";		//文件路径（上锁的文件，的路径）
string sf_ss_lj = "temp\\data\\lock_status.txt";	//是否上锁（记录文件是否已经上锁，防止过度加密、解密，导致文件错误）
string s_zt_lj = "temp\\data\\lock_state.txt";		//锁_状态_路径（记录锁的当前锁定方式）
string temp_text_lj= "temp\\logs\\test.txt";		//临时文本_路径
string yg_mm = "****";								//掩盖密码

//程序运行前，优先创建对应目录
namespace fs = std::filesystem;			//将fs设置为命名空间filesystem
string lssj_ml = "temp/data";			//临时数据目录
string lssj_ml2 = "temp/logs";			//临时数据目录2

class cxs						//程序锁
{
public:
	cxs(const char* wjlj, const char* wjmm)		//初始化（使用构造函数进行）
	{
		lj = wjlj;				//将传入的路径，赋值给路径成员变量
		mm = wjmm;

		if (!pdlj())			//判断有没有该路径，如果没有该路径
		{
			ofstream ofs;
			ofs.open(lj, ios::out);				//则创建一个对应路径的文件
			ofs.close();
			if (!pdlj())						//如果还是没有该路径（路径格式写错了）
			{
				cout << "文件路径错误" << endl;
				system("pause");
				exit(0);
			}
		}

		if (pdnr())				//判断文件中是否有内容
		{
			return;				//有内容就不进行初始化
		}

		ofstream ofs;
		ofs.open(lj, ios::out);
		ofs << 1 << endl;							//将1（初始状态），传入文件中
		ofs.close();
	}

	//锁的状态分为6种：1（5次输入次数，用完后进入状态4）、2（3次输入次数，用完后进入状态5）、3（3次输入次数，用完后进入状态6）
					 //4（锁定系统30秒，然后进入状态2）、5（锁定系统2分钟，然后进入状态3）、6（锁定系统30分钟，然后进入状态3）
	void s()
	{
		string sr;						//输入内容
		string zt;						//状态

		ifstream ifs;
		ifs.open(lj, ios::in);
		ifs >> zt;						//将状态从文件中读入
		ifs.close();


		int i = 0;
		while (1)										//进入循环解锁界面
		{
			//（密码输入界面）
			if (zt == "1" || zt == "2" || zt == "3")			//如果状态为1、2、3（系统未锁定状态）
			{
				if (zt == "1")									//状态1有5次解锁机会
				{
					i = 4;
				}
				else											//状态2和状态3有3次解锁机会
				{
					i = 2;
				}

				while (1)
				{					
					//cout << "程序锁" << endl << endl;
					cout << "请输入密码" << endl;
					cin >> sr;

					if (sr == "0")						//特殊功能键，退出程序
					{
						exit(0);
					}

					if (sr == mm)						//如果输入内容等于密码内容，则退出循环，锁为截停式锁，退出循环（锁的代码部分）即为解开锁
					{
						//system("cls");				//在解锁后，顺便帮别人把屏幕清空一下
						return;
					}

					if (i == 0)							//如果在输入错误后，发现剩余次数为0，则退出该输入密码界面，进入系统锁定界面
					{
						if (zt == "1")
						{
							gxzt("4");
							zt = "4";
							system("cls");
							break;
						}
						else if (zt == "2")
						{
							gxzt("5");
							zt = "5";
							system("cls");
							break;
						}
						else if (zt == "3")
						{
							gxzt("6");
							zt = "6";
							system("cls");
							break;
						}

						//如果将状态改成int类型，可以将上面改成这样，不过需要将状态改成数字类型需要用到zfzsz函数，那样就更麻烦了
						//gxzt(mm, i + 3);				//将表示它们状态的数字，加3，文件中和函数中的都要更新，更新完后退出输入界面
						//i += 3;
						//break;
					}

					cout << "密码错误" << endl;			//密码输入错误后，提示内容
					cout << "剩余" << i << "次机会" << endl;
					system("pause");
					system("cls");

					i--;
				}
			}
			//（系统锁定界面）
			else
			{
				//system("cls");
				//cout << "程序锁" << endl << endl;
				cout << "系统已锁定" << endl;

				if (zt == "4")								//如果状态为4
				{
					//Sleep(30 * 1000);						//系统延时30秒

					for (int m = 30;m > 0;m--)
					{
						system("cls");
						cout << "系统已锁定" << endl;

						cout << "请在" << m - 1 << "秒后继续" << endl;
						Sleep(1000);
					}
					//Sleep(2000);							//延时2秒（测试用）

					gxzt("2");								//延时结束后将状态更新为2
					zt = "2";
				}
				else if (zt == "5")
				{
					//Sleep(2 * 60 * 1000);

					for (int f = 2;f > 0;f--)
					{
						for (int m = 60;m > 0;m--)
						{
							system("cls");
							cout << "系统已锁定" << endl;

							cout << "请在";
							if (f > 1)
							{
								cout << f - 1 << "分";
							}
							cout << m - 1 << "秒后继续" << endl;
							Sleep(1000);
						}
					}

					//Sleep(2000);							//延时2秒（测试用）

					gxzt("3");
					zt = "3";
				}
				else if (zt == "6")
				{
					//Sleep(30 * 60 * 1000);

					//显示倒计时的方法（s代表时，f代表分，m代表秒）
					for (int s = 1;s > 0;s--)
					{
						for (int f = 30;f > 0;f--)
						{
							for (int m = 60;m > 0;m--)
							{
								system("cls");
								cout << "系统已锁定" << endl;

								cout << "请在";
								if (s > 1)
								{
									cout << s - 1 << "时";
								}
								if (s > 1 || f > 1)
								{
									cout << f - 1 << "分";
								}
								cout << m - 1 << "秒后继续" << endl;
								Sleep(1000);
							}
						}
					}

					//Sleep(2000);							//延时2秒（测试用）

					gxzt("3");
					zt = "3";
				}
				else								//如果状态不是1~6的数字，可能是有人修改了保存状态的文件
				{
					gxzt("4");						//恢复状态，这里将状态恢复成为4
					zt = "4";
					system("cls");
					continue;
				}

				//system("pause");
				system("cls");
			}
		}
	}

	void gxzt(string zt)							//更新状态，将状态传入，更新文件中保存的状态信息
	{												//参数：(状态）
		ofstream ofs;
		ofs.open(lj, ios::out);						//将文件中的信息删除
		ofs << zt << endl;							//然后将状态重新输入进去
		ofs.close();
	}

	bool pdlj()										//判断文件（路径）是否可以打开
	{
		ifstream ifs;
		ifs.open(lj, ios::in | ios::binary);
		if (ifs.is_open())							//如果打开成功
		{
			ifs.close();
			return true;							//返回true
		}
		else										//否则（打开失败）
		{
			ifs.close();
			return false;							//返回false
		}
	}

	bool pdnr()
	{
		string temp;

		ifstream ifs;
		ifs.open(lj, ios::in | ios::binary);		//为防止程序崩，这里再判断一次文件是否打开成功
		if (!ifs.is_open())
		{
			ifs.close();
			return false;							//打开失败也返回false
		}

		if (getline(ifs, temp))						//如果文件中有内容
		{
			ifs.close();
			return true;							//返回true
		}
		else										//否则（文件中没有内容）
		{
			ifs.close();
			return false;							//返回false
		}
	}

	string lj;
	string mm;
};
class wjl
{
public:
	wjl(const char* temp_lj, const char* temp_sfss)
	{
		cclj = temp_lj;
		sfss_lj = temp_sfss;

		if (pdnr(cclj))								//如果存储路径对应的文件中有内容
		{
			ifstream ifs;
			ifs.open(cclj, ios::in);
			ifs.getline(wjlj, sizeof(wjlj));			//则将该文件中的内容，赋值给文件路径
			ifs.close();

			//由于如果文件路径，带有空格，将采用手动输入存储路径的方式保存路径，这样会跳过保护路径的创建，和保护路径的数据采集，这里添加一个措施
			if (!pdlj(sfss_lj))							//如果打开失败
			{
				ofstream ofs;
				ofs.open(sfss_lj, ios::out);			//则创建一个对应路径的文件
				ofs.close();
				if (!pdlj(sfss_lj))						//如果还是没有该路径（路径格式写错了或权限过低）
				{
					cout << "文件路径错误" << endl;
					system("pause");
					exit(0);							//则退出程序
				}

#if 0
				ifs.open(wjlj, ios::in);				//打开文件路径和保护路径
				ofs.open(sfss_lj, ios::out);

				char ch;
				int cs = 1;								//传输的次数

				for (;cs > 0;cs--)
				{
					ifs >> ch;							//将文件路径中的cs个字符
					ofs << ch;							//传入保护路径中
				}

				ifs.close();
				ofs.close();
#endif
			}					

			//判断路径只能用于判断是否存在该路径，不能判断文件是否进行过加密或解密
			//（由于路径判断是否可打开的方式为，判断是否为空，加密文件在ifs.is_open()中，属于不为空的路径）
			//if (!pdlj(wjlj))						//如果解密后，仍不可打开，则进行保护措施
			//{
			//	wjbh();
			//}
		}
		else										//如果没有，则初始化一个路径
		{
			ofstream ofs;
			ofs.open(cclj, ios::out);

			while (1)
			{
				cout << "请输入要进行上锁的文件的路径" << endl;
				cin >> wjlj;						//输入一个路径
				if (pdlj(wjlj))						//如果该路径可以打开
				{
					break;							//则退出循环
				}
				cout << "没有该路径" << endl;		//如果该路径不可以打开，则提示一下，然后进入循环
				system("pause");
				system("cls");
			}
		
			ofs << wjlj;							//将该文件路径保存到存储文件中
			ofs.close();

			sf_ccszlj = true;
		
#if 0
			ifstream ifs;
			ifs.open(wjlj, ios::in);				//然后打开文件路径和保护路径
			ofs.open(bhlj, ios::out);

			char ch;
			int cs = 1;								//传输的次数

			for (;cs > 0;cs--)
			{
				ifs >> ch;							//将文件路径中的cs个字符
				ofs << ch;							//传入保护路径中
			}

			ifs.close();
			ofs.close();
#endif
		}
	}
	void dk()										//打开该文件
	{
		if (!pdlj(wjlj))					//如果文件已不存在
		{
			cout << "文件不存在，无法打开，已取消操作" << endl;

			ofstream temp_ofs;
			temp_ofs.open(wj_lj, ios::out);		//删除文件路径
			temp_ofs.close();

			temp_ofs.open(sf_ss_lj, ios::out);	//删除上锁状态
			temp_ofs.close();

			temp_ofs.open(s_zt_lj, ios::out);	//锁_状态设回1
			temp_ofs << 1 << endl;
			temp_ofs.close();

			system("pause");
			exit(0);

			return;
		}

		//由于部分软件的路径有空格，而只有文件输入才允许有空格，system函数不允许有空格，
		//这里的打开文件，采用加上双引号的方式打开
		char temp[255] = { 0 };					//使用strcat_s必须初始化字符数组
		
		strcat_s(temp, "\"");
		strcat_s(temp, wjlj);
		strcat_s(temp, "\"");

		system(temp);
	}
	void kao_bei(const char* lj, const char* lj2)	//文件拷贝（将路径1的内容，复制给路径2）
	{
		//将文件路径的内容，复制一份到文件路径2中
		int a = 0;
		char ch[1024];

		FILE* file;
		FILE* file2;
		fopen_s(&file, lj, "rb");
		fopen_s(&file2, lj2, "wb");

		while ((a = fread(ch, 1, 1024, file)) != 0)
		{
			fwrite(ch, 1, a, file2);
		}
		fclose(file);
		fclose(file2);
	}	
	void jl_yss()				//记录_已上锁
	{
		ofstream ofs;
		ofs.open(sfss_lj, ios::out);

		ofs << "1" << endl;

		ofs.close();
	}
	void jl_yjs()				//记录_已解锁
	{
		ofstream ofs;
		ofs.open(sfss_lj, ios::out);

		ofs << "0" << endl;

		ofs.close();
	}
	void jia_mi(const char* lj)					//文件加密（对传入的文件进行加密）
	{
		if (!pdlj(wjlj))						//如果文件已不存在
		{
			cout << "文件不存在，已取消上锁" << endl;

			ofstream temp_ofs;
			temp_ofs.open(wj_lj, ios::out);		//删除文件路径
			temp_ofs.close();

			temp_ofs.open(sf_ss_lj, ios::out);	//删除上锁状态
			temp_ofs.close();

			temp_ofs.open(s_zt_lj, ios::out);	//锁_状态设回1
			temp_ofs << 1 << endl;
			temp_ofs.close();

			system("pause");
			exit(0);

			return;
		}
		else if (!pdnr(wjlj))
		{
			cout << "文件无内容，已取消上锁" << endl;

			ofstream temp_ofs;
			temp_ofs.open(sf_ss_lj, ios::out);	//删除上锁状态
			temp_ofs.close();

			temp_ofs.open(s_zt_lj, ios::out);	//锁_状态设回1
			temp_ofs << 1 << endl;
			temp_ofs.close();
			return;
		}

		//获取一个路径为，文件路径2的路径
		string temp_lj = lj;
		char lj2[255];
		int wz = temp_lj.rfind(".");
		temp_lj.insert(wz, "2");

		ofstream ofs;
		ofs.open(temp_text_lj, ios::out);
		ofs << temp_lj;
		ofs.close();

		ifstream ifs;
		ifs.open(temp_text_lj, ios::in);
		ifs.getline(lj2, sizeof(lj2));
		ifs.close();

		//将文件路径的内容，加密后传入文件路径2中
		int a = 0;
		char ch[1024];

		FILE* file;
		FILE* file2;
		fopen_s(&file, lj, "rb");
		fopen_s(&file2, lj2, "wb");

		while ((a = fread(ch, 1, 1024, file)) != 0)
		{
			for (int i = 0;i < a;i++)
			{
				ch[i]++;
			}
			fwrite(ch, 1, a, file2);
		}
		fclose(file);
		fclose(file2);

		remove(lj);								//删除原文件
		rename(lj2, lj);						//将加密文件，改成原文件名称

		jl_yss();				//记录_已上锁
		cout << "已上锁" << endl;
	}
	void jie_mi(const char* lj)						//文件解密
	{
		if (!pdlj(wjlj))					//如果文件已不存在
		{
			cout << "文件不存在，已取消解锁" << endl;

			ofstream temp_ofs;
			temp_ofs.open(wj_lj, ios::out);		//删除文件路径
			temp_ofs.close();

			temp_ofs.open(sf_ss_lj, ios::out);	//删除上锁状态
			temp_ofs.close();

			temp_ofs.open(s_zt_lj, ios::out);	//锁_状态设回1
			temp_ofs << 1 << endl;
			temp_ofs.close();

			system("pause");
			exit(0);

			return;
		}
		else if (!pdnr(wjlj))
		{
			cout << "文件无内容，已取消解锁" << endl;

			ofstream temp_ofs;
			temp_ofs.open(sf_ss_lj, ios::out);	//删除上锁状态
			temp_ofs.close();

			temp_ofs.open(s_zt_lj, ios::out);	//锁_状态设回1
			temp_ofs << 1 << endl;
			temp_ofs.close();
			return;
		}

		//获取一个路径为，文件路径2的路径
		string temp_lj = lj;
		char lj2[255];
		int wz = temp_lj.rfind(".");
		temp_lj.insert(wz, "2");

		ofstream ofs;
		ofs.open(temp_text_lj, ios::out);
		ofs << temp_lj;
		ofs.close();

		ifstream ifs;
		ifs.open(temp_text_lj, ios::in);
		ifs.getline(lj2, sizeof(lj2));
		ifs.close();

		//将文件路径的内容，加密后传入文件路径2中
		int a = 0;
		char ch[1024];

		FILE* file;
		FILE* file2;
		fopen_s(&file, lj, "rb");
		fopen_s(&file2, lj2, "wb");

		while ((a = fread(ch, 1, 1024, file)) != 0)
		{
			for (int i = 0;i < a;i++)
			{
				ch[i]--;
			}
			fwrite(ch, 1, a, file2);
		}
		fclose(file);
		fclose(file2);

		remove(lj);								//删除原文件
		rename(lj2, lj);						//将加密文件，改成原文件名称

		jl_yjs();				//记录_已解锁
		cout << "已解锁" << endl;
	}
	void js()
	{		
		if (x_bhcs())						//检测一次文件是否上锁，是的话再解锁
		{
			jie_mi(wjlj);					//然后对文件路径进行一次解密
		}
	}
#if 0
	//如果因为误操作导致文件进行过加密或解密，且自己并不清楚已经加密或解密过多少次，导致无法正常打开文件时，这里提供一个加密解密的保护措施
	//该函数会将文件给复原回来（注意：修复方式为：将文件反复进行加密或解密，然后尝试打开文件，所以，检测过程中不能退出，不然会导致文件处于更严重的加密状态）
	void wjbh()
	{
		int cs = 10;								//修复的次数

		cs = cs * 2 + 1;
		int num2 = 0;

		//获取一个路径为，文件路径2的路径
		string temp_lj = wjlj;
		char wjlj2[255];
		int wz = temp_lj.rfind(".");
		temp_lj.insert(wz, "2");

		ofstream ofs;
		ofs.open(temp_text_lj, ios::out);
		ofs << temp_lj;
		ofs.close();

		ifstream ifs;
		ifs.open(temp_text_lj, ios::in);
		ifs.getline(wjlj2, sizeof(wjlj2));
		ifs.close();

		//将文件路径的内容，复制给文件路径2
		kao_bei(wjlj, wjlj2);

		for (int num = 0;num < cs;num++)			//修复
		{
			for (num2 = num + 1;num2 > 0;num2--)	//该次加密或解密的次数
			{
				if (num / 2 == 1)					//当前轮次为单数，则对副本（文件路径2）进行加密
				{
					jia_mi(wjlj2);
				}
				else
				{
					jie_mi(wjlj2);					//当前轮次为双数，则对副本（文件路径2）进行解密
				}
			}

			if (wjlj2)								//每进行完一次加密、解密，就判断一次文件是否可以打开
			{
				kao_bei(wjlj2, wjlj);				//如果可以打开了，就将该文件的内容，拷贝给文件路径
				return;								//然后退出循环
			}
		}

		//如果仍然无法打开，则提示一下
		cout << "文件无法打开，解决方法：尝试增加加密、解密次数，或更改加密、解密算法" << endl;
		system("pause");
	}
	bool bhcs()										//文件加密解密保护措施（如果文件确实未经过加密，
	{												//说明上次关闭文件时误操作了，返回true提示这次不用加密）
		ifstream ifs;
		ifstream ifs2;

		ifs.open(wjlj, ios::in);
		ifs2.open(bhlj, ios::in);

		char ch;
		char ch2;

		ifs >> ch;
		ifs2 >> ch2;

		if (ch == ch2)
		{
			return true;
		}
		return false;
	}
#endif
	bool x_bhcs()
	{
		ifstream ifs;

		ifs.open(sfss_lj, ios::in);
		if (!ifs.is_open())			//不存在该文件
		{
			ifs.close();
			return false;			//默认当未上锁
		}

		char ch = '0';

		ifs >> ch;

		if (ch == '1')
		{
			return true;
		}
		return false;
	}
	bool pdlj(string lj)							//判断路径（是否可以打开）
	{
		ifstream ifs;
		ifs.open(lj, ios::in | ios::binary);
		if (ifs.is_open())							//如果打开成功
		{
			ifs.close();
			return true;							//返回true
		}
		else										//否则（打开失败）
		{
			ifs.close();
			return false;							//返回false
		}
	}
	bool pdnr(string temp_pdnr_lj)					//判定文件中是否有内容
	{
		string temp;

		ifstream ifs;
		ifs.open(temp_pdnr_lj, ios::in | ios::binary);	//为防止程序崩，这里再判断一次文件是否打开成功
		if (!ifs.is_open())
		{
			ifs.close();
			return false;							//打开失败也返回false
		}

		if (getline(ifs, temp))						//如果文件中有内容
		{
			ifs.close();
			return true;							//返回true
		}
		else										//否则（文件中没有内容）
		{
			ifs.close();
			return false;							//返回false
		}
	}
	
#if 0
	string bhlj = "文件保护路径.txt";				//已弃用（文件保护路径（不需要初始化，位置固定化））
#endif

	string cclj;									//数据存储路径（必须初始化）
	string sfss_lj;									//是否上锁_路径
	char wjlj[255];									//文件路径
	bool sf_ccszlj = false;							//是否初次设置路径
};
void ygmm_sc()							//掩盖密码_输出
{
	system("cls");
	cout << "请输入密码" << endl;
	cout << yg_mm << endl;
}

int main()
{
	//在当前目录下，创建一个临时数据目录，如果目录存在会自动跳过
	fs::create_directories(lssj_ml);
	fs::create_directories(lssj_ml2);


	//1.初始化文件类、锁类
	wjl wj(wj_lj.c_str(), sf_ss_lj.c_str());
	cxs s(s_zt_lj.c_str(), "1234");
	
	//2.进入输入密码，输入正确后才能执行下面的操作
	if (!wj.sf_ccszlj)							//只要不是初次设置路径
	{
		s.s();
	}	

	//3.掩盖一次密码
	if (!wj.sf_ccszlj)							//只要不是初次设置路径
	{
		ygmm_sc();
	}	

	//4.文件解锁（文件解密，并记录“已解锁”到文件中）
	wj.js();

	//5.打开文件，关闭文件后自动加密（并记录“已上锁”到文件中）
	wj.dk();									//打开该文件
	wj.jia_mi(wj.wjlj);							//关闭后，对文件进行一次加密






	/*/防止误操作用
	int num = 0;

	while (1)
	{
		system("cls");
		cout << "此处为防止误操作用" << endl << endl;
		cout << "1.恢复文件" << endl;
		cout << "2.更改软件" << endl;
		cout << "0.退出" << endl;

		num = _getch();						//输入一个字符

		if (num == '1')						//如果输入1
		{
			if (!_kbhit())					//且后面没有内容
			{
				wj.dk();					//打开该文件
				wj.jia_mi(wj.wjlj);			//关闭后，加密一次文件
			}
		}
		else if (num == '2')				//如果输入0
		{
			if (_kbhit())					//如果后面有内容，则跳过该次内容
			{
				continue;
			}
			ofstream ofs;
			ofs.open(wj.cclj, ios::out);

			while (1)
			{
				cout << "请输入要进行上锁的软件的路径" << endl;
				cin >> wj.wjlj;						//输入一个路径
				if (wj.pdlj(wj.wjlj))				//如果该路径可以打开
				{
					break;							//则退出循环
				}
				cout << "没有该路径" << endl;		//如果该路径不可以打开，则提示一下，然后进入循环
				system("pause");
				system("cls");
			}

			ofs << wj.wjlj;							//将该文件路径保存到存储文件中
			ofs.close();
		}
		else if (num == '0')				//如果输入0
		{
			if(!_kbhit())					//且后面没有内容
			{
				exit(0);					//则退出
			}
		}
	}
	//*/





	return 0;
}