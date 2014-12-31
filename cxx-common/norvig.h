#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

const string alphabet="abcdefghijklmnopqrstuvwxyz";

template<class FO> void deletes(const string&word, FO &f)
{
  for (auto n=0;n<word.length();++n){
    string s=word;
    s.erase(n,1);
    f(s);
  }
}

template<class FO> void replaces(const string&word, FO &f)
{
  for (auto n=0;n<word.length();n++){
    string s = word;
    for (auto i=0;i<alphabet.length();i++){
      s[n] = alphabet[i];
      f(s);
    }
  }
}

template<class FO> void inserts(const string&word, FO &f)
{
  auto N = word.length();
  string s;
  
  for (auto n=0;n<=N;n++){
    s.resize(N+1);
    for (auto j=0;j<n;j++)
      s[j] = word[j];
    for (auto j=n;j<N;j++)
      s[j+1] = word[j];
    for (auto i=0;i<alphabet.length();i++){
      s[n] = alphabet[i];
      f(s);
    }
  }
}

template<class FO> void transposes(const string&word, FO &f)
{
  for (auto n=0;n<word.length()-1;n++){
    string s = word;
    swap(s[n],s[n+1]);
    f(s);
 }
}

template<class FO> void edits1(const string &word,FO &f)
{
  deletes(word,f);
  replaces(word,f);
  transposes(word,f);
  inserts(word,f);
}

template<class Freq> class Norvig {
  Freq freq;
public:
  void train(istream &s);
  string correct(const string &);
  int main(int argc,char *argv[]);
};

template<class Freq> struct MaxState
{
  Freq &m;
  long max;
  string maxarg;
  MaxState<Freq>(Freq &mod) : m(mod),max(0) {}
  void operator()(const string &s){
    long val = m.lookup(s);
    if (val>max){
      max = val;
      maxarg = s;
    }
  }
};

template<class T> class TwoState
{
  T &inner;
public:
  TwoState<T>(T &in) : inner(in) {}
  void operator()(const string &s) {edits1(s,inner);}
};

template<class F>
string Norvig<F>::correct(const string &word)
{
  if (freq.lookup(word))
    return word;
  MaxState<F> ms(freq);
  edits1(word,ms);
  if (ms.max)
    return ms.maxarg;
  TwoState<MaxState<F> > twoedit(ms);
  edits1(word,twoedit);
  if (ms.max)
    return ms.maxarg;
  return word;
}

template<class F>
void Norvig<F>::train(istream &fs)
{
  while (fs){
    char c;
    string wrd;
    while (fs.get(c) && !isalpha(c))
      ;
    while(fs && isalpha(c)){
      wrd+=tolower(c);
      fs.get(c);
    }
    freq.update(wrd);
  }
}

template<class F>
int Norvig<F>::main(int argc,char*argv[])
{
  if (argc!=2){
    cerr<<"Usage: norvig [training dataset]\n";
    return 2;
  }
  ifstream fs(argv[1]);
  if (!fs){
    cerr<<argv[1]<<": "<<strerror(errno)<<'\n';
    return 1;
  }
  train(fs);
  fs.close();
  
  string word;
  while (cin){
    getline(cin,word);
    if (word.empty())
      continue;
    for (auto n=0;n<word.length();n++)
      word[n] = tolower(word[n]);
    cout<<word<<", "<<correct(word)<<'\n';
  }
  return 0;
}
