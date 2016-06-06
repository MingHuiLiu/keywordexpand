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
		//commom::DEBUG_INFO(strComGet);
		system(strComGet.c_str());
		string filein =  PATH + taskid + ".check";
		//commom::DEBUG_INFO(filein);
		FILE*fi = fopen(filein.c_str(),"r");
		if (fi != NULL) {
			commom::DEBUG_INFO("updata");
			mySql.UpdataTask(taskid,string("1"));
			fclose(fi);
			string strrm = "rm -rf ./data/" + taskid + ".check";
			system(strrm.c_str());
		}else{
			mySql.UpdataTask(taskid,string("0"));
		}
		return true;
	}
	bool Check::HdfsToLocal(string taskid){
		mySql.UpdataTask(taskid,string("5"));
		string hadooppath = "/usr/local/hadoop-0.20.1-tdw.0.2/bin/hadoop";
		string hadooppasswd = "-Dfs.default.name=hdfs://tl-if-nn-tdw.tencent-distribute.com:54310 -Dhadoop.job.ugi=tdw_seanxywang:tdw_seanxywang,g_cdg_weixin";
		string strremotepath = "hdfs://tl-if-nn-tdw.tencent-distribute.com:54310/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/out/" + taskid + "/attempt*";
		string strremotepathcheck = "hdfs://tl-if-nn-tdw.tencent-distribute.com:54310/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/out/" + taskid + "/" + taskid + ".check";
		string strComGet = hadooppath + " fs " + hadooppasswd + " -get "  + " " + strremotepath + " " + PATH + taskid + ".temp";
		commom::DEBUG_INFO(strComGet);
		system(strComGet.c_str());
		strComGet = hadooppath + " fs " + hadooppasswd + " -get "  + " " + strremotepathcheck + " " + PATH;
		commom::DEBUG_INFO(strComGet);
		system(strComGet.c_str());
		mySql.UpdataTask(taskid,string("2"));
		return true;
	}
	bool Check::Process(string taskid,string uinnumber){
		mySql.UpdataTask(taskid,string("5"));
		string filepath = PATH + taskid + ".temp";
		string fileout = PATH + taskid;
		FILE* fi = fopen(filepath.c_str(), "r");
		FILE*fo = fopen(fileout.c_str(),"ab+");
		if ((fi == NULL)||(fo == NULL)) {
			commom::LOG_INFO("open file error");
			mySql.UpdataTask(taskid,string("2"));
			return false;
		}		
		std::string str = "";
		char buffer[MAX_LENTH];	
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer) + "\n";
			f.WiteLine(str.c_str(), fo);
		}
		fclose(fi);
		fclose(fo);
		mySql.UpdataTask(taskid,string("3"));
	}
	bool Check::Send(string taskid){
		mySql.UpdataTask(taskid,string("5"));
		//string strscp = "rsync -av test_seanxywang qspace@10.234.151.147::mmocgameuin/from_data_apply";10.234.133.11
		//string strscp = "rsync -av test_seanxywang qspace@10.234.133.11::mmocgameuin/from_data_apply";
		//system(strscp.c_str());
		commom::LOG_INFO("send");
		mySql.UpdataTask(taskid,string("4"));
	}

	bool Check::CheckTask(){
		while(1){
			//1:查询数据库状态
			std::vector<taskstaue> tasklist;
			mySql.SelectTask(tasklist);
			for(int i =0; i< tasklist.size(); ++i){
				//commom::DEBUG_INFO(tasklist.at(i).staue);
				//2:根据状态处理相应的任务
				if(tasklist.at(i).staue == "0"){
					CheckLz(tasklist.at(i).taskid);
				}else if(tasklist.at(i).staue == "1"){
					HdfsToLocal(tasklist.at(i).taskid);
				}else if(tasklist.at(i).staue == "2"){
					Process(tasklist.at(i).taskid, tasklist.at(i).num);
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