#include "lz.h"
namespace wordexpand{

	//初始化索引
	bool Lz::Init(){
		taskParams["taskName"]="";
		taskParams["taskType"]="75";
		taskParams["cycleUnit"]="O";
		taskParams["brokerIp"]="any";
		taskParams["cycleNum"]="1";
		taskParams["startDate"]="2015-01-01 10:00:00";
		taskParams["endDate"]="2999-01-01 10:00:00";
		taskParams["selfDepend"]="2";
		taskParams["taskAction"]="";
		taskParams["startHour"]="08";
		taskParams["startMin"]="30";
		taskParams["retriable"]="1";
		taskParams["notes"]="xxxxx";
		taskParams["inCharge"]="seanxywang";
		taskParams["sourceServer"]="hdfs_tl-if-nn-tdw.tencent-distribute.com";
		taskParams["targetServer"]="tdw_tl";
		taskParams["creater"]="seanxywang";
		taskParams["tryLimit"]="3";
		taskParams["bgId"]="8";
		taskParams["productId"]="304";
		taskParams["tdwAppGroup"]="g_cdg_weixin";
		taskParams["taskExt"]="taskExt";
		return true;
	}

	size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
		size_t index = data->size;
		size_t n = (size * nmemb);
		char* tmp;
		data->size += (size * nmemb);
		tmp = (char*)realloc(data->data, data->size + 1); 
		if(tmp) {
			data->data = tmp;
		} else {
			if(data->data) {
				free(data->data);
			}
			fprintf(stderr, "Failed to allocate memory.\n");
			return 0;
		}
		memcpy((data->data + index), ptr, n);
		data->data[data->size] = '\0';
		return size * nmemb;
	}
	string Lz::LzApiPost(string params){	
		//commom::DEBUG_INFO(params);
		//headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded; Accept: text/plain");
		curl = curl_easy_init();    // 初始化
		string url = "10.222.106.18:80/LService/LhotseTask";
		struct url_data data;
		data.size = 0;
		data.data = (char*)malloc(4096);
		data.data[0] = '\0';
		if (curl){ 
			
			//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
			//curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // 指定cookie文件
			commom::DEBUG_INFO("add post");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());    // 指定post内容
			commom::DEBUG_INFO("set post");
			curl_easy_setopt(curl,CURLOPT_POST,1);
			commom::DEBUG_INFO("add url");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
			commom::DEBUG_INFO("connect post");
			CURLcode res = curl_easy_perform(curl);
			string str = data.data;
			if(res != CURLE_OK){
				commom::DEBUG_INFO("connect error");
				curl_easy_cleanup(curl);
				return "";
			}else{
				return str;
			}	
			return "";
		}

	}
	string ParamToStr(std::map<string,string>& dcitExtParam){
		string str = "";
		for(std::map<string,string>::iterator it = dcitExtParam.begin(); it != dcitExtParam.end(); it++){
			it++;
			if(it == dcitExtParam.end()){
				it--;
				str += it->first;
				str += "=";
				str += it->second;
			}else{
				it--;
				str += it->first;
				str += "=";
				str += it->second;
				str += "&";
			}
		}
		return str;
		//[{"propValue": "source2uin.py", "propName": "file_name"}, {"propValue": "201605311975", "propName": "params"}]
	}
	string TaskExtToStr(std::map<string,string>& dcitExtParam){
		string str = "";
		str += "[";
		for(std::map<string,string>::iterator it = dcitExtParam.begin(); it != dcitExtParam.end(); it++){
			it++;
			if(it == dcitExtParam.end()){
				it--;
				str += "{";
				str += "\"propName\":";
				str += "\"" + it->first + "\"";
				str += ",";
				str += "\"propValue\":";
				str += "\"" + it->second + "\"";
				str += "}";
			}else{
				it--;
				str += "{";
				str += "\"propName\":";
				str += "\"" + it->first + "\"";
				str += ",";
				str += "\"propValue\":";
				str += "\"" + it->second + "\"";
				str += "}, ";
			}
		}
		str += "]";
		return str;
		//[{"propValue": "source2uin.py", "propName": "file_name"}, {"propValue": "201605311975", "propName": "params"}]
	}
	bool Lz::HDFSToTDW(const char*filename,const char* parentid,const char* taskId){
		//taskParams
		std::map<string,string> dcitExtParam;
		dcitExtParam["charSet"] = "UTF-8";
		dcitExtParam["databaseName"] = "wxg_data_valueless";			
		dcitExtParam["delimiter"] = "9";				
		dcitExtParam["failedOnZeroWrited"] = "1";				
		dcitExtParam["loadMode"] = "TRUNCATE";				
		dcitExtParam["partitionType"] = "P_"+ string(taskId);		
		dcitExtParam["sourceColumnNames"] = "taskid,id,tag,flag,score";			
		dcitExtParam["sourceFileNames"] = filename;					
		dcitExtParam["sourceFilePath"] = "/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/";			
		dcitExtParam["tableName"] = "wxy_sourceid_partition";					
		dcitExtParam["tdw"] = "tdw_tl";					
		dcitExtParam["targetColumnNames"] = "taskid,id,tag,flag,score";	
		//taskParams["parentTaskId"] = "{"+string(parentid)+":1}";
		taskParams["taskType"] = "75";
		taskParams["taskName"] = parentid;
		taskParams["taskExt"] = TaskExtToStr(dcitExtParam);
		LzApiPost(ParamToStr(taskParams));
		return true;
	}



}

/*
char szJsonData[1024];  
memset(szJsonData, 0, sizeof(szJsonData));  
std::string strJson = "{";  
strJson += "\"user_name\" : \"test\",";  
strJson += "\"password\" : \"test123\"";  
strJson += "}";  
strcpy(szJsonData, strJson.c_str());  
try   
{  
CURL *pCurl = NULL;  
CURLcode res;  
// In windows, this will init the winsock stuff  
curl_global_init(CURL_GLOBAL_ALL);  

// get a curl handle  
pCurl = curl_easy_init();  
if (NULL != pCurl)   
{  
// 设置超时时间为1秒  
curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 1);  

// First set the URL that is about to receive our POST.   
// This URL can just as well be a   
// https:// URL if that is what should receive the data.  
curl_easy_setopt(pCurl, CURLOPT_URL, "http://192.168.0.2/posttest.svc");  
//curl_easy_setopt(pCurl, CURLOPT_URL, "http://192.168.0.2/posttest.cgi");  

// 设置http发送的内容类型为JSON  
curl_slist *plist = curl_slist_append(NULL,   
"Content-Type:application/json;charset=UTF-8");  
curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);  

// 设置要POST的JSON数据  
curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, szJsonData);  

// Perform the request, res will get the return code   
res = curl_easy_perform(pCurl);  
// Check for errors  
if (res != CURLE_OK)   
{  
printf("curl_easy_perform() failed:%s\n", curl_easy_strerror(res));  
}  
// always cleanup  
curl_easy_cleanup(pCurl);  
}  
curl_global_cleanup();  
}  
*/