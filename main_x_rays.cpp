using namespace std;
#include "include/find_first_char.h"
#include "include/make_clauses.h"

void cleanup_matrix(char **&matrix, int R);

//Chech the str if it is integer (not real number because real numbers have dot (.))
bool isInteger(const string& str) {
    if (str.empty()) return false;
    size_t i = 0;

    //Check for a negative sign
    if (str[0] == '-' && str.size() > 1) i = 1;

    //Ensure all characters are digits
    for (; i < str.size(); ++i) {
        if (!isdigit(str[i])) return false;
    }

    return true;
}

int main() {
	ifstream file("tests/ts1_input.txt"); //Open the file
    if (!file.is_open()) {
        cerr<<"Error: Could not open the file."<<endl;
        return 1;
    }
    //T is the number of instances (matrix) that we will check (maximum 100)
    //R and C (row - columns) the dimensions of matrix we will check from .txt file
    int T = 100, R = 0, C = 0; //Variables to hold the three numbers
    file >> T;
    char **matrix;
    string line;
    int index_i = 0, count_ints = 0,  cases_counter = 1;
    while (getline(file, line)) { //Read line by line of the txt file
        istringstream iss(line);
        string word;

        while (iss >> word) { //Split line into words
            //Create dynamically the matrix array
            if (isInteger(word)) {
                //If we found 2 ints, that  means we have read the dimensions of the matrix from txt file
                count_ints++;
                if(count_ints == 1) R = stoi(word);
                if(count_ints == 2) {
                    C = stoi(word);
                    //Reset the integer counter
                    count_ints = 0;
                    matrix = new char*[R];
                    for(int i = 0; i < R ; i++) matrix[i] = new char[C];
                }
            }
            //As we read the R and C, fill the matrix, find the clauses and return possible (or not) solution
            else {
                //Fill the matrix. we have C columns =  word.size() (symbols) * R rows 
                for (size_t j = 0; j < word.size(); ++j) matrix[index_i][j] = word[j];
                
                index_i++;
                //As we have create the matrix, find the clauses and return if it is possible to find solution
                if(index_i >= R){
                    bool agree = make_clauses_solver(matrix, R,C);
                    if(agree) cout<<"Case #"<<cases_counter<<": POSSIBLE"<<endl;
                    else cout<<"Case #"<<cases_counter<<": IMPOSSIBLE"<<endl;
                    cases_counter++;
                    index_i = 0;
                    cleanup_matrix(matrix, R);
                    T--;
                    if(T > 0) cout<<endl;
                }
            }
        }
    }
	return 0;
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