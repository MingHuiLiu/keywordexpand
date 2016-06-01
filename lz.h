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
		std::map<string,string>taskParams;
	public://biz
		bool Init();
		bool post();
		string LzApiPost(string params);
		bool HDFSToTDW(const char*filename,const char* parentid,const char* taskId);
	
	};

}
#endif





































