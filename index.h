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
		////commom::Func f;	;	;
		seg::Wordseg mseg;
		std::map<string,string>expanddict;
		std::map<string, int>gamefilterdict;
		//Sql mySql;

	public://biz
		bool Init(const char* dictpath);
		bool InitRetrieval(const char* dictpath);
		bool RoomIndex(const char* filein, const char* dbpath);
		bool BizIndex(const char* filein, const char* dbpath);
		bool Expand(std::vector<std::string>& query, std::vector<string>& v);
		bool Retrieval(string& source, string& query,string& uinnumber,Sql& mySql,std::map<string, string>& taskinfo,
				std::vector<bizinfo>& bizresults,
				std::vector<roominfo>& roomresults);
		bool RoomRetrieval(std::vector<string>& querylist,std::vector<roominfo>& roomresults);
		bool BizRetrieval(std::vector<string>& querylist,std::vector<bizinfo>& results);
		bool BizRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
						 std::vector<bizinfo>& results,const char* relationship);
		string JionQuery(std::vector<string>& querylist, string str);
		bool Rank(Xapian::MSet& matches,std::vector<bizinfo>& results);
		
	public://article
		bool FilerGame(string str);
		string ArticleJionQuery(std::vector<string>& querylist);
		bool ArticleRank(Xapian::MSet& matches,std::vector<articleinfo>& results);
		bool ArticleIndex(const char* filein, const char* dbpath);
		bool ArticleRetrieval(std::vector<string>& querylist,std::vector<articleinfo>& results);
		bool ArticleRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
			std::vector<articleinfo>& results,const char* relationship);

	public:
		void TestRank(Xapian::MSet& matches);
	};

}
#endif








































