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
			for(int i = 0; i< v.size(); i++){
				if(v.at(i).size() >= 4){
					str += (v.at(i) + " ");
				}
			}
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
	bool Index::Retrieval(string& source, string& query,Sql& mySql,std::map<string, string>& taskinfo,
		std::vector<bizinfo>& bizresults,
		std::vector<articleinfo>& articleresults){
		std::vector<std::string> v ;
		//step1: query 检查,检查query结构， 参数是否完整		
		//step2: query 解析
		f.Split("_", query, v);
		//step3: query 查询
		//更新任务表
		mySql.AddTask(taskinfo);
		//更新关键词表
		//UpdataKeywords(v,taskinfo);

		//选择内容源
		if(source.at(0) == '1'){
			if(BizRetrieval(v,bizresults) != true){
				commom::LOG_INFO("BizRetrieval Error");
			}
		}
		if(source.at(1) == '1'){
			commom::LOG_INFO("friends");
		}
		if(source.at(2) == '1'){
			if(ArticleRetrieval(v,articleresults) != true){
				commom::LOG_INFO("ArticleRetrieval Error");
			}
		}
		
		//UpdataBiz(bizresults, taskinfo);		
		//step4: query 组合排序、输出
		return true;

	}
	bool Index::BizRetrieval(std::vector<string>& querylist,std::vector<bizinfo>& results){
		
		Xapian::Database db;
		db.add_database(Xapian::Database("../../data/database/bizindex/indexnew/"));
		commom::DEBUG_INFO("OPEN BIZINDEX OK");
		Xapian::Enquire enquire(db);
		return BizRetrieval(enquire, querylist,results,"OR");
	}

	bool Bizrank(const bizinfo& x, const bizinfo& y){
		return x.score > y.score;
	}
	bool Articlerank(const articleinfo& x, const articleinfo& y){
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
		int x =  results.size() < 100 ? results.size() : 100 ;
		for(int i =0; i< x; i++){
			commom::DEBUG_INFO(results.at(i).bizname + "\t" + f.ConvertToStr(results.at(i).score));
		}
		return true;
	}

	//文章排序
	bool Index::ArticleRank(Xapian::MSet& matches,std::vector<articleinfo>& results){
		articleinfo tmp;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			tmp.articleuin = i.get_document().get_data();
			tmp.title = i.get_document().get_value(1);
			tmp.score = i.get_weight() ;
			results.push_back(tmp);			
		}
		sort(results.begin(), results.end(),Articlerank);
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
			query += ("(title:" + querylist.at(i) + ")");
			query += (" " + str + " ");
			query += ("content:" + querylist.at(i));	
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
		Xapian::Database db;
		std::map<string,int> dblist;
		f.GetDirfile(dblist,"../../data/database/gamearticleindex/");
		for(std::map<string,int>::iterator it = dblist.begin(); it != dblist.end(); it++){
			if((it->first.size() != 8)||(it->first > "20160610")||(it->first < "20160500")){
				continue;
			}
			string dbpath = "../../data/database/gamearticleindex/" + it->first + string("/");
			commom::DEBUG_INFO(dbpath);
			db.add_database(Xapian::Database(dbpath.c_str()));
		}
		/*
		for(int i = 20160506; i<=20160610; i++){
			string dbpath = "../../data/database/gamedailyarticle/" + f.ConvertToStr(i) + string("/");
			commom::DEBUG_INFO(dbpath);
			db.add_database(Xapian::Database(dbpath.c_str()));
		}
		*/
		commom::DEBUG_INFO("OPEN ARTICLEINDEX OK");
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
				query_string += querylist.at(i);
				if(i+1 <querylist.size()){
					query_string += " OR ";
				}
			}
			commom::DEBUG_INFO(query_string);
			Xapian::Query query = qp.parse_query(query_string);
			enquire.set_query(query);
			//enquire.set_sort_by_relevance_then_value(2);
			Xapian::MSet matches = enquire.get_mset(0, 1000000);
			commom::DEBUG_INFO(f.ConvertToStr(matches.size()));
			ArticleRank(matches,results);
			return true;
	}

}