#include "lz.h"
namespace wordexpand{

	//初始化索引
	bool Lz::Init(std::map<string,string>& inittaskParams){
		inittaskParams["taskName"]="";
		inittaskParams["taskType"]="75";
		inittaskParams["cycleUnit"]="O";
		inittaskParams["brokerIp"]="any";
		inittaskParams["cycleNum"]="1";
		inittaskParams["startDate"]="2015-01-01 10:00:00";
		inittaskParams["endDate"]="2999-01-01 10:00:00";
		inittaskParams["selfDepend"]="2";
		inittaskParams["taskAction"]="";
		inittaskParams["startHour"]="08";
		inittaskParams["startMin"]="30";
		inittaskParams["retriable"]="1";
		inittaskParams["notes"]="xxxxx";
		inittaskParams["inCharge"]="seanxywang";
		inittaskParams["sourceServer"]="hdfs_tl-if-nn-tdw.tencent-distribute.com";
		inittaskParams["targetServer"]="tdw_tl";
		inittaskParams["creater"]="seanxywang";
		inittaskParams["tryLimit"]="3";
		inittaskParams["bgId"]="8";
		inittaskParams["productId"]="304";
		inittaskParams["tdwAppGroup"]="g_cdg_weixin";
		inittaskParams["taskExt"]="taskExt";
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
		curl = curl_easy_init();    // 初始化
		string url = "10.222.106.18:80/LService/LhotseTask";
		struct url_data data;
		data.size = 0;
		data.data = (char*)malloc(4096);
		data.data[0] = '\0';
		if (curl){ 
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());    // 指定post内容
			curl_easy_setopt(curl,CURLOPT_POST,1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
			CURLcode res = curl_easy_perform(curl);
			string str = data.data;
			commom::DEBUG_INFO(str);
			if(res != CURLE_OK){
				commom::DEBUG_INFO("connect error");
				curl_easy_cleanup(curl);
				return "";
			}else{
				return str;
			}			
		}
		return "";
	}
	string Lz::SendUinStauePost(string& taskid,string& real_magnitude,string& package_path){
		string params = "taskid=";
		params += taskid;
		params +="&real_magnitude=";
		params += real_magnitude;
		params +="&package_path=";
		params += package_path;
		//commom::DEBUG_INFO(params);
		curl = curl_easy_init();    // 初始化
		string url = "game.mmbiz.oa.com/cgi-bin/game/packagenotify";
		//10.191.130.34:14748
		struct url_data data;
		data.size = 0;
		data.data = (char*)malloc(4096);
		data.data[0] = '\0';
		if (curl){ 
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params.c_str());    // 指定post内容
			curl_easy_setopt(curl,CURLOPT_POST,1);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); 
			CURLcode res = curl_easy_perform(curl);
			string str = data.data;
			commom::DEBUG_INFO("RETURN: " + str);
			if(res != CURLE_OK){
				commom::DEBUG_INFO(taskid + "connect error");
				curl_easy_cleanup(curl);
				return "";
			}else{
				commom::DEBUG_INFO(taskid + "\t SEND STAUE OK");
				return str;
			}			
		}
		return "";
	}

	bool Lz::SendUinStaue(string& taskid,string real_magnitude,string& package_path){		
		if(SendUinStauePost(taskid,real_magnitude,package_path) == ""){
			return false;
		}
		return true;
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
	}
	string Lz::GetStaue(string str){
		commom::DEBUG_INFO(str);
		string copystr ="";
		copystr.append(str,2,str.size()-4);
		commom::DEBUG_INFO(copystr);
		str = copystr;
		std::vector<string> v;
		std::vector<string> tmp;
		std::map<string, string> dict;
		commom::Split(",", str, v);
		commom::DEBUG_INFO(commom::ConvertToStr(v.size()));
		if(v.size() == 0){
			return "";
		}else{
			for(int i =0; i< v.size(); i++){
				commom::Split(":", v.at(i),tmp);
				if(tmp.size() == 2){
					string firststr ="";
					string secondstr ="";
					firststr.append(tmp.at(0),1,tmp.at(0).size()-2);
					secondstr.append(tmp.at(1),1,tmp.at(1).size()-2);
					commom::DEBUG_INFO(firststr);
					commom::DEBUG_INFO(secondstr);
					dict[firststr] = secondstr;
				}
			}
		}
		return dict["taskId"];
	}

	bool Lz::PutLocalFileToHFDS(const char* filepath){
		string hadooppath = "/usr/local/hadoop-0.20.1-tdw.0.2/bin/hadoop";
		string hadooppasswd = "-Dfs.default.name=hdfs://tl-if-nn-tdw.tencent-distribute.com:54310 -Dhadoop.job.ugi=tdw_seanxywang:tdw_seanxywang,g_cdg_weixin";
		string strremotepath = "hdfs://tl-if-nn-tdw.tencent-distribute.com:54310/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/";
		string strComPut = hadooppath + " fs " + hadooppasswd + " -put " +  string(filepath) + " " + strremotepath;
		system(strComPut.c_str());
		return true;
	}
	string Lz::AddPartition(string& taskid){
		std::map<string,string> taskParams;
		Init(taskParams);
		std::map<string,string> dcitExtParam;
		dcitExtParam["file_name"] = "add_source2uin_partition.py";
		dcitExtParam["params"] = taskid;				
		taskParams["taskType"] = "121";
		taskParams["taskName"] = "addpartition:(" + taskid + ")";
		taskParams["taskExt"] = TaskExtToStr(dcitExtParam);
		commom::DEBUG_INFO(ParamToStr(taskParams));
		return GetStaue(LzApiPost(ParamToStr(taskParams)));
	}
	string Lz::HDFSToTDW(string& parentid,string& taskid){
		std::map<string,string> taskParams;
		Init(taskParams);
		std::map<string,string> dcitExtParam;
		dcitExtParam["charSet"] = "UTF-8";
		dcitExtParam["databaseName"] = "wxg_data_valueless";			
		dcitExtParam["delimiter"] = "9";				
		dcitExtParam["failedOnZeroWrited"] = "1";				
		dcitExtParam["loadMode"] = "TRUNCATE";				
		dcitExtParam["partitionType"] = "P_"+ taskid;		
		dcitExtParam["sourceColumnNames"] = "taskid,id,tag,flag,score";			
		dcitExtParam["sourceFileNames"] = taskid;					
		dcitExtParam["sourceFilePath"] = "/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/";			
		dcitExtParam["tableName"] = "wxy_sourceid_partition";					
		dcitExtParam["tdw"] = "tdw_tl";					
		dcitExtParam["targetColumnNames"] = "taskid,id,tag,flag,score";	
		taskParams["parentTaskId"] = "{"+ parentid + ":1}";
		taskParams["taskType"] = "75";
		taskParams["taskName"] = "hdfstotdw" + taskid;
		taskParams["taskExt"] = TaskExtToStr(dcitExtParam);
		commom::DEBUG_INFO(ParamToStr(taskParams));
		return GetStaue(LzApiPost(ParamToStr(taskParams)));
	}

	string Lz::Getuins(string& parentid,string& taskid){
		std::map<string,string> taskParams;
		Init(taskParams);
		std::map<string,string> dcitExtParam;
		//dcitExtParam["file_name"] = "source2uin.py";
		dcitExtParam["file_name"] = "source2uinfortest.py";
		dcitExtParam["params"] = string(taskid);	
		taskParams["parentTaskId"] ="{"+parentid+":1}";
		taskParams["taskType"] = "121";
		taskParams["taskName"] = "getuin_:(" + taskid + ")";
		taskParams["taskExt"] = TaskExtToStr(dcitExtParam);
		commom::DEBUG_INFO(ParamToStr(taskParams));
		return GetStaue(LzApiPost(ParamToStr(taskParams)));
	}

	string Lz::TDW2HDFS(string& parentid,string& taskid){
		string destFilePath = "/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/out/";
		string destFileName = taskid;
		string destFilePathName = destFilePath +destFileName;
		std::map<string,string> taskParams;
		Init(taskParams);
		std::map<string,string> dcitExtParam;
		dcitExtParam["destCheckFileName"] = taskid + ".check";
		dcitExtParam["databaseName"] = "wxg_data_valueless";			
		dcitExtParam["destCheckFilePath"] = destFilePathName;			
		dcitExtParam["destFileDelimiter"] = "9";				
		dcitExtParam["destFilePath"] = destFilePathName;	
		string strsql = "select uin,tag,score FROM wxy_daily_game_active_uin where taskid =" + taskid + " order by score";		
		dcitExtParam["filterSQL"] = strsql;		
		taskParams["targetServer"]="hdfs_tl-if-nn-tdw.tencent-distribute.com";
		taskParams["sourceServer"]="tdw_tl";
		taskParams["parentTaskId"] = "{"+string(parentid)+":1}";
		taskParams["taskType"] = "76";
		taskParams["taskName"] = "tdw2hdfs" +taskid;
		taskParams["taskExt"] = TaskExtToStr(dcitExtParam);
		commom::DEBUG_INFO(ParamToStr(taskParams));
		return GetStaue(LzApiPost(ParamToStr(taskParams)));
	}
	bool Lz::ChecKLzTask(string& taskid){
		return true;
	}
	bool Lz::LzTaskApi(Sql& mySql,string& taskid, const char* filepath,string& uinnumber){
		if(!PutLocalFileToHFDS(filepath)){
			commom::LOG_INFO(taskid + ":\tPutLocalFileToHFDS Error");
			return false;
		}
		string  addpartitionid = AddPartition(taskid);
		if(addpartitionid == ""){
			commom::LOG_INFO(taskid + ":\tAddPartition Error");
			return false;
		}
		string hdfstotdwid = HDFSToTDW(addpartitionid,taskid);
		if(hdfstotdwid == ""){
			commom::LOG_INFO(taskid + ":\tHDFSToTDW Error");
			return false;
		}
		string getuinsid = Getuins(hdfstotdwid, taskid);
		if(getuinsid == ""){
			commom::LOG_INFO(taskid + ":\tGetuins Error");
			return false;
		}
		string lzid = TDW2HDFS(getuinsid,taskid);
		if(lzid == ""){
			commom::LOG_INFO(taskid + ":\tTDW2HDFS Error");
			return false;
		}else{
			return mySql.AddTask(taskid,lzid,uinnumber);
		}
	}
}
