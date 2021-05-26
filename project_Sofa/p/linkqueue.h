#include <string>
#include <queue>
#include <set>
#include <pthread.h>
using namespace std;

typedef void (*linkHandler)(string);
class linkQueue 
{
    private:    
    queue<string> _linkQueue;
    set<linkHandler> _handlers;
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t condvar;

    public:
    linkQueue();
    void addLink(string url);
    void registerHandler (linkHandler handler);  
    friend void * msgReceiver(void * arg);  
    friend void * HandlerInvoker(void * arg);

};