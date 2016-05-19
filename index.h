//Ë÷ÒýÀà
#ifndef _INDEX_H
#define _INDEX_H
#include "../commom/func.h"
#include "../wordseg/seg.h"
#include <xapian.h>
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



namespace wordexpand{
 class Index{
	public:
		Index(){}
		~Index(){}
	public:
		commom::Func f;
		seg::Wordseg mseg;
		bool Init(const char* dictpath);
		bool BizIndex(const char* filein, const char* dbpath);
		bool ArticleIndex(const char* filein, const char* dbpath);
		bool Retrieval();
		bool Retrieval(std::vector<string>& querylist,
			std::vector<std::pair<string, float> >& results);
		bool ArticleRetrieval(std::vector<string>& querylist,std::vector<std::pair<string, float> >& results);
		bool ArticleRetrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
			std::vector<std::pair<string, float> >& results,const char* relationship);
		bool Retrieval(Xapian::Enquire& enquire, std::vector<string>& querylist,
			std::vector<std::pair<string, float> >& results,const char* relationship);
		string JionQuery(std::vector<string>& querylist, string str);
		bool Rank(Xapian::MSet& matches,std::vector<std::pair<string, float> >& results);
	public:
		void TestRank(Xapian::MSet& matches);
	};

}
#endif








































