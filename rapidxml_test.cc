#include <iostream>
#include <cstddef>
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_utils.hpp"
#include "../rapidxml/rapidxml_print.hpp"
using namespace rapidxml;
using std::cout;
using std::endl;

int main(){
  xml_document<> doc;
  xml_node<>* rot = doc.allocate_node(node_pi, doc.allocate_string("xml version='1.0' encoding='utf-8'"));
  doc.append_node(rot);
  xml_node<>* node = doc.allocate_node(node_element, "config", "information");
  doc.append_node(node);

  xml_node<>* color = doc.allocate_node(node_element, "key001", 0);
  node->append_node(color);
  color->append_node(doc.allocate_node(node_element, "key", "87"));
  color->append_node(doc.allocate_node(node_element, "skey", "123"));
  color->append_node(doc.allocate_node(node_element, "interval", "150"));

  xml_node<>* size = doc.allocate_node(node_element, "key002", 0);
  node->append_node(size);
  size->append_node(doc.allocate_node(node_element, "key", "87"));
  size->append_node(doc.allocate_node(node_element, "skey", "123"));
  size->append_node(doc.allocate_node(node_element, "interval", "150"));

  // write to file
  // std::cout << "writing file" << std::endl;

  std::ofstream out("config.xml");
  out << doc;
  out.close();

  file<> fdoc("config.xml");
  cout << fdoc.data() << endl;
  xml_document<> doc2;
  doc2.parse<0>(fdoc.data());
  // get root
  xml_node<>* root = doc2.first_node();
  for(auto i=root->first_node(); i; i=i->next_sibling()){
    cout << i->name() << endl;
    for(auto j=i->first_node(); j; j=j->next_sibling())
      cout << "  " << j->name() << ": " << j->value() << endl;
  }
  cout << root->name() << endl;
  
  return EXIT_SUCCESS;
}

