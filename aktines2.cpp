#include <bits/stdc++.h> // all C/C++ libs
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>
#include <set>
using namespace std;

char find_first_char_up(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, int& new_index_j, set<pair<int, int>>& balls, char init_symbol);
char find_first_char_down(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, int& new_index_j, set<pair<int, int>>& balls, char init_symbol);
char find_first_char_left(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, int& new_index_j, set<pair<int, int>>& balls, char init_symbol);
char find_first_char_right(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, int& new_index_j, set<pair<int, int>>& balls, char init_symbol);

const int MAX = 100000;

// data structures used to implement Kosaraju's
// Algorithm. Please refer
// https://www.geeksforgeeks.org/strongly-connected-components/
vector<int> adj[MAX];
vector<int> adjInv[MAX];
bool visited[MAX];
bool visitedInv[MAX];
stack<int> s;

// this array will store the SCC that the
// particular node belongs to
int scc[MAX];

// counter maintains the number of the SCC
int counter = 1;

// adds edges to form the original graph
void addEdges(int a, int b)
{
    adj[a].push_back(b);
}

// add edges to form the inverse graph
void addEdgesInverse(int a, int b)
{
    adjInv[b].push_back(a);
}

// for STEP 1 of Kosaraju's Algorithm
void dfsFirst(int u)
{
    if(visited[u])
        return;

    visited[u] = 1;

    for (int i=0;i<adj[u].size();i++)
        dfsFirst(adj[u][i]);

    s.push(u);
}

// for STEP 2 of Kosaraju's Algorithm
void dfsSecond(int u)
{
    if(visitedInv[u])
        return;

    visitedInv[u] = 1;

    for (int i=0;i<adjInv[u].size();i++)
        dfsSecond(adjInv[u][i]);

    scc[u] = counter;
}

// function to check 2-Satisfiability
bool is2Satisfiable(int n, int m, int a[], int b[])
{
    // adding edges to the graph
    for(int i=0;i<m;i++)
    {
        // variable x is mapped to x
        // variable -x is mapped to n+x = n-(-x)

        // for a[i] or b[i], addEdges -a[i] -> b[i]
        // AND -b[i] -> a[i]
        if (a[i]>0 && b[i]>0)
        {
            addEdges(a[i]+n, b[i]);
            addEdgesInverse(a[i]+n, b[i]);
            addEdges(b[i]+n, a[i]);
            addEdgesInverse(b[i]+n, a[i]);
        }

        else if (a[i]>0 && b[i]<0)
        {
            addEdges(a[i]+n, n-b[i]);
            addEdgesInverse(a[i]+n, n-b[i]);
            addEdges(-b[i], a[i]);
            addEdgesInverse(-b[i], a[i]);
        }

        else if (a[i]<0 && b[i]>0)
        {
            addEdges(-a[i], b[i]);
            addEdgesInverse(-a[i], b[i]);
            addEdges(b[i]+n, n-a[i]);
            addEdgesInverse(b[i]+n, n-a[i]);
        }

        else
        {
            addEdges(-a[i], n-b[i]);
            addEdgesInverse(-a[i], n-b[i]);
            addEdges(-b[i], n-a[i]);
            addEdgesInverse(-b[i], n-a[i]);
        }
    }

    // STEP 1 of Kosaraju's Algorithm which
    // traverses the original graph
    for (int i=1;i<=2*n;i++)
        if (!visited[i])
            dfsFirst(i);

    // STEP 2 of Kosaraju's Algorithm which
    // traverses the inverse graph. After this,
    // array scc[] stores the corresponding value
    while (!s.empty())
    {
        int n = s.top();
        s.pop();

        if (!visitedInv[n])
        {
            dfsSecond(n);
            counter++;
        }
    }

    for (int i=1;i<=n;i++)
    {
        // for any 2 variable x and -x lie in
        // same SCC
        if(scc[i]==scc[i+n])
        {
            /*cout << "The given expression "
                 "is unsatisfiable." << endl;*/
            return false;
        }
    }

    // no such variables x and -x exist which lie
    // in same SCC
    /*cout << "The given expression is satisfiable."
         << endl;*/
    return true;
}

void resetGlobals(int n) {
    for (int i = 0; i <= 2*n; i++) {
        adj[i].clear();
        adjInv[i].clear();
        visited[i] = false;
        visitedInv[i] = false;
        scc[i] = 0;
    }
    while (!s.empty()) s.pop();
    counter = 1;
}

bool isInteger(const string& str) {
    if (str.empty()) return false;
    int i = 0;

    //Check for a negative sign
    if (str[0] == '-' && str.size() > 1) i = 1;

    //Ensure all characters are digits
    for (; i < str.size(); ++i) {
        if (!isdigit(str[i])) return false;
    }

    return true;
}


char find_first_char_left(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, 
                            int& new_index_j, set<pair<int, int>>& balls, char init_symbol){
    j--;
    //cout<<"j: "<<j<<endl;
    //x means that the lazer went out of array
    char symbol = 'x';
    while(j >= 0){
        if(matrix[i][j] == '.') {
            balls.insert({i, j});
            j--;
            continue;
        }
        else {
            symbol = matrix[i][j];
            //Return the index of conflict between "-" and "|"
            if(symbol == '|' || symbol == '-') {
                new_index_i = i;
                new_index_j = j;
                collision = true;
                return symbol;
                //cout<<"kill L: "<<endl;
            }
            if(symbol == '\\') {
                
                symbol = find_first_char_up(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls, init_symbol);
            }
            else if(symbol == '/') {
                symbol = find_first_char_down(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls, init_symbol);
            }
            return symbol;
        }
    }
    return symbol;
}

char find_first_char_right(char **matrix, int R, int C, int i, int j, bool& collision, 
            int& new_index_i, int& new_index_j, set<pair<int, int>>& balls, char init_symbol){
    j++;
    //x means that the lazer went out of array
    char symbol = 'x';
    while(j < C){
        if(matrix[i][j] == '.') {
            balls.insert({i, j});
            j++;
            continue;
        }
        else {
            symbol = matrix[i][j];
            if(symbol == '|' || symbol == '-'){
                new_index_i = i;
                new_index_j = j;
                collision = true;
                return symbol;
                //cout<<"kill R: "<<endl;
            } 
            if(symbol == '\\') {
                symbol = find_first_char_down(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls, init_symbol);
            }
            else if(symbol == '/') {
                symbol = find_first_char_up(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls, init_symbol);
            }
            return symbol;
            
        }
    }
    return symbol;
}

char find_first_char_up(char **matrix, int R, int C, int i, int j, bool& collision, 
                    int& new_index_i, int& new_index_j, set<pair<int, int>>& balls, char init_symbol){
    i--;
    //x means that the lazer went out of array
    char symbol = 'x';
    while(i >= 0){
        if(matrix[i][j] == '.') {
            balls.insert({i, j});
            i--;
            continue;
        }
        else {
            symbol = matrix[i][j];
            if(symbol == '|' || symbol == '-'){
                new_index_i = i;
                new_index_j = j;
                collision = true;
                return symbol;
                //cout<<"kill U: "<<endl;
            } 
            if(symbol == '\\') {
                symbol = find_first_char_left(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls, init_symbol);
            }
            else if(symbol == '/') {
                symbol = find_first_char_right(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls, init_symbol);
            }
            return symbol;
            
        }
    }
    return symbol;
}

char find_first_char_down(char **matrix, int R, int C, int i, int j, bool& collision, 
            int& new_index_i, int& new_index_j, set<pair<int, int>>& balls, char init_symbol){
    i++;
    //x means that the lazer went out of array
    char symbol = 'x';
    while(i < R){
        if(matrix[i][j] == '.') {
            balls.insert({i, j});
            i++;
            continue;
        }
        else {
            symbol = matrix[i][j];
            if(symbol == '|' || symbol == '-'){
                new_index_i = i;
                new_index_j = j;
                collision = true;
                return symbol;
                //cout<<"kill D: "<<endl;
            }  
            if(symbol == '\\') {
                symbol = find_first_char_right(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls, init_symbol);
            }
            else if(symbol == '/') {
                symbol = find_first_char_left(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls, init_symbol);
            }
            return symbol;
        }
    }
    return symbol;
}

void change_boolean(map<pair<int, int>, vector<tuple<int, int, bool>>>& closures, pair<int, int> guns_i){
    for (auto& entry : closures) {
        //Extract values (vector of tuples) and update boolean
        for (auto& t : entry.second) {
            int i, j;
            bool& b = get<2>(t); // Directly reference the bool value
            tie(i, j, b) = t;
            pair<int, int> check = {i,j};
            if(check == guns_i) b = !b;  //Toggle the boolean value
        }
    }
}

void print_map(map<pair<int, int>, vector<tuple<int, int, bool>>>& closures){

    cout<<"Closures:"<<endl;
    for (const auto& entry : closures) {
        //Extract key
        pair<int, int> key = entry.first;
        cout<<"Ball: ("<< key.first<<", "<<key.second<<") -> Pistols: ";

        //Extract values (vector of tuples)
        for (const auto& t : entry.second) {
            int x, y;
            bool b;
            tie(x, y, b) = t; //Unpack tuple
            cout<<"("<<x<<", "<<y<<", "<<(b ? "true" : "false") << ") ";
        }
        cout<<endl;
    }
}



bool check_matrix(char **matrix, int R, int C){
    //cout<<"Array size: "<<R<<"*"<<C<<endl;
    bool collision = false;
    char temp_sybol_a, temp_sybol_b;
    int new_index_i = -1, new_index_j = -1;
    set<pair<int, int>> balls;
    map<pair<int, int>, set<pair<int, int>>> pistol_hit_balls;
    set<pair<int, int>> no_flip;
       
    //Find no flip pistols (constraint), find collisions between pistols and change the direction of pistols if necessary 
    for (int i = 0; i < R; i++){
        //cout<<endl;
        for (int j = 0; j < C; j++) {
            //cout<<" "<<matrix[i][j];
            if(matrix[i][j] == '|'){
                collision = false;
                temp_sybol_a = find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);                
                //Change direction in '|'
                if(collision) {
                    matrix[i][j] = '-';
                    no_flip.insert({i,j});
                    //cout<<"conflict: "<<i<<","<<j<<"   "<<" => "<<new_index_i<<","<<new_index_j<<endl;
                }
                //if(collision) cout<<"{"<<temp_sybol_a<<"}";
                //cout<<"conflict: ("<<i<<", "<<j<<")"<<" and ("<<new_index_i<<", "<<new_index_j<<")"<<endl;
                if(!collision){
                    temp_sybol_b = find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                    //Change direction in '|'
                    if(collision) {
                        matrix[i][j] = '-';
                        no_flip.insert({i,j});
                        //cout<<"conflict: "<<i<<","<<j<<"   "<<" => "<<new_index_i<<","<<new_index_j<<endl;
                    }

                }
            }
            else if(matrix[i][j] == '-'){
                collision = false;
                temp_sybol_a = find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                //Change direction in '-'
                if(collision) {
                    matrix[i][j] = '|';
                    no_flip.insert({i,j});
                    //cout<<"conflict: "<<i<<","<<j<<"   "<<" => "<<new_index_i<<","<<new_index_j<<endl;
                }
                
                if(!collision){
                    //Change direction in '-'
                    temp_sybol_b = find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                    if(collision) {
                        matrix[i][j] = '|';
                        no_flip.insert({i,j});
                        //cout<<"conflict: "<<i<<","<<j<<"   "<<" => "<<new_index_i<<","<<new_index_j<<endl;
                    }
                }

            }
        }
    }
    //cout<<endl;
    //"Flip" all the guns and check for collisions again. Now dont change the direction of guns
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++) {
            if(matrix[i][j] == '-'){
                collision = false;
                temp_sybol_a = find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) {
                    no_flip.insert({i,j});
                    //cout<<"conflict: "<<i<<","<<j<<"   "<<" => "<<new_index_i<<","<<new_index_j<<endl;
                    collision = false;
                }
                temp_sybol_b = find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) {
                    no_flip.insert({i,j});
                    //cout<<"conflict: "<<i<<","<<j<<"   "<<" => "<<new_index_i<<","<<new_index_j<<endl;
                    collision = false;
                }
              
                balls.clear();
            }
            else if(matrix[i][j] == '|'){
                collision = false;
                temp_sybol_a = find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) {
                    no_flip.insert({i,j});
                    //cout<<"conflict: "<<i<<","<<j<<"   "<<" => "<<new_index_i<<","<<new_index_j<<endl;
                    collision = false;
                }
                
                temp_sybol_b = find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) {
                    no_flip.insert({i,j});
                    //cout<<"conflict: "<<i<<","<<j<<"   "<<" => "<<new_index_i<<","<<new_index_j<<endl;
                    collision = false;
                }
                balls.clear();
            }
        }
    }

    /*cout<<endl<<"No flip: "<<endl;
    for (const auto& element : no_flip) {
        cout << "(" <<element.first << ", " <<element.second << ")"<<endl;
    }*/

    balls.clear();
    //Check for collisions between pistols and insert into pistol_hit_balls the key and the balls
    collision = false;
    for (int i = 0; i < R; i++){
        if(collision) break;
        for (int j = 0; j < C; j++) {
            //cout<<" "<<matrix[i][j];
            if(matrix[i][j] == '|'){
                temp_sybol_a = find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                if(collision) {
                    //cout<<"{"<<matrix[i][j]<<"} => {"<<matrix[new_index_i][new_index_j]<<"}"<<endl;
                    break;
                }
                temp_sybol_b = find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) {
                    //cout<<"{"<<matrix[i][j]<<"} => {"<<matrix[new_index_i][new_index_j]<<"}"<<endl;
                    break;
                }
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                balls.clear();
            }
            else if(matrix[i][j] == '-'){
                temp_sybol_a = find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                if(collision) {
                    //cout<<"{"<<matrix[i][j]<<"} => {"<<matrix[new_index_i][new_index_j]<<"}"<<endl; 
                    break;
                }
                temp_sybol_b = find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) {
                    //cout<<"{"<<matrix[i][j]<<"} => {"<<matrix[new_index_i][new_index_j]<<"}"<<endl;
                    break;
                }
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                balls.clear();
            }
        }
    }
    //cout<<"Collision?: "<<collision<<endl;
    if(collision) return false;

    /*cout<<endl<<"Map, pistol => ball: "<<endl;
    for (const auto& entry : pistol_hit_balls) {
        cout << "Key: (" << entry.first.first << ", " << entry.first.second << ") -> Values: ";
        for (const auto& value : entry.second) {
            cout << "(" << value.first << ", " << value.second << ") ";
        }
        cout << "\n";
    }*/

    //cout<<endl;
    char up, down, left, right;
    balls.clear();

    //Find the pistols that can hit specific dots, 
    //use vector to find dots that can be hit 2 times by the same weapon (with flip gun '-' => '|')
    map<pair<int, int>, vector<pair<int, int>>> balls_find_pistols;
    vector<pair<int, int>> no_closure_dot;
    int count = 0;
    for (int i = 0; i < R; i++){
        //if(collision) break;
        //cout<<endl;
        for (int j = 0; j < C; j++) {
            //cout<<" "<<matrix[i][j];
            if(matrix[i][j] == '.'){
                collision = false;
                up = find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                down = find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                left = find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                right = find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls, matrix[i][j]);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                balls.clear();
                //An ena dot to xtipaei ena pistoli kai orizontia kai katheta, periptosi pou iparxoun pano apo 2 elements
                if(balls_find_pistols[{i, j}].size() >= 2){
                    
                    for (const auto& entry : balls_find_pistols[{i,j}]){
                        
                        count = std::count(balls_find_pistols[{i,j}].begin(), balls_find_pistols[{i,j}].end(), entry);
                        //Des an afto to pistoli epitrepetai na kanei flip kai istera vale to dot ston vector pou 
                        //den tha to simperilavoume os closure
                        
                        if(count > 1) {
                            //An iparxei to entry ston no_flip vector
                            if (no_flip.find(entry) != no_flip.end()){
                                //Diegrapse ta dots ta opoia xtipiountai apo ena pistoli katheta kai orizontia
                                auto it = std::find(no_closure_dot.begin(), no_closure_dot.end(), entry);
                                if(it == no_closure_dot.end()){
                                    no_closure_dot.push_back({i,j});
                                }
                                balls_find_pistols.erase({i,j});
                            }
                            //break;
                        }
                    }
                }
            }
        }
    }

    //If a dot cannot reach pistol (empty vector dot => pistol) return false
    for (const auto& entry : balls_find_pistols) {
        if(entry.second.empty()) return false;
    }
    //cout<<endl<<"Map, ball => pistol: "<<endl;
    map<pair<int, int>, vector<tuple<int, int, bool>>> closures;
    for (const auto& entry : balls_find_pistols) {
        //cout << "Ball: (" << entry.first.first << ", " << entry.first.second << ") -> Pistols: ";
        for (const auto& value : entry.second) {

            //cout << "(" << value.first << ", " << value.second << ") ";
            // Check if entry.first is in pistol_hit_balls[value]
            if (pistol_hit_balls[value].find(entry.first) != pistol_hit_balls[value].end()) {
                //cout<<endl<<"("<<entry.first.first<<", "<<entry.first.second<<") hited from "<<"("<<value.first<< ", " <<
                       //value.second<< ")"<<endl;
                   
                closures[{entry.first.first, entry.first.second}].push_back({value.first, value.second, true});
                //cout<<endl<<"YES("<<value.first<< ", " <<value.second<< ")"<<endl;
            }
            else{
                //cout<<endl<<"("<<entry.first.first<<", "<<entry.first.second<<") DIDNT hited from "<<"("<<value.first
                        //<<", "<<value.second<< ")"<<endl;
                if(no_flip.find(value) == no_flip.end()){
                    closures[{entry.first.first, entry.first.second}].push_back({value.first, value.second, false});
                    //cout<<endl<<"NO("<<value.first<< ", " <<value.second<< ")"<<endl;
                }
            }
        }
        //cout<<endl;
    }

    //print_map(closures);
    //If we dont have clauses, means the solution is IMPOSSIBLE
    if(closures.empty()) return false;
    vector<pair<int, int>> unique_pistols;
    pair<int, int> pair_ij;
    int x, y;
    bool boolean_for;
    //Closures
    for (const auto& entry : closures) {
        //Extract values (vector of tuples)
        pair<int, int> key = entry.first;
        //cout<<"My Ball: ("<< key.first<<", "<<key.second<<") -> Pistols: ";
        for (const auto& t : entry.second) {
            tie(x, y, boolean_for) = t; //Unpack tuple
            pair_ij = make_pair(x, y);
            //cout<<"("<<x<<", "<<y<<", "<<(boolean_for ? "true" : "false") << ") "<<endl;
            //cout<<"Whaaat"<<endl;
            //If we have variable no fliping and true, remove all the closure because will be true
            //cout<<"bool "<<boolean_for<<endl;
            if(find(no_flip.begin(), no_flip.end(), pair_ij) != no_flip.end() && boolean_for){
                //closures.erase(entry.first);
                auto it = std::find(no_closure_dot.begin(), no_closure_dot.end(), entry.first);
                if(it == no_closure_dot.end()){
                    no_closure_dot.push_back(entry.first);
                }
                
                //cout<<endl<<"Closure: "<<entry.first.first<<"   "<<entry.first.second<<" REMOVED"<<endl;
                
                //If it was the last closure, we are done!
                if(closures.size() == 0){
                    //cout<<"special case!"<<endl;
                    return true;
                }
            }
            
            if (find(unique_pistols.begin(), unique_pistols.end(), pair_ij) == unique_pistols.end()) {
                unique_pistols.push_back(pair_ij); // Insert if not found
            }
        }
    }

    for (const auto& entry : closures) {
        const pair<int, int>& key = entry.first;  // Current map key
        const vector<tuple<int, int, bool>>& vec = entry.second;  // Current vector
    
        for (const auto& t : vec) {
            int count = std::count(vec.begin(), vec.end(), t);  // Count occurrences of 't' in vec
            //cout<<"count: "<<count<<endl;
            if (count > 1) {  // If the element appears more than once
                // If the key exists in the no_flip set
                if (no_flip.find(key) == no_flip.end()) {
                    // Check if key is already in no_closure_dot
                    if (std::find(no_closure_dot.begin(), no_closure_dot.end(), key) == no_closure_dot.end()) {
                        no_closure_dot.push_back(key);  // Add to no_closure_dot
                    }
                }
            }
        }
    }
    

    
    //cout<<"No closure cases:"<<endl;
    for (const auto& entry : no_closure_dot){
        closures.erase({entry.first, entry.second});
        //cout<<"("<<entry.first<<","<<entry.second<<")"<<endl;
    }
    //print_map(closures);
    //map<pair<int, int>, vector<tuple<int, int, bool>>> closures_copy = closures;
    //Copy all elements from the set into the vector
    vector<pair<int, int>> flexible_vars;
    for (const auto& pair : unique_pistols) {
        if (no_flip.find(pair) == no_flip.end()) {  // If pair is not in no_flip
            flexible_vars.push_back(pair);
        }
    }
    //print_map(closures);
    /*cout<<endl;
    cout<<"Num of variables: unique_pistols: "<<unique_pistols.size()<<endl;
    for (const auto& pair : unique_pistols) {
        cout<<"("<<pair.first<<","<<pair.second<<")  ";
    }
    cout<<endl;*/

    /*cout<<"Num of variables: flexible_vars: "<<flexible_vars.size()<<endl;
    for (const auto& pair : flexible_vars) {
        cout<<"("<<pair.first<<","<<pair.second<<")  ";
    }
    cout<<endl;*/

    //cout<<endl;
    int a[closures.size()];
    int b[closures.size()];
    
    
    int i = 0;
    for (const auto& entry : closures) {
     
        const auto& vec = entry.second;
        if (vec.size() == 1) {
            int x, y;
            bool boolean;
            tie(x, y, boolean) = vec[0]; //Unpack tuple
            pair<int,int> temp_xy = {x,y};

            auto it = find(flexible_vars.begin(), flexible_vars.end(), temp_xy);

            if (it != flexible_vars.end()) {
                int index = distance(flexible_vars.begin(), it) + 1; // Zero-based index
                //cout<<" index "<<index<<endl;
                //cout << "1Index of (" << x << ", " << y << ") is: " << index << endl;
                if(boolean) {
                    a[i] = index;
                    b[i] = index;
                }
                else {
                    if (find(no_flip.begin(), no_flip.end(), temp_xy) == no_flip.end()) {
                        a[i] = -index;
                        b[i] = -index;
                    }
                    //Case with 1 variable false and we cannot flip it
                    else return false;
                }
                i++;
            } 
            else {
                cout << "Pair not found." << endl;
            }
        }
        else if (vec.size() == 2) {
            // Case: Two elements in the vector
            int x1, y1, x2, y2;
            bool b1, b2;
            tie(x1, y1, b1) = vec[0];
            tie(x2, y2, b2) = vec[1];
            pair<int,int> temp_xy1 = {x1,y1};
            pair<int,int> temp_xy2 = {x2,y2};
            auto it1 = find(flexible_vars.begin(), flexible_vars.end(), temp_xy1);
            auto it2 = find(flexible_vars.begin(), flexible_vars.end(), temp_xy2);
            if (it1 != flexible_vars.end() && it2 != flexible_vars.end()) {
                int index1 = distance(flexible_vars.begin(), it1) + 1; // Zero-based index
                int index2 = distance(flexible_vars.begin(), it2) + 1; // Zero-based index
                //cout << "2Index of (" << x1 << ", " << y1 << ") is: " << index1 << endl;
                //cout << "2Index of (" << x2 << ", " << y2 << ") is: " << index2 << endl;
                if(b1) a[i] = index1;
                else a[i] = -index1;
        
                if(b2) b[i] = index2;
                else b[i] = -index2;
                i++;
            }
            else  cout << "Pair not found. 22" << endl;
        }
    }
    int num_of_variables = flexible_vars.size();
    resetGlobals(num_of_variables); //Clean everything from 2 sat algorithm

    int num_of_clauses = closures.size();
    /*cout << "Index | a[i]  | b[i]" << endl;
    cout << "----------------------" << endl;
    for (int i = 0; i < num_of_clauses; ++i) {
        cout << i << "     | " << a[i] << "   | " << b[i] << endl;
    }
    
    cout<<"closures.size() "<<num_of_clauses<<endl;
    cout<<"num_of_variables "<<num_of_variables<<endl;*/
    bool final_solution = is2Satisfiable(num_of_variables, num_of_clauses, a, b);
   

    return final_solution;
}


void cleanup_matrix(char **&matrix, int R) {
    if (matrix) {
        for (int i = 0; i < R; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        matrix = nullptr;
    }
}


int main() {
	ifstream file("ts1_input.txt"); // Open the file
    if (!file.is_open()) {
        cerr<<"Error: Could not open the file."<<endl;
        return 1;
    }
    int R = 0, C = 0; // Variables to hold the three numbers
    int T = 100;
    file >> T;
    cout<<"T: "<<T<<endl;
    char **matrix;
    string line;
    int count_ints = 0;
    int index_i = 0, index_j = 0;
    int cases_counter = 1;
    while (getline(file, line)) { // Read line by line
        istringstream iss(line);
        string word;

        while (iss >> word) { // Split line into words
            if (isInteger(word)) {
                count_ints++;
                if(count_ints == 1) R = stoi(word);
                if(count_ints == 2) {
                    C = stoi(word);
                    count_ints = 0;
                    matrix = new char*[R];
                    for(int i = 0; i < R ; i++) matrix[i] = new char[C];
                }
            } 
            else {
                for (int i = 0; i < word.size(); ++i) {
                    matrix[index_j][i] = word[i];
                }
                index_j++;
                if(index_j >= R){
                    bool agree = check_matrix(matrix, R,C);
                    if(agree) cout<<"Case #"<<cases_counter<<": POSSIBLE"<<endl;
                    else cout<<"Case #"<<cases_counter<<": IMPOSSIBLE"<<endl;
                    cases_counter++;
                    index_j = 0;
                    cleanup_matrix(matrix, R);
                    T--;
                    if(T > 0) cout<<endl;
                }
            }
        }
    }
	return 0;
}