#include "../commom/func.h"
#include "../wordseg/seg.h"
std::map<string,int> dict;
std::map<string,string> bizdict;
seg::Wordseg mseg;
void loadbizdict(const char* file, const char* dictpath){
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
		bizdict[v.at(0)] = v.at(1);
	}
	fclose(fi);
}


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
		commom::DEBUG_INFO(v.at(0));
		dict[v.at(0)] = atoi(v.at(1).c_str());
	}
	commom::DEBUG_INFO(commom::ConvertToStr(dict.size()));
	fclose(fi);
}

int score(string str){
	//commom::DEBUG_INFO(str);
	int s = 0;
	std::map<string,int> v ;
	commom::Split(" ",str, v );
	for(std::map<string,int>::iterator it = v.begin(); it != v.end(); it++){
		if(dict.find(it->first) != dict.end()){
			s += dict[it->first];
			//commom::LOG_INFO(it->first);
		}		
	}
	//commom::DEBUG_INFO(commom::ConvertToStr(s));
	return s;
}

string bizscore(string str){
	//commom::DEBUG_INFO(str);
	string s = "";
	std::map<string,int> v ;
	commom::Split(" ",str, v );
	for(std::map<string,int>::iterator it = v.begin(); it != v.end(); it++){
		if(bizdict.find(it->first) != bizdict.end()){
			if(bizdict[it->first] != ""){
				if(s == ""){
					s = bizdict[it->first];
				}else{
					s += ("," + bizdict[it->first]);
				}
				
			}
		}		
	}
	commom::Split(",",s, v );
	s = "";
	for(std::map<string,int>::iterator it = v.begin(); it != v.end(); it++){
		if(s == ""){
			s = it->first;
		}else{
			s += ("," + it->first);
		}
	}
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
	//std::vector<string> temp;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		//commom::Split(" ",mseg.QuickSegement(str.c_str()),v);
		commom::Split("\t", str, v);
		if(v.size() != 2){
			continue;
		}
		if(score(mseg.QuickSegement(v.at(0).c_str())) > 0){
			//commom::LOG_INFO("#################");
			//commom::LOG_INFO(str);
			str += "\n";
			commom::WiteLine(str.c_str(), fo);			
			//commom::LOG_INFO(v.at(0));
			//commom::DEBUG_INFO(mseg.QuickSegement(v.at(0).c_str()));
		}
	}
	fclose(fi);
	fclose(fo);
}
bool comp(const std::pair<string, string>& x, const std::pair<string, string>& y){
	return x.first.size() < y.first.size();
}
void gameprocess(const char* inpath, const char* outpath){
	FILE* fi = fopen(inpath,"r");
	FILE* fo = fopen(outpath,"ab+");
	std::map<string,string> dict;
	dict.clear();
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
		str = commom::GetLine(buffer); 
		if(str == "")continue;
		commom::Split("\t", str, v);
		if(v.size() > 1){
			for(int j = 1; j< v.size(); j++){
				dict[v.at(0)] += ("," + v.at(j));
			}	
		}else if(v.size() == 1){
			dict[v.at(0)] += "";
		}
		commom::DEBUG_INFO(dict[v.at(0)]);
	}
	fclose(fi);
	std::vector<std::pair<string, string> >vec;
	for(std::map<string,string>::iterator it = dict.begin(); it != dict.end(); it++){
		//str = it->first + "\t" + it->second + "\n";
		str = it->second;
		std::map<string,int>d;
		commom::DEBUG_INFO(it->second);
		commom::Split(",", it->second, d);
		//str = it->first + "\t";
		str = "";
		for(std::map<string,int>::iterator bt = d.begin(); bt != d.end(); bt++){
			str += ("," + bt->first);
		}
		//str += "\n";
		vec.push_back(std::pair<string,string>(it->first, str));
		//commom::WiteLine(str.c_str(), fo);
	}
	sort(vec.begin(), vec.end(), comp);
	for(int t = 0; t <vec.size(); t++){
		str = vec.at(t).first + "\t" + vec.at(t).second + "\n";
		commom::WiteLine(str.c_str(), fo);
	}
	fclose(fo);
}


void bizclassfy(const char* inpath, const char* outpath){
	FILE* fi = fopen(inpath,"r");
	FILE* fo = fopen(outpath,"ab+");
	std::map<string,string> dict;
	dict.clear();
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
		str = commom::GetLine(buffer); 
		if(str == "")continue;
		commom::Split("\t", str, v);
		if(v.size() >= 4){
			str = v.at(1) + v.at(2);
			string type = bizscore(mseg.Segement(str.c_str()));
			str = v.at(0) + "\t" + v.at(1) + "\t" + v.at(2) + "\t" + v.at(3) + "\t" + type + "\n";
 			//commom::DEBUG_INFO(mseg.Segement(str.c_str()));
			commom::DEBUG_INFO(v.at(1) + " :" + type);
			commom::WiteLine(str.c_str(), fo);
		}
		//commom::DEBUG_INFO(dict[v.at(0)]);
	}
}

void count(const char* inpath){
	FILE* fi = fopen(inpath,"r");
	std::map<string,int> dict;
	dict.clear();
	if(fi == NULL){
		commom::DEBUG_INFO("open file error");
		return;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
		str = commom::GetLine(buffer); 
		if(str == "")continue;
		commom::Split("\t", str, v);
		if(v.size() == 3){
			dict[v.at(1)]++;
		}
	}
	for(std::map<string,int>::iterator it = dict.begin(); it != dict.end(); it++){
		commom::DEBUG_INFO(it->first);
		commom::DEBUG_INFO(commom::ConvertToStr(it->second));
	}
}

string getchinese (string& str){
	//<dt class="basicInfo-item name">游戏平台</dt>
	string s = "";
	string::size_type indexa = str.find(">");
	s.append(str,indexa+1, str.size() -6 - indexa);
	commom::DEBUG_INFO(s);
	return s;
} 
//game 百度百科游戏解析
void baikegame(const char* inpath, const char* outpath){
	FILE* fi = fopen(inpath,"r");
	FILE* fo = fopen(outpath,"ab+");
	std::map<string,string> dict;
	dict.clear();
	if((fi == NULL)||(fo == NULL)){
		commom::DEBUG_INFO("open file error");
		return;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;

	bool gametag = false;
	bool title = false;
	bool lemmaSummary = false;
	bool lemmaSummary_info = false;
	string lemmaSummary_info_tmp = "";
	string titlestr = "";
	string tmp = "";
	string desc ="";
	string wordstr = "";
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL){
		str = commom::GetLine(buffer); 
		if(str == "")continue;
		commom::Split(":", str, v);
		if(v.size() > 0){
			if(v.at(0) == "seanxy"){
				gametag == true;
				wordstr = str + "\n";
				commom::WiteLine(wordstr.c_str(),fo);
				continue;
			}
		}
		if(title == true){
			if(str.find("<h1") != string::npos){
				titlestr += (" " + getchinese(titlestr));
			}else if(str.find("<h2") != string::npos){
				titlestr += (" " + getchinese(titlestr));
			}else{
				title = false;
				titlestr += "\n";
				commom::WiteLine(titlestr.c_str(),fo);
				titlestr = "title:";
				continue;
			}
		}else if(lemmaSummary == true){
			if(lemmaSummary_info == true){
				if(str.find("</dl></div>") != string::npos) {
					lemmaSummary_info = false;
					lemmaSummary_info_tmp += "\n";
					commom::WiteLine(lemmaSummary_info_tmp.c_str(),fo);
					lemmaSummary_info_tmp = "";
					continue;
				}else{
					if(getchinese(str) != ""){
						lemmaSummary_info_tmp += (";" + getchinese(str) + ";");
					}
				}
			}else{
				if(str.find("<div class=\"basic-info cmn-clearfix\">") != string::npos){
					lemmaSummary_info = true;
					lemmaSummary_info_tmp = "info:";
					continue;
				}else {
					desc = "desc:" + getchinese(str) + "\n";
					commom::WiteLine(desc.c_str(),fo);
				}
			}

		}
		//description
		if(str.find("<meta name=\"description\"") != string::npos){
			//<meta name="description" content="
			string t = "<meta name=\"description\" content=\"";
			string s = "";
			s.append(str,t.size(), str.size()-2 - t.size());
			tmp = "description:" + s + "\n";
			commom::WiteLine(tmp.c_str(),fo);
			continue;
		}
		//title
		if(str.find("<title>") != string::npos){
			//
			string s = "";
			s.append(str,7, str.size()-10);
			tmp = "description:" + s + "\n";
			commom::WiteLine(tmp.c_str(),fo);
			continue;
		}
		//keywords
		if(str.find("<meta name=\"keywords\"") != string::npos){
			//<meta name="keywords" content="
			string t = "<meta name=\"keywords\" content=\"";
			string s = "";
			s.append(str,t.size(), str.size()-2 - t.size());
			tmp = "description:" + s + "\n";
			commom::WiteLine(tmp.c_str(),fo);
			continue;
		}
		//lemmaWgt-lemmaTitle-title <dd class="lemmaWgt-lemmaTitle-title">
		if(str.find("lemmaWgt-lemmaTitle-title") != string::npos){
			title = true;
			continue;
		}
		if(str.find("module=\"lemmaSummary\"") != string::npos){
			lemmaSummary = true;
			continue;
		}
		//commom::DEBUG_INFO(dict[v.at(0)]);
	}
}


int main(int argc, char *argv[]) {
	//gameprocess(argv[1],argv[2]);
	//loaddict(argv[1],argv[2]);
	//loadbizdict(argv[1],argv[2]);
	//bizclassfy(argv[3],argv[4]);
	//count(argv[1]);
	baikegame(argv[1],argv[2]);
	return 0;
}