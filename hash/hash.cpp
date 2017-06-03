#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;
class node{
  public:
    string word;
    node *next;
    node *pre;
    int frequency;
};
bool is_digit(string astr){
  for(int i = 0; i < astr.length(); i++){
    if(int(astr[i]) < 48 || int(astr[i]) > 57){
      return 0;
    }
  }
  if(astr.length() > 10){
    return 0;
  }
  else if(astr.length() == 10){
    if(int(astr[0])-48 > 4) return 0;
    if(int(astr[0])-48 == 4 && int(astr[1])-48 > 2) return 0;
    if(int(astr[0])-48 == 4 && int(astr[1])-48 == 2 && int(astr[2])-48 > 9) return 0;
    if(int(astr[0])-48 == 4 && int(astr[1])-48 == 2 && int(astr[2])-48 == 9 && int(astr[3])-48 > 4) return 0;
    if(int(astr[0])-48 == 4 && int(astr[1])-48 == 2 && int(astr[2])-48 == 9 && int(astr[3])-48 == 4 && int(astr[4])-48 > 9) return 0;
    if(int(astr[0])-48 == 4 && int(astr[1])-48 == 2 && int(astr[2])-48 == 9 && int(astr[3])-48 == 4 && int(astr[4])-48 == 9 && int(astr[5])-48 > 6) return 0;
    if(int(astr[0])-48 == 4 && int(astr[1])-48 == 2 && int(astr[2])-48 == 9 && int(astr[3])-48 == 4 && int(astr[4])-48 == 9 && int(astr[5])-48 == 6 && int(astr[6])-48 > 7) return 0;
    if(int(astr[0])-48 == 4 && int(astr[1])-48 == 2 && int(astr[2])-48 == 9 && int(astr[3])-48 == 4 && int(astr[4])-48 == 9 && int(astr[5])-48 == 6 && int(astr[6])-48 == 7 && int(astr[7])-48 > 2) return 0;
    if(int(astr[0])-48 == 4 && int(astr[1])-48 == 2 && int(astr[2])-48 == 9 && int(astr[3])-48 == 4 && int(astr[4])-48 == 9 && int(astr[5])-48 == 6 && int(astr[6])-48 == 7 && int(astr[7])-48 == 2 && int(astr[8])-48 > 9) return 0;
    if(int(astr[0])-48 == 4 && int(astr[1])-48 == 2 && int(astr[2])-48 == 9 && int(astr[3])-48 == 4 && int(astr[4])-48 == 9 && int(astr[5])-48 == 6 && int(astr[6])-48 == 7 && int(astr[7])-48 == 2 && int(astr[8])-48 == 9 && int(astr[9])-48 > 5) return 0;

  }
  return 1;
}
int str2rint(string astr){
  int result;
  stringstream ss;
  ss<<astr;
  ss>>result;
  return result;
}
int str2int(string astr, int m){
  unsigned int result;
  stringstream ss;
  ss<<astr;
  ss>>result;
  return result % m;
}
int str_hash(string astr, int m){
  int anum = 0;
  for(int i = 0; i < astr.length(); i++){
    if(i != 0) anum = anum * 256 % m;
    unsigned char temp_char = astr[i];
    unsigned int temp = int(temp_char);
    //if(astr == "´º°ª")cout<<temp<<endl;
    anum += temp % m;
  }
  return anum % m;
}
bool if_exist(node *anode, string astr){
  node *ptr = anode;
  /*if(astr == "335472198"){
    cout<<"~~~~~start~~~~~~"<<endl;
    node* ap = ptr;
    while(ap != NULL){
      cout<<ap->word<<endl;
      ap = ap->next;
    }
    cout<<"~~~~~end~~~~~~"<<endl;
  }*/
  bool judge = 0;
  while(ptr != NULL){
    if(ptr->word == astr){
      judge = 1;
      break;
    }
    ptr = ptr->next;
  }
  return judge;
}
node* ptr2text(node *anode, string astr){
  node *ptr = anode;
  while(ptr != NULL){
    if(ptr->word == astr){
      break;
    }
    ptr = ptr->next;
  }
  return ptr;
}
void delete_node(node *anode, string text){
  node *test = anode;
  while(anode != NULL){
    if(anode->word == text){
      break;
    }
    anode = anode->next;
  }
  node null_node;
  null_node.word = "";
  null_node.frequency = 0;
  null_node.pre = NULL;
  null_node.next = NULL;
  if(anode->next == NULL){
    if(anode->pre == NULL) *anode = null_node;
    else *anode->pre->next = null_node;
  }
  else{
    if(anode->pre == NULL){
      *anode = *anode->next;
      anode->pre = NULL;
    }
    else{
      anode->pre->next = anode->next;
      anode->next->pre = anode->pre;
    }
  }
}
void insert_node(node *aptr, node *anode){
  node* temp = new node;
  temp->word = aptr->word;
  temp->frequency = aptr->frequency;
  temp->pre = aptr->pre;
  temp->next = aptr->next;
  *aptr = *anode;
  if(temp->word != ""){
    aptr->next = temp;
    aptr->next->pre = aptr;
  }
}
int main(int argc, char *argv[]){
  int int_length = 9;
  int int_factor_100 = 75;
  FILE* word_file;
  FILE* stop_word_file;
  for(int i = 0; i < argc; i++){
    string astr = argv[i];
    if(astr == "-f") int_factor_100 = str2rint(argv[i + 1]);
    if(astr == "-l") int_length = str2rint(argv[i + 1]);
    if(astr == "-w") word_file = fopen(argv[i + 1], "rb");
    if(astr == "-s") stop_word_file = fopen(argv[i + 1], "rb");
  }
  node* hash_table = new node[int_length];
  int size = 0;
  for(int i = 0; i < int_length; i++){
    hash_table[i].word = "";
    hash_table[i].next = NULL;
    hash_table[i].pre = NULL;
    hash_table[i].frequency = 0;
  }
  string text = "";
  int count = 0;
  fseek(word_file, 0, SEEK_END);
  int lSize = ftell(word_file);
  rewind(word_file);
  node* head = NULL;
  head = &hash_table[0];
  while(lSize != 0){
    unsigned char c;
    fread(&c, 1, 1, word_file);
    if(int(c) != 9 && int(c) != 10 && int(c) != 13 && int(c) != 32) text += c;
    else if(text != ""){
      size += 1;
      int value = 0;
      if(is_digit(text)){
        value = str2int(text, int_length);
        while(text[0] == '0'){
          if(text == "0") break;
          string newstr = "";
          for(int i = 1; i < text.length(); i++){
            newstr += text[i];
          }
          text = newstr;
        }
      }
      else value = str_hash(text, int_length);
      node new_one;
      new_one.word = text;
      new_one.pre = NULL;
      new_one.next = NULL;
      new_one.frequency = 1;
      node* ptr2node = head + value;
      node* correct = ptr2node;
      while(correct != NULL){
        if(correct->next != NULL)correct->next->pre = correct;
        correct = correct->next;
      }
      if(if_exist(ptr2node, text)){
        node *ptr_to_text = NULL;
        ptr_to_text = ptr2text(ptr2node, text);
        /*if(text == "I<qX,4Ubq;dUG&eh5ma") {
          node* haha = ptr2node;
          while(haha != NULL){
            cout<<"text "<<haha->word<<endl;
            cout<<"own "<<haha<<endl;
            cout<<"pre "<<haha->pre<<endl;
            cout<<"next "<<haha->next<<endl;
            haha = haha->next;
          }
          cout<<"----------"<<endl;
        }*/
        ptr_to_text->frequency += 1;
        size -= 1;
      }
      else insert_node(ptr2node, &new_one);
      //cout<<"insert "<<text<<" "<<value<<" "<<value<<" "<<int_length<<" "<<size<<endl;
      if(size * 100 > int_length * int_factor_100){
        int_length = int_length * 2 + 1;node new_table[int_length];
        for(int i = 0; i < int_length; i++){
          new_table[i].word = "";
          new_table[i].next = NULL;
          new_table[i].pre = NULL;
          new_table[i].frequency = 0;
        }
        for(int i = 0; i < (int_length - 1) / 2; i++){
          node* current = head + i;
          while(current->word != ""){
            string text2 = current->word;
            int value2 = 0;
            if(is_digit(text2)) value2 = str2int(text2, int_length);
            else value2 = str_hash(text2, int_length);
            node new_one2;
            new_one2.word = text2;
            new_one2.pre = NULL;
            new_one2.next = NULL;
            new_one2.frequency = current->frequency;
            insert_node(&new_table[value2], &new_one2);
            current = current->next;
            if(current == NULL) break;
          }
        }
        hash_table = new node[int_length];
        for(int i = 0; i < int_length; i++) hash_table[i] = new_table[i];
        head = &hash_table[0];
      }
      text = "";
    }
    lSize -= 1;
  }
  fclose(word_file);
  fseek(stop_word_file, 0, SEEK_END);
  int lSize2 = ftell(stop_word_file);
  rewind(stop_word_file);
  while(lSize2 != 0){
    unsigned char c;
    fread(&c, 1, 1, stop_word_file);
    if(int(c) != 9 && int(c) != 10 && int(c) != 13 && int(c) != 32) text += c;
    else if(text != ""){
      //cout<<"get"<<endl;
      int value = 0;
      if(is_digit(text)){
        value = str2int(text, int_length);
        while(text[0] == '0'){
          if(text == "0") break;
          string newstr = "";
          for(int i = 1; i < text.length(); i++){
            newstr += text[i];
          }
          text = newstr;
        }
      }
      else value = str_hash(text, int_length);
      node* ptr2node = head + value;
      /*if(text == "I<qX,4Ubq;dUG&eh5ma"){
        node* haha = ptr2node;
        while(haha!=NULL){
          cout<<haha->word<<endl;
          haha = haha->next;
        }
      }*/
      node* correct = ptr2node;
      while(correct != NULL){
        if(correct->next != NULL)correct->next->pre = correct;
        correct = correct->next;
      }
      if(if_exist(ptr2node, text)){
        //cout<<"out"<<endl;
        delete_node(ptr2node, text);
      }
      text = "";
      size -= 1;
    }
    lSize2 -= 1;
  }
  fclose(stop_word_file);
  cout<<"["<<endl;
  node* current = NULL;
  for(int i = 0; i < int_length; i++){
    current = head + i;
    cout<<"{";
    if(current->word != "") cout<<"\""<<current->word<<"\""<<":"<<current->frequency;
    while(current->next != NULL){
      current = current->next;
      if(current->word == "") break;
      cout<<","<<"\""<<current->word<<"\""<<":"<<current->frequency;
    }
    if(i < int_length - 1) cout<<"},"<<endl;
    else cout<<"}"<<endl;
    current = NULL;
  }
  cout<<"]"<<endl;
  return 0;
}
