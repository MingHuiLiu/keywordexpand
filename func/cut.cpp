#include "../commom/func.h"
void cut(const char* filein, const char* dbpath){
	//////commom::Func f;	;	;	
	FILE*fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	FILE* foa = fopen((dbpath + string("a")).c_str(),"ab+");
	FILE* fob = fopen((dbpath + string("b")).c_str(),"ab+");
	FILE* foc = fopen((dbpath + string("c")).c_str(),"ab+");
	FILE* fod = fopen((dbpath + string("d")).c_str(),"ab+");
	FILE* foe = fopen((dbpath + string("e")).c_str(),"ab+");
	FILE* fof = fopen((dbpath + string("f")).c_str(),"ab+");
	FILE* fog = fopen((dbpath + string("g")).c_str(),"ab+");
	FILE* foh = fopen((dbpath + string("h")).c_str(),"ab+");
	FILE* foi = fopen((dbpath + string("i")).c_str(),"ab+");
	FILE* foj = fopen((dbpath + string("j")).c_str(),"ab+");

	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	int linenum = 0;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 5)continue;
		int fre = atoi(v.at(3).c_str());
		str += "\n";
		if(fre > 5000){
			commom::WiteLine(str.c_str(),foa);
		}else if(fre > 1500){
			commom::WiteLine(str.c_str(),fob);
		}else if(fre > 800){
			commom::WiteLine(str.c_str(),foc);
		}else if(fre > 600){
			commom::WiteLine(str.c_str(),fod);
		}else if(fre > 500){
			commom::WiteLine(str.c_str(),foe);
		}else if(fre > 400){
			commom::WiteLine(str.c_str(),fof);
		}else if(fre > 300){
			commom::WiteLine(str.c_str(),fog);
		}else if(fre > 200){
			commom::WiteLine(str.c_str(),foh);
		}else if(fre > 150){
			commom::WiteLine(str.c_str(),foi);
		}else{
			commom::WiteLine(str.c_str(),foj);
		}
	}
	fclose(foa);
	fclose(fob);
	fclose(foc);
	fclose(fod);
	fclose(foe);
	fclose(fof);
	fclose(fog);
	fclose(foh);
	fclose(foi);
	fclose(foj);
}

void cut_file(const char* filein, const char* dbpath){
	//////commom::Func f;	;	;	
	typedef FILE* _FILE;
	
	FILE*fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	_FILE* fo = new _FILE[30];
	fo[0] = fopen((dbpath + string("20160501") + string("/article")).c_str() ,"ab+");
	fo[1] = fopen((dbpath + string("20160502") + string("/article")).c_str(),"ab+");
	fo[2] = fopen((dbpath + string("20160503") + string("/article")).c_str(),"ab+");
	fo[3] = fopen((dbpath + string("20160504") + string("/article")).c_str(),"ab+");
	fo[4] = fopen((dbpath + string("20160505") + string("/article")).c_str(),"ab+");
	fo[5] = fopen((dbpath + string("20160506") + string("/article")).c_str(),"ab+");
	fo[6] = fopen((dbpath + string("20160507") + string("/article")).c_str(),"ab+");
	fo[7] = fopen((dbpath + string("20160508") + string("/article")).c_str(),"ab+");
	fo[8] = fopen((dbpath + string("20160509") + string("/article")).c_str(),"ab+");
	fo[9] = fopen((dbpath + string("20160510") + string("/article")).c_str(),"ab+");
	fo[10] = fopen((dbpath + string("20160511") + string("/article")).c_str(),"ab+");
	fo[11] = fopen((dbpath + string("20160512") + string("/article")).c_str(),"ab+");
	fo[12] = fopen((dbpath + string("20160513") + string("/article")).c_str(),"ab+");
	fo[13] = fopen((dbpath + string("20160514") + string("/article")).c_str(),"ab+");
	fo[14] = fopen((dbpath + string("20160515") + string("/article")).c_str(),"ab+");
	fo[15] = fopen((dbpath + string("20160516") + string("/article")).c_str(),"ab+");
	fo[16] = fopen((dbpath + string("20160517") + string("/article")).c_str(),"ab+");
	fo[17] = fopen((dbpath + string("20160518") + string("/article")).c_str(),"ab+");
	fo[18] = fopen((dbpath + string("20160519") + string("/article")).c_str(),"ab+");
	fo[19] = fopen((dbpath + string("20160520") + string("/article")).c_str(),"ab+");
	fo[20] = fopen((dbpath + string("20160521") + string("/article")).c_str(),"ab+");
	fo[21] = fopen((dbpath + string("20160522") + string("/article")).c_str(),"ab+");
	fo[22] = fopen((dbpath + string("20160523") + string("/article")).c_str(),"ab+");
	fo[23] = fopen((dbpath + string("20160524") + string("/article")).c_str(),"ab+");
	fo[24] = fopen((dbpath + string("20160525") + string("/article")).c_str(),"ab+");
	fo[25] = fopen((dbpath + string("20160526") + string("/article")).c_str(),"ab+");
	fo[26] = fopen((dbpath + string("20160527") + string("/article")).c_str(),"ab+");
	fo[27] = fopen((dbpath + string("20160528") + string("/article")).c_str(),"ab+");
	fo[28] = fopen((dbpath + string("20160529") + string("/article")).c_str(),"ab+");
	fo[29] = fopen((dbpath + string("20160530") + string("/article")).c_str(),"ab+");

	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	int linenum = 0;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 5)continue;
		int fre = atoi(v.at(4).c_str()) - 20160501;
		if((fre < 0 )||(fre > 29)){
			commom::DEBUG_INFO(str);
			continue;
		}
		str += "\n";
		commom::WiteLine(str.c_str(),fo[fre]);
	}
	for(int i =0; i < 30; i++){
		fclose(fo[i]);
	}	
	delete fo;
}

void readfile(const char* filein){
	//////commom::Func f;	;	;	
	FILE*fi = fopen(filein,"r");
	if (fi == NULL) {
		commom::LOG_INFO("open file error" + string(filein));
		return ;
	}
	char buffer[MAX_LENTH];		
	std::string str = "";
	std::vector<std::string> v ;
	while ( commom::ReadLine(buffer,MAX_LENTH,fi)!=NULL)	{
		str = commom::GetLine(buffer); 
		commom::Split("\t", str, v);
		if(v.size() != 3)continue;
		if(v.at(1) == "1"){
			commom::DEBUG_INFO(str);
		}
	}
	fclose(fi);
}
int main(int argc, char *argv[]) {
	readfile(argv[1]);
	return 0;
}




/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For fcntl */
#include <fcntl.h>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_LINE 16384

void do_read(evutil_socket_t fd, short events, void *arg);
void do_write(evutil_socket_t fd, short events, void *arg);

char
rot13_char(char c)
{
    /* We don't want to use isalpha here; setting the locale would change
     * which characters are considered alphabetical. */
    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    else
        return c;
}

void
readcb(struct bufferevent *bev, void *ctx)
{
    struct evbuffer *input, *output;
    char *line;
    size_t n;
    int i;
    input = bufferevent_get_input(bev);
    output = bufferevent_get_output(bev);

    while ((line = evbuffer_readln(input, &n, EVBUFFER_EOL_LF))) {
        for (i = 0; i < n; ++i)
            line[i] = rot13_char(line[i]);
        evbuffer_add(output, line, n);
        evbuffer_add(output, "\n", 1);
        free(line);
    }

    if (evbuffer_get_length(input) >= MAX_LINE) {
        /* Too long; just process what there is and go on so that the buffer
         * doesn't grow infinitely long. */
        char buf[1024];
        while (evbuffer_get_length(input)) {
            int n = evbuffer_remove(input, buf, sizeof(buf));
            for (i = 0; i < n; ++i)
                buf[i] = rot13_char(buf[i]);
            evbuffer_add(output, buf, n);
        }
        evbuffer_add(output, "\n", 1);
    }
}

void
errorcb(struct bufferevent *bev, short error, void *ctx)
{
    if (error & BEV_EVENT_EOF) {
        /* connection has been closed, do any clean up here */
        /* ... */
    } else if (error & BEV_EVENT_ERROR) {
        /* check errno to see what error occurred */
        /* ... */
    } else if (error & BEV_EVENT_TIMEOUT) {
        /* must be a timeout event handle, handle it */
        /* ... */
    }
    bufferevent_free(bev);
}

void
do_accept(evutil_socket_t listener, short event, void *arg)
{
    struct event_base *base = arg;
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    if (fd < 0) {
        perror("accept");
    } else if (fd > FD_SETSIZE) {
        close(fd);
    } else {
        struct bufferevent *bev;
        evutil_make_socket_nonblocking(fd);
        bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, readcb, NULL, errorcb, NULL);
        bufferevent_setwatermark(bev, EV_READ, 0, MAX_LINE);
        bufferevent_enable(bev, EV_READ|EV_WRITE);
    }
}

void
run(void)
{
    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base *base;
    struct event *listener_event;

    base = event_base_new();
    if (!base)
        return; /*XXXerr*/

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(40713);

    listener = socket(AF_INET, SOCK_STREAM, 0);
    evutil_make_socket_nonblocking(listener);

#ifndef WIN32
    {
        int one = 1;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
#endif

    if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return;
    }

    if (listen(listener, 16)<0) {
        perror("listen");
        return;
    }

    listener_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);
    /*XXX check it */
    event_add(listener_event, NULL);

    event_base_dispatch(base);
}

int
main(int c, char **v)
{
    setvbuf(stdout, NULL, _IONBF, 0);

    run();
    return 0;
}