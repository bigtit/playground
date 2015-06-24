// 2 algorithms to count subseqs
// sscount_better uses dp therefore has a better speed
#include <iostream>
#include <string>
using std::string;

int ccount(char c, string S)
{
  int i, count = 0;
  for(i = 0; i < S.length(); ++i){
    if(S[i] == c) ++count;
  }
  return count;
}

int sscount(string T, string S)
{
  int p, count = 0;

  if(T.length() == 1)
    return ccount(T[0], S);
  else{
    while((p = S.find(T[0], 0)) != string::npos){
      S = S.substr(p+1);
      count += sscount(T.substr(1), S);
    }
  }
  return count;
}

int sscount_better(string T, string S){
  int i, j, count = 0;
  int** dp = new int*[T.length() + 1];
  for(i = 0; i < T.length()+1; ++i)
    dp[i] = new int[S.length() + 1];

  dp[0][0] = 1;
  for(i = 1; i <= T.length(); i++)
    dp[i][0] = 0;
  for(j = 1; j <= S.length(); j++)
    dp[0][j] = 1;

  for(i = 1; i <= T.length(); i++){
    for(j = 1; j <= S.length(); j++){
      dp[i][j] = dp[i][j - 1];
      if(T.at(i - 1) == S.at(j - 1)){
        dp[i][j] += dp[i - 1][j - 1];
      }
    }
  }
  count = dp[T.length()][S.length()];
  for(i = 0; i < T.length()+1; ++i)
    delete[] dp[i];
  delete[] dp;
  return count;
} 

int main()
{
  string t = "bddabcae";
  string s = "aabdbaabeeadcbbdedacbbeecbabebaeeecaeabaedadcbdbcdaabebdadbbaeabdadeaabbabbecebbebcaddaacccebeaeedababedeacdeaaaeeaecbe";
  std::cout << sscount_better(t, s) << std::endl;
  std::cout << sscount(t, s) << std::endl;
  return 0;
}
