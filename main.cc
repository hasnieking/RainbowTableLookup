#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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

//gets the hash from the leaked file
string linetostring(char fs, int col, string line) {
    int fscounter = 0; //counts field seperators
    char currentchar; //char that gets copied or tested
    string substring;

    for (unsigned int i = 0; i < line.length(); i++) {
        currentchar = line[i];
        if (fscounter == (col - 1)) {
            if (currentchar != fs) {
                substring.push_back(currentchar);
            }
        }
        //count field seperators
        if (currentchar == fs) {
            fscounter++;
        }
    }
    return substring;
}

//source: https://thispointer.com/find-and-replace-all-occurrences-of-a-sub-string-in-c/
void printline(string line, string toSearch, string replaceStr)
{
	// Get the first occurrence
	size_t pos = line.find(toSearch);
 
	// Repeat till end is reached
	while( pos != string::npos)
	{
		// Replace this occurrence of Sub String
		line.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos =line.find(toSearch, pos + replaceStr.size());
	}

    cout << line << endl;
}

//compares hashes
bool compare(string leakhash, string rainbowline, int hashlenght) {
    string rainbowhash = rainbowline.substr(0,hashlenght); //get hash from leak

    return (leakhash.compare(rainbowhash) == 0); //compare the hashes
}

//gets the plaintext password once hashes are the same
string getplaintext(char fsrainbow, int col, string line) {
    string plaintext;
    plaintext = linetostring(fsrainbow, col, line);

    return plaintext;
}

//double loop that tests every value in the rainbow table
void leakedloop(string leakname, string rainbowname, int passwordcol, int hashlenght, int plaincol, char fsleak, char fsrainbow) {
    string leakline, rainbowline; //current line
    string leakhash; //leaked hash
    string plaintext; //password in plaintext
    ifstream leak(leakname.c_str(), ios::in); //leaked file
    ifstream rainbow(rainbowname.c_str(), ios::in); //rainbow table

    //the loop
    getline(leak, leakline); //test for data
    while (!leak.eof()) {
        leakhash = linetostring(fsleak, passwordcol, leakline);
        leakhash.pop_back();
        getline(rainbow, rainbowline); //test for data
        while (!rainbow.eof()) {
            if(compare(leakhash, rainbowline, hashlenght)) { //if the hashes are the same print
                plaintext = getplaintext(fsrainbow, plaincol, rainbowline); //get plaintext pw
                printline(leakline, leakhash, plaintext); //print
                break;
            } else {
                getline(rainbow, rainbowline); //get next hash
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
    leakedloop(leakname, rainbowname, passwordcol, hashlenght, plaincol, fsleak, fsrainbow); //test all hashes

    return 0;
}