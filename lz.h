#ifndef _LZ_H
#define _LZ_H
#include <stdio.h>
#include <curl/curl.h>
#include "../commom/func.h"
struct url_data {
	size_t size;
	char* data;
};
namespace wordexpand{
 class Lz{
	public:
		Lz(){
			headers = NULL;
			curl = NULL;
		}
		~Lz(){}
	public://biz
		commom::Func f;
		struct curl_slist *headers;
		CURL *curl;
		
	public://biz
		bool Init(std::map<string,string>& inittaskParams);
		string GetStaue(string str);
		string LzApiPost(string params);
		bool LzTaskApi(string& taskid, const char* filepath);
		bool PutLocalFileToHFDS(const char* filepath);
		string AddPartition(string& taskid);
		string HDFSToTDW(const char*filename,string& parentid,string& taskid);
		string Getuins(string& parentid,string& taskid);
		string TDW2HDFS(string& parentid,string& taskid);
		bool ChecKLzTask(string& taskid);
	};

}
#endif





































