#include "index.h"
#include "sever.h"
#include "lz.h"
#include "task.h"
typedef FILE* _FILE;
#define dbpath "/data/qspace/seanxywang/data/article/gamedailyarticle/"
int main(int argc, char *argv[]) {
	wordexpand::Index index;
	if(atoi(argv[1]) == 1){
		index.Init(argv[4]);
		index.BizIndex(argv[2],argv[3]);
	}else if(atoi(argv[1]) == 2){
		index.Init(argv[4]);
		index.RoomIndex(argv[2],argv[3]);
	}
	return 0;
}