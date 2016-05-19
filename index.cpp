#include "index.h"
namespace wordexpand{
	bool Index::Init(const char* dictpath){
		mseg.InitDict(dictpath);
		commom::DEBUG_INFO("INIT OK");
		return true;
	}
	bool Index::BizIndex(const char* filein, const char* dbpath){
		commom::DEBUG_INFO("begin index");
		Xapian::WritableDatabase dba(dbpath + string("indexa/"),Xapian::DB_CREATE_OR_OPEN);
		Xapian::WritableDatabase dbb(dbpath + string("indexb/"),Xapian::DB_CREATE_OR_OPEN);
		Xapian::WritableDatabase dbc(dbpath + string("indexc/"),Xapian::DB_CREATE_OR_OPEN);
		Xapian::WritableDatabase dbd(dbpath + string("indexd/"),Xapian::DB_CREATE_OR_OPEN);
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
			indexer.index_text(mseg.Segement(biz.bizname.c_str()), 1, "T");
			indexer.index_text(mseg.Segement(biz.bizdesc.c_str()), 1, "C");	
			/*
			indexer.index_text(biz.bizname);
			indexer.index_text(biz.bizdesc);	
			for (Xapian::TermIterator iter = doc.termlist_begin(); iter != doc.termlist_end(); ++iter){
				std::cout<<*iter<<std::endl;
			}
			*/
			doc.add_value(0,biz.bizname);
			doc.add_value(1,biz.bizdesc);
			doc.add_value(2,biz.bizfunsnum);
			doc.add_value(3,biz.massmsgcount);
			double fre = atof(biz.bizfunsnum.c_str());
			if(fre > 5000){
				dba.add_document(doc);
			}else if(fre > 700){
				dbb.add_document(doc);
			}else if(fre > 250){
				dbc.add_document(doc);
			}else{
				dbd.add_document(doc);
			}			
			if((linenum)++%5000 == 0){
				linenum = 0;
				dba.flush();
				dbb.flush();
				dbc.flush();
				dbd.flush();
			}			
		}
		dba.flush();
		dbb.flush();
		dbc.flush();
		dbd.flush();
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
			/*
			for (Xapian::TermIterator iter = doc.termlist_begin(); iter != doc.termlist_end(); ++iter){
				std::cout<<*iter<<std::endl;
			}
			*/
			doc.add_value(0,biz.ds);
			db.add_document(doc);		
			if((linenum++)%500 == 0){
				linenum = 0;
				db.flush();
			}			
		}
		db.flush();
		return true;
	}

	bool Index::Retrieval(){
		FILE*fi = fopen("./input","r");
		if (fi == NULL) {
			commom::LOG_INFO("open file error");
			return false;
		}
		char buffer[MAX_LENTH];		
		std::string str = "";
		std::vector<std::string> v ;
		std::vector<std::pair<string, float> > results;		
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer); 
			f.Split("\t", str, v);
			Retrieval(v,results);	
		}
		return true;	
		//OP_VALUE_GE 条件查询


	}
	bool Index::Retrieval(std::vector<string>& querylist,std::vector<std::pair<string, float> >& results){
		commom::DEBUG_INFO(f.ConvertToStr(querylist.size()));
		string P="../../data/indexdb/";
		Xapian::Database db(P);
		Xapian::Enquire enquire(db);
		commom::DEBUG_INFO("BEGIN RETRIEAL");
		Retrieval(enquire, querylist,results,"OR");
		return true;
	}

	bool Index::ArticleRetrieval(std::vector<string>& querylist,std::vector<std::pair<string, float> >& results){
		commom::DEBUG_INFO(f.ConvertToStr(querylist.size()));
		string P="../../data/articleindexdb/";
		Xapian::Database db(P);
		Xapian::Enquire enquire(db);
		commom::DEBUG_INFO("BEGIN RETRIEAL");
		ArticleRetrieval(enquire, querylist,results,"OR");
		return true;
	}


	string Index::JionQuery(std::vector<string>& querylist, string str){
		string query = "";
		for(int i = 0; i < querylist.size(); i++){
			query += "(";
			query += ("title:" + querylist.at(i));
			query += (" " + str + " ");
			query += ("content:" + querylist.at(i));	
			query += ")";
			if(i+1 <querylist.size()){
				query += " OR ";
			}
		}
		return query;
	}

	//检索标题和描述
	bool Index::Retrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
		std::vector<std::pair<string, float> >& results, const char* relationship){
		Xapian::QueryParser qp;
		qp.add_prefix("title", "T");
		qp.add_prefix("content", "C");
		for(int i =0; i< querylist.size(); i++){
			commom::DEBUG_INFO(querylist.at(i));
		}
		string query_string = JionQuery(querylist, relationship);
		Xapian::Query query = qp.parse_query(query_string);
		enquire.set_query(query);
		enquire.set_sort_by_relevance_then_value(2);
		Xapian::MSet matches = enquire.get_mset(0, 1000000);
		Rank(matches,results);
		TestRank(matches);
		return true;
	}

	bool Index::ArticleRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
		std::vector<std::pair<string, float> >& results, const char* relationship){
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
			Rank(matches,results);
			TestRank(matches);
			return true;

	}
	bool Index::Rank(Xapian::MSet& matches,std::vector<std::pair<string, float> >& results){
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			string str = i.get_document().get_data();
			float score = (2*i.get_weight() + 3*log(atof((i.get_document().get_value(2)).c_str())) 
				+ log( atof((i.get_document().get_value(3)).c_str())))/100.00; 
			results.push_back(std::pair<string, float>(str,score));			
		}
		sort(results.begin(), results.end(), f.SortBySecondGreater);
		return true;
	}

	void Index::TestRank(Xapian::MSet& matches){
		std::vector<std::pair<string, float> > results;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			string str = i.get_document().get_data()+"\t" 
						+ f.ConvertToStr(i.get_weight()) + "\t"
						+ f.ConvertToStr(i.get_document().get_value(0)) + "\t"
						+ f.ConvertToStr(i.get_document().get_value(2));
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

}