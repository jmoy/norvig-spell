#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cctype>

using namespace std;

const string alphabet="abcdefghijklmnopqrstuvwxyz";

/*
  Editing functions. With each edit found they call the
  provided function object 'f'.
*/
template<class FO> void deletes(const string&word, FO &f)
{
  for (size_t n=0;n<word.length();++n){
    string s=word;
    s.erase(n,1);
    f(s);
  }
}

template<class FO> void replaces(const string&word, FO &f)
{
  string s = word;
  for (size_t n=0;n<word.length();n++){
    char c = s[n];
    for (size_t i=0;i<alphabet.length();i++){
      s[n] = alphabet[i];
      f(s);
    }
    s[n] = c;
  }
}

template<class FO> void inserts(const string&word, FO &f)
{
  auto N = word.length();
  string s;
  
  for (size_t n=0;n<=N;n++){
    s.resize(N+1);
    for (size_t j=0;j<n;j++)
      s[j] = word[j];
    for (size_t j=n;j<N;j++)
      s[j+1] = word[j];
    for (size_t i=0;i<alphabet.length();i++){
      s[n] = alphabet[i];
      f(s);
    }
  }
}

template<class FO> void transposes(const string&word, FO &f)
{
  string s = word;
  for (size_t n=0;n<word.length()-1;n++){
    swap(s[n],s[n+1]);
    f(s);
    swap(s[n],s[n+1]);
 }
}

template<class FO> void edits1(const string &word,FO &f)
{
  deletes(word,f);
  replaces(word,f);
  transposes(word,f);
  inserts(word,f);
}

/*
  Function object, parametrised over the type of a frequency table.
  For the words with which it has been called it keeps track of the 
  one with the maximum frequency in the table.
*/
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

/*
  Function object that calls 'edits1' with an inner function
  object it has been initialized with.
  We use it to visit words at edit distance 2.
*/
template<class T> class TwoState
{
  T &inner;
public:
  TwoState<T>(T &in) : inner(in) {}
  void operator()(const string &s) {edits1(s,inner);}
};

/*
  The main class.
  It is parametrised over the type of frequency table.
  It expects the table to provide two public methods

    void update(const string &s) 

  which adds 1 to the observed frequency of 's', and

    unsigned long lookup(const string &s)

  which returns the observed frequency of 's'.
*/
template<class Freq> class Norvig {
  Freq freq;
public:
  void train(istream &s);
  string correct(const string &);
  int main(int argc,char *argv[]);
};

/*
  Given a word return its correction. Return the word
  itself if no correction is found.
*/
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

/*
  Train the model based on word frequencies in the given file
*/
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
    if (!wrd.empty())
      freq.update(wrd);
  }
}

/*
  Train the model from 'argv[1].
  Then read one word per line from standard input and
  print lines of the form

    [word], [correction]
*/
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
  while (getline(cin,word)){
    if (word.empty())
      continue;
    for (size_t n=0;n<word.length();n++)
      word[n] = tolower(word[n]);
    cout<<word<<", "<<correct(word)<<'\n';
  }
  return 0;
}
