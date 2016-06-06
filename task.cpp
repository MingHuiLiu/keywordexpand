#include "task.h"
namespace wordexpand{
	Task::Task(){
		localfiledir = "./tmpdata/";
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
		commom::DEBUG_INFO(str);
		return false;
	}
	//参数设置
	bool Task::Config(string& query,std::map<string, string>& taskinfo){	
		//解析传入参数
		commom::DEBUG_INFO("Config");
		FILE*fi  = fopen("./input","r");
		if (fi == NULL) {
			commom::LOG_INFO("open file error");
			return false;
		}		
		std::string str = "";
		char buffer[MAX_LENTH];		
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer);
		}
		taskid = "20160602" +  f.ConvertToStr(mySql.GetTaskId());
		commom::DEBUG_INFO("taskid : " + taskid);
		//query = inputconf.keywds;
		//query = str;
		inputconf.keywds = str;
		inputconf.source = "2";
		inputconf.num = "20";
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
		mindex.Retrieval(inputconf.source,inputconf.keywds,mySql, taskinfo, bizresults, articleresults);
		return true;
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
		for(std::vector<bizinfo>::iterator it =  bizresults.begin(); it != bizresults.end(); it++){
			str = taskid + string("\t") + (*it).uin + string("\t0\t1\t") +f.ConvertToStr((*it).score) + "\n";
			f.WiteLine(str.c_str(),fo);
		}	
		for(std::vector<articleinfo>::iterator it = articleresults.begin(); it != articleresults.end(); it++){
			str = taskid + string("\t") + (*it).articleuin + string("\t1\t1\t") +f.ConvertToStr((*it).score) + "\n";
			f.WiteLine(str.c_str(),fo);
		}	
		fclose(fo);
		return true;
	}

	//LZ接口
	bool Task::CallLz(){
		commom::DEBUG_INFO("CallLz Api");
		mlz.LzTaskApi(taskid, (localfiledir+taskid).c_str(),inputconf.num);
		return true;
	}

	bool Task::TaskApi(const char* str){
		if(ArgumentParsing(str) != true){
			commom::LOG_INFO("ArgumentParsing error");
			//return false;
		}
		string query = "";
		std::map<string, string> taskinfo;
		Config(query, taskinfo);
		Retrieval(taskinfo);
		UinToFile();
		CallLz();
	}

}