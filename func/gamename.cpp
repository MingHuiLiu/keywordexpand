#include "../commom/func.h"
#include <algorithm>

void Brackets(string& str, std::map<std::string, int>& dict){
	//commom::DEBUG_INFO(str);
	//////commom::Func f;	;	;	
	string::size_type indexa = str.find("《");
	string::size_type indexad = 0;
	string strcopy = "";
	while(indexa != string::npos){
		//commom::DEBUG_INFO(str);
		indexad =  str.find("》");
		//commom::DEBUG_INFO("indexad: " + commom::ConvertToStr(indexad));
		if((indexad != string::npos)&&(indexad > indexa)){
			string temp = "";
			temp.append(str,indexa + 3,indexad - indexa -3);
			dict[temp]++;
			//commom::DEBUG_INFO(temp);
			strcopy = "";
			strcopy.append(str,indexad +3, str.size()-1);
			str = strcopy;
			//commom::DEBUG_INFO(str);
		}else{
			return;
		}
		//commom::DEBUG_INFO(str);
		indexa = str.find("《");
		//commom::DEBUG_INFO("indexa: "  + commom::ConvertToStr(indexa));
	}
}

void getname(const char* filein, const char* dbpath){
	//////commom::Func f;	;	;	
	FILE*fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	FILE* fo = fopen(dbpath,"ab+");
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	std::map<std::string, int> dict ;
	int linenum = 0;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		//commom::DEBUG_INFO(commom::ConvertToStr(v.size()));
		if(v.size() != 5){
			continue;
		}
		str = v.at(2);
		Brackets(str,dict);		
	}
	std::vector<std::pair<string, float> > vv;
	for(std::map<std::string, int>::iterator it = dict.begin(); it != dict.end(); it++){
		vv.push_back(std::pair<string, float>(it->first, it->first.size()));
	}
	//commom::DEBUG_INFO(commom::ConvertToStr(v.size()));
	sort(vv.begin(), vv.end(),commom::SortBySecondLess);
	for(int j =0; j < vv.size(); j++){
		commom::WiteLine((vv.at(j).first + string("\n")).c_str(), fo);
	}
	fclose(fi);
	fclose(fo);
}

void filter(const char* filein, const char* dictpa,const char* dbpath){
	//////commom::Func f;	;	;	
	FILE*fi = fopen(dictpa,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(dictpa));
		return ;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	std::map<std::string, int> dict ;
	int linenum = 0;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split(" ", str, v);
		if(v.size() == 0){
			continue;
		}
		dict[v.at(0)]++;
	}
	fclose(fi);
	commom::DEBUG_INFO("ok");

	fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	FILE* fo = fopen(dbpath,"ab+");
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		if(dict[str] == 0){
			commom::WiteLine((str + string("\n")).c_str(), fo);
		}	
	}
	fclose(fi);
	fclose(fo);
}

int main(int argc, char *argv[]) {
	//getname(argv[1], argv[2]);
	filter(argv[1], argv[2],argv[3]);
	return 0;
}