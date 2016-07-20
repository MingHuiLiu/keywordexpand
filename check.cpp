#include "check.h"
namespace wordexpand{
	Check::Check(){
		Init();
		sendpath = "10.234.151.147::mmocgameuin//from_data_apply/";
	}
	Check::~Check(){}
	bool Check::Init(){
		mySql.InitMysql();
	}
	bool Check::CheckLz(string taskid){
		mySql.UpdataTask(taskid,string("5"));
		string hadooppath = "/usr/local/hadoop-0.20.1-tdw.0.2/bin/hadoop";
		string hadooppasswd = "-Dfs.default.name=hdfs://tl-if-nn-tdw.tencent-distribute.com:54310 -Dhadoop.job.ugi=tdw_seanxywang:tdw_seanxywang,g_cdg_weixin";
		string strremotepath = "hdfs://tl-if-nn-tdw.tencent-distribute.com:54310/stage/outface/wxg/g_cdg_weixin/seanxy/tdw/out/" + taskid + "/*.check";
		string strComGet = hadooppath + " fs " + hadooppasswd + " -get "  + " " + strremotepath + " " + PATH + taskid + ".check";
		system(strComGet.c_str());
		string filein =  PATH + taskid + ".check";
		FILE*fi = fopen(filein.c_str(),"r");
		if (fi != NULL) {		
			commom::LOG_INFO(taskid + ":\t open file ok");
			mySql.UpdataTask(taskid,string("1"));
			fclose(fi);
			string strrm = "rm -rf ./data/" + taskid + ".check";
			system(strrm.c_str());
		}else{
			delete fi;
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
		string strComGet = hadooppath + " fs " + hadooppasswd + " -get "  + " " + strremotepath + " " + PATH;
		if(system(strComGet.c_str()) == 0){
			commom::LOG_INFO(taskid + ":\t get hdfs file ok");
		}
		strComGet = hadooppath + " fs " + hadooppasswd + " -get "  + " " + strremotepathcheck + " " + PATH;
		if(system(strComGet.c_str()) == 0){
			commom::LOG_INFO(taskid + ":\t get check file ok");
		}
		mySql.UpdataTask(taskid,string("2"));
		return true;
	}

	//uin \t tag \t flag
	bool Check::AddRandom(const char* path, int num){
		srand( (unsigned)time( NULL ) );
		FILE* fi = fopen(RANDOMPATH, "r");
		FILE*fo = fopen(path,"ab+");
		if ((fi == NULL)||(fo == NULL)) {
			commom::LOG_INFO("open file error");
			return false;
		}		
		std::string str = "";
		std::vector<string>v;
		char buffer[MAX_LENTH];	
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = commom::GetLine(buffer);
			commom::Split("\t",str,v);
			if(v.size() != 2) continue;
			str = v.at(0);
			if(rand()%UINTOTAL < num){
				str += "\t9\t0\n";
				commom::WiteLine(str.c_str(), fo);
			}
		}

		fi = fopen(PINGTAIPATH, "r");
		if ((fi == NULL)||(fo == NULL)) {
			commom::LOG_INFO("open file error");
			return false;
		}		
		int score;
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = commom::GetLine(buffer);
			commom::Split("\t",str,v);
			if(v.size() != 2) continue;
			str = v.at(0);
			score = atof(v.at(1).c_str());
			if(score*rand()%100 > 150 ){
				str += "\t8\t0\n";
				commom::WiteLine(str.c_str(), fo);
			}
		}
		fclose(fi);
		fclose(fo);
		//free(buffer);
		return true;
	}
	bool comp(const std::pair<string, uininfo> & x, const std::pair<string, uininfo> & y){
		return x.second.score > y.second.score;
	}

	//tag 0:nogame;1:game 8:pingtai 9:random
	bool Check::Process(string taskid,string uinnumber){
		mySql.UpdataTask(taskid,string("5"));
		//读取check,读出文件名
		FILE* fi = fopen(string(PATH +taskid + ".check").c_str(), "r");
		//string filepath = ;
		FILE*fo = fopen(string(PATH + taskid + ".temp").c_str(),"ab+");
		if ((fi == NULL)||(fo == NULL)) {
			commom::LOG_INFO(taskid + ":\topen file error");
			mySql.UpdataTask(taskid,string("2"));
			return false;
		}
		std::string str = "";
		std::vector<string> v;
		char buffer[MAX_LENTH];	
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{			
			str = commom::GetLine(buffer);
			if(str.size() > 10){
				v.push_back(str);
			}
		}
		fclose(fi);
	    
		int line = 0;
		for(int j =0; j< v.size(); j++){			
			fi = fopen((PATH +v.at(j)).c_str(), "r");
			if (fi == NULL) {
				commom::LOG_INFO(taskid + ":\topen attemp file  error");
				mySql.UpdataTask(taskid,string("2"));
				return false;
			}	
			while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
				str = commom::GetLine(buffer);
				str += "\n";
				commom::WiteLine(str.c_str(), fo);
				line++;
			}
			fclose(fi);
		}
		fclose(fo);	
		//addpingtai addrandom
		AddRandom(string(PATH + taskid + ".temp").c_str(), line/10);	

		string fileout = PATH + taskid;
		fi = fopen(string(PATH + taskid + ".temp").c_str(), "r");
		fo = fopen(fileout.c_str(),"ab+");
		if ((fi == NULL)||(fo == NULL)) {
			commom::LOG_INFO(taskid + ":\topen file error");
			mySql.UpdataTask(taskid,string("2"));
			return false;
		}		
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = commom::GetLine(buffer);
			commom::Split("\t",str,v);
			if(v.size() != 3)continue;
			str = v.at(0) + "\n";
			commom::WiteLine(str.c_str(), fo);
		}
		fclose(fi);
		fclose(fo);
		//free(buffer);
		commom::LOG_INFO(taskid + ":\t process ok");
		mySql.UpdataTask(taskid,string("3"));
	}
	bool Check::Send(string taskid){
		mySql.UpdataTask(taskid,string("5"));
		string strscp = "rsync -av ";
		strscp += (PATH + taskid);
		strscp += " qspace@10.234.151.147:10.234.151.147::mmocgameuin/from_data_apply";//10.234.133.11
		system(strscp.c_str());
		//path += taskid;

		string fileout = PATH + taskid;
		FILE* fi = fopen(fileout.c_str(), "r");
		if (fi == NULL) {
			commom::LOG_INFO("open file error");
			mySql.UpdataTask(taskid,string("3"));
			return false;
		}		
		int linenumber = 0;
		char buffer[MAX_LENTH];	
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			linenumber++ ;
		}
		string sendstrpath = sendpath + taskid;
		if(myLz.SendUinStaue(taskid,commom::ConvertToStr(linenumber),sendstrpath)){
			commom::LOG_INFO("send");
			mySql.UpdataTask(taskid,string("4"));
		}else{
			mySql.UpdataTask(taskid,string("3"));
		}	
		fclose(fi);
		return false;		
	}

	/*
	bool Check::CheckTask(){
		std::vector<taskstaue> tasklist;
		while(1){
			//1:查询数据库状态			
			mySql.SelectTask(tasklist);			
			for(int i =0; i< tasklist.size(); ++i){
				//2:根据状态处理相应的任务
				if(tasklist.at(i).staue == "0"){
					CheckLz(tasklist.at(i).taskid);
				}else if(tasklist.at(i).staue == "1"){
					HdfsToLocal(tasklist.at(i).taskid);
				}else if(tasklist.at(i).staue == "2"){
					//Process(tasklist.at(i).taskid, tasklist.at(i).num);
				}else if(tasklist.at(i).staue == "3"){
					//Send(tasklist.at(i).taskid);
				}
			}
			std::vector<taskstaue>().swap(tasklist);
			//Sleep(10);
			for(int i =0; i< 100000; i++){
				for(int j =0; j< 100000; j++){
					//
				}
			}

		}
	}
	*/
	bool CheckTask(){
		int i,j = 0;
		//
		std::vector<taskstaue> tasklist;
		Check checker;
		while(1){			
			checker.mySql.SelectTask(tasklist);			
			for(i =0; i< tasklist.size(); ++i){
				//commom::DEBUG_INFO("check");
				//2:根据状态处理相应的任务
				if(tasklist.at(i).staue == "0"){
					checker.CheckLz(tasklist.at(i).taskid);
				}else if(tasklist.at(i).staue == "1"){
					checker.HdfsToLocal(tasklist.at(i).taskid);
				}else if(tasklist.at(i).staue == "2"){
					checker.Process(tasklist.at(i).taskid, tasklist.at(i).num);
				}else if(tasklist.at(i).staue == "3"){
					checker.Send(tasklist.at(i).taskid);
				}
			}			
			tasklist.clear();
			std::vector<taskstaue>().swap(tasklist);
			//Sleep(10);
			for(i =0; i< 100000; i++){
				for(j =0; j< 10000; j++){}
			}
			//checker.~Check();

		}
	}
}