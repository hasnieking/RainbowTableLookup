#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// leakfile rainbowfile col#
//-l: field seperator leak
//-r: field seperator rainbow

//converts char array to int
int charstoint(string number, bool &correct) {
    int result = 0;
    for (unsigned int i = 0; i < number.length(); i++) {
        if (number[i] >= '0' && number[i] <= '9') {
            result *= 10;
            result += (number[i] - '0');
        } else {
            correct = false;
        }
    }
    return result;
}

int main(int argc, char **argv) {
    bool correctsyntax = true; //false when error in command

    //values from arguments
    int colnr = 0; //column number
    char fsleak = '\t', fsrainbow = ':'; //field seperators
    string leakfile, rainbowfile; //filenames


    if (argc > 9 || argc < 4) correctsyntax = false;
    //get values from 
    for(int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch(argv[i][1]) {
                case 'l' : i++; fsleak = argv[i][0]; break;
                case 'r' : i++; fsrainbow = argv[i][0]; break;
                default : correctsyntax = false;
            }
        } else {
            if (leakfile.length() == 0) {
                leakfile = argv[i];
            } else if (rainbowfile.length() == 0) {
                rainbowfile = argv[i];
            } else if (colnr == 0) {
                colnr = charstoint(argv[i], correctsyntax);
            } else {
                correctsyntax = false;
            }
        }
    }

    //quit program when using wrong syntax
    if (!correctsyntax) {
        cout << "WRONG!" << endl;
        return 1;
    }

    //convert rainbow table to tree

    //search all hashes in tree
    
    return 0;
}