#include "../commom/func.h"
#include "../wordseg/seg.h"
int main(int argc, char *argv[]) {
	seg::Wordseg mseg;
	commom::Func f;
	mseg.InitDict(argv[1]);
	FILE*fi = fopen(argv[2],"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(argv[2]));
		return false;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = f.GetLine(buffer); 
		commom::DEBUG_INFO(str);
		commom::DEBUG_INFO(mseg.QuickSegement(str.c_str()));
	}
	return 0;
}
