#include <iostream>
#include <cstdio>
#include <string>
using namespace std;
class roads{
  public:
    int start; //smaller one
    int end; //bigger one
    int weight; //length
    int set;
};
class city{
  public:
    int num;
    city* next;
};
city* set_find(city* ptr){
  while(ptr->next != NULL){
    ptr->next = set_find(ptr->next);
    ptr = ptr->next;
  }
  return ptr;
}
int min(int num1, int num2){
  if(num1 <= num2) return num1;
  else return num2;
}
int max(int num1, int num2){
  if(num1 <= num2) return num2;
  else return num1;
}
bool if_shorter(roads road1, roads road2){
  if(road1.weight < road2.weight){
    return 1;
  }
  else if(road1.weight == road2.weight){
    int min1 = min(road1.start, road1.end);
    int min2 = min(road2.start, road2.end);
    int max1 = max(road1.start, road1.end);
    int max2 = max(road2.start, road2.end);
    if(min1 < min2) return 1;
    else if(min1 == min2 && max1 < max2) return 1;
    else return 0;
  }
  else return 0;
}
class node{
  public:
    int num;
    node* next;
    int citynum;
  };
void swap(roads* road1, roads* road2){
  roads new_road;
  new_road = *road1;
  *road1 = *road2;
  *road2 = new_road;
}
int min_city(roads* aro, node* ptr){
  int min = (aro + ptr->num)->start;
  if((aro + ptr->num)->start > (aro + ptr->num)->end) min = (aro + ptr->num)->end;
  while(ptr->next != NULL){
    ptr = ptr->next;
    if((aro + ptr->num)->start < min && ptr->num != -1) min = (aro + ptr->num)->start;
    if((aro + ptr->num)->end < min && ptr->num != -1) min = (aro + ptr->num)->end;
  }
  return min;
}
int num_cities;
int old_roads;
node* out_array = new node[num_cities];
node* out_array2 = new node[num_cities];
roads* old = new roads[old_roads];
city* city_array = new city[num_cities];
int main(){
  num_cities = 0;
  old_roads = 0;
  cin>>num_cities;
  cin>>old_roads;
  if(old_roads != 0){
    delete old;
    old = new roads[old_roads];
    for(int i = 0; i < old_roads; i++){
      cin>>old[i].start;
      cin>>old[i].end;
      cin>>old[i].weight;
      old[i].set = -1;
    }
    /////////////////////////heap_sort////////////////////////////
    int k = old_roads / 2 - 1;
    while(k > -1){
      int kk = k;
      while(kk < old_roads / 2){
        if(if_shorter(old[kk], old[2*kk + 1])){
          if(2*kk + 2 < old_roads && if_shorter(old[kk], old[2*kk + 2])){
            if(if_shorter(old[2*kk + 2], old[2*kk + 1])){
              swap(&old[kk], &old[2*kk + 1]);
              kk = 2*kk + 1;
            }
            else{
              swap(&old[kk], &old[2*kk + 2]);
              kk = 2*kk + 2;
            }
          }
          else{
            swap(&old[kk], &old[2*kk + 1]);
            kk = 2*kk + 1;
          }
        }
        else if(2*kk + 2 < old_roads && if_shorter(old[kk], old[2*kk + 2])){
          swap(&old[kk], &old[2*kk + 2]);
          kk = 2*kk + 2;
        }
        else break;
      }
      k -= 1;
    }
    for(int i = 0; i < old_roads; i++){
      //cout<<"---------before---------"<<endl;
      //for(int j = 0; j < old_roads; j++) cout<<old[j].start<<" "<<old[j].end<<" "<<old[j].weight<<endl;
      //cout<<"---------before---------"<<endl;
      swap(old[0],old[old_roads - 1 - i]);
      int kk2 = 0;
      while(2*kk2 + 1 < old_roads - 1 - i){
        if(if_shorter(old[kk2], old[2*kk2 + 1])){
          if(2*kk2 + 2 < old_roads - 1 - i && if_shorter(old[kk2], old[2*kk2 + 2])){
            if(if_shorter(old[2*kk2 + 2], old[2*kk2 + 1])){
              swap(&old[kk2], &old[2*kk2 + 1]);
              //cout<<old[kk2].weight<<" "<<old[2*kk2 + 1].weight<<endl;
              kk2 = 2*kk2 + 1;
            }
            else{
              swap(&old[kk2], &old[2*kk2 + 2]);
              //cout<<old[kk2].weight<<" "<<old[2*kk2 + 2].weight<<endl;
              kk2 = 2*kk2 + 2;
            }
          }
          else{
            swap(&old[kk2], &old[2*kk2 + 1]);
            //cout<<old[kk2].weight<<" "<<old[2*kk2 + 1].weight<<endl;
            kk2 = 2*kk2 + 1;
          }
        }
        else{
          if(2*kk2 + 2 < old_roads - 1 - i && if_shorter(old[kk2], old[2*kk2 + 2])){
            swap(&old[kk2], &old[2*kk2 + 2]);
            //cout<<old[kk2].weight<<" "<<old[2*kk2 + 2].weight<<endl;
            kk2 = 2*kk2 + 2;
          }
          else break;
        }
      }
      //cout<<"after "<<old[0].weight<<endl;
      //cout<<"---------after---------"<<endl;
      //for(int j = 0; j < old_roads; j++) cout<<old[j].start<<" "<<old[j].end<<" "<<old[j].weight<<endl;
      //cout<<"---------after---------"<<endl;
    }
    /////////////////////////heap_sort////////////////////////////
    //for(int j = 0; j < old_roads; j++) cout<<old[j].start<<" "<<old[j].end<<" "<<old[j].weight<<endl;
    delete city_array;
    city_array = new city[num_cities];
    for(int i = 0;  i < num_cities; i++){
      city_array[i].num = i;
      city_array[i].next = NULL;
    }
    for(int i = 0; i < old_roads; i++){
      int begin = old[i].start;
      int over = old[i].end;
      city* begin_parent = set_find(&city_array[begin]);
      city* over_parent = set_find(&city_array[over]);
      if(begin_parent->num != over_parent->num){
        over_parent->next = begin_parent;
        old[i].set = begin_parent->num;
      }
    }
    delete out_array;
    delete out_array2;
    out_array = new node[num_cities];
    out_array2 = new node[num_cities];
    //cout<<"here"<<endl;
    for(int i = 0; i < num_cities; i++){
      out_array[i].num = -1;
      out_array[i].next = NULL;
      out_array[i].citynum = 0;
      out_array2[i].num = -1;
      out_array2[i].next = NULL;
      out_array2[i].citynum = 0;
    }
    for(int i = 0; i < old_roads; i++){
      if(old[i].set != -1){
        city* set_mid = set_find(&city_array[old[i].start]);
        old[i].set = set_mid->num;
        node* temp_next = out_array[old[i].set].next;
        out_array[old[i].set].next = new node;
        out_array[old[i].set].next->next = temp_next;
        out_array[old[i].set].next->num = out_array[old[i].set].num;
        out_array[old[i].set].num = i;
        out_array[old[i].set].citynum += 1;
      }
    }

    int set_count = 0;
    for(int i = 0; i < num_cities; i++){
      if(out_array[i].next != NULL){
        set_count += 1;
        int temp_value = out_array[i].citynum;\
        if(out_array2[temp_value].next == NULL) out_array2[temp_value] = out_array[i];
        else{
          if(min_city(&old[0], &out_array2[temp_value]) > min_city(&old[0], &out_array[i])){
            node temp_node = out_array2[temp_value];
            out_array2[temp_value] = out_array[i];
            temp_value += 1;
            while(out_array2[temp_value].next != NULL){
              if(min_city(&old[0], &out_array2[temp_value]) > min_city(&old[0], &temp_node)){
                node temp2 = out_array2[temp_value];
                out_array2[temp_value] = temp_node;
                temp_node = temp2;
              }
              temp_value += 1;
            }
            out_array2[temp_value] = temp_node;
          }
          else{
            node temp_node = out_array[i];
            temp_value += 1;
            while(out_array2[temp_value].next != NULL){
              if(min_city(&old[0], &out_array2[temp_value]) > min_city(&old[0], &temp_node)){
                node temp2 = out_array2[temp_value];
                out_array2[temp_value] = temp_node;
                temp_node = temp2;
              }
              temp_value += 1;
            }
            out_array2[temp_value] = temp_node;
          }
        }
      }
    }
    cout<<"["<<endl;
    for(int i = num_cities - 1; i > -1; i--){
      node* ptr_trace = &out_array2[i];
      while(ptr_trace->next != NULL){
        if(ptr_trace == &out_array2[i]) cout<<"["<<endl;
        if(old[ptr_trace->num].start<old[ptr_trace->num].end) cout<<"["<<old[ptr_trace->num].start<<","<<old[ptr_trace->num].end<<","<<old[ptr_trace->num].weight<<"]";
        else cout<<"["<<old[ptr_trace->num].end<<","<<old[ptr_trace->num].start<<","<<old[ptr_trace->num].weight<<"]";
        ptr_trace = ptr_trace->next;
        if(ptr_trace->next != NULL) cout<<","<<endl;
        else{
          cout<<endl;
          set_count -= 1;
          if(set_count != 0) cout<<"],"<<endl;
          else cout<<"]"<<endl;
        }
      }
    }
    cout<<"]"<<endl;
  }
  else{
    cout<<"["<<endl;
    cout<<"]"<<endl;
  }
  delete city_array;
  delete out_array;
  delete out_array2;
  delete old;
  return 0;
}
