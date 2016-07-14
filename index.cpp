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
		commom::DEBUG_INFO("InitRetrieval");
		mseg.InitDict(dictpath);
		commom::DEBUG_INFO("INIT OK");
		return true;
	}
	
	//*******************************INDEX**************************************//
	bool Index::RoomIndex(const char* filein, const char* dbpath){
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
		int linenum = 0;
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = commom::GetLine(buffer); 
			commom::Split("\t", str, v);
			if(v.size() != 2){
				commom::DEBUG_INFO("continue");
				continue;
			}
			Xapian::Document doc;			
			doc.set_data(v.at(1));	
			doc.add_value(0,v.at(0));
			indexer.set_document(doc);
			indexer.index_text(mseg.Segement(v.at(0).c_str(),1));
			db.add_document(doc);	
			if((++linenum)% 10000 == 0){
				commom::DEBUG_INFO(string(filein) + "\t" + commom::ConvertToStr(linenum));
				db.flush();
			}			
		}
		db.flush();
		return true;
	}

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
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = commom::GetLine(buffer); 
			commom::Split("\t", str, v);
			if(v.size() != 5){
				commom::DEBUG_INFO("continue");
				continue;
			}
			biz.uin = v.at(0);
			biz.bizname = v.at(1);
			biz.bizdesc = v.at(2);
			biz.bizfunsnum = v.at(3);
			biz.type = "";
			biz.topic = "";
			Xapian::Document doc;			
			doc.set_data(biz.uin);	
			indexer.set_document(doc);
			//indexer.index_text(mseg.Segement((biz.bizname + " " + biz.bizdesc + " " + biz.type).c_str(),1));
			indexer.index_text(mseg.Segement((biz.bizname + " " + biz.bizdesc + " " + biz.type).c_str()));
			/*
			doc.add_value(0,biz.bizname);
			doc.add_value(1,biz.bizdesc);
			doc.add_value(2,biz.bizfunsnum);
			doc.add_value(3,biz.type);
			*/
			doc.add_value(0,biz.bizname);
			doc.add_value(1,biz.bizfunsnum);
			db.add_document(doc);	
			if((++linenum)% 10000 == 0){
				commom::DEBUG_INFO(string(filein) + "\t" + commom::ConvertToStr(linenum));
				db.flush();
			}			
		}
		db.flush();
		return true;
	}

	bool Index::Retrieval(string& source, string& query,string& uinnumber,Sql& mySql,std::map<string, string>& taskinfo,
		std::vector<bizinfo>& bizresults,
		std::vector<roominfo>& roomresults){
		std::vector<std::string> v ;
		//step1: query 检查,检查query结构， 参数是否完整		
		//step2: query 解析
		commom::Split("_", query, v);
		for(std::vector<std::string>::iterator it = v.begin(); it != v.end();){
			if((*it == "")||((*it).find(" ")!= string::npos)){
				it = v.erase(it);
			}else{
				it++;
			}
		}
		//step3: query 查询
		//更新任务表
		mySql.AddTask(taskinfo);
		//更新关键词表
		mySql.UpdataKeywords(v,taskinfo);

		//扩散
		if(uinnumber == ""){
			//不扩散，关联扩散
		}else{
			//扩散，品类扩散和关联扩散
			Expand(v,v);
			commom::LOG_INFO("EXPANDWORDS");
			for(std::vector<std::string>::iterator it= v.begin(); it != v.end(); it++ ){
				commom::DEBUG_INFO(*it);
			}
		}
		//选择源
		if(source.at(1) == '1'){
			//群扩散
			if(RoomRetrieval(v,roomresults) != true){
				commom::LOG_INFO("BizRetrieval Error");
			}	
		}
		if(source.at(2) == '1'){
			if(BizRetrieval(v,bizresults) != true){
				commom::LOG_INFO("BizRetrieval Error");
			}
		}
		//mySql.UpdataBiz(bizresults, taskinfo);		
		return true;
	}

	bool Bizrank(const bizinfo& x, const bizinfo& y){
		return x.score > y.score;
	}
	bool Index::Expand(std::vector<std::string>& query, std::vector<string>& v){
		Xapian::Database db;
		std::vector<bizinfo> results;
		db.add_database(Xapian::Database("../../data/database/gamebizindex/"));
		commom::DEBUG_INFO("OPEN BIZINDEX OK");
		Xapian::Enquire enquire(db);
		Xapian::QueryParser qp;
		string query_string = JionQuery(query);
		commom::DEBUG_INFO(query_string);
		Xapian::Query querystr = qp.parse_query(query_string);
		enquire.set_query(querystr);
		enquire.set_sort_by_relevance_then_value(2);
		Xapian::MSet matches = enquire.get_mset(0, 1000000);
		bizinfo tmp;
		tmp.topic = "";
		std::map<string,float>dict;
		string str = "";
		std::vector<string> vv;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			tmp.uin = i.get_document().get_data();
			tmp.bizname = i.get_document().get_value(0);
			tmp.bizdesc = i.get_document().get_value(1);
			float score = (2*i.get_weight() + 3*log(atof((i.get_document().get_value(2)).c_str())))/100.00; 
			tmp.score = score;
			tmp.type = i.get_document().get_value(3);
			results.push_back(tmp);	
		}
		sort(results.begin(), results.end(),Bizrank);
		for(int i = 0; i< 3; i++){
			str = results.at(i).type;
			commom::DEBUG_INFO(str);
			commom::Split(",", str, vv);
			commom::DEBUG_INFO(commom::ConvertToStr(vv.size()));
			for(int j =0 ;j< vv.size(); j++){
				if(vv.at(j) != ""){
					dict[vv.at(j)] += pow(results.at(i).score,3);
				}
			}
		}
		vv.clear();
		string typestr = "";
		float num = 0;
		for(std::map<string,float>::iterator it =dict.begin(); it != dict.end(); it++){
			commom::DEBUG_INFO(commom::ConvertToStr(it->second));
			if(it->second > num){
				num = it->second;
				typestr = it->first;
			}
		}
		db.close();
		commom::DEBUG_INFO("expandtype");
		commom::DEBUG_INFO(typestr);

		string expandpath = "../../data/game/type.dict";
		FILE*fi = fopen(expandpath.c_str(),"r");
		if (fi == NULL) {
			commom::LOG_INFO("open file error" + string(expandpath));
			return false;
		}
		char buffer[MAX_LENTH];		
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			str = commom::GetLine(buffer); 
			commom::Split("\t", str, vv);
			if(vv.size() != 2)continue;
			if(typestr == vv.at(1)){
				v.push_back(vv.at(0));
			}					
		}
		fclose(fi);
		return true;
	}

	bool Index::RoomRetrieval(std::vector<string>& querylist,std::vector<roominfo>& roomresults){
		Xapian::Database db;
		db.add_database(Xapian::Database("../../data/database/roomindex/"));
		commom::DEBUG_INFO("OPEN BIZINDEX OK");
		Xapian::Enquire enquire(db);
		Xapian::QueryParser qp;
		string query_string = JionQuery(querylist);
		commom::DEBUG_INFO(query_string);
		Xapian::Query query = qp.parse_query(query_string);
		enquire.set_query(query);
		Xapian::MSet matches = enquire.get_mset(0, 1000000);
		roominfo tmp;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			tmp.score = i.get_weight();
			tmp.roomid  = i.get_document().get_data();
			tmp.title = i.get_document().get_value(0);
			//commom::LOG_INFO(tmp.title + " \t" + commom::ConvertToStr(i.get_weight()));
			roomresults.push_back(tmp);	
		}	
		return true;
	}

	bool Index::BizRetrieval(std::vector<string>& querylist,std::vector<bizinfo>& results){		
		Xapian::Database db;
		db.add_database(Xapian::Database("../../data/database/gamebizindex/"));
		commom::DEBUG_INFO("OPEN BIZINDEX OK");
		Xapian::Enquire enquire(db);
		Xapian::QueryParser qp;
		for(int i =0; i< querylist.size(); i++){
			commom::DEBUG_INFO(querylist.at(i));
		}
		string query_string = JionQuery(querylist);
		commom::DEBUG_INFO(query_string);
		Xapian::Query query = qp.parse_query(query_string);
		enquire.set_query(query);
		enquire.set_sort_by_relevance_then_value(2);
		Xapian::MSet matches = enquire.get_mset(0, 1000000);
		bizinfo tmp;
		tmp.topic = "";
		tmp.type = "";
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			tmp.uin = i.get_document().get_data();
			tmp.bizname = i.get_document().get_value(0);
			tmp.bizdesc = i.get_document().get_value(1);
			float score = (2*i.get_weight() + 3*log(atof((i.get_document().get_value(2)).c_str())))/100.00; 
			tmp.score = score;
			tmp.type = i.get_document().get_value(3);
			results.push_back(tmp);	
		}
		sort(results.begin(), results.end(),Bizrank);		
		for(int i =0; i< results.size(); i++){
			commom::DEBUG_INFO(results.at(i).bizname + "\t" + results.at(i).bizdesc + "\t" + commom::ConvertToStr(results.at(i).score));	
		}
		return true;
	}


	//检索全量公众号
	bool Index::BizRetrievalAll(std::vector<string>& querylist,std::vector<bizinfo>& results){
		Xapian::Database db;
		db.add_database(Xapian::Database("../../data/database/bizindex/index/"));
		commom::DEBUG_INFO("OPEN BIZINDEX OK");
		Xapian::Enquire enquire(db);
		Xapian::QueryParser qp;
		string query_string = JionQuery(querylist);
		commom::DEBUG_INFO(query_string);
		Xapian::Query query = qp.parse_query(query_string);
		enquire.set_query(query);
		enquire.set_sort_by_relevance_then_value(1);
		Xapian::MSet matches = enquire.get_mset(0, 1000000);
		bizinfo tmp;
		tmp.topic = "";
		tmp.type = "";
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			tmp.uin = i.get_document().get_data();
			tmp.bizname = i.get_document().get_value(0);
			float score = (2*i.get_weight() + 3*log(atof((i.get_document().get_value(1)).c_str())))/100.00; 
			tmp.score = score;			
			results.push_back(tmp);	
		}	
		for(int i =0; i< results.size(); i++){
			commom::DEBUG_INFO(results.at(i).bizname + "\t" + results.at(i).bizdesc + "\t" + commom::ConvertToStr(results.at(i).score));	
		}
		db.close();
		return true;
	}
	string Index::JionQuery(std::vector<string>& querylist){
		string query = "";
		//seg
		std::vector<string> v;
		for(int i = 0; i < querylist.size(); i++){	
			commom::Split(" ",mseg.Segement(querylist.at(i).c_str()), v);
			if(v.size() == 0) return "";
			if(v.size() == 1){
				query += querylist.at(i);
			}else{
				query += "(";
				for(int j =0; j< v.size() -1; j++){
					query += (v.at(j) + " AND ");
				}
				query += (v.at(v.size() -1));
				query += ")";
			}	
			if(i+1 <querylist.size()){
				query += " OR ";
			}
		}
		return query;
	}
}