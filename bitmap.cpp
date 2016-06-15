#include "bitmap.h"
namespace wordexpand{
	void BitMap::set(int i){
		data[i>>SHIFT] |=  (1<<(i & MASK)); 
	}
	void BitMap::clr(int i){
		data[i>>SHIFT] &= ~(1<<(i & MASK));  
	}
	int BitMap::get(int i){
		return data[i>>SHIFT] &  (1<<(i & MASK));
	}

	bool BitMap::GameLoadFile(const char* file){
		FILE* fi = fopen(file, "r");
		if(fi == NULL){
			return false;
		}
		std::string str = "";
		std::vector<string>v;
		char buffer[MAX_LENTH];	
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
			str = f.GetLine(buffer);
			f.Split("\t",str,v);
			if(v.size() >= 2){
				int x = atoi(v.at(1).c_str());
				if(x > 0){
					set(x);
				}
			}

	    }
		return true;
	}

	bool BitMap::Check(string uin){
		if(get(atoi(uin.c_str())) > 0){
			commom::DEBUG_INFO("true");
			return true;
		}
		commom::DEBUG_INFO("false");
		return false;
	}
}
