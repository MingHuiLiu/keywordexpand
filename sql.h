#ifndef _SQL_H
#define _SQL_H
#include "../commom/func.h"
#include <mysql.h>
typedef struct{
	string uin;
	string bizname;
	string bizdesc;
	string bizfunsnum;
	//string massmsgcount;
	string type;
	string topic;
} bizuin; 

typedef struct{
	string uin;
	string bizname;
	string bizdesc;
	float score;
	string type;
	string topic;
} bizinfo; 

typedef struct{
	string bizuin;
	string appmsgid;
	string itemidx;
	string title;
	string ds;
} articleuin; 

typedef struct{
	string articleuin;
	string title;
	float score;
} articleinfo; 

typedef struct{
	string taskid;
	string ds;
	string lzid;
	string num;
	string staue;
} taskstaue; 
namespace wordexpand{
	class Sql{
	public:
		Sql(){}
		~Sql(){}
	public://biz
		//////commom::Func f;	;	;	
		MYSQL myCont;
	public://mysql
		bool InitMysql();
		bool ExeQuery(string& query);
		bool AddTask(std::map<string, string>& taskinfo);
		bool UpdataKeywords(std::vector<std::string>& v, std::map<string, string>& taskinfo);
		bool UpdataBiz(std::vector<bizinfo>& results, std::map<string, string>& taskinfo);
		bool UpdataArticle();
		int GetTaskId();
	public://数据状态表操作
		bool AddTask(string taskid, string lzid,string& uinnumber);
		bool SelectTask(std::vector<taskstaue>& tasklist);
		bool UpdataTask(string taskid, string staue);

	};
}
#endif








































