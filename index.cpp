#include "index.h"
namespace wordexpand{

	//初始化索引
	bool Index::Init(const char* dictpath){
		mseg.InitDict(dictpath);
		commom::DEBUG_INFO("INIT OK");
		return true;
	}

	//初始化检索
	bool Index::InitRetrieval(const char* dictpath){
		mseg.InitDict(dictpath);
		FILE*fi = fopen((string(dictpath) + "gamefilterdict").c_str(),"r");
		if (fi == NULL) {
			commom::LOG_INFO("open file error");
			return false;
		}
		std::string str = "";
		char buffer[MAX_LENTH];	
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer); 
			f.Split(" ", str, gamefilterdict);
		}
		fclose(fi);
		InitMysql();
		commom::DEBUG_INFO("INIT OK");
		return true;
	}


	//*******************************INDEX**************************************//
	bool Index::BizIndex(const char* filein, const char* dbpath){
		commom::DEBUG_INFO("begin index");
		Xapian::WritableDatabase db(dbpath ,Xapian::DB_CREATE_OR_OPEN);
		Xapian::TermGenerator indexer;
		FILE*fi = fopen(filein,"r");
		if (fi == NULL) {
			commom::LOG_INFO("open file error" + string(filein));
			return false;
		}
		char buffer[MAX_LENTH];		
		std::string str = "";
		std::vector<std::string> v ;
		bizuin biz;
		int linenum = 0;
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer); 
			f.Split("\t", str, v);
			if(v.size() != 5)continue;
			biz.uin = v.at(0);
			biz.bizname = v.at(1);
			biz.bizdesc = v.at(2);
			biz.bizfunsnum = v.at(3);
			biz.massmsgcount = v.at(4);
			Xapian::Document doc;			
			doc.set_data(biz.uin);	
			indexer.set_document(doc);
			indexer.index_text(mseg.QuickSegement(biz.bizname.c_str()), 1, "T");
			indexer.index_text(mseg.QuickSegement(biz.bizdesc.c_str()), 1, "C");	
			
			/*
			for (Xapian::TermIterator iter = doc.termlist_begin(); iter != doc.termlist_end(); ++iter){
				std::cout<<*iter<<std::endl;
			}
			*/
			doc.add_value(0,biz.bizname);
			doc.add_value(1,biz.bizdesc);
			doc.add_value(2,biz.bizfunsnum);
			doc.add_value(3,biz.massmsgcount);
			db.add_document(doc);	
			if((linenum++)% 10000 == 0){
				commom::DEBUG_INFO(string(filein) + "\t" + f.ConvertToStr(linenum));
				db.flush();
			}			
		}
		db.flush();
		return true;
	}

	bool Index::ArticleIndex(const char* filein, const char* dbpath){
		commom::DEBUG_INFO("begin index");
		Xapian::WritableDatabase db(dbpath ,Xapian::DB_CREATE_OR_OPEN);
		Xapian::TermGenerator indexer;
		FILE*fi = fopen(filein,"r");
		if (fi == NULL) {
			commom::LOG_INFO("open file error" + string(filein));
			return false;
		}
		char buffer[MAX_LENTH];		
		std::string str = "";
		std::vector<std::string> v ;
		articleuin biz;
		int linenum = 0;
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer); 
			f.Split("\t", str, v);
			if(v.size() != 5)continue;
			biz.bizuin = v.at(0);
			biz.appmsgid = v.at(1);
			biz.itemidx = v.at(2);
			biz.title = v.at(3);
			biz.ds = v.at(4);
			Xapian::Document doc;			
			doc.set_data(biz.bizuin + "_" + biz.appmsgid + "_" + biz.itemidx);	
			indexer.set_document(doc);
			f.Split(" ", mseg.QuickSegement(biz.title.c_str()), v);
			str = "";
			/*
			for(int i = 0; i< v.size(); i++){
				if(v.at(i).size() >= 4){
					str += (v.at(i) + " ");
				}
			}
			*/
			indexer.index_text(str);
			doc.add_value(0,biz.ds);
			doc.add_value(1,biz.title);
			db.add_document(doc);		
			if((linenum++)%10000 == 0){
				db.commit();
			}			
		}
		db.flush();
		return true;
	}
	//*******************************INDEX**************************************//

	//*******************************RETREVIAL**********************************//
	bool Index::Retrieval(string& query,string& conf){
		/*构造conf*/
		conf = "";
		conf += ("task_id:" + f.ConvertToStr(GetTaskId()));
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

		std::vector<std::string> v ;
		std::vector<std::string> res ;
		std::vector<std::string> tmp ;
		std::map<string, string> taskinfo;
		std::vector<bizinfo> bizresults;	
		std::vector<articleinfo> articleresults;	
		
		//step1: query 检查,检查query结构， 参数是否完整		
		//step2: query 解析
		f.Split(",", query, v);
		f.Split(";", conf, res);
		for(int i = 0; i < res.size(); i++){
			f.Split(":",res.at(i),tmp);
			if(tmp.size() == 2){
				taskinfo[tmp.at(0)] = tmp.at(1);
			}
		}
		//step3: query 查询
		//更新任务表
		AddTask(taskinfo);
		//更新关键词表
		UpdataKeywords(v,taskinfo);
		UpdataKeywords(v,taskinfo);
		if(BizRetrieval(v,bizresults) != true){
			commom::LOG_INFO("BizRetrieval Error");
		}
		UpdataBiz(bizresults, taskinfo);
		if(ArticleRetrieval(v,articleresults) != true){
			commom::LOG_INFO("ArticleRetrieval Error");
		}
		//step4: query 组合排序、输出
		string dir = "./tmpdata/";
		string taskid = "111";
		string str = "";
		FILE*fo = fopen((dir + taskid).c_str(),"ab+");
		if (fo == NULL) {
			commom::LOG_INFO("open file error" + string(dir + taskid));
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
	bool Index::BizRetrieval(std::vector<string>& querylist,std::vector<bizinfo>& results){
		
		Xapian::Database db;
		db.add_database(Xapian::Database("../../data/database/bizindex/index/"));
		Xapian::Enquire enquire(db);
		return BizRetrieval(enquire, querylist,results,"OR");
	}

	bool Bizrank(const bizinfo& x, const bizinfo& y){
		return x.score > y.score;
	}
	//rank
	bool Index::Rank(Xapian::MSet& matches,std::vector<bizinfo>& results){
		bizinfo tmp;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			tmp.uin = i.get_document().get_data();
			tmp.bizname = i.get_document().get_value(0);
			tmp.bizdesc = i.get_document().get_value(1);
			if(FilerGame(tmp.bizname + tmp.bizdesc)){
				continue;
			}
			float score = (2*i.get_weight() + 3*log(atof((i.get_document().get_value(2)).c_str())) 
				+ log( atof((i.get_document().get_value(3)).c_str())))/100.00; 
			tmp.score = score;
			results.push_back(tmp);			
		}
		sort(results.begin(), results.end(),Bizrank);
		/*
		int x =  results.size() < 100 ? results.size() : 100 ;
		for(int i =0; i< x; i++){
			commom::DEBUG_INFO(results.at(i).bizname + "\t" + f.ConvertToStr(results.at(i).score));
		}
		*/
		return true;
	}

	

	bool Index::ArticleRank(Xapian::MSet& matches,std::vector<articleinfo>& results){
		articleinfo tmp;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			tmp.articleuin = i.get_document().get_data();
			tmp.title = i.get_document().get_value(1);
			tmp.score = i.get_weight() ;
			results.push_back(tmp);			
		}
		sort(results.begin(), results.end(),Bizrank);
		int x =  results.size() < 100 ? results.size() : 100 ;
		for(int i =0; i< x; i++){
			commom::DEBUG_INFO(results.at(i).title + "\t" + f.ConvertToStr(results.at(i).score));
		}
		return true;
	}



	//filter
	bool Index::FilerGame(string str){
		std::vector<std::string> v ;
		f.Split(" ",mseg.QuickSegement(str.c_str()),v);
		for(std::vector<string>::iterator it = v.begin(); it != v.end(); it++){
			if(gamefilterdict.find(*it) != gamefilterdict.end()){
				return true;
			}
		}
		return false;
	}

	string Index::JionQuery(std::vector<string>& querylist, string str){
		string query = "";
		//seg
		std::vector<string> v;
		for(int i = 0; i < querylist.size(); i++){
			query += "(";
			/*词内部之间关系，针对包含字母的*/
			f.Split(" ", mseg.QuickSegement(querylist.at(i).c_str()), v);
			if(v.size() == 1){
				query += ("(title:" + querylist.at(i) + ")");
			}else{
				//联合查询
				query += "(";
				for(int j = 0; j< v.size()-1; j++){
					query += ("(title:" + v.at(j) + ") OR ") ;
				}	
				query += ("(title:" + v.at(v.size() -1));
				query += "))";
			}
			/*词组之间关系*/
			query += (" " + str + " ");
			//f.Split(" ", querylist.at(i), v);
			if(v.size() == 1){
				query += ("(content:" + querylist.at(i) + ")");
			}else{
				//联合查询
				query += "(";
				for(int j = 0; j< v.size()-1; j++){
					query += ("content:" + v.at(j) + " OR ") ;
				}	
				query += ("content:" + v.at(v.size() -1));
				query += ")";
			}
			//query += ("content:" + querylist.at(i));	
			query += ")";
			if(i+1 <querylist.size()){
				query += " OR ";
			}
		}
		return query;
	}

	bool Index::BizRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
							std::vector<bizinfo>& results, const char* relationship){
		Xapian::QueryParser qp;
		qp.add_prefix("title", "T");
		qp.add_prefix("content", "C");
		for(int i =0; i< querylist.size(); i++){
			commom::DEBUG_INFO(querylist.at(i));
		}
		string query_string = JionQuery(querylist, relationship);
		commom::DEBUG_INFO(query_string);
		Xapian::Query query = qp.parse_query(query_string);
		enquire.set_query(query);
		enquire.set_sort_by_relevance_then_value(2);
		Xapian::MSet matches = enquire.get_mset(0, 1000000);
		Rank(matches,results);
		//TestRank(matches);
		return true;
	}


	bool Index::ArticleRetrieval(std::vector<string>& querylist,std::vector<articleinfo>& results){
		commom::DEBUG_INFO(f.ConvertToStr(querylist.size()));
		string P="../../data/articleindexdb/";
		Xapian::Database db(P);
		Xapian::Enquire enquire(db);
		commom::DEBUG_INFO("BEGIN RETRIEAL");
		ArticleRetrieval(enquire, querylist,results,"OR");
		return true;
	}
	bool Index::ArticleRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
		std::vector<articleinfo>& results, const char* relationship){
			Xapian::QueryParser qp;
			string query_string = "";
			for(int i = 0; i < querylist.size(); i++){
				query_string += query_string.at(i);
				if(i+1 <querylist.size()){
					query_string += " OR ";
				}
			}
			Xapian::Query query = qp.parse_query(query_string);
			enquire.set_query(query);
			enquire.set_sort_by_relevance_then_value(2);
			Xapian::MSet matches = enquire.get_mset(0, 1000000);
			ArticleRank(matches,results);
			return true;
	}
	//*******************************RETREVIAL**********************************//

	//////////////////////////////***********TEST************/////////////////
	void Index::TestRank(Xapian::MSet& matches){
		std::vector<std::pair<string, float> > results;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			if((FilerGame(i.get_document().get_data() + i.get_document().get_value(1)))){
				continue;
			}
			string str = i.get_document().get_data()+"\t" 
						+ f.ConvertToStr(i.get_weight()) + "\t"
						+ f.ConvertToStr(i.get_document().get_value(0)) + "\t"
						+ f.ConvertToStr(i.get_document().get_value(2))+ "\t"
						+ f.ConvertToStr(i.get_document().get_value(1));
			float score = (2*i.get_weight() + 3*log(atof((i.get_document().get_value(2)).c_str())) 
						+ log( atof((i.get_document().get_value(3)).c_str())))/100.00; 		
			results.push_back(std::pair<string, float>(str,score));				        
		}
		sort(results.begin(), results.end(), f.SortBySecondGreater);
		int x =  results.size() < 100 ? results.size() : 100 ;
		for(int i =0; i< x; i++){
			commom::DEBUG_INFO(results.at(i).first + "\t" + f.ConvertToStr(results.at(i).second));
		}
	}

	bool Index::TestRetrieval(){
		FILE*fi  = fopen("./input","r");
		if (fi == NULL) {
			commom::LOG_INFO("open file error");
			return false;
		}		
		std::string str = "";
		char buffer[MAX_LENTH];		
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer);
			string teststr = "";
			Retrieval(str, teststr);
		}
		return true;	
	}
	//////////////////////////////***********TEST************/////////////////

	//////////////////////////////***********MYSQL************/////////////////
	bool Index::InitMysql(){
		mysql_init(&myCont);
		if(!mysql_real_connect(&myCont,"10.234.148.106","qspace","forconnect","stat",3306,NULL,0)){
			commom::LOG_INFO("connect mysql error");
			return false;
		}
		commom::DEBUG_INFO("init mysql ok");
		return true;
	}

	bool Index::ExeQuery(string& query){
		if(mysql_query(&myCont, query.c_str())){  
			commom::LOG_INFO("Query Error:" + string(mysql_error(&myCont)));
			return false;
		}else{
			//检查插入结果
			commom::DEBUG_INFO("ExeQuery OK");
			return true;
		}
	}

	bool Index::AddTask(std::map<string, string>& taskinfo){
		string strsql = "";
		strsql += "insert into keyword_task set ";
		strsql += ("task_id= " + taskinfo["task_id"]);
		strsql += (", task_version= "+ taskinfo["task_version"]);
		strsql += (", task_name= " + taskinfo["task_name"]);
		strsql += (", task_cname= " + taskinfo["task_cname"]);
		strsql += (", input= " + taskinfo["input"]);
		strsql += (", step= " + taskinfo["step"]);
		strsql += (", status= " + taskinfo["status"]);
		strsql += (", pack_name= " + taskinfo["pack_name"]);;
		strsql += (", done_name= " + taskinfo["done_name"]);
		strsql += (", user_num= " + taskinfo["user_num"]);
		strsql += (", biz_url=" + taskinfo["biz_url"]);
		strsql += (", creator=" + taskinfo["creator"]);
		strsql += (", modify_user= " + taskinfo["modify_user"]);
		strsql += (", create_time= " + taskinfo["create_time"]);
		strsql += (", modify_time= " + taskinfo["modify_time"]);
		commom::DEBUG_INFO(strsql);
		return ExeQuery(strsql);
	}
	bool Index::UpdataKeywords(std::vector<std::string>& v, std::map<string, string>& taskinfo){
		for(int i = 0; i< v.size(); i++){
			string strsql = "";
			strsql += "insert into keyword_word_filter_result set ";
			strsql += (" task_id= " + taskinfo["task_id"]);
			strsql += (", task_version= " + taskinfo["task_version"]);
			strsql += (", keyword= \"" + v.at(i));
			strsql += ("\", source= \"\"");
			strsql += (", score= 1");
			strsql += (", iteration= 1");
			strsql += (", delete_status = 0");
			strsql += (", modify_user= " + taskinfo["modify_user"]);
			strsql += (", modify_time= " + taskinfo["modify_time"]);
			commom::DEBUG_INFO(strsql);
			ExeQuery(strsql);
		}	
		return true;
	}
	bool Index::UpdataBiz(std::vector<bizinfo>& results,std::map<string, string>& taskinfo){
		for(int i = 0; i< results.size(); i++){
			string strsql = "";
			strsql += "insert into keyword_biz_filter_result set ";
			strsql += (" task_id= " + taskinfo["task_id"]);
			strsql += (", task_version= " + taskinfo["task_version"]);
			strsql += (", biz= \"" + results.at(i).uin);
			strsql += ("\", biz_title=\" " + results.at(i).bizname);
			strsql += ("\", biz_info= \"" + results.at(i).bizdesc);
			strsql += ("\", biz_auth=\"\"");
			strsql += (", biz_sub= 1");
			strsql += (", match_list = \"\"");
			strsql += (", type= 0");
			strsql += (", source_word=\"\"");
			strsql += (", source = \"\"");
			strsql += (", score= " + f.ConvertToStr(results.at(i).score));
			strsql += (", iteration = 1");
			strsql += (", delete_status = 0");
			strsql += (", modify_user= " + taskinfo["modify_user"]);
			strsql += (", modify_time= " + taskinfo["modify_time"]);
			commom::DEBUG_INFO(strsql);
			ExeQuery(strsql);
		}	
		return true;
	}

	int Index::GetTaskId(){
		int id = 0;
		string sqlstr = "select max(task_id) as task_id from keyword_task";
		ExeQuery(sqlstr);
		MYSQL_RES *result = mysql_use_result(&myCont);
		MYSQL_ROW row;  
		if(mysql_field_count(&myCont) != 1){
			commom::LOG_INFO("GetTaskId Error");
		}else{
			row = mysql_fetch_row(result);  
		}
		if(mysql_num_fields(result) != 1){
			commom::DEBUG_INFO("GetTaskId Error");
		}else{
			id =  atoi(row[0]);
		}
		mysql_free_result(result);  
		return id + 1;
	}
	//////////////////////////////***********MYSQL************////////////////
}