#include "check.h"
namespace wordexpand{
	Check::Check(){}
	Check::~Check(){}
	bool Check::Init(const char* dictpath){

	}
	bool Check::CheckLz(string& taskid){
		if(myLz.ChecKLzTask(taskid)){
			mySql.UpdataTask(taskid,string("1"));
		}
	}
	bool Check::HdfsToLocal(string& taskid){
		mySql.UpdataTask(taskid,string("4"));
		string hadooppath = "/usr/local/hadoop-0.20.1-tdw.0.2/bin/hadoop";
		string hadooppasswd = "-Dfs.default.name=hdfs://tl-if-nn-tdw.tencent-distribute.com:54310 -Dhadoop.job.ugi=tdw_seanxywang:tdw_seanxywang,g_cdg_weixin";
		string strremotepath = "hdfs://tl-if-nn-tdw.tencent-distribute.com:54310/stage/outface/wxg/g_cdg_weixin/seanxywang/temp/" + taskid + "/*";
		string strComGet = hadooppath + " fs " + hadooppasswd + " -get "  + " " + strremotepath + " " + PATH;
		system(strComGet.c_str());
		mySql.UpdataTask(taskid,string("2"));
		return true;
	}
	bool Check::Process(string& taskid){
		mySql.UpdataTask(taskid,string("3"));
	}
	bool Check::Send(string& taskid){
		//send
		mySql.UpdataTask(taskid,string("4"));
	}

	bool Check::CheckTask(){
		while(1){
			//1:查询数据库状态
			std::vector<taskstaue> tasklist;
			mySql.SelectTask(tasklist);
			for(int i =0; i< tasklist.size(); ++i){
				//2:根据状态处理相应的任务
				if(tasklist.at(i).staue == "0"){
					CheckLz(tasklist.at(i).taskid);
				}else if(tasklist.at(i).staue == "1"){
					HdfsToLocal(tasklist.at(i).taskid);
				}else if(tasklist.at(i).staue == "2"){
					Process(tasklist.at(i).taskid);
				}else if(tasklist.at(i).staue == "3"){
					Send(tasklist.at(i).taskid);
				}
			}

		}
	}
}