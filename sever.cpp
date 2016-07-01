#include "sever.h"
#include <pthread.h>
string str = "";
namespace wordexpand{
	bool Sever::Init(const char* dictpath){
		return true;
	}
	void CallBackOld(struct evhttp_request *req, void *arg){
		commom::DEBUG_INFO("lisenling");
		struct evbuffer *buf = evbuffer_new();
		if(!buf){  
			puts("failed to create response buffer");  
			return;  
		}  	
		char *decode_uri = strdup((char*) evhttp_request_uri(req));
		Task mTask;
		string re = "";
		if(mTask.TaskApi(decode_uri)){
			re = "0";
		}else{
			re = "-1";
		}
		struct evkeyvalq http_query;
		evhttp_parse_query(decode_uri, &http_query);
		free(decode_uri);
		evhttp_send_reply(req, HTTP_OK, re.c_str(), buf);
		evhttp_clear_headers(&http_query);
		evbuffer_free(buf);
	}

	void* CallApi(void* arg){
		commom::DEBUG_INFO("waiting");			
		string *pstru = (string *)arg;
		commom::LOG_INFO(pstru->c_str());
		string t = pstru->c_str();
		commom::LOG_INFO(t);
		Task mTask;		
		string restr = "{\"ret\":";
		if(mTask.TaskApi(t.c_str())){
			restr += "\"0\"";
		}else{
			restr += "\"-1\"";
		}
		restr += ",";
		restr += "\"desc\":";
		restr += "\"" + mTask.desc +"\"";
		restr += ",";
		restr += "\"taskid\":";
		restr += "\"" + mTask.taskid +"\"";
		restr += "}";
		commom::LOG_INFO(restr);
		pthread_exit(NULL);		
	}

	void CallBack(struct evhttp_request *req, void *arg){
		commom::Func f;
		commom::DEBUG_INFO("lisenling");
		struct evkeyvalq http_query_post;
		int buffer_data_len = EVBUFFER_LENGTH(req->input_buffer);
		char* decode_post_uri;
		char *post_data = (char *) malloc(buffer_data_len + 1);
		memset(post_data, '\0', buffer_data_len + 1);
		memcpy(post_data, EVBUFFER_DATA(req->input_buffer), buffer_data_len);
		//std::cout<<post_data<<std::endl;

		Task mTask;
		string restr = "{\"ret\":";
		if(mTask.ArgumentParsing(post_data)){
			restr += "\"0\"";
		}else{
			restr += "\"-1\"";
		}
		/*
		if(mTask.TaskApi(post_data)){
			restr += "\"0\"";
		}else{
			restr += "\"-1\"";
		}
		*/
		restr += ",";
		restr += "\"desc\":";
		restr += "\"" + mTask.desc +"\"";
		restr += ",";
		restr += "\"taskid\":";
		restr += "\"" + mTask.taskid +"\"";
		restr += "}";
		
		commom::LOG_INFO(f.GetTime() + "\t:" + restr);		
		struct evbuffer *buf = evbuffer_new();
		if(!buf){  
			puts("failed to create response buffer");  
			return;  
		}  	
		evhttp_add_header(req->output_headers, "Content-Type", "application/json");
		evhttp_add_header(req->output_headers, "Connection", "close");		
		evbuffer_add_printf(buf, "%s", restr.c_str());
		evhttp_send_reply(req, HTTP_OK, restr.c_str(), buf);
		evbuffer_free(buf);
		//string str = post_data;
		str = post_data;	
		//callbackconf
		commom::DEBUG_INFO(str);
		pthread_t tid;
		int ret = pthread_create(&tid, NULL, CallApi, &(str));
		commom::LOG_INFO("pthread creat ok");
		if (ret != 0){
			commom::DEBUG_INFO("pthread_create error: error_code=");
		}
		free(post_data);
	}

	bool Sever::StartSever(){
		commom::DEBUG_INFO("begin");
		char *host_ip = "10.223.48.117";
		int host_port = 9091;
		int timeout = 500;
		struct evhttp *httpd;  
		event_init();  
		httpd = evhttp_start(host_ip, host_port);
		if(!httpd){  
			return 1;  
		}  
		evhttp_set_timeout(httpd, timeout);
		evhttp_set_gencb(httpd, CallBack, NULL);  
		printf("httpd server start OK!\n");  
		event_dispatch(); 
		evhttp_free(httpd);  	
		return true;
	}
}
