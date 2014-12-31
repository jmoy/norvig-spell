#include <unordered_map>
#include <string>
using namespace std;

#include "norvig.h"

class UMapFreq
{
  unordered_map<string,unsigned long> m;
public:
  void update(const string &s) {m[s]++;}
  unsigned long lookup(const string &s){
    auto it = m.find(s);
    if (it==m.end())
      return 0;
    return it->second;
  }
};

int main(int argc,char *argv[])
{
  Norvig<UMapFreq> norvig;
  return norvig.main(argc,argv);
}

 
