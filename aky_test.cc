#include <iostream>
#include <set>
#include <map>

typedef struct kkk{
  char sk;
  int itv;
  int timer; // should be uint
}kkk;

std::map<char, kkk> ktbl;
std::map<char, std::set<char>> sktbl;

int main(){
  bool kup, kdn;
  char vk, vsk;
  vk = vsk = 100;
  kup = kdn = true;
  for(int i=0; i<10; ++i)
    ktbl.insert(std::pair<char, kkk>(i, {static_cast<char>(i+1), i+2, i+3}));
  sktbl[0].insert(0);
  // switching mode
  if(kup && sktbl.find(vsk)!=sktbl.end()){
    for(auto i:sktbl[vsk]){
      if(ktbl[i].timer){} // kill timers
      else{} // create timers
    }
  }
  // holding mode
  else if(ktbl.find(vk)!=ktbl.end()){
    if(kup && ktbl[vk].timer){} // kill timer
    else if(kdn && !ktbl[vk].timer){} // create a timer
  }
  return 0;
}

// add a group of key
// replace the sethk func
int addk(char k, char sk, int itv){
  if(ktbl.find(k)!=ktbl.end()) return 1; // key already exist
  ktbl.insert(std::pair<char, kkk>(k, {sk, itv, 0}));
  sktbl[sk].insert(k);
  return 0;
}

int rmk(char k){
  if(ktbl.find(k)==ktbl.end()) return 1; // no key found
  sktbl[ktbl[k].sk].erase(k);
  ktbl.erase(k);
  return 0;
}
