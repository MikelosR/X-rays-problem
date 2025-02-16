#ifndef FIND_FIRST_CHAR
#define FIND_FIRST_CHAR

#include "libraries.h"
using namespace std;

char find_first_char_up(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, int& new_index_j, set<pair<int, int>>& balls);
char find_first_char_down(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, int& new_index_j, set<pair<int, int>>& balls);
char find_first_char_left(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, int& new_index_j, set<pair<int, int>>& balls);
char find_first_char_right(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, int& new_index_j, set<pair<int, int>>& balls);
void revised_pistol_hit_balls_func(char **matrix, int R, int C, map<pair<int, int>, set<pair<int, int>>>& revised_pistol_hit_balls);
void print_map(map<pair<int, int>, vector<tuple<int, int, bool>>>& clauses);
void print_balls_find_pistols(const map<pair<int, int>, vector<pair<int, int>>>& balls_find_pistols);
void print_noflip(const set<pair<int, int>>& no_flip);
void print_pistol_hit_balls(const map<pair<int, int>, set<pair<int, int>>>& pistol_hit_balls);
void print_no_clauses_for_this_dot(const vector<pair<int, int>>& no_clauses_for_this_dot);

#endif 