#include <string>
#include <iostream>
using namespace std;

class Repo
{
public:
    virtual void SaveLink(string link) = 0;
    virtual void SaveSite(string url, string content) = 0;
    virtual string GetSite(string url) = 0;


};