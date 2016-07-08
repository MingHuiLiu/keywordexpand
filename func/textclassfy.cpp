#include "../commom/func.h"
#include "../wordseg/seg.h"
std::map<string,int> dict;
seg::Wordseg mseg;
void loaddict(const char* file, const char* dictpath){
	mseg.InitDict(dictpath);
	FILE* fi = fopen(file,"r");
	if(fi == NULL){
		commom::DEBUG_INFO("open file error");
		return;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 2)continue;
		dict[v.at(0)] = atoi(v.at(1).c_str());
	}
	fclose(fi);
}

int score(string str){
	//commom::DEBUG_INFO(str);
	int s = 0;
	std::map<string,int> v ;
	commom::Split(" ",str, v );
	/*
	for(int i =0; i< v.size(); i++){
		s += dict[v.at(i)];
	}
	*/
	for(std::map<string,int>::iterator it = v.begin(); it != v.end(); it++){
		s += dict[it->first];
	}
	//commom::DEBUG_INFO(commom::ConvertToStr(s));
	return s;
}

void classfy(const char* inpath, const char* outpath){
	FILE* fi = fopen(inpath,"r");
	FILE* fo = fopen(outpath,"ab+");
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;
	}
	//////commom::Func f;	;	;	
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 9){
			continue;
		}
		int type = atoi(v.at(5).c_str());
		if(type == 1) continue;
		str = v.at(2) + " " +  v.at(3) + " " + v.at(4) + " " + v.at(6);
		if(score(mseg.Segement(str.c_str())) >= 2){
			str = v.at(1) + "\t" + v.at(2) + "\t" + v.at(3) + "\t" + v.at(7) + "\t" + v.at(8) + "\n";
			//str = v.at(1) + "\t" + v.at(2) + "\t" + v.at(3) + "\t" + v.at(4) + "\t" + v.at(6) + "\n";
			commom::WiteLine(str.c_str(), fo);
		}
	}
	fclose(fi);
	fclose(fo);
}

void textfilter(const char* inpath, const char* outpath){
	FILE* fi = fopen(inpath,"r");
	FILE* fo = fopen(outpath,"ab+");
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;
	}
	//////commom::Func f;	;	;	
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 2){
			continue;
		}
		if(v.at(0) == ""){
			continue;
		}else{
			str += "\n";
			commom::WiteLine(str.c_str(), fo);
		}
	}
	fclose(fi);
	fclose(fo);
}

void filter(const char* inpath, const char* outpath){
	std::vector<string> v;
	commom::GetDirfile(v, inpath);
	for(int i =0; i<v.size(); i++){
		commom::DEBUG_INFO(v.at(i));
		textfilter((string(inpath) + v.at(i)).c_str(), outpath);		
	}
}


void _filter(const char* inpath, const char* outpath){
	FILE* fi = fopen(inpath,"r");
	FILE* fo = fopen(outpath,"ab+");
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;
	}	
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 2){
			continue;
		}
		if(score(mseg.QuickSegement(v.at(0).c_str())) > 0){
			//commom::DEBUG_INFO(str);
			continue;
		}else{
			str =v.at(1) + "\n";
			commom::WiteLine(str.c_str(), fo);
		}
	}
	fclose(fi);
	fclose(fo);
}

void textclassfy(const char* inpath, const char* outpath){
	FILE* fi = fopen(inpath,"r");
	FILE* fo = fopen(outpath,"ab+");
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split(" ",mseg.QuickSegement(str.c_str()),v);
		commom::Split("\t", str, v);
		if(v.size() != 2){
			continue;
		}
		if(score(mseg.QuickSegement(v.at(0).c_str())) > 0){
			str += "\n";
			commom::WiteLine(str.c_str(), fo);
		}
	}
	fclose(fi);
	fclose(fo);
}

void gameprocess(const char* inpath, const char* outpath){
	FILE* fi = fopen(inpath,"r");
	FILE* fo = fopen(outpath,"ab+");
	std::map<string,string> dict;
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() == 2){
			if(dict.find(v.at(0)) == dict.end()){
				dict[v.at(0)] = v.at(1);
			}else{
				dict[v.at(0)] += ("," + v.at(1));
			}		
		}else if(v.size() == 1){
			dict[v.at(0)] += "";
		}
	}
	fclose(fi);
	for(std::map<string,string>::iterator it = dict.begin(); it != dict.end(); it++){
		str = it->first + "\t" + it->second + "\n";
		commom::WiteLine(str.c_str(), fo);
	}
	fclose(fo);
}
int main(int argc, char *argv[]) {
	gameprocess(argv[1],argv[2]);
	//loaddict(argv[1],argv[2]);
	//textclassfy(argv[3],argv[4]);
	//_filter(argv[3],argv[4]);
	return 0;
}