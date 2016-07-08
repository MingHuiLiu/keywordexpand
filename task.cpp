#include "task.h"
namespace wordexpand{
	Task::Task(){
		desc = "SUCESS";
		taskid = "";
		inputconf.id = "";
		inputconf.keywds = "";
		inputconf.source = "";
		inputconf.num = "";
		localfiledir = "./tmpdata/";
		mySql.InitMysql();
		mindex.InitRetrieval("../dict/bizdict/");
		
	}
	Task::~Task(){
		bizresults.clear();
		articleresults.clear();
	}
	//参数解析
	bool Task::ArgumentParsing(const char* str){
		string confstr = str;
		//commom::DEBUG_INFO("confstr :" + confstr);
		std::vector<string> v;
		std::vector<string> tmp;
		std::map<string,string>dict;
		commom::Split("&", confstr, v);
		if(v.size() != 4){
			desc = "param number error!";
			return false;
		}else{
			for(int j =0; j < v.size(); j++){
				string ss = v.at(j);
				commom::Split("=",ss,tmp);
				if(tmp.size() != 2){
					desc = "param wrong!";
					return false;
				}else{
					dict[tmp.at(0)] = tmp.at(1);
				}
				commom::LOG_INFO(v.at(j));
			}

		}
		if(dict["code"] != "13BCF8DE22DB318A01EF4172AA0C199A"){
			desc = "passcode error!";
			return false;
		}
		string keywd = dict["keywd"];
		if(keywd == ""){
			desc = "keywd error!";
			return false;
		}
		commom::Split("_",keywd,tmp);
		if(tmp.size() < 1){
			desc = "keywd error!";
			return false;
		}

		if(dict["source"].size() != 1){
			desc = "source error!";
			return false;
		}
		int sourceint = atoi(dict["source"].c_str());
		commom::DEBUG_INFO(commom::ConvertToStr(sourceint));
		if(sourceint == 0){
			return false;
		}else if(sourceint == 1){
			inputconf.source = "001";
		}else if(sourceint == 2){
			inputconf.source = "010";
		}else if(sourceint == 3){
			inputconf.source = "011";
		}else if(sourceint == 4){
			inputconf.source = "100";
		}else if(sourceint == 5){
			inputconf.source = "101";
		}else if(sourceint == 6){
			inputconf.source = "110";
		}else if(sourceint == 7){
			inputconf.source = "111";
		}else{
			return false;
		}

		if(inputconf.source.size() != 3){
			desc = "source error!";
			return false;
		}
		if(dict["uinnumber"] == ""){

		}else if((atoi(dict["uinnumber"].c_str()) == 0)||(atoi(dict["uinnumber"].c_str()) > 10000*10000)){
			desc = "uinnumber error!";
			return false;
		}
		taskid = commom::GetDate() +  commom::ConvertToStr(mySql.GetTaskId());
		commom::DEBUG_INFO("taskid : " + taskid);
		inputconf.id = taskid;
		inputconf.keywds = keywd;
		inputconf.num =dict["uinnumber"];
		commom::LOG_INFO(inputconf.id);
		commom::LOG_INFO(inputconf.keywds);
		commom::LOG_INFO(inputconf.source);
		commom::LOG_INFO(inputconf.num);
		return true;
	}
	//参数设置,网页显示
	bool Task::Config(std::map<string, string>& taskinfo){	
		//解析传入参数
		commom::DEBUG_INFO("Config");
		string conf = "";
		conf += ("task_id:" + commom::ConvertToStr(mySql.GetTaskId()));
		conf += ";";
		conf += ("task_version:1");
		conf += ";";
		conf += ("task_name:\"" + inputconf.keywds);
		conf += "\";";	
		conf += ("task_cname:\"" + inputconf.keywds);
		conf += "\";";	
		conf += ("input:\"" + inputconf.keywds);
		conf += "\";";	
		conf += ("step:1");
		conf += ";";
		conf += ("status:0");
		conf += ";";
		conf += ("pack_name:\"\"");
		conf += ";";
		conf += ("done_name:\"\"");
		conf += ";";
		conf += ("user_num:0");
		conf += ";";
		conf += ("biz_url:\"\"");
		conf += ";";
		conf += ("creator:\"seanxywang\"");
		conf += ";";
		conf += ("modify_user:\"seanxywang\"");
		conf += ";";
		//commom::GetDate()
		conf += ("create_time:\"" + commom::GetTime());
		conf += "\";";	
		conf += ("modify_time:\"" + commom::GetTime());
		conf += "\";";	
		std::vector<std::string> res ;
		std::vector<std::string> tmp ;
		//commom::DEBUG_INFO(conf);
		commom::Split(";", conf, res);
		for(int i = 0; i < res.size(); i++){
			commom::Split(":",res.at(i),tmp);
			if(tmp.size() == 2){
				taskinfo[tmp.at(0)] = tmp.at(1);
			}
		}
		//commom::DEBUG_INFO(commom::ConvertToStr(taskinfo.size()));
		return true;
	}

	//检索
	bool Task::Retrieval(std::map<string, string>& taskinfo){
		commom::DEBUG_INFO("Begin Retrieval");
		return mindex.Retrieval(inputconf.source,inputconf.keywds,mySql, taskinfo, bizresults, articleresults);
	}

	//排序
	bool Task::Rank(){
		return true;
	}
	//读写文件
	bool Task::UinToFile(){
		commom::DEBUG_INFO("Begin UinToFile");
		string str = "";
		FILE*fo = fopen((localfiledir + taskid).c_str(),"ab+");
		if (fo == NULL) {
			commom::LOG_INFO("open file error" + string(localfiledir + taskid));
			return false;
		}
		int i = 0;
		for(std::vector<bizinfo>::iterator it =  bizresults.begin(); it != bizresults.end(); it++){
			if(i++ > 500){
				break;
			}
			str = taskid + string("\t") + (*it).uin + string("\t0\t1\t") +commom::ConvertToStr((*it).score) + "\n";
			commom::WiteLine(str.c_str(),fo);
		}	
		i = 0;
		for(std::vector<articleinfo>::iterator it = articleresults.begin(); it != articleresults.end(); it++){
			if(i++ > 10000){
				break;
			}
			str = taskid + string("\t") + (*it).articleuin + string("\t1\t1\t") +commom::ConvertToStr((*it).score) + "\n";
			commom::WiteLine(str.c_str(),fo);
		}	
		fclose(fo);
		return true;
	}

	//LZ接口
	bool Task::CallLz(){
		commom::DEBUG_INFO(taskid + ":\tCallLz Api");
		return mlz.LzTaskApi(mySql, taskid, (localfiledir+taskid).c_str(),inputconf.num);
	}
	bool Task::TaskApi(const char* str){
		if(ArgumentParsing(str) != true){
			commom::LOG_INFO(taskid + ":\tArgumentParsing error");
			return false;
		}
		std::map<string, string> taskinfo;
		if(!Config(taskinfo)){
			commom::LOG_INFO(taskid + ":\tConfig error");
			return false;
		}

		if(!Retrieval(taskinfo)){
			commom::LOG_INFO(taskid + ":\tRetrieval error");
			return false;
		}
		if(!UinToFile()){
			commom::LOG_INFO(taskid + ":\tuinToFile error");
			return false;
		}
		//return false;
		return CallLz();
	}

}