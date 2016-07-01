#include "../commom/func.h"
#include "../wordseg/seg.h"
std::map<string,int> dict;
seg::Wordseg mseg;
commom::Func f;
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
	while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = f.GetLine(buffer); 
		f.Split("\t", str, v);
		if(v.size() != 2)continue;
		dict[v.at(0)] = atoi(v.at(1).c_str());
	}
	fclose(fi);
}

int score(string str){
	//commom::DEBUG_INFO(str);
	int s = 0;
	std::map<string,int> v ;
	f.Split(" ",str, v );
	/*
	for(int i =0; i< v.size(); i++){
		s += dict[v.at(i)];
	}
	*/
	for(std::map<string,int>::iterator it = v.begin(); it != v.end(); it++){
		s += dict[it->first];
	}
	//commom::DEBUG_INFO(f.ConvertToStr(s));
	return s;
}

void classfy(const char* inpath, const char* outpath){
	FILE* fi = fopen(inpath,"r");
	FILE* fo = fopen(outpath,"ab+");
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;
	}
	commom::Func f;
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = f.GetLine(buffer); 
		f.Split("\t", str, v);
		if(v.size() != 9){
			continue;
		}
		int type = atoi(v.at(5).c_str());
		if(type == 1) continue;
		str = v.at(2) + " " +  v.at(3) + " " + v.at(4) + " " + v.at(6);
		if(score(mseg.Segement(str.c_str())) >= 2){
			str = v.at(1) + "\t" + v.at(2) + "\t" + v.at(3) + "\t" + v.at(7) + "\t" + v.at(8) + "\n";
			//str = v.at(1) + "\t" + v.at(2) + "\t" + v.at(3) + "\t" + v.at(4) + "\t" + v.at(6) + "\n";
			f.WiteLine(str.c_str(), fo);
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