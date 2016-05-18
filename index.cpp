#include "index.h"
namespace wordexpand{
	bool Index::Init(const char* dictpath){
		mseg.InitDict(dictpath);
		commom::DEBUG_INFO("INIT OK");
		return true;
	}
	bool Index::BizIndex(const char* filein, const char* dbpath){
		commom::DEBUG_INFO("begin index");
		Xapian::WritableDatabase db(dbpath,Xapian::DB_CREATE_OR_OPEN);
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
			db.add_document(doc);
			if(linenum++%200 == 0){
				linenum = 0;
				db.flush();
			}			
		}
		db.flush();
		return true;
	}

	bool Index::Retrieval(){
		string P="./db";
		Xapian::Database db(P);
		Xapian::Enquire enquire(db);
		
		string query_string = "a";
		Xapian::QueryParser qp;
		//#define QUERY "title:新闻 AND content:男篮"
		qp.add_prefix("title", "T");
		qp.add_prefix("content", "C");

		//AND
		commom::DEBUG_INFO("AND");
		query_string = "title:a AND content:a";
		//Xapian::Query query = qp.parse_query(query_string,7U,"T");
		Xapian::Query query = qp.parse_query(query_string);
		enquire.set_query(query);
		Xapian::MSet matches = enquire.get_mset(0, 10);
		std::cout << matches.get_matches_estimated() << " results found.\n";
		std::cout << "Matches 1-" << matches.size() << ":\n" << std::endl;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			//std::cout << i.get_rank() + 1 <<": "  << " docid=" << *i<<std::endl;
			std::cout<<i.get_document().get_data()<<std::endl;
		}
		//OR

		commom::DEBUG_INFO("OR");
		query_string = "title:a OR content:a";
		//Xapian::Query query = qp.parse_query(query_string,7U,"T");
		query = qp.parse_query(query_string);
		enquire.set_query(query);
		matches = enquire.get_mset(0, 10);
		std::cout << matches.get_matches_estimated() << " results found.\n";
		std::cout << "Matches 1-" << matches.size() << ":\n" << std::endl;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			//std::cout << i.get_rank() + 1 <<": "  << " docid=" << *i<<std::endl;
			std::cout<<i.get_document().get_data()<<std::endl;
		}
		//NOT

		commom::DEBUG_INFO("NOT");
		query_string = "title:a NOT content:a";
		//Xapian::Query query = qp.parse_query(query_string,7U,"T");
		query = qp.parse_query(query_string);
		enquire.set_query(query);
		matches = enquire.get_mset(0, 10);
		std::cout << matches.get_matches_estimated() << " results found.\n";
		std::cout << "Matches 1-" << matches.size() << ":\n" << std::endl;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			//std::cout << i.get_rank() + 1 <<": "  << " docid=" << *i<<std::endl;
			std::cout<<i.get_document().get_data()<<std::endl;
		}


	}
	bool Retrieval(std::vector<string>& querylist,std::vector<std::pair<string, float> >& results){
		string P="./db";
		Xapian::Database db(P);
		Xapian::Enquire enquire(db);

		string query_string = "a";
		Xapian::QueryParser qp;
		//#define QUERY "title:新闻 AND content:男篮"
		qp.add_prefix("title", "T");
		qp.add_prefix("content", "C");

		//AND
		commom::DEBUG_INFO("AND");
		query_string = "title:a AND content:a";
		//Xapian::Query query = qp.parse_query(query_string,7U,"T");
		Xapian::Query query = qp.parse_query(query_string);
		enquire.set_query(query);
		Xapian::MSet matches = enquire.get_mset(0, 10);
		std::cout << matches.get_matches_estimated() << " results found.\n";
		std::cout << "Matches 1-" << matches.size() << ":\n" << std::endl;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			//std::cout << i.get_rank() + 1 <<": "  << " docid=" << *i<<std::endl;
			std::cout<<i.get_document().get_data()<<std::endl;
		}

		return true;
	}

	string JionQuery(std::vector<string>& querylist, string str){
		string query = "";
		for(std::vector<string>::iterator it = querylist.begin(); it != querylist.end(); it++){
			query += ("title:" + *it);
			query += (" " + str + " ");
			query += ("content:" + *it);
		}
	}
	bool RetrievalAll(Xapian::Enquire& enquire, std::vector<string>& querylist,
		std::vector<std::pair<string, float> >& results){
		Xapian::QueryParser qp;
		qp.add_prefix("title", "T");
		qp.add_prefix("content", "C");
		string query_string = JionQuery(querylist, "AND");
		Xapian::Query query = qp.parse_query(query_string);
		enquire.set_query(query);
		enquire.set_sort_by_relevance_then_value(2);
		//Xapian::MSet matches = enquire.get_mset(0,1000);
		Xapian::MSet matches = enquire.get_mset(0, 10);
		std::cout << matches.get_matches_estimated() << " results found.\n";
		std::cout << "Matches 1-" << matches.size() << ":\n" << std::endl;
		for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i){
			std::cout<<i.get_document().get_data()<<"\t"<<i.get_document().get_value(2)<<
			i.get_document().get_value(0)<<"\t"<<i.get_document().get_value(1)<<std::endl;
		}
		return true;
	}


}