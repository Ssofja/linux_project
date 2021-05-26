#include "linkqueue.h"
#include <unistd.h>
#include <iostream>
using namespace std;
#undef _TRACE
#ifdef _TRACE
#define Trace(msg) std::cout << msg << std::endl;
#else
#define Trace(msg)
#endif
using namespace std;
void * HandlerInvoker(void * p){
	linkQueue* linkqueue = (linkQueue*)p;
    pthread_mutex_lock(&linkqueue-> mutex);

    while(linkqueue -> _linkQueue.size())
    {
        pthread_mutex_lock(&linkqueue->mutex);
        string link = linkqueue->_linkQueue.front();
        Trace(link);
        linkqueue->_linkQueue.pop();
        Trace("removing link from queue");
        pthread_mutex_unlock(&linkqueue->mutex);


       for(auto i = linkqueue->_handlers.begin(); i != linkqueue->_handlers.end(); ++i)
        {
            (*i)(link);
        }
    }
         
}
void* msgReceiver(void* arg)
{
    Trace("Broadcaster thread started");
    linkQueue* linkqueue = (linkQueue*) arg;
    while(true)
    {
        Trace("Waiting for link");
        pthread_cond_wait(&linkqueue->condvar, &linkqueue->mutex);
        Trace("Link received");
       pthread_t th;
       pthread_create(&th, NULL, HandlerInvoker, arg);
       pthread_mutex_unlock(&linkqueue->mutex);
	   
      
    }
}

linkQueue::linkQueue()
{
	pthread_create(&this->thread, nullptr, msgReceiver, (void*)this);
	pthread_mutex_init(&this->mutex, NULL);
	pthread_cond_init(&this->condvar, NULL);
	sleep(1);
}


void linkQueue::addLink(string link)
{
	
	Trace("Locking mutex");
	pthread_mutex_lock(&mutex);
	Trace("Adding link");
	this->_linkQueue.push(link);
	Trace("Link added");
    pthread_cond_signal(&condvar);
	Trace("Cond var is signaled");
	pthread_mutex_unlock(&mutex);
}

void linkQueue::registerHandler(linkHandler handler)
{
    this->_handlers.insert(handler);
}