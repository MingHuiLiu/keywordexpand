#include "../commom/func.h"
#include "../wordseg/seg.h"
std::map<string,int> dict;
seg::Wordseg mseg;
//////commom::Func f;	;	;	
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

int score(string str, string word){
	int s = 0;
	std::map<string,int> v ;
	commom::Split(" ",str, v );
	for(std::map<string,int>::iterator it = v.begin(); it != v.end(); it++){
		if(it->first == word){
			return 1;
		}
	}
	return 0;
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
		if(v.size() != 4){
			continue;
		}
		string s = v.at(1) + " " + v.at(2);
		string segstr = mseg.Segement(s.c_str());
		bool flag = false;
		for(std::map<string,int>::iterator it = dict.begin(); it != dict.end(); it++){
			if(score(segstr,it->first) == 1){
				commom::WiteLine(string(it->first + "\t" + str + "\n").c_str(), fo);
				flag = true;
			}
		}
		if(flag == false){
			commom::WiteLine(string( "\t" + str + "\n").c_str(), fo);
		}
	}
	fclose(fi);
	fclose(fo);

}

int main(int argc, char *argv[]) {
	loaddict(argv[1],argv[2]);
	classfy(argv[3],argv[4]);
	return 0;
}