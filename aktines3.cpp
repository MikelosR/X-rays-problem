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
void is2Satisfiable(int n, int m, int a[], int b[])
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
            cout << "The given expression "
                 "is unsatisfiable." << endl;
            return;
        }
    }

    // no such variables x and -x exist which lie
    // in same SCC
    cout << "The given expression is satisfiable."
         << endl;
    return;
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

//Vertical = False
//horizontal  = True
struct PistolInfo {
    bool state;                   // Boolean state (true/false)
    set<pair<int, int>> balls;    // Set of balls where this pistol appears
};

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

bool solver(map<pair<int, int>, vector<tuple<int, int, bool>>>& closures){
    bool true_closure = false;
    for (const auto& entry : closures) {
        true_closure = false;
        for (const auto& t : entry.second) {
            int x, y;
            bool b;
            tie(x, y, b) = t; //Unpack tuple
            if(b) true_closure = b;
        }
        if(!true_closure) return false;
    }
    return true;
}


bool make_clauses_solver(char **matrix, int R, int C){
    cout<<"Array size: "<<R<<"*"<<C<<endl;
    bool collision = false;
    char temp_sybol_a, temp_sybol_b;
    int new_index_i = -1, new_index_j = -1;
    set<pair<int, int>> balls;
    map<pair<int, int>, set<pair<int, int>>> pistol_hit_balls;
    set<pair<int, int>> no_flip;
       
    //Find no flip pistols (constraint), find collisions between pistols and change the direction of pistols if necessary
    //We have collision when a pistol ('|' or '-') have direction that it can hit other pistol.
    //If find colision, turn the pistol
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++) {
            if(matrix[i][j] == '|'){
                collision = false;
                temp_sybol_a = find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);                
                //Change direction in '|'
                if(collision) {
                    matrix[i][j] = '-';
                    no_flip.insert({i,j});
                    
                }
                if(!collision){
                    temp_sybol_b = find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                    //Change direction in '|'
                    if(collision) {
                        matrix[i][j] = '-';
                        no_flip.insert({i,j});
                    }
                }
            }
            else if(matrix[i][j] == '-'){
                collision = false;
                temp_sybol_a = find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                //Change direction in '-'
                if(collision) {
                    matrix[i][j] = '|';
                    no_flip.insert({i,j});
                }
                if(!collision){
                    //Change direction in '-'
                    temp_sybol_b = find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                    if(collision) {
                        matrix[i][j] = '|';
                        no_flip.insert({i,j});
                    }
                }
            }
        }
    }
    //If find colision here means dont turn the pistol, so store the pistol into set no_flip
    //Fake "Flip" all the guns and check for collisions again. Now dont change the direction of guns
    //Use this for loop to find all the pistols we shouldn't overturn.
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++) {
            if(matrix[i][j] == '-'){
                collision = false;
                temp_sybol_a = find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    no_flip.insert({i,j});
                    collision = false;
                }
                temp_sybol_b = find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    no_flip.insert({i,j});
                    collision = false;
                }
                balls.clear();
            }
            else if(matrix[i][j] == '|'){
                collision = false;
                temp_sybol_a = find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    no_flip.insert({i,j});
                    collision = false;
                }
                temp_sybol_b = find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    no_flip.insert({i,j});
                    collision = false;
                }
                balls.clear();
            }
        }
    }
    
    balls.clear();
    //Check for collisions between pistols and insert into pistol_hit_balls the key and the balls
    //If we have and here collisions (after for the first for loop set) means there is no solution
    //As we have the pistols in this direction, fill the pistol_hit_balls map
    collision = false;
    for (int i = 0; i < R; i++){
        if(collision) break;
        for (int j = 0; j < C; j++) {
            if(matrix[i][j] == '|'){
                temp_sybol_a = find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                if(collision) {
                    break;
                }
                temp_sybol_b = find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    break;
                }
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                balls.clear();
            }
            else if(matrix[i][j] == '-'){
                temp_sybol_a = find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                if(collision) {
                    break;
                }
                temp_sybol_b = find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    break;
                }
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                balls.clear();
            }
        }
    }
    if(pistol_hit_balls.empty()) return false;
    //print_pistol_hit_balls(pistol_hit_balls);
    //print_noflip(no_flip);
    if(collision) return false;
    char up, down, left, right;
    balls.clear();

    //Find the pistols that can hit specific dots, 
    //use vector to find dots that can be hited 2 times by the same weapon (with flip gun '-' => '|')
    map<pair<int, int>, vector<pair<int, int>>> balls_find_pistols;
    vector<pair<int, int>> no_clauses_for_this_dot;
    int count = 0;
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++) {
            if(matrix[i][j] == '.'){
                collision = false;
                up = find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                down = find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                left = find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                right = find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                //if(i == 1 && j == 5){
                    cout << "Set elements: "<<i<<" , "<<j<<endl;
                    for (const auto& p : balls) {
                        cout << "(" << p.first << ", " << p.second << ") ";
                    }
                    cout<<endl;
                //}
                balls.clear();
                
                if(balls_find_pistols[{i, j}].size() == 0){
                    //No variable for this clause (dot)
                    return false;
                }
                /*if(balls_find_pistols[{i, j}].size() > 2){
                    auto it = balls_find_pistols.find({i, j});
                    cout<<"Ball { "<<i<<","<<j<<"} ";
                    for (const auto& p : it->second) {
                        cout << "(" << p.first << ", " << p.second << ") ";
                    }
                    cout<<endl;
                }*/
            }
        }
    }
    //print_balls_find_pistols(balls_find_pistols);
    //If none dot can "hit" a pistol, solution is IMPOSSIBLE
    if(balls_find_pistols.empty()) return false;
    //print_balls_find_pistols(balls_find_pistols);
    
    //If a dot cannot reach pistol (empty vector dot => pistol) return false
    for (const auto& entry : balls_find_pistols) {
        if(entry.second.empty()) return false;
    }

    //If a pistol hit a dot with this direction, set the clause true, otherwise false
    map<pair<int, int>, vector<tuple<int, int, bool>>> clauses;
   
    for (const auto& entry : balls_find_pistols) {
        for (const auto& value : entry.second) {
            //Check if entry.first is in pistol_hit_balls[value], then means this dot hited from this gun with this direction
            if (pistol_hit_balls[value].find(entry.first) != pistol_hit_balls[value].end()) {
                clauses[{entry.first.first, entry.first.second}].push_back({value.first, value.second, true});
            }
            else{
                clauses[{entry.first.first, entry.first.second}].push_back({value.first, value.second, false});
            }
        }
    }
    
    map<pair<int, int>, vector<tuple<int, int, bool>>> copy_clauses = clauses;
    //Remove clauses with true non flipped variables
    for (const auto& entry : copy_clauses) {
        for (const auto& tpls : entry.second) {
            int x, y;
            bool boolean;
            tie(x, y, boolean) = tpls; //Unpack tuple
            pair<int,int> temp_xy = {x,y};
            if(boolean) {
                //True and no flip case
                if(find(no_flip.begin(), no_flip.end(), temp_xy) != no_flip.end()){
                    clauses.erase(entry.first);
                    //If it was the last clause, we are done!
                    if(clauses.size() == 0) return true;
                }
            }
        }
    }
    
    copy_clauses = clauses;
    //Remove clauses with false non flipped variables. Remove variables or whole clauses
    for (const auto& entry : copy_clauses) {
        for (const auto& tpls : entry.second) {
            int x, y;
            bool boolean;
            tie(x, y, boolean) = tpls; //Unpack tuple
            pair<int,int> temp_xy = {x,y};
            if(!boolean) {
                //False and no flip case
                if(find(no_flip.begin(), no_flip.end(), temp_xy) != no_flip.end()){
                    //Find the index of this tuple in the vector to remove it from the original map clauses
                    auto it = clauses.find(entry.first);
                    vector<tuple<int, int, bool>>& original_vector = it->second;
                    int index_to_remove = -1;
                    for (int i = 0; i < original_vector.size(); ++i) {
                        if (original_vector[i] == tpls) {
                            index_to_remove = i;
                            break; //Exit the loop once the tuple is found
                        }
                    }
                    if (index_to_remove != -1) {
                        original_vector.erase(original_vector.begin() + index_to_remove);
                        if(original_vector.size() == 0) clauses.erase(entry.first);
                    }
                    if(clauses.size() == 0) return false;
                }
            }  
        }
    }

    vector<pair<int, int>> unique_pistols;
    pair<int, int> pair_ij;
    int x, y;
    bool boolean_for;
    //After for removes variables (pistols), store in unique_pistols the rest variables
    /*for (const auto& entry : clauses) {
        //Extract values (vector of tuples)
        pair<int, int> key = entry.first;
        for (const auto& tpls : entry.second) {
            tie(x, y, boolean_for) = tpls; //Unpack tuple
            pair_ij = make_pair(x, y);

            //If we have variable no fliping and true, remove all the clauses because will be true
            if(find(no_flip.begin(), no_flip.end(), pair_ij) != no_flip.end() && boolean_for){
                //clauses.erase(entry.first);
                auto it = std::find(no_clauses_for_this_dot.begin(), no_clauses_for_this_dot.end(), entry.first);
                if(it == no_clauses_for_this_dot.end()){
                    no_clauses_for_this_dot.push_back(entry.first);
                }                
            }

            //If this pistol didnt exists in unique_pistols, insert it
            if (find(unique_pistols.begin(), unique_pistols.end(), pair_ij) == unique_pistols.end()) {
                unique_pistols.push_back(pair_ij); //Insert if not found
            }
        }
    }*/

    //Check the clauses if a variable exists 2 times in a clause
    for (const auto& entry : clauses) {
        const pair<int, int>& key = entry.first;  //Current map key
        const vector<tuple<int, int, bool>>& vec = entry.second;  //Current vector
    
        for (const auto& tpls : vec) {
            tie(x, y, boolean_for) = tpls; //Unpack tuple
            pair_ij = make_pair(x, y);
            int count = std::count(vec.begin(), vec.end(), tpls);  //Count occurrences of 't' in vec

            if (count > 1) {  //If the element appears more than once
                //If the dot (clause) didnt exist in the no_flip set
                if (no_flip.find(key) == no_flip.end()) {
                    //Check if key is already in no_clauses_for_this_dot
                    if (std::find(no_clauses_for_this_dot.begin(), no_clauses_for_this_dot.end(), key) == no_clauses_for_this_dot.end()) {
                        no_clauses_for_this_dot.push_back(key);  //Add to no_clauses_for_this_dot
                    }
                }
            }

            //If this pistol didnt exists in unique_pistols, insert it
            if (find(unique_pistols.begin(), unique_pistols.end(), pair_ij) == unique_pistols.end()) {
                unique_pistols.push_back(pair_ij); //Insert if not found
            }
        }
    }
    print_no_clauses_for_this_dot(no_clauses_for_this_dot);
    //Erase all the clauses (dots) where we find as no closure cases
    for (const auto& entry : no_clauses_for_this_dot){
        clauses.erase({entry.first, entry.second});
    }
    
    //flexible_vars are the variables (pistols) where is not in the no_flip (we can flip it)
    vector<pair<int, int>> flexible_vars;
    for (const auto& pair : unique_pistols) {
        if (no_flip.find(pair) == no_flip.end()) {  //If pair is not in no_flip
            flexible_vars.push_back(pair);
        }
    }
    
    //1D Arrays for 2Sat algorithm
    int a[clauses.size()];
    int b[clauses.size()];
    
    //print_map(clauses);

    int i = 0;
    for (const auto& entry : clauses) {
     
        const auto& vec = entry.second;
        if (vec.size() == 1) {
            int x, y;
            bool boolean;
            tie(x, y, boolean) = vec[0]; //Unpack tuple
            pair<int,int> temp_xy = {x,y};

            auto it = find(flexible_vars.begin(), flexible_vars.end(), temp_xy);
            //If the temp_xy pistol it is in the flexible_vars
            if (it != flexible_vars.end()) {
                int index = distance(flexible_vars.begin(), it) + 1; //Zero-based index
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
            else cout<<"Pistol not found1."<<endl;
            
        }
        else if (vec.size() == 2) {
            //Case: Two elements in the vector
            int x1, y1, x2, y2;
            bool b1, b2;
            tie(x1, y1, b1) = vec[0];
            tie(x2, y2, b2) = vec[1];
            pair<int,int> temp_xy1 = {x1,y1};
            pair<int,int> temp_xy2 = {x2,y2};
            auto it1 = find(flexible_vars.begin(), flexible_vars.end(), temp_xy1);
            auto it2 = find(flexible_vars.begin(), flexible_vars.end(), temp_xy2);
            if (it1 != flexible_vars.end() || it2 != flexible_vars.end()) {
                int index1 = distance(flexible_vars.begin(), it1) + 1; //Zero-based index
                int index2 = distance(flexible_vars.begin(), it2) + 1; //Zero-based index
                if(b1) a[i] = index1;
                else a[i] = -index1;
        
                if(b2) b[i] = index2;
                else b[i] = -index2;
                i++;
            }
            else {
                //if(it1 != flexible_vars.end())
                    cout <<"Pistol not found21."<<endl;
                //if(it2 != flexible_vars.end())
                    cout <<"Pistol not found22."<<endl;
            }
        }
        else{
            cout<<"Num of pistols: "<<vec.size()<<endl;
        }
    }
    
    int num_of_variables = flexible_vars.size();
    int num_of_clauses = clauses.size();
    bool final_solution = is2Satisfiable(num_of_variables, num_of_clauses, a, b);
   
    return final_solution;
}




int main() {
	ifstream file("input.txt"); // Open the file
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
                    if(agree) cout<<endl<<"Case #"<<cases_counter<<": POSSIBLE"<<endl;
                    else cout<<endl<<"Case #"<<cases_counter<<": IMPOSSIBLE"<<endl;
                    cases_counter++;
                    index_j = 0;
                    for(int i = 0 ; i < R ; i++)
                        delete [] matrix[i]; 
                    delete [] matrix;
                    T--;
                    if(T > 0) cout<<endl;
                    else cout<<endl;
                }
            }
        }
    }
	return 0;
}
