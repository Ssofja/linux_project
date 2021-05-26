#include <string>
#include <iostream>
#include "repo_in_memory.hpp"

void RepoInMemory::SaveLink(string link)
{
    this->_links.insert(link);
}

void RepoInMemory::SaveSite(string url, string content)
 {
     this->_sites[url] = content;
 }

string RepoInMemory::GetSite(string url)
{
    return this->_sites[url];
}