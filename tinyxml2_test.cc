#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include "../tinyxml2/tinyxml2.h"
using namespace tinyxml2;
using std::cout;
using std::endl;
using std::map;
using std::set;
using std::stoi;
using std::to_string;

typedef struct{
  char skey;
  int itv;
  int timer;
}kkk;

map<char, kkk> kt;
map<char, set<char>> skt;

void read_cfg(std::string&);
void write_cfg(std::string&);

int main(){
  std::string fin = "config.xml", fout = "config2.xml";
  read_cfg(fin);
  write_cfg(fout);
  return 0;
}

void read_cfg(std::string& fname){
  XMLDocument doc;
  doc.LoadFile(fname.c_str());
  auto cfg = doc.RootElement();
  auto keys = cfg->FirstChildElement();
  while(keys){
    auto k = keys->FirstChildElement("key");
    if(!k && kt.find(stoi(k->GetText()))!=kt.end()) return;
    char n1 = stoi(k->GetText());
    k = k->NextSiblingElement("skey");
    char n2 = stoi(k->GetText());
    k = k->NextSiblingElement("interval");
    int n3 = stoi(k->GetText());
    kt.insert(std::pair<char, kkk>(n1, {n2, n3, 0}));
    skt[n2].insert(n1);

    keys = keys->NextSiblingElement();
  }
}

void write_cfg(std::string& fname){
  XMLDocument doc;
  auto dec = doc.NewDeclaration();
  doc.InsertFirstChild(dec);
  dec->SetValue("xml version='1.0' encoding='UTF-8'");
  auto cfg = doc.NewElement("config");
  doc.InsertEndChild(cfg);

  // loop
  // use only kt;
  for(auto k:kt){
    auto keys = doc.NewElement("keys");
    cfg->InsertEndChild(keys);
    auto key = doc.NewElement("key");
    key->SetText(to_string(k.first).c_str());
    keys->InsertEndChild(key);
    auto skey = doc.NewElement("skey");
    skey->SetText(to_string(k.second.skey).c_str());
    keys->InsertEndChild(skey);
    auto itv = doc.NewElement("interval");
    itv->SetText(to_string(k.second.itv).c_str());
    keys->InsertEndChild(itv);
  }

  doc.SaveFile(fname.c_str());
}

