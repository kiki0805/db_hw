#include <iostream>
#include <cstdio>
#include <string>
using namespace std;
class tree{
  public:
    tree *left_tree;
    tree *right_tree;
    int frequency;
    int value;
    int size;
    int leaf_num;
};
bool is_leaf(tree atree){
  if(atree.left_tree == NULL && atree.right_tree == NULL) return 1;
  else return 0;
}
tree* depth_first(tree atree){
  tree *ptr = new tree[atree.size];
  tree* tree_array[atree.size];
  for(int i = 0; i < atree.size; i++){
    tree_array[i] = NULL;
  }
  tree_array[0] = &atree;
  if(atree.size == 1) return ptr;
  int num_of_leaf = 0;
  int length_array = 0;
  while(tree_array[0] != NULL){
    if(is_leaf(*tree_array[length_array])){
      tree temp_tree = *tree_array[length_array];
      ptr[num_of_leaf] = temp_tree;
      num_of_leaf = num_of_leaf + 1;
      tree_array[length_array] = NULL;
      length_array -= 1;
    }
    else{
      if(tree_array[length_array]->right_tree != NULL){
        if(tree_array[length_array]->left_tree != NULL){
          tree_array[length_array + 1] = tree_array[length_array]->left_tree;
          tree_array[length_array] = tree_array[length_array]->right_tree;
          length_array += 1;
        }
        else tree_array[length_array] = tree_array[length_array]->right_tree;
      }
      else{
        tree_array[length_array] = tree_array[length_array]->left_tree;
      }
    }
  }
  return ptr;//叶节点的数组第一个；
}
int min_leaves(tree aptr){
  if(aptr.size == 1) return aptr.value;
  tree* aleaf_array = depth_first(aptr);
  int min = aleaf_array->value;
  int sum = aptr.leaf_num;
  //cout<<sum<<endl;
  while(1){
    if(sum == 0) break;
    if(aleaf_array->value <= min) min = aleaf_array->value;
    aleaf_array += 1;
    //cout<<aleaf_array->value<<endl;
    sum -= 1;
    if(aleaf_array->value == 0 && sum != 0){
      min = 0;
      break;
    }
  }
  return min;
}
string* get_code(tree atree, int inkinds){//value 置零
  string* ptr = NULL;
  ptr = new string[inkinds * 2];
  tree* current = &atree;
  int index = 0;
  while(atree.right_tree != NULL || atree.left_tree != NULL){
    if(current->left_tree == NULL && current->right_tree == NULL){
      if(current->value != -1){
        index += 1;
        ptr[index] = char(current->value);
        //if(current->value == 73) cout<<ptr[index - 1]<<endl;
        index += 1;
      }
      else ptr[index] = "";
      if(index == inkinds * 2){
        current = NULL;
        break;
      }
      current = &atree;
    }
    else if(current->left_tree == NULL && current->right_tree != NULL){
      if(current->right_tree->left_tree == NULL && current->right_tree->right_tree == NULL){
        tree* tem = new tree;
        tem = current->right_tree;
        if(current->frequency != atree.frequency){
          current->right_tree = NULL;
          current = tem;
        }
        else{
          current->right_tree = NULL;
          current = tem;
          if(current->value != -1){
            ptr[index] += "0";
            ptr[index + 1] = char(current->value);
            //if(current->value == 0) cout<<ptr[index]<<endl;
            current = NULL;
            break;
          }
          else{
            ptr[index] = "";
            current = NULL;
            break;
          }
        }
      }
      else{
        current = current->right_tree;
      }
      ptr[index] += "0";
    }
    else if(current->left_tree != NULL){
      if(current->left_tree->left_tree == NULL && current->left_tree->right_tree == NULL){
        tree* tem = new tree;
        tem = current->left_tree;
        current->left_tree = NULL;
        current = tem;
      }
      else{
        current = current->left_tree;
      }
      ptr[index] += "1";
    }
  }
  return ptr;
}
tree build_tree(int* frequency_array, int inkinds){
  tree* huffman_tree[inkinds];
  for(int i = 0; i <inkinds; i++) huffman_tree[i] = NULL;
  int num = inkinds;
  for(int i = 0; i < inkinds; i++){
    huffman_tree[i] = new tree;
    huffman_tree[i]->right_tree = NULL;
    huffman_tree[i]->left_tree = NULL;
    huffman_tree[i]->value = *(frequency_array + i * 2);
    huffman_tree[i]->frequency = *(frequency_array + 1 + i * 2);
    huffman_tree[i]->size = 1;
    huffman_tree[i]->leaf_num = 1;
  }
  while(num != 1){
    tree temp_tree;
    int trace = 0;
    //if(huffman_tree[0]->value == 73 || huffman_tree[1]->value == 73) trace = 1;
    if(huffman_tree[0] != NULL ) temp_tree.left_tree = huffman_tree[0];
    if(huffman_tree[1] != NULL ) temp_tree.right_tree = huffman_tree[1];
    temp_tree.frequency = huffman_tree[0]->frequency + huffman_tree[1]->frequency;
    temp_tree.size = huffman_tree[0]->size + huffman_tree[1]->size + 1;
    temp_tree.value = -1;
    temp_tree.leaf_num = huffman_tree[0]->leaf_num + huffman_tree[1]->leaf_num;
    int n = 0;
    for(int i = 2; i <= num; i++){
      if(i == num){
        n = i;
        break;
      }
      if(huffman_tree[i]->frequency > temp_tree.frequency){
        n = i;
        break;
      }
      else if(huffman_tree[i]->frequency == temp_tree.frequency){
        if(min_leaves(*huffman_tree[i]) > min_leaves(temp_tree)){
          n = i;
          break;
        }
      }
    }
    //2----(n - 2)向前移动2格，n----(num - 1)向前移动1格，插入到(n - 1)
    for(int i = 0; i < (n - 2); i++){
      huffman_tree[i] = huffman_tree[i + 2];
    }
    for(int i = n - 1; i < (num - 1); i++){
      huffman_tree[i] = huffman_tree[i + 1];
    }
    if(n == num){
      huffman_tree[n - 1] = NULL;
    }
    huffman_tree[n - 2] = new tree;
    huffman_tree[n - 2]->frequency = temp_tree.frequency;
    huffman_tree[n - 2]->value = temp_tree.value;
    huffman_tree[n - 2]->size = temp_tree.size;
    huffman_tree[n - 2]->left_tree = temp_tree.left_tree;
    huffman_tree[n - 2]->right_tree = temp_tree.right_tree;
    huffman_tree[n - 2]->leaf_num = temp_tree.leaf_num;
    num -= 1;
  }
  return *huffman_tree[0];
}
char conversion(string atr){//将八位字符串转换为char
  int num = 0;
  int add = 1;
  for(int i = 0; i < 8; i++){
    if(atr[7 - i] == '1'){
      for(int j = 0; j < i; j++) add *= 2;
      num += add;
      add = 1;
    }
  }
  unsigned char ach = num;
  return ach;
}
string inverse(string astr){
  string str = "";
  for(int i = astr.length() - 1; i >= 0; i--){
    str += astr[i];
  }
  return str;
}
string conversion2(int anum){//将四位代码转换为字符串
  string astr = "";
  while(anum / 2 != 0){
    int remain = 0;
    remain = anum % 2;
    anum /= 2;
    if(remain == 1) astr = '1' + astr;
    else astr = '0' + astr;
    if(anum / 2 == 0){
      remain = anum % 2;
      anum /= 2;
      if(remain == 1) astr = '1' + astr;
      else astr = '0' + astr;
      break;
    }
  }
  if(astr.length() != 8){
    int temp_len = 0;
    temp_len = astr.length();
    for(int i = 0; i < 8 - temp_len; i++){
      astr = '0' + astr;
    }
  }
  if(anum == 1) return "10000000";
  string aastr = inverse(astr);
  //cout<<astr<<" ";
  return aastr;
}
int main(int argv, char *argc[]){
  if(argv > 1){
    int inkinds = 0;
    int count[256];//每个ascii码出现频率
    int exist[256];//存在的ascii码
    for(int i = 0; i < 256; i++){
      exist[i] = 0;
      count[i] = 0;
    }
    int count_num = 0;
    int temp;
    //读入
    fseek(stdin, 0, SEEK_END);
    int lSize = ftell(stdin);
    int realSize = lSize * 8;
    rewind(stdin);
    while(1){
      if(count_num >= 2){
        if(count_num == 258) break;
        int frequency = 0;
        fread(&frequency,4,1,stdin);
        realSize -= 32;
        if(frequency != 0){
          exist[inkinds] = count_num - 2;
          inkinds += 1;
          count[count_num - 2] = frequency;
        }
      }
      else{
        realSize -= 32;
        fread(&temp,4,1,stdin);
      }
      count_num += 1;
    }
    while(1){//将出现字符排序（低频率在前，高频率在后）
      int if_swap = 0;
      for(int i = 1; i < inkinds; i++){
        if(count[exist[i]] < count[exist [i - 1]]){
          int temp = exist[i - 1];
          exist[i - 1] = exist[i];
          exist[i] = temp;
          if_swap = 1;
        }
        else if(count[exist[i]] == count[exist [i - 1]] && exist[i] < exist[i - 1]){
          int temp = exist[i - 1];
          exist[i - 1] = exist[i];
          exist[i] = temp;
          if_swap = 1;
        }
      }
      if(if_swap == 0) break;
    }
    //得出频率数组和代压缩文本
    int frequency_array[inkinds][2];// [index][0] = ASCII; [index][1] = frequency;
    string code[inkinds][2];//[index][0] = code ; [index][1] = ASCII;
    for(int i = 0; i < inkinds; i++){//遍历赋值
      frequency_array[i][0] = exist[i];
      frequency_array[i][1] = count[exist[i]];
    }
    //建树得出code数组
    tree huffman_tree = build_tree(&frequency_array[0][0], inkinds);
    //遍历求code
    string* code_ptr = get_code(huffman_tree, inkinds);
    for(int i = 0; i < inkinds; i++){
      code[i][0] = *code_ptr;
      code_ptr += 1;
      code[i][1] = *code_ptr;
      code_ptr += 1;
      //unsigned char aaa = *code[i][1].c_str();
      //cout<<code[i][0]<<" "<<int(aaa)<<endl;
    }
    string in_str = "";
    while(realSize != 0){
      int text = 0;
      fread(&text,1,1,stdin);
      realSize -= 8;
      in_str += conversion2(text);
    }
    string code_str = "";
    int all = 0;
    //cout<<in_str<<endl;
    for(int i = 0; i < in_str.length(); i++){
      code_str += in_str[i];
      //cout<<code_str<<endl;
      for(int j = 0; j < inkinds; j++){
        if(code[j][0] == code_str){
          //cout<<"here "<<*code[j][1].c_str()<<endl;
          //if(code[j][0] == "0000000") cout<<endl<<"here "<<code_str<<endl;
          string atr = code[j][1];
          unsigned char temp_ch2 = *atr.c_str();
          count[temp_ch2] -= 1;
          //cout<<endl<<code_str<<" "<<temp_ch2<<endl;
          code_str = "";
          putchar(int(temp_ch2));
          //cout<<endl;
          for(int m = 0; m < 256; m++){
            if(count[m] == 0){
              all = 1;
            }
            else{
              all = 0;
              break;
            }
          }
          break;
        }
      }
      if(all == 1) break;
    }
  }
  else{
    //读入
    int inkinds = 0;
    int count[256];//每个ascii码出现频率
    int exist[256];//存在的ascii码
    for(int i = 0; i < 256; i++){
      exist[i] = 0;
      count[i] = 0;
    }
    char str;
    int asnum;
    string text;
    ////////////////////////
    fseek(stdin, 0, SEEK_END);
    int lSize = ftell(stdin);
    int realSize = lSize * 8;
    rewind(stdin);
    unsigned char ach;
    unsigned char temp_ach;
    while(lSize !=0){
      lSize -= 1;
      fread(&ach,1,1,stdin);
      text += ach;
      if(count[int(ach)] == 0){
        exist[inkinds] = int(ach);
        inkinds += 1;
      }
      count[int(ach)] += 1;
    }
    while(1){//将出现字符排序（低频率在前，高频率在后）
      int if_swap = 0;
      for(int i = 1; i < inkinds; i++){
        if(count[exist[i]] < count[exist [i - 1]]){
          int temp = exist[i - 1];
          exist[i - 1] = exist[i];
          exist[i] = temp;
          if_swap = 1;
        }
        else if(count[exist[i]] == count[exist [i - 1]] && exist[i] < exist[i - 1]){
          int temp = exist[i - 1];
          exist[i - 1] = exist[i];
          exist[i] = temp;
          if_swap = 1;
        }
      }
      if(if_swap == 0) break;
    }
    //得出频率数组和代压缩文本
    int frequency_array[inkinds][2];// [index][0] = ASCII; [index][1] = frequency;
    string code[inkinds][2];//[index][0] = code ; [index][1] = ASCII;
    for(int i = 0; i < inkinds; i++){//遍历赋值
      frequency_array[i][0] = exist[i];
      frequency_array[i][1] = count[exist[i]];
    }
    //建树得出code数组
    tree huffman_tree = build_tree(&frequency_array[0][0], inkinds);
    //遍历求code
    string* code_ptr = get_code(huffman_tree, inkinds);
    for(int i = 0; i < inkinds; i++){
      code[i][0] = *code_ptr;
      code_ptr += 1;
      code[i][1] = *code_ptr;
      code_ptr += 1;
      //unsigned char aaa = *code[i][1].c_str();
      //cout<<code[i][0]<<" "<<int(aaa)<<endl;
    }
    //输出
    cout<<'H'<<'U'<<'F'<<'F'<<'M'<<'A'<<'N'<<'\0';
    for(int i = 0; i < 256; i++){
      fwrite(&count[i],4,1,stdout);
    }
    string out_code = "";
    for(int i = 0; i < text.length(); i++){
      for(int j = 0; j < inkinds; j++){
        if(int(*code[j][1].c_str()) == int(text[i])){
          out_code += code[j][0];
          //cout<<code[j][0]<<endl;
        }
      }
    }
    //cout<<text;
    char a = '1';
    char b = '0';
    string out_str = "";
    for(int i = 0; i < out_code.length() / 8 + 1; i++){
      for(int j = 7; j > -1; j--){
        if(j + i * 8 < out_code.length()){
          if(out_code[j + i * 8] == '1') out_str += a;
          else out_str += b;
        }
        else{
          //cout<<out_str<<endl;
          j = out_code.length() % 8;
          if(j != 0){
            for(int m = 0; m < 8 - j; m++){
              //cout<<out_str<<endl;
              out_str += b;
            }
          }
          for(int m = j - 1; m > -1; m--){
            if(out_code[m + i * 8] == '1') out_str += a;
            else out_str += b;
          }
          //cout<<out_str<<endl;
        break;
        }
      }
    }
    //cout<<out_str<<endl;
    for(int i = 0; i < out_str.length() / 8; i++){
      string temp_str = "";
      for(int j = 0; j < 8; j++){
        temp_str += out_str[j + i * 8];
      }
      unsigned char temp_ch3 = conversion(temp_str);
      cout<<temp_ch3;
    }
  }
  return 0;
}
