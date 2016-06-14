#ifndef _CHECK_H
#define _CHECK_H
#include "../commom/func.h"
#include "lz.h"
#include "index.h"
#include "model.h"
#define PATH "./data/"
typedef struct{
	string tag;
	string flag;
	float score;
}uininfo;
namespace wordexpand{
	class Check{
	public:
		Check();
		~Check();
	public:
		//int uinsize;//ʵ������
		string path;
		Sql mySql;
		Lz myLz;
		commom::Func f;
	public:
		bool Init();
		bool CheckLz(string taskid);
		bool HdfsToLocal(string taskid);
		bool Process(string taskid,string uinnumber);
		bool Send(string taskid);
		bool CheckTask();
	};

}
#endif








































