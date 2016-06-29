#ifndef _BITMAP_H
#define _BITMAP_H
#include "../commom/func.h"
#define BITSPERWORD 32  
#define SHIFT 5  
#define MASK 0x1F  
#define N 3000000000  
namespace wordexpand{
	class BitMap{
	public:
		BitMap(){
			data = new int[1 + N/BITSPERWORD];
			for(int i =0; i<1 + N/BITSPERWORD; i++ ){
				data[i] = 0;
			}
		}
		~BitMap(){
			delete data;
		}
	public:
		commom::Func f;
	public:  
		int* data;
		void set(int i); 
		void clr(int i);
		int get(int i);
	public:  
		bool GameLoadFile(const char* file);
		bool Check(string uin);
	};
}
#endif





































