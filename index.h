//索引类
#ifndef _INDEX_H
#define _INDEX_H
#include "../commom/func.h"
#include <xapian.h>
typedef struct{
	string uin;
	string bizname;
	string bizdesc;
	int bizfunsnum;
} bizuin; 

namespace wordexpand{
 class Index{
	public:
		commom::Func f;
		bool bizindex(const char* filein, const char* dbpath);
	};

}
#endif

/*
#include<stdio.h>
#include <string.h>
#include <xapian.h>
#include <iostream>
#include <map>
#include "time.h"
#include<cmath>
#include <stdlib.h>   
#include <locale.h> 
#include <string>
#include <fstream>
#include <sstream>
#include "zlib.h"
#include <sys/types.h>    
#include <dirent.h>    
#include <errno.h>    
#include "NLPIR.h"
using namespace std;
string readFileIntoString(char *  filename)
{
ifstream ifile(filename);
ostringstream buf;
char ch;
while(buf&&ifile.get(ch))
buf.put(ch);
return buf.str();
}


int main()
{            
string DATABASEPATH="./DB";
Xapian::WritableDatabase db(DATABASEPATH, Xapian::DB_CREATE_OR_OPEN);
// char pp[50]="../TXT/D.txt";
ifstream in(pp, iOS::in);
istreambuf_iterator<char> beg(in), end;
string strdata(beg, end);
in.close();
string str=strdata;

////////////////////////////////////////////////////////////////////分词
const char* sInput=str.c_str();
const char* sResult;
if(!ICTCLAS_Init("..",UTF8_CODE))//数据在当前路径下，默认为GBK编码的分词
{
printf("ICTCLAS INIT FAILED!\n");
return 0;
}
//ICTCLAS_SetPOSmap(ICT_POS_MAP_SECOND);//词性标注
sResult = ICTCLAS_ParagraphProcess(sInput,0);
//NLPIR_FileProcess(sSentence,sResultFile)
//ICTCLAS_Exit();
string my_words=sResult;   
//得到带空格的分词
cout<<my_words<<endl;
Xapian::Document doc;

Xapian::TermGenerator indexer;
doc.set_data(str);
indexer.set_document(doc);
indexer.index_text(my_words);
db.add_document(doc);
db.flush();


cout<<"complete the index"<<endl;
return 1;             



}



2.搜索部分

#include<stdio.h>
#include <string.h>
#include <xapian.h>
#include <iostream>
#include <map>
#include "time.h"
#include<cmath>
#include <stdlib.h>   
#include <locale.h> 
#include <string>
#include <fstream>
#include <sstream>
#include "zlib.h"
using namespace std;






int main()
{





string P="..../fenci/DB";
Xapian::Database db(P);

Xapian::Enquire enquire(db);
cout<<"input the key_word"<<endl;
string query_string;
cin>>query_string;


Xapian::QueryParser qp;
Xapian::Query query = qp.parse_query(query_string);
cout << "Parsed query is: " << query.get_description() << endl;

enquire.set_query(query);
Xapian::MSet matches = enquire.get_mset(0, 10);


cout << matches.get_matches_estimated() << " results found.\n";
cout << "Matches 1-" << matches.size() << ":\n" << endl;


for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i)
{
cout << i.get_rank() + 1 <<": "  << " docid=" << *i<<endl;
cout<<i.get_document().get_data()<<endl;
}










}








































