#include "../commom/func.h"
void cut(const char* filein, const char* dbpath){
	commom::Func f;
	FILE*fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	FILE* foa = fopen((dbpath + string("a")).c_str(),"ab+");
	FILE* fob = fopen((dbpath + string("b")).c_str(),"ab+");
	FILE* foc = fopen((dbpath + string("c")).c_str(),"ab+");
	FILE* fod = fopen((dbpath + string("d")).c_str(),"ab+");
	FILE* foe = fopen((dbpath + string("e")).c_str(),"ab+");
	FILE* fof = fopen((dbpath + string("f")).c_str(),"ab+");
	FILE* fog = fopen((dbpath + string("g")).c_str(),"ab+");
	FILE* foh = fopen((dbpath + string("h")).c_str(),"ab+");
	FILE* foi = fopen((dbpath + string("i")).c_str(),"ab+");
	FILE* foj = fopen((dbpath + string("j")).c_str(),"ab+");

	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	int linenum = 0;
	while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = f.GetLine(buffer); 
		f.Split("\t", str, v);
		if(v.size() != 5)continue;
		int fre = atoi(v.at(3).c_str());
		str += "\n";
		if(fre > 5000){
			f.WiteLine(str.c_str(),foa);
		}else if(fre > 1500){
			f.WiteLine(str.c_str(),fob);
		}else if(fre > 800){
			f.WiteLine(str.c_str(),foc);
		}else if(fre > 600){
			f.WiteLine(str.c_str(),fod);
		}else if(fre > 500){
			f.WiteLine(str.c_str(),foe);
		}else if(fre > 400){
			f.WiteLine(str.c_str(),fof);
		}else if(fre > 300){
			f.WiteLine(str.c_str(),fog);
		}else if(fre > 200){
			f.WiteLine(str.c_str(),foh);
		}else if(fre > 150){
			f.WiteLine(str.c_str(),foi);
		}else{
			f.WiteLine(str.c_str(),foj);
		}
	}
	fclose(foa);
	fclose(fob);
	fclose(foc);
	fclose(fod);
	fclose(foe);
	fclose(fof);
	fclose(fog);
	fclose(foh);
	fclose(foi);
	fclose(foj);
}

void cut_file(const char* filein, const char* dbpath){
	commom::Func f;
	typedef FILE* _FILE;
	
	FILE*fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	_FILE* fo = new _FILE[30];
	fo[0] = fopen((dbpath + string("20160501") + string("/article")).c_str() ,"ab+");
	fo[1] = fopen((dbpath + string("20160502") + string("/article")).c_str(),"ab+");
	fo[2] = fopen((dbpath + string("20160503") + string("/article")).c_str(),"ab+");
	fo[3] = fopen((dbpath + string("20160504") + string("/article")).c_str(),"ab+");
	fo[4] = fopen((dbpath + string("20160505") + string("/article")).c_str(),"ab+");
	fo[5] = fopen((dbpath + string("20160506") + string("/article")).c_str(),"ab+");
	fo[6] = fopen((dbpath + string("20160507") + string("/article")).c_str(),"ab+");
	fo[7] = fopen((dbpath + string("20160508") + string("/article")).c_str(),"ab+");
	fo[8] = fopen((dbpath + string("20160509") + string("/article")).c_str(),"ab+");
	fo[9] = fopen((dbpath + string("20160510") + string("/article")).c_str(),"ab+");
	fo[10] = fopen((dbpath + string("20160511") + string("/article")).c_str(),"ab+");
	fo[11] = fopen((dbpath + string("20160512") + string("/article")).c_str(),"ab+");
	fo[12] = fopen((dbpath + string("20160513") + string("/article")).c_str(),"ab+");
	fo[13] = fopen((dbpath + string("20160514") + string("/article")).c_str(),"ab+");
	fo[14] = fopen((dbpath + string("20160515") + string("/article")).c_str(),"ab+");
	fo[15] = fopen((dbpath + string("20160516") + string("/article")).c_str(),"ab+");
	fo[16] = fopen((dbpath + string("20160517") + string("/article")).c_str(),"ab+");
	fo[17] = fopen((dbpath + string("20160518") + string("/article")).c_str(),"ab+");
	fo[18] = fopen((dbpath + string("20160519") + string("/article")).c_str(),"ab+");
	fo[19] = fopen((dbpath + string("20160520") + string("/article")).c_str(),"ab+");
	fo[20] = fopen((dbpath + string("20160521") + string("/article")).c_str(),"ab+");
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	int linenum = 0;
	while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = f.GetLine(buffer); 
		f.Split("\t", str, v);
		if(v.size() != 5)continue;
		int fre = atoi(v.at(4).c_str()) - 20160501;
		str += "\n";
		f.WiteLine(str.c_str(),fo[fre]);
	}
	for(int i =0; i < 20; i++){
		fclose(fo[i]);
	}	
}



int main(int argc, char *argv[]) {
	cut_file(argv[1], argv[2]);
	return 0;
}