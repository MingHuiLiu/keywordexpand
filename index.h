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
		commom::Func f;
		seg::Wordseg mseg;
		std::map<string, int>gamefilterdict;
		Sql mySql;

	public://biz
		bool Init(const char* dictpath);
		bool InitRetrieval(const char* dictpath);
		bool BizIndex(const char* filein, const char* dbpath);
		bool ArticleIndex(const char* filein, const char* dbpath);
		bool Retrieval(string& source, string& query,Sql& mySql,std::map<string, string>& taskinfo,
				std::vector<bizinfo>& bizresults,
				std::vector<articleinfo>& articleresults);
		bool BizRetrieval(std::vector<string>& querylist,std::vector<bizinfo>& results);
		bool BizRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
						 std::vector<bizinfo>& results,const char* relationship);
		string JionQuery(std::vector<string>& querylist, string str);
		string ArticleJionQuery(std::vector<string>& querylist);
		bool Rank(Xapian::MSet& matches,std::vector<bizinfo>& results);
		bool ArticleRank(Xapian::MSet& matches,std::vector<articleinfo>& results);
		bool FilerGame(string str);

	public://article
		bool ArticleRetrieval(std::vector<string>& querylist,std::vector<articleinfo>& results);
		bool ArticleRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
			std::vector<articleinfo>& results,const char* relationship);

	/*
	public://mysql
		bool InitMysql();
		bool ExeQuery(string& query);
		bool AddTask(std::map<string, string>& taskinfo);
		bool UpdataKeywords(std::vector<std::string>& v, std::map<string, string>& taskinfo);
		bool UpdataBiz(std::vector<bizinfo>& results, std::map<string, string>& taskinfo);
		bool UpdataArticle();
		int GetTaskId();
	*/

	public:
		//bool Bizrank(const bizinfo& x, const bizinfo& y);
		void TestRank(Xapian::MSet& matches);
	};

}
#endif








































