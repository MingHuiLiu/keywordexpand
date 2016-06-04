#ifndef _CHECK_H
#define _CHECK_H
#include "../commom/func.h"
#include "lz.h"
#include "index.h"
#include "model.h"
#define PATH "./data/"
namespace wordexpand{
	class Check{
	public:
		Check();
		~Check();
	public:
		Sql mySql;
		Lz myLz;
		commom::Func f;
	public:
		bool Init(const char* dictpath);
		bool CheckLz(string& taskid);
		bool HdfsToLocal(string& taskid);
		bool Process(string& taskid);
		bool Send(string& taskid);
		bool CheckTask();
	};

}
#endif








































