//Ë÷ÒýÀà
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
	string bizuin;
	string appmsgid;
	string itemidx;
	string title;
	string ds;
} articleuin; 
/*
typedef struct{
	string task_id;
	string task_version;
	string input;
	string task_name;
	string task_cname;
	string step;
		pack_name
		done_name
		user_num
		gamecenter_id
		performance_name
		status
		biz_url
		creator
		modify_user
		create_time
		modify_time


} taskinfo;

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
		bool BizRetrieval(std::vector<string>& querylist,
			std::vector<std::pair<string, float> >& results);
		bool BizRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
			std::vector<std::pair<string, float> >& results,const char* relationship);
		string JionQuery(std::vector<string>& querylist, string str);
		bool Rank(Xapian::MSet& matches,std::vector<std::pair<string, float> >& results);
		bool FilerGame(string str);

	public://article
		bool ArticleRetrieval(std::vector<string>& querylist,std::vector<std::pair<string, float> >& results);
		bool ArticleRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
			std::vector<std::pair<string, float> >& results,const char* relationship);


	public://mysql
		bool InitMysql();
		bool ExeQuery(string& query);
		bool UpdataTask(std::map<string, string>& taskinfo);
		bool UpdataKeywords();
		bool UpdataBiz();
		bool UpdataArticle();
		

	public:
		bool TestRetrieval();
		void TestRank(Xapian::MSet& matches);
	};

}
#endif








































