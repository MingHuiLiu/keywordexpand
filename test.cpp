#include "index.h"
int main(int argc, char *argv[]) {
	wordexpand::Index index;
	if(atoi(argv[1]) == 1){
		index.Init(argv[4]);
		index.BizIndex(argv[2],argv[3]);
	}else if(atoi(argv[1]) == 2){
		index.Retrieval();
	}else if(atoi(argv[1]) == 3){
		index.Init(argv[4]);
		index.ArticleIndex(argv[2],argv[3]);
	}
	return 0;
}