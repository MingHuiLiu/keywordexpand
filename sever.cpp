#include "sever.h"
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
		//std::cout<<decode_uri<<std::endl;
		//Sever::test();
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
	void onTime(int sock, short event, void *arg) { 
		commom::DEBUG_INFO("HELLO");

		struct timeval tv; 
		tv.tv_sec = 1; 
		tv.tv_usec = 0; 
		for(int j =0; j<100000; j++){
			for(int i =0; i<100000; i++){

			}
		}
		commom::DEBUG_INFO("WORLD");
		event_add((struct event*)arg, &tv); 
	} 

	void CallBack(struct evhttp_request *req, void *arg){
		commom::DEBUG_INFO("lisenling");
		struct evkeyvalq http_query_post;
		int buffer_data_len = EVBUFFER_LENGTH(req->input_buffer);
		char* decode_post_uri;
		char *post_data = (char *) malloc(buffer_data_len + 1);
		memset(post_data, '\0', buffer_data_len + 1);
		memcpy(post_data, EVBUFFER_DATA(req->input_buffer), buffer_data_len);
		std::cout<<post_data<<std::endl;

		Task mTask;
		string restr = "{\"ret\":";
		if(mTask.TaskApi(post_data)){
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
		free(post_data);
		//mTask.~Task();
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
	}
	bool Sever::StartSever(){
		commom::DEBUG_INFO("begin");
		char *host_ip = "10.223.48.117";
		int host_port = 9093;
		int timeout = 500;
		struct evhttp *httpd;  
		event_init();  

		/*
		struct event evTime; 
		evtimer_set(&evTime, onTime, &evTime); 
		struct timeval tv; 
		tv.tv_sec = 1; 
		tv.tv_usec = 0; 
		// 添加定时事件 
		evtimer_add(&evTime, &tv); 
		*/
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

/*
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <event.h>
#include <evhttp.h>

#define RELOAD_TIMEOUT 5
#define DEFAULT_FILE "sample.html"

char *filedata;
time_t lasttime = 0;
char filename[80];
int counter = 0;

void read_file()
{
int size = 0;
char *data;
struct stat buf;

stat(filename,&buf);

if (buf.st_mtime > lasttime)
{
if (counter++)
fprintf(stderr,"Reloading file: %s",filename);
else
fprintf(stderr,"Loading file: %s",filename);

FILE *f = fopen(filename, "rb");
if (f == NULL)
{
fprintf(stderr,"Couldn't open file\n");
exit(1);
}

fseek(f, 0, SEEK_END);
size = ftell(f);
fseek(f, 0, SEEK_SET);
data = (char *)malloc(size+1);
fread(data, sizeof(char), size, f);
filedata = (char *)malloc(size+1);
strcpy(filedata,data);
fclose(f);


fprintf(stderr," (%d bytes)\n",size);
lasttime = buf.st_mtime;
}
}

void load_file()
{
struct event *loadfile_event;
struct timeval tv;

read_file();

tv.tv_sec = RELOAD_TIMEOUT;
tv.tv_usec = 0;

loadfile_event = malloc(sizeof(struct event));

evtimer_set(loadfile_event,
load_file,
loadfile_event);

evtimer_add(loadfile_event,
&tv);
}

void generic_request_handler(struct evhttp_request *req, void *arg)
{
struct evbuffer *evb = evbuffer_new();

evbuffer_add_printf(evb, "%s",filedata);
evhttp_send_reply(req, HTTP_OK, "Client", evb);
evbuffer_free(evb);
}

int main(int argc, char *argv[])
{
short          http_port = 8081;
char          *http_addr = "192.168.0.22";
struct evhttp *http_server = NULL;

if (argc > 1)
{
strcpy(filename,argv[1]);
printf("Using %s\n",filename);
}
else
{
strcpy(filename,DEFAULT_FILE);
}

event_init();

load_file();

http_server = evhttp_start(http_addr, http_port);
evhttp_set_gencb(http_server, generic_request_handler, NULL);
fprintf(stderr, "Server started on port %d\n", http_port);
event_dispatch();
}
*/