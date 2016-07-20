//Ë÷ÒýÀà
#ifndef _INDEX_H
#define _INDEX_H
#include "../commom/func.h"
#include "../wordseg/seg.h"
#include <xapian.h>
#include "sql.h"
namespace wordexpand{
 class Index{
	public:
		Index(){}
		~Index(){}
	public://biz
		seg::Wordseg mseg;
	public:
		bool Init(const char* dictpath);
		bool InitRetrieval(const char* dictpath);
		bool RoomIndex(const char* filein, const char* dbpath);
		bool BizIndex(const char* filein, const char* dbpath);
		bool Expand(std::vector<std::string>& query, std::vector<string>& v);
		bool Retrieval(string& source, string& query,string& uinnumber,Sql& mySql,std::map<string, string>& taskinfo,
				std::vector<bizinfo>& bizresults,
				std::vector<roominfo>& roomresults);
		bool RoomRetrieval(Xapian::Query& query,std::vector<roominfo>& roomresults,std::map<string,int>& bizdict);
		bool BizRetrievalAll(Xapian::Query& query,std::vector<bizinfo>& results,std::map<string,int>& bizdict);
		bool BizRetrieval(Xapian::Query& query,std::vector<bizinfo>& results,std::map<string,int>& bizdict);
		
	public://article
		string JionQuery(std::vector<string>& querylist);
	public:
		void TestRank(Xapian::MSet& matches);
	};

}
#endif








































