#include "../commom/func.h"
void readfile(const char* filein){
	std::map<int, int> dict1;
	std::map<int, int> dict2;
	FILE*fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	int i =0;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t",str,v);
		if(v.size() == 4){
			int x = atoi(v.at(3).c_str());
			if(v.at(2) == "1"){
				i++;
				dict1[x]++;
			}else{
				dict2[x]++;
			}		
			
		}else{
			int x = atoi(v.at(2).c_str());
			if(v.at(1) == "1"){
				i++;
				dict1[x]++;
			}else{
				dict2[x]++;
			}	
		}
	}
	fclose(fi);
	commom::DEBUG_INFO(commom::ConvertToStr(i));
	for(std::map<int, int>::iterator it = dict1.begin(); it != dict1.end(); it++){
		commom::DEBUG_INFO(commom::ConvertToStr(it->first) + "\t" + commom::ConvertToStr(it->second));
	}
	for(std::map<int, int>::iterator it = dict2.begin(); it != dict2.end(); it++){
		commom::DEBUG_INFO(commom::ConvertToStr(it->first) + "\t" + commom::ConvertToStr(it->second));
	}
}
int main(int argc, char *argv[]) {
	readfile(argv[1]);
	return 0;
}