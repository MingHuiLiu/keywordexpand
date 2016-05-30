#ifndef _SEVER_H
#define _SEVER_H
#include "../commom/func.h"
#include <stdio.h>  
#include <event.h>  
#include <evhttp.h>  
#include <xapian.h>
typedef struct{
	string ip;
	int port;
	int timeout;
}httpconf;
namespace wordexpand{
 class Sever{
	public:
		Sever(){}
		~Sever(){}
	public:
		commom::Func f;
	public:
		bool Init(const char* dictpath);
		bool StartSever();
		//void CallBack(struct evhttp_request *req, void *arg);
	};

}
#endif








































