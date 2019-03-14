#include <iostream>
#include <fstream>
#include <string>

using namespace std;


void fieldseperator(char &fs);

void questions(string &leakname, string &rainbowname, int &passwordcol, int &hashlenght, int &plaincol, char &fsleak, char &fsrainbow) {
    cout << "What is the filename of the leaked file?" << endl;
    cout << "Filename: ";
    cin >> leakname;
    cout << "What is the filename of the rainbow table?" << endl;
    cout << "Filename: ";
    cin >> rainbowname;
    cout << "In which # column is the password hash in the leaked file?" << endl;
    cout << "Column Nr: ";
    cin >> passwordcol;
    cout << "In which # column is the plain password in the rainbow table?" << endl;
    cout << "Column Nr: ";
    cin >> plaincol;
    cout << "How many characters long is the hash?" << endl;
    cout << "Lenght: ";
    cin >> hashlenght;
    cout << "What is the field seperator for the leaked file?" << endl;
    fieldseperator(fsleak);
    cout << "What is the field seperator for the rainbow table?" << endl;
    fieldseperator(fsrainbow);
}

//sets the field seperator.
void fieldseperator(char &fs) {
    int choose; //int for choosing
    cout << "[1]TAB, [2]Other" << endl;
    cout << "Choose: ";
    cin >> choose;
    switch(choose) {
        case 1 : fs = '\t'; break;
        case 2 : cout << "Choose character: ";
            cin >> fs; break;
        default: cout << "Incorrect value." << endl;
            fieldseperator(fs);
    }
}

//gets the hash from the leaked file
string leaktohash(char fs, int col, string line) {
    int fscounter = 0; //counts field seperators
    char currentchar; //char that gets copied or tested
    string hash;

    for (unsigned int i = 0; i < line.length(); i++) {
        currentchar = line[i];
        if (fscounter == (col - 1)) {
            if (currentchar != fs) {
                hash.push_back(currentchar);
            }
        }
        //count field seperators
        if (currentchar == fs) {
            fscounter++;
        }
    }
    return hash;
}

//compares hashes
bool compare(string leakhash, string rainbowline, int hashlenght) {
    string rainbowhash = rainbowline.substr(0,hashlenght); //get hash from leak
    return (leakhash.compare(rainbowhash) == 0); //compare the hashes
}

//double loop that tests every value in the rainbow table
void leakedloop(string leakname, string rainbowname, int passwordcol, int hashlenght, int plaincol, char fsleak, char fsrainbow) {
    string leakline, rainbowline; //current line
    string leakhash; //leaked hash
    ifstream leak(leakname.c_str(), ios::in); //leaked file
    ifstream rainbow(rainbowname.c_str(), ios::in); //rainbow table

    //the loop
    getline(leak, leakline); //test for data
    while (!leak.eof()) {
        leakhash = leaktohash(fsleak, passwordcol, leakline);
        getline(rainbow, rainbowline); //test for data
        while (!rainbow.eof()) {
            if(compare(leakhash, rainbowline, hashlenght)) {
                //printline(leakline, rainbowline);
                break;
            } else {
                getline(rainbow, rainbowline);
            }
        }
        //go back to the top of the rainbow file
        rainbow.clear();
        rainbow.seekg(0, ios::beg);
        getline(leak, leakline);
    }
}

int main() {
    string leakname, rainbowname; //filenames
    int passwordcol, plaincol; //column # of passwords/hashes
    int hashlenght; //lenght of hash
    char fsleak, fsrainbow; //field seperators

    //info
    cout << "Hash to password rainbow table lookup" << endl;
    cout << "Prints data from original files of found hashes and replaces it with plain text." << endl;
    cout << "Make sure the hash is stored as the first field in the rainbow table." << endl << endl;

    //ask questions
    questions(leakname, rainbowname, passwordcol, hashlenght, plaincol, fsleak, fsrainbow); //ask all needed questions

    //begin lookup
    leakedloop(leakname, rainbowname, passwordcol, hashlenght, plaincol, fsleak, fsrainbow);

    return 0;
}