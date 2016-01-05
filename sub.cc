#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::move;
using size_t = unsigned int;

size_t char_count(const char a, const string& s){
  size_t c = 0;
  for(size_t i=0; i<s.length(); ++i)
    c += (a==s[i]?1:0);
  return c;
}

size_t tail_sscount(const string& t, const string& s){
  if(t.empty()) return 1;
  else if(t.length()>s.length()) return 0;
  else if(t[0]!=s[0])
    return tail_sscount(t, s.substr(1));
  else // child procedures of the two procedures could be overlapped
    return tail_sscount(t, s.substr(1))+tail_sscount(t.substr(1), s.substr(1));
}

size_t dp_sscount(const string& t, const string& s){
  size_t len1 = t.length();
  size_t len2 = s.length();
  auto dp = new size_t[(len1+1)*(len2+1)];
  // init
  for(int i=1; i<len1+1; ++i)
    dp[i] = 0;
  for(int i=0; i<len2+1; ++i)
    dp[(len1+1)*i] = 1;
  // traverse strings
  for(int i=1; i<len2+1; ++i){
    for(int j=1; j<len1+1; ++j){
      dp[i*(len1+1)+j] = dp[(i-1)*(len1+1)+j];
      if(t[j-1]==s[i-1]) dp[i*(len1+1)+j] += dp[(i-1)*(len1+1)+j-1];
    }
  }
  auto c = dp[(len1+1)*(len2+1)-1];
  delete[] dp;
  return c;
}

int main(){
  string t = "bddabcae";
  string s = "aabdbaabeeeadbcdeaabcdebdabbaddcdddaebbdbaabbcdeaabbddceebdabbcadbeabcecddaddbadbeadcccabdbdacdaebd";
  cout << dp_sscount(t, s) << '\n';
  cout << tail_sscount(t, s) << '\n';
  return 0;
}
