#include<stdio.h>  
#include<string.h>  
#include<errno.h>  
  
#include<unistd.h>  
#include<event.h>  
  
void accept_cb(int fd, short events, void* arg);  
void socket_read_cb(int fd, short events, void *arg);  
int tcp_server_init(int port, int listen_num);  
  
int main(int argc, char** argv)  
{  
    int listener = tcp_server_init(9999, 10);  
    if( listener == -1 )  
    {  
        perror(" tcp_server_init error ");  
        return -1;  
    }  
  
    struct event_base* base = event_base_new();  
  
    //���Ӽ����ͻ������������¼�  
    struct event* ev_listen = event_new(base, listener, EV_READ | EV_PERSIST,  //������listener
                                        accept_cb, 
										base);
    event_add(ev_listen, NULL);//����

    event_base_dispatch(base);//ѭ������
  
    return 0;  
}
  
void accept_cb(int fd, short events, void* arg)  
{  
    evutil_socket_t sockfd;  
  
    struct sockaddr_in client;  
    socklen_t len = sizeof(client);  
  
    sockfd = ::accept(fd, (struct sockaddr*)&client, &len );  
    evutil_make_socket_nonblocking(sockfd);  
  
    printf("accept a client %d\n", sockfd);  
  
    struct event_base* base = (event_base*)arg;  
  
    //������Ϊ�˶�̬����һ��event�ṹ��  
    struct event *ev = event_new(NULL, -1, 0, NULL, NULL);  
    //���ﻹ��Ҫ�����ո������ϵ�socket
    event_assign(ev, base, sockfd, EV_READ | EV_PERSIST,  
                 socket_read_cb, (void*)ev);  
  
    event_add(ev, NULL);
}  
  
  
void socket_read_cb(int fd, short events, void *arg)  
{  
    char msg[4096];  
    struct event *ev = (struct event*)arg;  
    int len = read(fd, msg, sizeof(msg) - 1);  
    if( len <= 0 )
    {
		if(0 == len)
			printf("client closed\n");
        else
			printf("some error happen when read\n");  
        event_free(ev);  
        close(fd);  
        return ;  
    }
  
    msg[len] = '\0';  
    printf("recv the client msg: %s", msg);  
  
    char reply_msg[4096] = "I have recvieced the msg: ";  
    strcat(reply_msg + strlen(reply_msg), msg);  
  
    write(fd, reply_msg, strlen(reply_msg) );  
}  
  
int tcp_server_init(int port, int listen_num)  
{  
    int errno_save;  
    evutil_socket_t listener;  
  
    listener = ::socket(AF_INET, SOCK_STREAM, 0);  
    if( listener == -1 )  
        return -1;  
  
    //������ΰ�ͬһ����ַ��Ҫ����socket��bind֮��  
    evutil_make_listen_socket_reuseable(listener);  
  
    struct sockaddr_in sin;  
    sin.sin_family = AF_INET;  
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(port);  
  
    if( ::bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0 )  
        goto error;  
  
    if( ::listen(listener, listen_num) < 0)  
        goto error;  
  
  
    //��ƽ̨ͳһ�ӿڣ����׽�������Ϊ������״̬  
    evutil_make_socket_nonblocking(listener);  
  
    return listener;  
  
    error: 
        errno_save = errno;  
        evutil_closesocket(listener);  
        errno = errno_save;  
  
        return -1;  
}
/*
int evutil_make_listen_socket_reuseable(evutil_socket_t sock)
{
#ifndef WIN32
	int one = 1;
	return setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*) &one,
	    (ev_socklen_t)sizeof(one));
#else
	return 0;
#endif
}



int event_assign����struct event * ev��struct event_base * base��evutil_socket_t fd,short event��event_callback_fn fn��void * arg��
����event_assign����׼���ڽ�����event_add������event_del�����ĵ�����ʹ�õ��¼��ṹev ��
�¼���׼��������fn����ָ���ĺ�����ʹ��ָʾ�ļ���������int������ָʾ�¼����͵�short������arg������ָ����void *������fd��ʾӦ�����¼����ļ����������¼�������EV_READ��EV_WRITE�����ߡ�ָʾӦ�ó�����Էֱ���ļ���������ȡ��д�����������
��ʹ�ô����¼����ļ��������ͽ�ΪEV_TIMEOUT��EV_SIGNAL��EV_READ��EV_WRITE���¼����͵��ú���fn�����ӱ�־EV_PERSISTʹevent_add�����־ã�ֱ��event_del���������á�

��ע�⣬ʹ��event_assign�����������Ѿ��������¼��ṹ��������ͨ����Ҫ���Ĵ��������ڽṹ�Ĵ�С����������������δ���汾��Libevent�����ݡ�������ƺ���һ�������⣬ʹ��event_new������event_free������

������
ev		Ҫ�޸ĵ��¼��ṹ
base	evӦ�ø��ӵ��¼�������
fd		Ҫ���ӵ��ļ�������
event	�������¼�����; ������EV_READ��/��EV_WRITE
fn		���¼�����ʱ���õĻص�����
arg		һ��Ҫ���ݸ��ص������Ĳ���
���أ�
����ɹ�����Ϊ0������Ч����Ϊ-1��

*/