#include "index.h"
#include "sever.h"
#include "lz.h"
#include "task.h"
typedef FILE* _FILE;
#define dbpath "/data/qspace/seanxywang/data/article/gamedailyarticle/"
void ArtIndex(wordexpand::Index& index,const char* filein, const char* database){
	//////commom::Func f;	;	;	
	std::map<string, _FILE>filedict;
	FILE*fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	int linenum = 0;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 5)continue;
		string datestr = v.at(4);
		if(filedict.find(datestr) == filedict.end()){
			string tmpfath = dbpath + datestr;  
			filedict[datestr] = fopen(tmpfath.c_str(),"ab+");
			if(filedict[datestr] != NULL){
				commom::WiteLine((str + "\n").c_str(), filedict[datestr]);
			}
		}else{
			commom::WiteLine((str + "\n").c_str(), filedict[datestr]);
		}
	}
	fclose(fi);
	for(std::map<string, _FILE>::iterator it = filedict.begin(); it != filedict.end(); it++){
		fclose(it->second);
	}
	//index;
	for(std::map<string, _FILE>::iterator it = filedict.begin(); it != filedict.end(); it++){
		string inpath = dbpath + it->first;
		string db = database + it->first + "/";
		index.ArticleIndex(inpath.c_str(), db.c_str());
	}
}

int main(int argc, char *argv[]) {
	wordexpand::Index index;
	if(atoi(argv[1]) == 1){
		index.Init(argv[4]);
		index.BizIndex(argv[2],argv[3]);
	}else if(atoi(argv[1]) == 2){
		index.Init(argv[4]);
		index.RoomIndex(argv[2],argv[3]);
	}
	return 0;
}