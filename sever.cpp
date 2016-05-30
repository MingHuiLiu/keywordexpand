#include "sever.h"
namespace wordexpand{
	bool Sever::Init(const char* dictpath){
		return true;
	}
	void CallBack(struct evhttp_request *req, void *arg){
		commom::DEBUG_INFO("lisenling");
		struct evbuffer *buf = evbuffer_new();
		if(!buf){  
			puts("failed to create response buffer");  
			return;  
		}  	
		char *decode_uri = strdup((char*) evhttp_request_uri(req));
		std::cout<<decode_uri<<std::endl;
		struct evkeyvalq http_query;
		evhttp_parse_query(decode_uri, &http_query);
		free(decode_uri);
		evhttp_send_reply(req, HTTP_OK, "OKOK", buf);
		evhttp_clear_headers(&http_query);
		evbuffer_free(buf);
	}
	bool Sever::StartSever(){
		commom::DEBUG_INFO("begin");
		char *host_ip = "10.223.48.117";
		int host_port = 9091;
		int timeout = 3;
		struct evhttp *httpd;  
		/*
		WSADATA wsaData;  
		DWORD Ret;  
		if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)  {  
			printf("WSAStartup failed with error %d\n", Ret);  
			return -1;  
		}   
		*/
		event_init();  
		//创建一个evhttp,绑定到端口和地址
		httpd = evhttp_start(host_ip, host_port);
		if(!httpd){  
			return 1;  
		}  
		evhttp_set_timeout(httpd, timeout);
		evhttp_set_gencb(httpd, CallBack, NULL);  
		printf("httpd server start OK!\n");  
		event_dispatch(); 
		evhttp_free(httpd);  
		//WSACleanup(); 		
		return true;
	}
}