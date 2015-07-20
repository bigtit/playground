#include <iostream>
#include <set>
#include <map>

typedef struct kkk{
  char sk;
  unsigned int timer;
}kkk;

int main(){
  std::map<char, kkk> ktbl;
  std::map<char, std::set<char>> sktbl;
  for(unsigned int i=0; i<10; ++i)
    ktbl.insert(std::pair<char, kkk>(i, {static_cast<char>(i+1), i+2}));
  for(auto i:ktbl)
    std::cout << static_cast<int>(i.first) << static_cast<int>(i.second.sk) << i.second.timer << std::endl;
  return 0;
}

