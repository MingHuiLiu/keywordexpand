#ifndef _TASK_H
#define _TASK_H
#include "../commom/func.h" 
#include "lz.h"
#include "index.h"
#include "model.h"
#include "../log/log.h"
typedef struct{
	string id;
	string keywds;
	string source;
	string num;
}confignfo; 
namespace wordexpand{
	class Task{
	public:
		Task();
		~Task();
	public:		
		Lz mlz;	
		//commom::Log mylog;
		Sql mySql;
		Index mindex;
		commom::Func f;
	public:
		string taskid;
		string desc;
		confignfo inputconf;	
		string localfiledir;
		std::vector<bizinfo> bizresults;
		std::vector<articleinfo> articleresults;
	public:
		bool ArgumentParsing(const char* str);
		bool Config(std::map<string, string>& taskinfo);
		bool Retrieval(std::map<string, string>& taskinfo);
		bool Rank();
		bool UinToFile();
		bool CallLz();
		bool TaskApi(const char* str);
	};
}
#endif