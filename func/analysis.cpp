#include "../commom/func.h"
void readfile(const char* filein, const char* file){
	commom::Func f;
	std::map<string,int>dict;
	FILE*fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	int i =0;
	while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = f.GetLine(buffer); 
		i++;
		dict[str]++;
	}
	fclose(fi);
	commom::DEBUG_INFO(f.ConvertToStr(i) + " \t" + f.ConvertToStr(dict.size()));

	fi = fopen(file,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(file));
		return ;
	}
	i =0;
	int j =0;
	while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = f.GetLine(buffer); 
		f.Split("\t", str, v);
		str = v.at(0);
		i++;
		if(dict.find(str) != dict.end()){
			j++;
		}
	}
	fclose(fi);
	commom::DEBUG_INFO(f.ConvertToStr(i) + " \t" + f.ConvertToStr(j));

}
int main(int argc, char *argv[]) {
	readfile(argv[1],argv[2]);
	return 0;
}