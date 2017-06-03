#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;
void change_format_kernel(int month, int day, int year, int mtype);
int hex_converter(string astr);
void generate_year(int year);
string hex_reverse_converter(int anum);
string month_array[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
string st_mon[] = {"Sist", "Spst","Slst", "Sem", "Siais",  "Ihuman"};
string leap_mon[] = {"SIST", "SPST",  "SLST",  "SEM",  "SIAIS",  "IHUMAN"};
string matrix[92][72];
int digit_sum(int num){
	int sum = 0;
  int digit;
	while(num){
		digit = num % 10;
		sum += digit;
		num = (num - digit) / 10;
	}
	return sum;
}
bool if_shleap_year(int year){
	bool if_var = 0;
	if((year + digit_sum(year)) % 13 == 0)if_var = 1;
	return if_var;
}
bool if_gcleap_year(int year){
  bool if_var = 0;
  if(!(year % 4000))if_var = 0;
	if(!(year % 400) && (year % 4000))if_var = 1;
	if(!(year % 100) && (year % 400))if_var = 0;
	if(!(year % 4) && (year % 100))if_var = 1;
	if((year % 4))if_var = 0;
  return if_var;
}
void change_format(string month_str, string day_str, int year){
	int month;
	int day;
	int mtype;//1-sh;2-gc;3-sh_leap;
	for(int i = 0; i < 6; i++){
		if(month_str == st_mon[i]){
			month = i + 1;
			mtype = 1;
		}
		if(month_str == month_array[i]){
			month =  i + 1;
			mtype = 2;
		}
		else if(month_str == month_array[i + 6]){
			month = i + 7;
			mtype = 2;
		}
		if(month_str == leap_mon[i]){
			month = i + 1;
			mtype = 3;
		}
	}
	int size = day_str.length();
	string new_day_str = "";
	for(int index = 0; index < size - 1; index++) new_day_str += day_str[index];
	if(mtype != 2)day = hex_converter(new_day_str);
	else {
		stringstream astream;
		astream << new_day_str;
		astream >> day;
	}
	change_format_kernel(month, day, year, mtype);
}
void change_format_kernel(int month, int day, int year, int mtype){//1-sh;2-gc;3-sh_leap;
	struct date{
		int day;
		int month;
		int year;
	};
	//type 2 finished
	//cout<<"mtype="<<mtype<<endl;
	date day_out = {0, 1, 1};
	int month_count[12] = {31, 28 + if_gcleap_year(1), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int sum_day = 0;
	int sum2_day = 0;
	if(mtype == 1){
		day_out.day = 1;
		///caculate sum_day
		for(int i = 1; i <= (year - 1); i++){
			for(int j = 1; j <= 6; j++){
				sum_day += 60 + (10 * i - j) % 3;
				if(j == (i - digit_sum(i)) % 6 + 1 && if_shleap_year(i)){
					sum_day += 60 + (10 * i - j) % 3;
				}
			}
		}
		for(int i = 1; i <= (month - 1); i++){
			sum_day += 60 + (10 * year - i) % 3;
			if(if_shleap_year(year) && i == (year - digit_sum(year)) % 6 + 1){
				sum_day += 60 + (10 * year - i) % 3;
			}
		}
		sum_day += day;
		///caclate sum_day;
		for(int i = 1; sum2_day < sum_day; i++){
			sum2_day += (365 + if_gcleap_year(i));
			if(sum2_day >= sum_day) day_out.year = i;
		}
		sum2_day -= (365 + if_gcleap_year(day_out.year));
		month_count[1] = 28 + if_gcleap_year(day_out.year);
		for(int i = 1; sum2_day < sum_day; i++){
			sum2_day += month_count[i - 1];
			if(sum2_day >= sum_day) day_out.month = i;
		}
		sum2_day -= month_count[day_out.month - 1];
		day_out.day = sum_day - sum2_day;
		cout<<month_array[day_out.month - 1]<<" "<<day_out.day<<", "<<day_out.year<<endl;
	}
	else if(mtype == 2){
		for(int i = 1; i <= (year - 1); i++) sum_day += 365 +if_gcleap_year(i);
		month_count[1] = 28 + if_gcleap_year(year);
		for(int i = 1; i <= (month - 1); i++) sum_day += month_count[i - 1];
		for(int i = 1; i <= day; i++) sum_day += 1;
		int st_sum_day = 0;
		sum_day -= 1;
		//cout<<sum_day<<endl;
		bool checked = 0;
		while(1){//find the first day in st_calendar
			if(if_shleap_year(day_out.year)){
				int leap_month = (day_out.year - digit_sum(day_out.year)) % 6 + 1;
				for(int i = 1; i <= 6; i++){
					st_sum_day += 60 + (10 * day_out.year - i) % 3;
					if(st_sum_day >= sum_day){
						checked = 1;
						day_out.month = i;
						day_out.day = 1 + (60 + (10 * day_out.year - i) % 3) - (st_sum_day - sum_day);
						break;
					}
					if(i == leap_month){
						st_sum_day += 60 + (10 * day_out.year - i) % 3;
						if(st_sum_day >= sum_day){
							checked = 1;
							day_out.month = 0 - i;
							day_out.day = 1 + (60 + (10 * day_out.year - i) % 3) - (st_sum_day - sum_day);
							break;
						}
					}
				}
			}
			else {
				for(int i = 1; i <= 6; i++){
					st_sum_day += 60 + (10 * day_out.year - i) % 3;
					if(st_sum_day >= sum_day){
						checked = 1;
						day_out.month = i;
						day_out.day = 1 + (60 + (10 * day_out.year - i) % 3) - (st_sum_day - sum_day);
						break;
					}
				}
			}
			if(checked) break;
			day_out.year += 1;
		}
		//cout<<day_out.month<<endl<<day_out.day<<endl<<day_out.year<<endl;
		if(day_out.month > 0) cout<<st_mon[day_out.month - 1]<<" "<<hex_reverse_converter(day_out.day)<<", "<<day_out.year<<endl;
		if(day_out.month < 0) cout<<leap_mon[0 - day_out.month - 1]<<" "<<hex_reverse_converter(day_out.day)<<", "<<day_out.year<<endl;
	}
	else {//mtype == 3
		day_out.day = 1;
		///caculate sum_day
		for(int i = 1; i <= (year - 1); i++){
			for(int j = 1; j <= 6; j++){
				sum_day += 60 + (10 * i - j) % 3;
				if(j == (i - digit_sum(i)) % 6 + 1 && if_shleap_year(i)){
					sum_day += 60 + (10 * i - j) % 3;
				}
			}
		}
		for(int i = 1; i <= (month - 1); i++){
			sum_day += 60 + (10 * year - i) % 3;
			if(if_shleap_year(year) && i == (year - digit_sum(year)) % 6 + 1){
				sum_day += 60 + (10 * year - i) % 3;
			}
		}
		if(month == (year - digit_sum(year)) % 6 + 1) sum_day += 60 + (10 * year - month) % 3;
		sum_day += day;
		///caclate sum_day;
		for(int i = 1; sum2_day < sum_day; i++){
			sum2_day += (365 + if_gcleap_year(i));
			if(sum2_day >= sum_day) day_out.year = i;
		}
		sum2_day -= (365 + if_gcleap_year(day_out.year));
		month_count[1] = 28 + if_gcleap_year(day_out.year);
		for(int i = 1; sum2_day < sum_day; i++){
			sum2_day += month_count[i - 1];
			if(sum2_day >= sum_day) day_out.month = i;
		}
		sum2_day -= month_count[day_out.month - 1];
		day_out.day = sum_day - sum2_day;
		cout<<month_array[day_out.month - 1]<<" "<<day_out.day<<", "<<day_out.year<<endl;
	}
}
void outmonth(int month, int year){
	//cout<<month<<endl<<year<<endl;
	generate_year(year);
	for(int i = 23 * ((month - 1) / 3); i < 23 * ((month - 1) / 3) + 23; i++){
		for(int j = 24 * ((month - 1) % 3); j < 24 * ((month - 1) % 3) + 24; j++){
			cout<<matrix[i][j];
		}
		cout<<endl;
	}
}
int if_break = 0;
void generate_year(int year){// key function
	//cout<<year<<endl;
	int sum_day = 0;//of (year - 1)
	for(int i = 1; i < year; i++){
		sum_day += 365;
		if(if_gcleap_year(i)) sum_day += 1;
	}
	struct a_date{
		int gc_month;
		int gc_day;
		int gc_year;
		int st_month;
		int st_day;
		int st_year;
	};
	a_date current_day = {1, 1, 1, 1, 1, 1};
	int weekday = sum_day % 7 + 1;
	current_day.gc_year = year;
	int st_sum_day = 0;
	bool checked = 0;
	while(1){//find the first day in st_calendar
		if(if_shleap_year(current_day.st_year)){
			int leap_month = (current_day.st_year - digit_sum(current_day.st_year)) % 6 + 1;
			for(int i = 1; i <= 6; i++){
				st_sum_day += 60 + (10 * current_day.st_year - i) % 3;
				if(st_sum_day >= sum_day){
					checked = 1;
					current_day.st_month = i;
					current_day.st_day = 1 + (60 + (10 * current_day.st_year - i) % 3) - (st_sum_day - sum_day);
					break;
				}
				if(i == leap_month){
					st_sum_day += 60 + (10 * current_day.st_year - i) % 3;
					if(st_sum_day >= sum_day){
						checked = 1;
						current_day.st_month = 0 - i;
						current_day.st_day = 1 + (60 + (10 * current_day.st_year - i) % 3) - (st_sum_day - sum_day);
						break;
					}
				}
			}
		}
		else {
			for(int i = 1; i <= 6; i++){
				st_sum_day += 60 + (10 * current_day.st_year - i) % 3;
				if(st_sum_day >= sum_day){
					checked = 1;
					current_day.st_month = i;
					current_day.st_day = 1 + (60 + (10 * current_day.st_year - i) % 3) - (st_sum_day - sum_day);
					break;
				}
			}
		}
		if(checked) break;
		current_day.st_year += 1;
	}
	///////////fill the matrix
	int month_count[12] = {31, 28 + if_gcleap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	for(int line_base = 4; line_base <= 73; line_base += 23){
		for(int column_base = 1; column_base <= 49; column_base += 24){
			int count = 0;
			for(int sline_index = 0; sline_index <= 15; sline_index += 3){
				for(int scolunm_index = 0; scolunm_index <= 19; scolunm_index += 3){
					if(count == 0 && weekday != 7) scolunm_index = weekday * 3;
					int X_index = line_base + sline_index;
					int Y_index = column_base + scolunm_index;
					stringstream astrstream;
					string astr;
					astrstream << current_day.gc_day;
					astrstream >> astr;
					//fill gc
					if(astr.length() == 1) matrix[X_index][Y_index + 1] = astr[0];
					else {
						matrix[X_index][Y_index] = astr[0];
						matrix[X_index][Y_index + 1] = astr[1];
					}
					//fill st
					string astr2;
					astr2 = hex_reverse_converter(current_day.st_day);
					if(astr2.length() == 1) matrix[X_index + 1][Y_index + 1] = astr2[0];
					else {
						matrix[X_index + 1][Y_index] = astr2[0];
						matrix[X_index + 1][Y_index + 1] = astr2[1];
					}
					current_day.gc_day += 1;
					current_day.st_day += 1;
					///process in st calendar
					if((current_day.st_day - 1) == 60 + (10 * current_day.st_year - current_day.st_month) % 3){
						current_day.st_day = 1;
						if(if_shleap_year(current_day.st_year)){
							int leap_month = (current_day.st_year - digit_sum(current_day.st_year)) % 6 + 1;
							if(leap_month != current_day.st_month) current_day.st_month += 1;
						}
						else current_day.st_month += 1;
						if(current_day.st_month == 7){
							current_day.st_month = 1;
							current_day.st_year += 1;
						}
					}
					///process in st calendar
					month_count[current_day.gc_month - 1] -= 1;
					count += 1;
					if(!month_count[current_day.gc_month - 1]){
						current_day.gc_day = 1;
						current_day.gc_month += 1;
						weekday = (weekday + count) % 7;
						if_break = 1;
						break;
					}
				}
				if(if_break){
					if_break = 0;
					break;
				}
			}
		}
	}
	///////////fill the matrix
}
string hex_reverse_converter(int anum){// 10 - 16
	string str = "";
	while(anum){
		switch(anum % 16){
      case 0:str = "0" + str;break;
			case 1:str = "1" + str;break;
			case 2:str = "2" + str;break;
			case 3:str = "3" + str;break;
			case 4:str = "4" + str;break;
			case 5:str = "5" + str;break;
			case 6:str = "6" + str;break;
			case 7:str = "7" + str;break;
			case 8:str = "8" + str;break;
			case 9:str = "9" + str;break;
			case 10:str = "A" + str;break;
			case 11:str = "B" + str;break;
			case 12:str = "C" + str;break;
			case 13:str = "D" + str;break;
			case 14:str = "E" + str;break;
			case 15:str = "F" + str;break;
		}
		anum = anum / 16;
	}
  return str;
}
int hex_converter(string astr){//16 - 10
  int num = 0;
  int mul = 1;
  for(int i = astr.length() - 1; i >= 0; i--){
    switch(astr[i]) {
      case '1':num += mul;break;
      case '2':num += mul * 2;break;
      case '3':num += mul * 3;break;
      case '4':num += mul * 4;break;
      case '5':num += mul * 5;break;
      case '6':num += mul * 6;break;
      case '7':num += mul * 7;break;
      case '8':num += mul * 8;break;
      case '9':num += mul * 9;break;
      case 'A':num += mul * 10;break;
      case 'B':num += mul * 11;break;
      case 'C':num += mul * 12;break;
      case 'D':num += mul * 13;break;
      case 'E':num += mul * 14;break;
      case 'F':num += mul * 15;break;
    }
    mul *= 16;
  }
  return num;
}

int main(int argc, char const *argv[]){
	//////////////////Creat table
  for(int i = 0; i < 92; i++){
    for(int j = 0; j < 72; j++){
      if(i != 22 && i != 45 && i != 68 && i != 91) matrix[i][j] = " ";
    }
  }
  for(int num_month = 0; num_month < 12; num_month++){
    for(int line = 0; line < 4; line++){
      for(int column = 0; column < 3; column++){
        int X1 = line * 23 + 1;
        int Y1 = column * 24;
        int X2 = (line + 1) * 23 - 2;
        int Y2 = (column + 1) * 24 - 3;
        //Print ─
        for(int sub_line = 0; sub_line < 8; sub_line++){
          for(int each_column = 0; each_column < 22; each_column++){
            if(sub_line == 0){
              matrix[X1][Y1 + each_column] = "─";
            }
            else{
              matrix[X1 + sub_line * 3 - 1][Y1 + each_column] = "─";
            }
          }
        }
        //Print │
        for(int sub_column = 0; sub_column < 8; sub_column++){
          for(int each_line = 1; each_line < 20; each_line++){
            matrix[X1 + each_line][Y1 + sub_column * 3] = "│";
          }
        }
        for(int sub_line = 0; sub_line < 8; sub_line++){
          for(int sub_column = 0; sub_column < 8; sub_column++){
            if(sub_line == 0) matrix[X1][Y1 + sub_column * 3] = "┬";
            else if(sub_line == 7) matrix[X1 + sub_line * 3 - 1][Y1 + sub_column * 3] = "┴";
            else if(sub_column == 0) matrix[X1 + sub_line * 3 - 1][Y1 + sub_column * 3] = "├";
            else if(sub_column == 7) matrix[X1 + sub_line * 3 - 1][Y1 + sub_column * 3] = "┤";
            else matrix[X1 + sub_line * 3 - 1][Y1 + sub_column * 3] = "┼";
          }
        }
        matrix[X1][Y1] = "┌";
        matrix[X2][Y1] = "└";
        matrix[X1][Y2] = "┐";
        matrix[X2][Y2] = "┘";
        matrix[X1 + 1][Y1 + 1] = "S";
        matrix[X1 + 1][Y1 + 2] = "u";
        matrix[X1 + 1][Y1 + 4] = "M";
        matrix[X1 + 1][Y1 + 5] = "o";
        matrix[X1 + 1][Y1 + 7] = "T";
        matrix[X1 + 1][Y1 + 8] = "u";
        matrix[X1 + 1][Y1 + 10] = "W";
        matrix[X1 + 1][Y1 + 11] = "e";
        matrix[X1 + 1][Y1 + 13] = "T";
        matrix[X1 + 1][Y1 + 14] = "h";
        matrix[X1 + 1][Y1 + 16] = "F";
        matrix[X1 + 1][Y1 + 17] = "r";
        matrix[X1 + 1][Y1 + 19] = "S";
        matrix[X1 + 1][Y1 + 20] = "a";
      }
    }
  }
	string full_month_array[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	int mon_count = 0;
	for(int line_index = 0; line_index < 70; line_index += 23){
		for(int colunm_index = 0; colunm_index < 49; colunm_index += 24){
			for(int i = 0; i < full_month_array[mon_count].length(); i++){
				matrix[line_index][colunm_index + i] = full_month_array[mon_count][i];
			}
			mon_count += 1;
		}
	}
  //////////////////Creat table
	int in_argument[2];//month year
	stringstream year_stream;
	stringstream astream;
	switch (argc) {
		case 2:
			year_stream << argv[1];
			year_stream >> in_argument[1];
			generate_year(in_argument[1]);
			cout<<in_argument[1]<<endl;
			for(int i = 0; i < 92; i++){
				for(int j = 0; j < 72; j++){
					cout<<matrix[i][j];
				}
				cout<<endl;
			}
			break;
		case 3:
			year_stream << argv[1];
			year_stream >> in_argument[0];
			astream << argv[2];
			astream >> in_argument[1];
			cout<<argv[2]<<endl;
			outmonth(in_argument[0],in_argument[1]);
			break;
		case 4:
			year_stream << argv[3];
			year_stream >> in_argument[1];
			change_format(argv[1],argv[2],in_argument[1]);
			break;
	}
	return 0;
}
