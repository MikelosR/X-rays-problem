
using namespace std;
#include "../include/make_clauses.h"
#include "../include/find_first_char.h"
#include "../include/Two_SAT_Algorithm.h"

bool make_clauses_solver(char **matrix, int R, int C){
    cout<<"Array size: "<<R<<"*"<<C<<endl;
    bool collision = false;
    //char temp_sybol_a, temp_sybol_b;
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
                find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);                
                //Change direction in '|'
                if(collision) {
                    matrix[i][j] = '-';
                    no_flip.insert({i,j});
                    
                }
                if(!collision){
                    find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                    //Change direction in '|'
                    if(collision) {
                        matrix[i][j] = '-';
                        no_flip.insert({i,j});
                    }
                }
            }
            else if(matrix[i][j] == '-'){
                collision = false;
                find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                //Change direction in '-'
                if(collision) {
                    matrix[i][j] = '|';
                    no_flip.insert({i,j});
                }
                if(!collision){
                    //Change direction in '-'
                    find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
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
                find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    no_flip.insert({i,j});
                    collision = false;
                }
                find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    no_flip.insert({i,j});
                    collision = false;
                }
                balls.clear();
            }
            else if(matrix[i][j] == '|'){
                collision = false;
                find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    no_flip.insert({i,j});
                    collision = false;
                }
                find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
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
                find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                if(collision) {
                    break;
                }
                find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    break;
                }
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                balls.clear();
            }
            else if(matrix[i][j] == '-'){
                find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                if(collision) {
                    break;
                }
                find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) {
                    break;
                }
                pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                balls.clear();
            }
        }
    }
    //if(pistol_hit_balls.empty()) return false;
    if(collision) return false;

    map<pair<int, int>, set<pair<int, int>>> revised_pistol_hit_balls;
    //If we change the direction o f guns, give me the dots where i will hit
    revised_pistol_hit_balls_func(matrix, R, C, revised_pistol_hit_balls);
    
    balls.clear();
    //If a pistol hit a dot from 2 directions, remove this dot (clause)
    set<pair<int, int>> no_clauses_dots;
    for (const auto& entry : pistol_hit_balls) {
        for (const auto& value : entry.second) {
            if (revised_pistol_hit_balls[entry.first].find(value) != revised_pistol_hit_balls[entry.first].end()){
                no_clauses_dots.insert(value);
            }
        }
    }
    //char up, down, left, right;
    //Find the pistols that can hit specific dots, 
    //use vector to find dots that can be hited 2 times by the same weapon (with flip gun '-' => '|')
    map<pair<int, int>, vector<pair<int, int>>> balls_find_pistols;
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++) {
            if(matrix[i][j] == '.'){
                collision = false;
                find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(collision) balls_find_pistols[{i,j}].push_back({new_index_i, new_index_j});
                collision = false;
                balls.clear();
                
                if(balls_find_pistols[{i, j}].size() == 0){
                    //No variable for this clause (dot)
                    return false;
                }
            }
        }
    }
    //If none dot can "hit" a pistol, solution is IMPOSSIBLE
    if(balls_find_pistols.empty()) return false;
    
    
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
                    for (size_t i = 0; i < original_vector.size(); ++i) {
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
    //Take the last useful variables (pistols)
    for (const auto& entry : clauses) {
        const vector<tuple<int, int, bool>>& vec = entry.second;  //Current vector
    
        for (const auto& tpls : vec) {
            tie(x, y, boolean_for) = tpls; //Unpack tuple
            pair_ij = make_pair(x, y);
            //If this pistol didnt exists in unique_pistols, insert it
            if (find(unique_pistols.begin(), unique_pistols.end(), pair_ij) == unique_pistols.end()) {
                unique_pistols.push_back(pair_ij); //Insert if not found
            }
        }
    }
    //Erase all the clauses (dots) where we find as no closure cases
    for (const auto& entry : no_clauses_dots){
        clauses.erase({entry.first, entry.second});
        if(clauses.empty()) return true;
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
            
        }
    }
    
    int num_of_variables = flexible_vars.size();
    int num_of_clauses = clauses.size();
    bool final_solution = is2Satisfiable(num_of_variables, num_of_clauses, a, b);
   
    return final_solution;
}