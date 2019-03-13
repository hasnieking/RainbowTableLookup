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

void leakedloop(string leakname, string rainbowname, int passwordcol, int hashlenght, int plaincol, char fsleak, char fsrainbow) {
    string leakline, rainbowline; //current line
    ifstream leak(leakname.c_str(), ios::in); //leaked file
    ifstream rainbow(rainbowname.c_str(), ios::in); //rainbow table

    //the loop
    getline(leak, leakline); //test for data
    while (!leak.eof()) {
        cout << leakline << endl;
        getline(rainbow, rainbowline); //test for data
        while (!rainbow.eof()) {
            cout << rainbowline << endl;
            getline(rainbow, rainbowline);
        }
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
    cout << "Make sure the hash is stored as the first field in the rainbow table." << endl << endl;

    //ask questions
    questions(leakname, rainbowname, passwordcol, hashlenght, plaincol, fsleak, fsrainbow); //ask all needed questions

    //begin lookup
    leakedloop(leakname, rainbowname, passwordcol, hashlenght, plaincol, fsleak, fsrainbow);

    return 0;
}