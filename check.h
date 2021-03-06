#ifndef _CHECK_H
#define _CHECK_H
#include "../commom/func.h"
//#include <unistd.h>
#include "lz.h"
#include "index.h"
#include <time.h> 
#include "model.h"
#define PATH "./data/"
#define RANDOMPATH "./data/random"
#define PINGTAIPATH "./data/random"
#define UINTOTAL 20000000
typedef struct{
	string tag;
	string score;
}uininfo;
namespace wordexpand{
	class Check{
	public:
		Check();
		~Check();
	public:
		string sendpath;
		Sql mySql;
		Lz myLz;
	public:
		bool Init();
		bool CheckLz(string taskid);
		bool HdfsToLocal(string taskid);
		bool Process(string taskid,string uinnumber);
		bool Send(string taskid);
		//bool CheckTask();
		bool AddRandom(const char* path,int num);
	};
	bool CheckTask();
}
#endif








































