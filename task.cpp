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
		//mylog.Init("./log/");
		mySql.InitMysql();
		mindex.InitRetrieval("../dict/easyseg/");
		
	}
	Task::~Task(){
		bizresults.clear();
		articleresults.clear();
	}
	//参数解析
	bool Task::ArgumentParsing(const char* str){
		//taskid=201606021814&keywords=游戏 美丽&souce=1&number=1000
		string confstr = str;
		commom::DEBUG_INFO("confstr :" + confstr);
		//mylog.LOG(taskid,("INPUTPARAM :" + string(confstr) + "\n").c_str());
		std::vector<string> v;
		std::vector<string> tmp;
		std::map<string,string>dict;
		f.Split("&", confstr, v);
		if(v.size() != 4){
			desc = "param number error!";
			return false;
		}else{
			for(int j =0; j < v.size(); j++){
				string ss = v.at(j);
				f.Split("=",ss,tmp);
				if(tmp.size() != 2){
					desc = "param wrong!";
					return false;
				}else{
					dict[tmp.at(0)] = tmp.at(1);
				}
				commom::LOG_INFO(v.at(j));
			}

		}
		//
		if(dict["code"] != "13BCF8DE22DB318A01EF4172AA0C199A"){
			desc = "passcode error!";
			//mylog.LOG(taskid,("ERROR :" + desc + "\n").c_str());
			return false;
		}
		string keywd = dict["keywd"];
		//
		//keywd = "保卫萝卜_植物大战僵尸_帝国塔防3_塔防_帝国塔防_开心消消乐_糖果消消乐_极地冒险_炮塔_天天消消乐_魔法_植物大战僵尸";
		if(keywd == ""){
			desc = "keywd error!";
			//mylog.LOG(taskid,("ERROR :" + desc + "\n").c_str());
			return false;
		}
		f.Split("_",keywd,tmp);
		if(tmp.size() < 1){
			desc = "keywd error!";
			//mylog.LOG(taskid,("ERROR :" + desc + "\n").c_str());
			return false;
		}

		if(dict["source"].size() != 1){
			desc = "source error!";
			//mylog.LOG(taskid,("ERROR :" + desc + "\n").c_str());
			return false;
		}
		int sourceint = atoi(dict["source"].c_str());
		commom::DEBUG_INFO(f.ConvertToStr(sourceint));
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
		/*
		if(dict["source"].find("1") != string::npos){
			inputconf.source += "1";
		}else{
			inputconf.source += "0";
		}
		if(dict["source"].find("2") != string::npos){
			inputconf.source += "1";
		}else{
			inputconf.source += "0";
		}
		if(dict["source"].find("4") != string::npos){
			inputconf.source += "1";
		}else{
			inputconf.source += "0";
		}
		/*
		if((dict["source"].size() <0)||(dict["source"].size() >3)){
		desc = "source error!";
		//mylog.LOG(taskid,("ERROR :" + desc + "\n").c_str());
		return false;
		}
		if(dict["source"].find("1") != string::npos){
			inputconf.source += "1";
		}else{
			inputconf.source += "0";
		}
		if(dict["source"].find("2") != string::npos){
			inputconf.source += "1";
		}else{
			inputconf.source += "0";
		}
		if(dict["source"].find("4") != string::npos){
			inputconf.source += "1";
		}else{
			inputconf.source += "0";
		}
		*/


		if(inputconf.source.size() != 3){
			desc = "source error!";
			//mylog.LOG(taskid,("ERROR :" + desc + "\n").c_str());
			return false;
		}
		if(dict["uinnumber"] == ""){

		}else if((atoi(dict["uinnumber"].c_str()) == 0)||(atoi(dict["uinnumber"].c_str()) > 10000*10000)){
			desc = "uinnumber error!";
			//mylog.LOG(taskid,("ERROR :" + desc + "\n").c_str());
			return false;
		}
		taskid = f.GetDate() +  f.ConvertToStr(mySql.GetTaskId());
		commom::DEBUG_INFO("taskid : " + taskid);
		inputconf.id = taskid;
		inputconf.keywds = keywd;
		//inputconf.source = dict["source"];
		inputconf.num =dict["uinnumber"];
		commom::LOG_INFO(inputconf.id);
		commom::LOG_INFO(inputconf.keywds);
		commom::LOG_INFO(inputconf.source);
		commom::LOG_INFO(inputconf.num);
		return true;
	}
	//参数设置
	bool Task::Config(string& query,std::map<string, string>& taskinfo){	
		//解析传入参数
		commom::DEBUG_INFO("Config");
		string conf = "";
		conf += ("task_id:" + f.ConvertToStr(mySql.GetTaskId()));
		conf += ";";
		conf += ("task_version:1");
		conf += ";";
		conf += ("task_name:\"\"");
		conf += ";";
		conf += ("task_cname:\"\"");
		conf += ";";
		conf += ("input:\"" + query);
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
		conf += ("create_time:20160526");
		conf += ";";
		conf += ("modify_time:20160526");	
		std::vector<std::string> res ;
		std::vector<std::string> tmp ;
		f.Split(";", conf, res);
		for(int i = 0; i < res.size(); i++){
			f.Split(":",res.at(i),tmp);
			if(tmp.size() == 2){
				taskinfo[tmp.at(0)] = tmp.at(1);
			}
		}
		return true;
	}

	//检索
	bool Task::Retrieval(std::map<string, string>& taskinfo){
		commom::DEBUG_INFO("Begin Retrieval");
		//mylog.LOG(taskid,"DEBUG_INFO : Begin Retrieval ");
		return mindex.Retrieval(inputconf.source,inputconf.keywds,mySql, taskinfo, bizresults, articleresults);
	}

	//排序
	bool Task::Rank(){
		return true;
	}
	//读写文件
	bool Task::UinToFile(){
		commom::DEBUG_INFO("Begin UinToFile");
		//mylog.LOG(taskid,"DEBUG_INFO : Begin UinToFile ");
		string str = "";
		FILE*fo = fopen((localfiledir + taskid).c_str(),"ab+");
		if (fo == NULL) {
			commom::LOG_INFO("open file error" + string(localfiledir + taskid));
			return false;
		}
		int i = 0;
		for(std::vector<bizinfo>::iterator it =  bizresults.begin(); it != bizresults.end(); it++){
			if(i++ > 200){
				break;
			}
			str = taskid + string("\t") + (*it).uin + string("\t0\t1\t") +f.ConvertToStr((*it).score) + "\n";
			f.WiteLine(str.c_str(),fo);
		}	
		i = 0;
		for(std::vector<articleinfo>::iterator it = articleresults.begin(); it != articleresults.end(); it++){
			if(i++ > 1000){
				break;
			}
			str = taskid + string("\t") + (*it).articleuin + string("\t1\t1\t") +f.ConvertToStr((*it).score) + "\n";
			f.WiteLine(str.c_str(),fo);
		}	
		fclose(fo);
		return true;
	}

	//LZ接口
	bool Task::CallLz(){
		commom::DEBUG_INFO("CallLz Api");
		//mylog.LOG(taskid,"DEBUG_INFO : CallLz Api ");
		//mylog.~Log();
		mlz.LzTaskApi(taskid, (localfiledir+taskid).c_str(),inputconf.num);
		return true;
	}

	bool Task::TaskApi(const char* str){
		if(ArgumentParsing(str) != true){
			commom::LOG_INFO("ArgumentParsing error");
			//mylog.LOG(taskid,"DEBUG_INFO : ArgumentParsing error");
			return false;
		}
		//mylog.LOG(taskid,"DEBUG_INFO : ArgumentParsing OK");
		string query = "";
		std::map<string, string> taskinfo;
		if(!Config(query, taskinfo)){
			commom::LOG_INFO("Config error");
			//mylog.LOG(taskid,"DEBUG_INFO : Config error");
			return false;
		}
		//mylog.LOG(taskid,"DEBUG_INFO : Config OK");
		if(!Retrieval(taskinfo)){
			commom::LOG_INFO("Retrieval error");
			//mylog.LOG(taskid,"DEBUG_INFO : Retrieval error");
			return false;
		}
		//mylog.LOG(taskid,"DEBUG_INFO : Retrieval OK");
		if(!UinToFile()){
			commom::LOG_INFO("UinToFile error");
			//mylog.LOG(taskid,"DEBUG_INFO : UinToFile error");
			return false;
		}
		//return false;
		//mylog.LOG(taskid,"DEBUG_INFO : UinToFile OK");
		return CallLz();
	}

}