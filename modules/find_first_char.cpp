#include "../include/find_first_char.h"
using namespace std;

//For the symbol matrix[i][j] find the first charachter from the left side. Stop when you find '-', '|', '#', 'x'
//Store every dot (.) where you find in the route, in the set balls
//In the new_index_j and new_index_i store the coordinates of the symbol that we have collision
//We have Collision when the matrix[i][j] is '-' or '|' and also the matrix[new_index_i][new_index_j] is '-' or '|'
void find_first_char_left(char **matrix, int R, int C, int i, int j, bool& collision, int& new_index_i, 
                            int& new_index_j, set<pair<int, int>>& balls){
    j--;
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
                return;
            }
            if(symbol == '\\') {
                find_first_char_up(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls);
            }
            else if(symbol == '/') {
                find_first_char_down(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls);
            }
            return;
        }
    }
    return;
}

//Εxact what it does the find_first_char_left but from the right side
void find_first_char_right(char **matrix, int R, int C, int i, int j, bool& collision, 
            int& new_index_i, int& new_index_j, set<pair<int, int>>& balls){
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
                return;
            } 
            if(symbol == '\\') {
                find_first_char_down(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls);
            }
            else if(symbol == '/') {
                find_first_char_up(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls);
            }
            return;
        }
    }
    return;
}

//Εxact what it does the find_first_char_left but from the upper side
void find_first_char_up(char **matrix, int R, int C, int i, int j, bool& collision, 
                    int& new_index_i, int& new_index_j, set<pair<int, int>>& balls){
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
                return;
            } 
            if(symbol == '\\') {
                find_first_char_left(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls);
            }
            else if(symbol == '/') {
                find_first_char_right(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls);
            }
            return;
            
        }
    }
    return;
}

//Εxact what it does the find_first_char_left but from the bottom side
void find_first_char_down(char **matrix, int R, int C, int i, int j, bool& collision, 
            int& new_index_i, int& new_index_j, set<pair<int, int>>& balls){
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
                return;
            }  
            if(symbol == '\\') {
                find_first_char_right(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls);
            }
            else if(symbol == '/') {
                find_first_char_left(matrix, R, C, i, j, collision, new_index_i, new_index_j, balls);
            }
            return;
        }
    }
    return;
}

//If we change the direction o f guns, give me the dots where i will hit
void revised_pistol_hit_balls_func(char **matrix, int R, int C, map<pair<int, int>, set<pair<int, int>>>& revised_pistol_hit_balls){
    bool collision = false;
    set<pair<int, int>> balls;
    int new_index_i, new_index_j;
    //char temp_sybol_a, temp_sybol_b;
    for (int i = 0; i < R; i++){
        for (int j = 0; j < C; j++) {
            if(matrix[i][j] == '-'){
                find_first_char_up(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);                
                find_first_char_down(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                //If we have collision, dont give me back the set of dots
                if(!collision) revised_pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                else collision = false;
                
                balls.clear();
            }
            else if(matrix[i][j] == '|'){
                find_first_char_left(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                find_first_char_right(matrix,R,C,i,j, collision, new_index_i, new_index_j, balls);
                if(!collision) revised_pistol_hit_balls[{i,j}].insert(balls.begin(), balls.end());
                else collision = false;
                balls.clear();
            }
        }
    }
}

//Print the clauses map
void print_map(map<pair<int, int>, vector<tuple<int, int, bool>>>& clauses){

    cout<<"Clauses:"<<endl;
    for (const auto& entry : clauses) {
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

void print_balls_find_pistols(const map<pair<int, int>, vector<pair<int, int>>>& balls_find_pistols) {
    cout << "Balls hit the pistols: "<<endl;
    for (const auto& [ball, pistols] : balls_find_pistols) {
        cout << "Ball (" << ball.first << ", " << ball.second << ") -> Pistols: ";
        if (pistols.empty()) {
            cout << "None";
        } else {
            for (const auto& pistol : pistols) {
                cout << "(" << pistol.first << ", " << pistol.second << ") ";
            }
        }
        cout<<endl;
    }
}


void print_noflip(const set<pair<int, int>>& no_flip) {
    cout<<"No flip: ";
    cout << "{ ";
    for (const auto& p : no_flip) {
        cout<<"(" <<p.first<< ", "<< p.second << ") ";
    }
    cout<<"}"<<endl;
}


void print_pistol_hit_balls(const map<pair<int, int>, set<pair<int, int>>>& pistol_hit_balls) {
    cout << "Pistol hit the balls: "<<endl;
    for (const auto& entry : pistol_hit_balls) {
        cout << "Pistol:  (" << entry.first.first << ", " << entry.first.second << ") -> Balls: { ";
        for (const auto& p : entry.second) {
            cout << "(" << p.first << ", " << p.second << ") ";
        }
        cout << "}\n";
    }
    cout<<endl;
}

void print_no_clauses_for_this_dot(const vector<pair<int, int>>& no_clauses_for_this_dot) {
    cout << "No clauses for these dot: [ ";
    for (const auto& p : no_clauses_for_this_dot) {
        cout << "(" << p.first << ", " << p.second << ") ";
    }
    cout << "]" << endl;
}


