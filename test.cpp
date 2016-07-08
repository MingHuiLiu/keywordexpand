#include "index.h"
#include "sever.h"
#include "lz.h"
#include "task.h"
int main(int argc, char *argv[]) {
	/*
	wordexpand::Index index;
	if(atoi(argv[1]) == 1){
		index.Init(argv[4]);
		index.BizIndex(argv[2],argv[3]);
	}else if(atoi(argv[1]) == 3){
		index.Init(argv[4]);
		index.ArticleIndex(argv[2],argv[3]);
	}else if(atoi(argv[1]) == 4){
		index.Init(argv[4]);
		index.ArticleIndex(argv[2],argv[3]);
	}
	*/
	wordexpand::Sever mysever;
	mysever.StartSever();
	//wordexpand::BitMap bitmap;
	//bitmap.GameLoadFile(argv[1]);
	//bitmap.Check(string("100248"));
	//bitmap.Check(string("102148"));
	//bitmap.Check(string("101111"));
	
	//wordexpand::Lz mlz;
	//mlz.SendUinStauePost();
	//wordexpand::Task mtask;
	//mtask.TaskApi(argv[1]);
	return 0;
}