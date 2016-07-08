#include "../commom/func.h"
#include "../wordseg/seg.h"
std::map<string,int> dict;
seg::Wordseg mseg;
//////commom::Func f;	;	;	
void maerge(const char* path, const char* pinttaipath, const char* otherpath){
	//////commom::Func f;	;	;	
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	std::map<string,string>dict;
	string paout = path;
	paout +=  "gamebizall";
	FILE* fo = fopen(paout.c_str(),"ab+");
	for(int i =1; i< 25; i++){
		string pa = path + commom::ConvertToStr(i) + ".txt";
		FILE* fi = fopen(pa.c_str(),"r");
		if((fi == NULL)||(fo == NULL)){
			commom::DEBUG_INFO("open file error");
			continue;;
		}
		int line = 0;
		string wordstr = "";
		while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = commom::GetLine(buffer); 
			if(line++ == 0){
				wordstr = str;
				continue;
			}
			commom::Split("\t", str, v);
			if(v.size() != 4){
				continue;
			}
			if(v.at(0).size() != 10){
				commom::DEBUG_INFO(commom::ConvertToStr(i) + "uin error");
			}

			if(dict.find(str) == dict.end()){
				dict[str] = wordstr;
			}else{
				dict[str]  +=("," + wordstr);
			}
		}
		fclose(fi);
	}
	for(std::map<string,string>::iterator it = dict.begin(); it != dict.end(); it++){
		string tmp = it->first + "\t" + it->second + "\n";
		commom::WiteLine(tmp.c_str(), fo);
	}
	FILE* fi = fopen(pinttaipath,"r");
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer) + "\t\n";
		commom::WiteLine(str.c_str(), fo);
	}
	fclose(fi);
	fi = fopen(otherpath,"r");
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer) + "\t\n";
		commom::WiteLine(str.c_str(), fo);
	}
	fclose(fi);
	fclose(fo);
}


void sortbizuin(const char* path, const char* pinttaipath){
	//////commom::Func f;	;	;	
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	//std::map<string,string>dict;
	FILE* fo = fopen(pinttaipath,"ab+");
	FILE* fi = fopen(path,"r");
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;;
	}
	std::vector<std::pair<string, float> > dict; 
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 4){
			continue;
		}
		if(atof(v.at(3).c_str()) <4){
			continue;
		}else{
			dict.push_back(std::pair<string, float>(v.at(1), atof(v.at(3).c_str())));
		}
	}
	fclose(fi);
	sort(dict.begin(), dict.end(), commom::SortBySecondGreater);
	commom::DEBUG_INFO(commom::ConvertToStr(dict.size()));
	for(int j =0; j< dict.size(); j++){
		str = dict.at(j).first + "\t" + commom::ConvertToStr(dict.at(j).second) + "\n";
		commom::WiteLine(str.c_str(), fo);
	}
	fi = fopen(path,"r");
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;;
	}
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 4){
			continue;
		}
		if(atof(v.at(3).c_str()) <4){
			str = v.at(1) + "\t" + v.at(3) + "\n";
			commom::WiteLine(str.c_str(), fo);
		}
	}
	fclose(fi);
	fclose(fo);

}
int main(int argc, char *argv[]) {
	//maerge(argv[1],argv[2],argv[3]);
	sortbizuin(argv[1],argv[2]);
	return 0;
}