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


int main(int argc, char *argv[]) {
	cut(argv[1], argv[2]);
	return 0;
}