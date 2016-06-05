#include "check.h"
namespace wordexpand{
	Check::Check(){
		Init();
	}
	Check::~Check(){}
	bool Check::Init(){
		mySql.InitMysql();
	}
	bool Check::CheckLz(string taskid){
		commom::DEBUG_INFO("check lz");
		mySql.UpdataTask(taskid,string("5"));
		string hadooppath = "/usr/local/hadoop-0.20.1-tdw.0.2/bin/hadoop";
		string hadooppasswd = "-Dfs.default.name=hdfs://tl-if-nn-tdw.tencent-distribute.com:54310 -Dhadoop.job.ugi=tdw_seanxywang:tdw_seanxywang,g_cdg_weixin";
		string strremotepath = "hdfs://tl-if-nn-tdw.tencent-distribute.com:54310/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/out/" + taskid + "/*.check";
		string strComGet = hadooppath + " fs " + hadooppasswd + " -get "  + " " + strremotepath + " " + PATH + taskid + ".check";
		commom::DEBUG_INFO(strComGet);
		system(strComGet.c_str());
		string filein =  PATH + taskid + ".check";
		commom::DEBUG_INFO(filein);
		FILE*fi = fopen(filein.c_str(),"r");
		if (fi != NULL) {
			commom::DEBUG_INFO("updata");
			mySql.UpdataTask(taskid,string("1"));
			fclose(fi);
			string strrm = "rm -rf ./data/*.check";
			system(strrm.c_str());
		}else{
			mySql.UpdataTask(taskid,string("0"));
		}
		return true;
		/*
		if(myLz.ChecKLzTask(taskid)){
			mySql.UpdataTask(taskid,string("1"));
		}
		*/
	}
	bool Check::HdfsToLocal(string taskid){
		mySql.UpdataTask(taskid,string("4"));
		string hadooppath = "/usr/local/hadoop-0.20.1-tdw.0.2/bin/hadoop";
		string hadooppasswd = "-Dfs.default.name=hdfs://tl-if-nn-tdw.tencent-distribute.com:54310 -Dhadoop.job.ugi=tdw_seanxywang:tdw_seanxywang,g_cdg_weixin";
		string strremotepath = "hdfs://tl-if-nn-tdw.tencent-distribute.com:54310/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/out/" + taskid + "/*";
		string strComGet = hadooppath + " fs " + hadooppasswd + " -get "  + " " + strremotepath + " " + PATH;
		commom::DEBUG_INFO(strComGet);
		commom::DEBUG_INFO(f.ConvertToStr(system(strComGet.c_str())));
		mySql.UpdataTask(taskid,string("2"));
		return true;
	}
	bool Check::Process(string taskid){
		mySql.UpdataTask(taskid,string("3"));
	}
	bool Check::Send(string taskid){
		//send
		mySql.UpdataTask(taskid,string("4"));
	}

	bool Check::CheckTask(){
		while(1){
			//1:查询数据库状态
			std::vector<taskstaue> tasklist;
			mySql.SelectTask(tasklist);
			for(int i =0; i< tasklist.size(); ++i){
				commom::DEBUG_INFO(tasklist.at(i).staue);
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
			for(int i =0; i< 100000; i++){
				for(int j =0; j< 100000; j++){
					//
				}
			}

		}
	}
}