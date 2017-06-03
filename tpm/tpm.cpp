#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;
class node;
class package{
  public:
    string name;
    int version;
    node* depend;
    bool installed;
    int flag;//-1 uninstalled ; 0 dependency; 1 installed
};
class node{
  public:
    package apackage;
    node* next;
};
void insert_node(node* head, node *anode){//from a linked-list. insert last
  node* temp = head;
  if(temp != NULL){
    while(temp != NULL){
      if(temp->next == NULL){
        temp->next = new node;
        *temp->next = *anode;
        temp->next->next = NULL;
        break;
      }
      temp = temp->next;
    }
  }
}
void delete_node(node* head, string name, int version){//from a linked-list version == -1 all versions deleted
  node* temp = head;
  bool flag = 0;
  node* null_node = new node;
  null_node->apackage.name = "";
  null_node->next = NULL;
  while(temp != NULL){
    if((temp->apackage.name == name && temp->apackage.version == version) || (temp->apackage.name == name && temp->apackage.version == -1)){
      if(temp->next != NULL) *temp = *temp->next;
      else{
        *temp = *null_node;
        flag = 1;
        break;
      }
    }
    else temp = temp->next;
  }
  if(flag == 1){
    temp = head;
    while(temp->next->apackage.name != "") temp = temp->next;
    temp->next = NULL;
  }
}

void delete_node_check(node* anode){
  if(anode->next == NULL) anode = NULL;
  else *anode = *anode->next;
}
void sort1(node* head, int size);
bool if_in(node* alist, node* anode){
  node* temp = alist;
  if(alist == NULL) return 0;
  while(temp->next != NULL){
    if(temp->apackage.name == anode->apackage.name && temp->apackage.version == anode->apackage.version) return 1;
    temp = temp->next;
  }
  if(temp->apackage.name == anode->apackage.name && temp->apackage.version == anode->apackage.version) return 1;
  return 0;
}
node* delete_list = NULL;
bool is_depend(node* alist){//all depend deleted return 1; depend still exist return 0;
  node* temp = alist;
  while(temp != NULL){
    if(!if_in(delete_list, temp)) return 0;
    temp = temp->next;
  }
  return 1;
}
bool check_dependency(node* head, int anum){
  if(head != NULL){
    node* temp = new node;
    temp = head;
    node* backup = temp;
    int flag = 1;
    int count = 0;
    while(temp->next != NULL){
      if(temp->apackage.depend == NULL){
        if(delete_list == NULL){
          delete_list = new node;
          *delete_list = *temp;
          delete_list->next = NULL;
        }
        else insert_node(delete_list, temp);
        delete_node_check(temp);
        count += 1;
        temp = backup;
        flag = 0;
        continue;
      }
      else if(is_depend(temp->apackage.depend)){
        if(delete_list == NULL){
          delete_list = new node;
          *delete_list = *temp;
          delete_list->next = NULL;
        }
        else insert_node(delete_list, temp);
        delete_node_check(temp);
        count += 1;
        temp = backup;
        flag = 0;
        continue;
      }
      if(flag != 0) temp = temp->next;
      flag += 1;
      if(flag == anum) return 0;
    }
    if(count != anum - 1) return 0;
    if(is_depend(temp->apackage.depend)) return 1;
    else return 0;
  }
  return 1;
}

void delete_node_useless(node* useless, node* useful, node* pac_head){
  node* temp_useful = useful;
  while(temp_useful != NULL){
    cout<<useful->apackage.name<<":"<<useful->apackage.version<<endl;
    node* ptr = pac_head;
    while(ptr->apackage.name != useful->apackage.name || ptr->apackage.version != useful->apackage.version){
      ptr = ptr->next;
    }
    node* temp = new node;
    *temp = *ptr;
    temp->next = NULL;
    if(temp->apackage.depend == NULL){
      if(useless->next == NULL) useless->apackage.name = "";
      else delete_node(useless, temp->apackage.name, temp->apackage.version);
    }
    else{
      delete_node_useless(useless, temp->apackage.depend, pac_head);
      if(useless->next == NULL) useless->apackage.name = "";
      else delete_node(useless, temp->apackage.name, temp->apackage.version);
    }
    temp_useful = temp_useful->next;
  }
}
void check_useless(node* head, node* useless, node* pac_head){
  node* temp = head;
  node* useful = NULL;
  while(temp != NULL){
    if(temp->apackage.flag == 0){
      if(useless->apackage.name == ""){
        *useless = *temp;
        useless->next = NULL;
      }
      else insert_node(useless, temp);
    }
    else{
      if(useful == NULL){
        useful = new node;
        *useful = *temp;
        useful->next = NULL;
      }
      else insert_node(useful, temp);
    }
    temp = temp->next;
  }
  node* temp_useful = useful;
  while(temp_useful != NULL){
    //cout<<temp_useful->apackage.name<<":"<<temp_useful->apackage.version<<endl;
    temp_useful = temp_useful->next;
  }
  delete_node_useless(useless, useful, pac_head);
  node* test = useless;
  int size = 0;
  while(test != NULL){
    size += 1;
    test = test->next;
  }
  sort1(useless, size);
}


bool if_before(package package1, package package2){//if package2 should be in the position before package1
  if(package1.name == package2.name){
    if(package1.version > package2.version) return 1;
    else return 0;
  }
  else{
    int index = 0;
    while(index < package1.name.length() && index < package2.name.length()){
      if(int(package1.name[index]) > int(package2.name[index])) return 1;
      else if(int(package1.name[index]) < int(package2.name[index])) return 0;
      index += 1;
    }
    if(index < package1.name.length()) return 1;
    else return 0;
  }
}
void swap(package* ptr1, package* ptr2){
  package temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
}
void sort1(node* head, int size){
  node* ptr = head;
  package array[size];
  for(int i = 0; i < size; i++){
    array[i].name = "";
    array[i].version = -1;
    array[i].installed = 0;
    array[i].flag = -1;
    array[i].depend = NULL;
  }
  int index = 0;
  while(ptr != NULL){
    array[index] = ptr->apackage;
    //cout<<array[index].name<<endl;
    ptr = ptr->next;
    index += 1;
  }
  for(int i = 1; i < size; i++){
    int id = i;
    while(id != 0){
      if(if_before(array[id - 1], array[id])) swap(&array[id - 1], &array[id]);
      id -= 1;
    }
  }
  int index2 = 0;
  node* ptr2 = head;
  while(ptr2 != NULL){
    ptr2->apackage = array[index2];
    ptr2 = ptr2->next;
    index2 += 1;
  }

}
int main(){
  node* pac_head = NULL;
  node* loc_head = NULL;
  ifstream fp("package.dat");
  ifstream fl("local.dat");
  int size_pa = 0;
  int size_lo = 0;
  string p_temp_store;
  while(getline(fp, p_temp_store)){
    if(p_temp_store == "") break;
    stringstream fss(p_temp_store);
    string temp_fp;
    bool in_one = 0;
    while(fss>>temp_fp){
      int index = temp_fp.find_first_of(':');
      string temp_name = "";
      string temp_version = "";
      for(int i = 0; i < temp_fp.length(); i++){
        if(i < index) temp_name += temp_fp[i];
        else if(i > index) temp_version += temp_fp[i];
      }
      stringstream temp_ss(temp_version);
      int temp_true_version = -1;
      temp_ss>>temp_true_version;
      node* temp_node = new node;
      temp_node->next = NULL;
      temp_node->apackage.name = temp_name;
      temp_node->apackage.version = temp_true_version;
      temp_node->apackage.installed = 0;
      temp_node->apackage.flag = -1;
      //cout<<!in_one<<endl;
      if(!in_one){
        size_pa += 1;
        if(pac_head == NULL){
          pac_head = new node;
          *pac_head = *temp_node;
        }
        else if(!if_in(pac_head, temp_node)) insert_node(pac_head, temp_node);
        else if(if_in(pac_head, temp_node)) size_pa -= 1;
      }
      else{
        node* ptr = pac_head;
        while(ptr->next != NULL) ptr = ptr->next;
        //cout<<if_in(ptr->apackage.depend, temp_node)<<endl;
        if(!if_in(ptr->apackage.depend, temp_node)){
          if(ptr->apackage.depend != NULL) insert_node(ptr->apackage.depend, temp_node);
          else{
            ptr->apackage.depend = new node;
            *ptr->apackage.depend = *temp_node;
          }
        }
      }
      in_one = 1;
    }
  }
  string l_temp_store;
  while(getline(fl, l_temp_store)){
    size_lo += 1;
    if(l_temp_store == ""){
      size_lo -= 1;
      break;
    }
    stringstream lss(l_temp_store);
    string temp_fl;
    lss>>temp_fl;
    int index2 = temp_fl.find_first_of(':');
    string temp_name2 = "";
    string temp_version2 = "";
    for(int i = 0; i < temp_fl.length(); i++){
      if(i < index2) temp_name2 += temp_fl[i];
      else if(i > index2) temp_version2 += temp_fl[i];
    }
    stringstream temp_ss2(temp_version2);
    int temp_true_version2 = -1;
    temp_ss2>>temp_true_version2;
    node* temp_node2 = new node;
    temp_node2->next = NULL;
    temp_node2->apackage.name = temp_name2;
    temp_node2->apackage.version = temp_true_version2;
    temp_node2->apackage.installed = 1;
    int node_flag = 0;
    lss>>node_flag;
    temp_node2->apackage.flag = node_flag;
    if(loc_head == NULL){
      loc_head = new node;
      *loc_head = *temp_node2;
    }
    else if(!if_in(loc_head, temp_node2)) insert_node(loc_head, temp_node2);
    else if(if_in(loc_head, temp_node2)) size_lo -= 1;
  }
  ///////////////////////


  node* correct = loc_head;
  node* ptr_pa = pac_head;
  while(correct != NULL){
    while(ptr_pa->apackage.name != correct->apackage.name || ptr_pa->apackage.version != correct->apackage.version){
      ptr_pa = ptr_pa->next;
    }
    if(ptr_pa->apackage.depend != NULL){
      correct->apackage.depend = new node;
      *correct->apackage.depend = *ptr_pa->apackage.depend;
    }
    ptr_pa = pac_head;
    correct = correct->next;
  }
  ///////////////////////

  while(!cin.eof()){
    node* useless = new node;
    useless->apackage.name = "";
    node* broken = new node;
    broken->apackage.name = "";
    ////////////////////////
    cout<<">> ";
    string command, subcommand;
    getline(cin, command);
    stringstream css(command);
    if(command == "list all"){
      //cout<<size_pa<<endl;
      sort1(pac_head, size_pa);
      node* ptr = pac_head;
      if(ptr != NULL){
        while(ptr->next != NULL){
          if(ptr->apackage.name == ""){
            ptr = ptr->next;
            continue;
          }
          cout<<ptr->apackage.name<<":"<<ptr->apackage.version<<endl;
          ptr = ptr->next;
        }
        cout<<ptr->apackage.name<<":"<<ptr->apackage.version<<endl;
      }
    }
    else if(command == "list local"){
      //cout<<size_lo<<endl;
      sort1(loc_head, size_lo);
      node* ptr = loc_head;
      if(ptr != NULL){
        while(ptr->next != NULL){
          /*if(ptr->apackage.name == ""){
            cout<<"here"<<endl;
            ptr = ptr->next;
            continue;
          }*/
          cout<<ptr->apackage.name<<":"<<ptr->apackage.version;
          if(ptr->apackage.flag == 1) cout<<" *";
          cout<<endl;
          ptr = ptr->next;
        }
        cout<<ptr->apackage.name<<":"<<ptr->apackage.version;
        if(ptr->apackage.flag == 1) cout<<" *";
        cout<<endl;
      }
    }
    else if(command == "check circular dependency"){
      if(check_dependency(pac_head, size_pa)) cout<<"Circular dependency check passed."<<endl;
      else cout<<"Circular dependency check failed. At least one circular dependency is found."<<endl;
    }
    else if(command == "check useless"){
      check_useless(loc_head, useless, pac_head);
      if(useless->apackage.name != ""){
        cout<<"Useless package check failed. Here are all the useless packages:"<<endl;
        while(useless != NULL){
          cout<<useless->apackage.name<<":"<<useless->apackage.version<<endl;
          useless = useless->next;
        }
      }
      else{
        cout<<"Useless package check passed."<<endl;
      }
    }
    else if(command == "check broken"){

    }
    else{
      while(css>>subcommand){
      //cout<<subcommand<<endl;
      }
    }
  }
  return 0;
}
