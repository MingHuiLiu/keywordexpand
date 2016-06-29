#ifndef _LZ_H
#define _LZ_H
#include <stdio.h>
#include "sql.h"
#include <curl/curl.h>
#include "../commom/func.h"
#include "../log/log.h"
struct url_data {
	size_t size;
	char* data;
};
namespace wordexpand{
 class Lz{
	public:
		Lz(){
			//mySql.InitMysql();
			headers = NULL;
			curl = NULL;
			mylog.Init("./log/");
		}
		~Lz(){}
	public://
		//Sql mySql;
		commom::Func f;
		commom::Log mylog;
		struct curl_slist *headers;
		CURL *curl;
		
	public://洛子接口
		bool Init(std::map<string,string>& inittaskParams);
		string GetStaue(string str);
		string LzApiPost(string params);
		bool LzTaskApi(Sql& mySql,string& taskid, const char* filepath, string& uinnumber);
		bool PutLocalFileToHFDS(const char* filepath);
		string AddPartition(string& taskid);
		string HDFSToTDW(const char*filename,string& parentid,string& taskid);
		string Getuins(string& parentid,string& taskid);
		string TDW2HDFS(string& parentid,string& taskid);
		bool ChecKLzTask(string& taskid);
	public://其他接口
		string SendUinStauePost(string& taskid,string& real_magnitude,string& package_path);
		bool SendUinStaue(string& taskid,string real_magnitude,string& package_path);


	};

}
#endif





































