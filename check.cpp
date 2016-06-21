#include "check.h"
namespace wordexpand{
	Check::Check(){
		Init();
		//uinsize = 0;
		path = "";
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
			//commom::LOG_INFO(taskid + ":\t open file error");
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
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer);
			f.Split("\t",str,v);
			if(v.size() != 2) continue;
			str = v.at(0);
			if(rand()%UINTOTAL < num){
				str += "\t9\t0\n";
				f.WiteLine(str.c_str(), fo);
			}
		}
		return true;
	}

	bool comp(const std::pair<string, uininfo> & x, const std::pair<string, uininfo> & y){
		return x.second.score > y.second.score;
	}

	//tag 0:biz 1:article 9:random
	bool Check::Process(string taskid,string uinnumber){
		mySql.UpdataTask(taskid,string("5"));
		//读取check,读出文件名
		string checkfile = PATH +taskid + ".check";
		FILE* fi = fopen(checkfile.c_str(), "r");
		string filepath = PATH + taskid + ".temp";
		FILE*fo = fopen(filepath.c_str(),"ab+");
		if ((fi == NULL)||(fo == NULL)) {
			commom::LOG_INFO(taskid + ":\topen file error");
			mySql.UpdataTask(taskid,string("2"));
			return false;
		}	
		std::string str = "";
		std::vector<string> v;
		char buffer[MAX_LENTH];	
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{			
			str = f.GetLine(buffer);
			if(str.size() > 10){
				v.push_back(str);
			}
		}
		fclose(fi);
	
		for(int j =0; j< v.size(); j++){
			//commom::LOG_INFO(v.at(j));
			fi = fopen((PATH +v.at(j)).c_str(), "r");
			if (fi == NULL) {
				commom::LOG_INFO(taskid + ":\topen attemp file  error");
				mySql.UpdataTask(taskid,string("2"));
				return false;
			}	
			while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
				str = f.GetLine(buffer);
				str += "\n";
				f.WiteLine(str.c_str(), fo);
			}
			fclose(fi);
		}
		fclose(fo);		
		
		string fileout = PATH + taskid;
		fi = fopen(filepath.c_str(), "r");
		fo = fopen(fileout.c_str(),"ab+");
		if ((fi == NULL)||(fo == NULL)) {
			commom::LOG_INFO(taskid + ":\topen file error");
			mySql.UpdataTask(taskid,string("2"));
			return false;
		}		
		uininfo tmp;
		std::map<string,uininfo>dict;

		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer);
			f.Split("\t",str,v);
			if(v.size() != 3)continue;
			tmp.tag= v.at(1);
			tmp.flag = "1";
			tmp.score = atof(v.at(2).c_str());
			if(dict.find(v.at(0)) == dict.end()){
				dict[v.at(0)] = tmp;
			}else{
				dict[v.at(0)].tag = "2";
				dict[v.at(0)].score += tmp.score;
			}
		}
		//取所有
		if(uinnumber == ""){
			for(std::map<string,uininfo>::iterator it = dict.begin(); it != dict.end(); it++){
				f.WiteLine((it->first + "\t" + it->second.tag + "\t" + it->second.flag + "\n").c_str(), fo);
			}
			fclose(fo);
			AddRandom(fileout.c_str(), dict.size()/10);			
		}else{
			int number = atoi(uinnumber.c_str());
			if(number >= dict.size()){
				for(std::map<string,uininfo>::iterator it = dict.begin(); it != dict.end(); it++){
					f.WiteLine((it->first + "\t" + it->second.tag + "\t" + it->second.flag + "\n").c_str(), fo);
				}
				fclose(fo);
				AddRandom(fileout.c_str(), dict.size()/10);		
			}else{
				//排序
				std::vector<std::pair<string, uininfo> >tmpvec;
				for(std::map<string,uininfo>::iterator it = dict.begin(); it != dict.end(); it++){
					tmpvec.push_back(*it);
				}
				sort(tmpvec.begin(), tmpvec.end(),comp);
				for(int j =0; j< number; j++){
					f.WiteLine((tmpvec.at(j).first + "\t" + tmpvec.at(j).second.tag + "\t" + tmpvec.at(j).second.flag + "\n").c_str(), fo);
				}
				fclose(fo);
				AddRandom(fileout.c_str(), number/10);		
			}
		}
		commom::LOG_INFO(taskid + ":\t process ok");
		mySql.UpdataTask(taskid,string("3"));
	}
	bool Check::Send(string taskid){
		mySql.UpdataTask(taskid,string("5"));
		string strscp = "rsync -av ";
		strscp += (PATH + taskid);
		strscp += " qspace@10.234.151.147::mmocgameuin/from_data_apply";//10.234.133.11
		system(strscp.c_str());
		path = "10.234.151.147::mmocgameuin//from_data_apply/";
		path += taskid;

		string fileout = PATH + taskid;
		FILE* fi = fopen(fileout.c_str(), "r");
		if (fi == NULL) {
			commom::LOG_INFO("open file error");
			mySql.UpdataTask(taskid,string("3"));
			return false;
		}		
		int linenumber = 0;
		char buffer[MAX_LENTH];	
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			linenumber++ ;
		}
		if(myLz.SendUinStaue(taskid,f.ConvertToStr(linenumber),path)){
			commom::LOG_INFO("send");
			mySql.UpdataTask(taskid,string("4"));
		}else{
			mySql.UpdataTask(taskid,string("3"));
		}		
		return false;		
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