#include "index.h"
namespace wordexpand{
	bool Index::bizindex(const char* filein, const char* dbpath){
		Xapian::WritableDatabase db(dbpath,Xapian::DB_CREATE_OR_OPEN);
		Xapian::TermGenerator indexer;
		Xapian::Document doc;
		
		//doc.add_term("");
		doc.set_data("uin");//以uin 为内容		
		indexer.set_document(doc);
		indexer.index_text("title", 1, "T");
		indexer.index_text("desc", 1, "C");
		
		//


		//增加其他字段
		doc.add_value(1,"str1");
		doc.add_value(2,"str2");

		db.add_document(doc);
		db.flush();


		//增加域

		return true;
	}


}