#include <iostream>
#include <stdexcept>
#include <regex.h>
#include "easyhttpcpp/EasyHttp.h"
#include "linkqueue.h"
#include "repo_in_memory.hpp"
#include <sstream>
#include <iterator>
#include <unistd.h>
using namespace std;
string GetContents(std::string url){
  try{
	easyhttpcpp::EasyHttp::Builder httpClientBuilder;
    
    easyhttpcpp::EasyHttp::Ptr pHttpClient = httpClientBuilder.build();
    
    easyhttpcpp::Request::Builder requestBuilder;
    easyhttpcpp::Request::Ptr pRequest = requestBuilder.setUrl(url).build();
    easyhttpcpp::Call::Ptr pCall = pHttpClient->newCall(pRequest);
    easyhttpcpp::Response::Ptr pResponse = pCall->execute();
    if (!pResponse->isSuccessful()){
        cerr << "HTTP GET Error: (" << pResponse->getCode() << ")" << endl;
        return "";
    } else {
        cout << "HTTP GET Success!" << endl;
	    const string contentType = pResponse->getHeaderValue("Content-Type", "");
		    if (Poco::isubstr<string>(contentType, "text/html") != string::npos){
            return pResponse->getBody()->toString();
        }
            return "";
    }
  }
    catch(...){
        cerr << "Error while getting contents";
        return "";
    }
}

const int MAX_MATCHES = 10;
linkQueue linkqueue;
Repo* repo;


void linkAdded(string link){
    static int count=0;
    
    cout<<"Crawler:: LinkAdded! " << link << endl;
    if(!(link.find("http")==0)){
        return;
    }
    repo->SaveLink(link);
    string contents = GetContents(link);
    if(contents == "" || contents.size() < 100){
            return;

    }
    repo->SaveSite(link, contents);
    
    
    
const string link_prefix = "href=";
  size_t pos = 0;
  while((pos < contents.size()) && ((pos = contents.find(link_prefix, pos + 1)) != string::npos)){
    cout << "Position: " << pos << endl;
    pos += link_prefix.size() + 1;
    char quot = contents[pos -1];
    if(quot != '\'' && quot != '\"'){
      continue;
    }

    size_t end_pos = contents.find(quot, pos);

    if(end_pos == string::npos){
      break;
    }
    string link = contents.substr(pos, end_pos - pos);
    if(!( link.find("http") == 0 /*|| link[0] == '/'*/)){
      
      continue;
    }
    
    linkqueue.addLink(link);
  }
}

void TestHandler(std::string link){
    cout << "Link received " << link << std::endl;
}

void TestHandler2(std::string link){
 cout << "Link recived in test2 " << link << std::endl;
}

int run_server();
int main(){

    cout << "Running crawler" << std::endl;
    
    linkqueue.registerHandler(linkAdded);
    
    
    linkAdded("https://www.topuniversities.com/student-info/choosing-university/worlds-top-100-universities");
   
    cout << "Running server..." << std::endl;
    run_server();
    
    cout << "Running crawler - Done!" << std::endl;
    return 0;
}
int main_regex(){
    string contents = GetContents("https://www.topuniversities.com/student-info/choosing-university/worlds-top-100-universities");
    string regex = "(\\s)*(\t)*Mem([0-9]*) (\\s,\t)*= (\\s,\t)*[0-9]*(.)*[0-9]*" ;
    regex_t re;
    const int MAX_MATCHES = 10;
    if(regcomp(&re, regex.c_str(), REG_EXTENDED) != 0 ){
        cerr << "Cannot compile regex" << endl;
        return 1;
    }
    regmatch_t matches[MAX_MATCHES];
    if(regexec(&re, contents.c_str(), sizeof(matches)/sizeof(matches[0]), (regmatch_t*)&matches,0) == REG_NOMATCH){
        std::cerr << "No links found!!" << std::endl;
        return 1;
    }
    for (int i = 0, offset = 0; i < MAX_MATCHES && offset < contents.size(); ++i){
        cout << "Matches  " << i << "->" << contents.substr(matches[i].rm_so, matches[i].rm_eo - matches[i].rm_so + 1) << endl;
        offset = matches[i].rm_eo + 1;
    }
    return 0;
}