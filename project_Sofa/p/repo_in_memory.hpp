//RepoInMemory
#include "repo.hpp"
#include <iostream>
#include <string>
#include <set>
#include <map>
using namespace std;

class RepoInMemory : public Repo
{
public:
    virtual void SaveLink(string link);
    virtual void SaveSite(string url, string content);
    virtual string GetSite(string url);

private:
    set<string> _links;
    map<string, string> _sites;
};