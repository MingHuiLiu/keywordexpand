#ifndef _TASK_H
#define _TASK_H
#include "../commom/func.h" 
#include "lz.h"
#include "index.h"
#include "model.h"
typedef struct{
	string id;
	string keywds;
	string source;
	string num;
}confinfo; 
namespace wordexpand{
	class Task{
	public:
		Task();
		~Task();
	public:		
		Lz mlz;		
		Sql mySql;
		Index mindex;
		commom::Func f;
	public:
		string taskid;
		confinfo inputconf;	
		string localfiledir;
		std::vector<bizinfo> bizresults;
		std::vector<articleinfo> articleresults;
	public:
		bool ArgumentParsing(const char* str);
		bool Config(string& query,std::map<string, string>& taskinfo);
		bool Retrieval(string& query,std::map<string, string>& taskinfo);
		bool Rank();
		bool UinToFile();
		bool CallLz();
		bool TaskApi(const char* str);
	};
}
#endif