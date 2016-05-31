//索引类
#ifndef _INDEX_H
#define _INDEX_H
#include "../commom/func.h"
#include "../wordseg/seg.h"
//#include <mysql.h>
#include <xapian.h>
#include <mysql.h>
typedef struct{
	string uin;
	string bizname;
	string bizdesc;
	string bizfunsnum;
	string massmsgcount;
} bizuin; 

typedef struct{
	string uin;
	string bizname;
	string bizdesc;
	float score;
} bizinfo; 

typedef struct{
	string bizuin;
	string appmsgid;
	string itemidx;
	string title;
	string ds;
} articleuin; 

typedef struct{
	string articleuin;
	string title;
	float score;
} articleinfo; 


/*
stat_db = {}
stat_db['ip'] =  '10.191.150.220'
	stat_db['port'] = 3306
	stat_db['user'] = 'qspace'
	stat_db['pass'] = 'forconnect'
	stat_db['db'] = 'stat'

#mmwebitildb10
	datamining_db = {}
datamining_db['ip'] =  '10.234.148.106'
	datamining_db['port'] = 3306
	datamining_db['user'] = 'qspace'
	datamining_db['pass'] = 'forconnect'
	datamining_db['db'] = 'stat'

	datamining_db_new = {}
datamining_db_new['ip'] =  '10.234.148.106'
	datamining_db_new['port'] = 3306
	datamining_db_new['user'] = 'qspace'
	datamining_db_new['pass'] = 'forconnect'
	datamining_db_new['db'] = 'data'
/*
keyword_task
keyword_word_filter_result
keyword_biz_filter_result
keyword_article_category
表名
*/

namespace wordexpand{
 class Index{
	public:
		Index(){}
		~Index(){}
	public://biz
		commom::Func f;
		seg::Wordseg mseg;
		std::map<string, int>gamefilterdict;
		MYSQL myCont;

	public://biz
		bool Init(const char* dictpath);
		bool InitRetrieval(const char* dictpath);
		bool BizIndex(const char* filein, const char* dbpath);
		bool ArticleIndex(const char* filein, const char* dbpath);
		bool Retrieval(string& query,string& conf);
		bool BizRetrieval(std::vector<string>& querylist,std::vector<bizinfo>& results);
		bool BizRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
						 std::vector<bizinfo>& results,const char* relationship);
		string JionQuery(std::vector<string>& querylist, string str);
		bool Rank(Xapian::MSet& matches,std::vector<bizinfo>& results);
		bool ArticleRank(Xapian::MSet& matches,std::vector<articleinfo>& results);
		bool FilerGame(string str);

	public://article
		bool ArticleRetrieval(std::vector<string>& querylist,std::vector<articleinfo>& results);
		bool ArticleRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
			std::vector<articleinfo>& results,const char* relationship);


	public://mysql
		bool InitMysql();
		bool ExeQuery(string& query);
		bool AddTask(std::map<string, string>& taskinfo);
		bool UpdataKeywords(std::vector<std::string>& v, std::map<string, string>& taskinfo);
		bool UpdataBiz(std::vector<bizinfo>& results, std::map<string, string>& taskinfo);
		bool UpdataArticle();
		int GetTaskId();
		

	public:
		//bool Bizrank(const bizinfo& x, const bizinfo& y);
		bool TestRetrieval();
		void TestRank(Xapian::MSet& matches);
	};

}
#endif








































