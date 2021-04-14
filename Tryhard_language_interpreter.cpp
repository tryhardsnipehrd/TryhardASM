/*
	This is an interpreter designed for an Assembly like language called TryhardLanguage
	In development
	Started 04/08/2021
*/

/*
	Planned things to add:
	
	Instructions:
	[o] STO
	[o] STY
	[o] STX
	[o] SVY
	[o] SVX
	[o] INX
	[o] INY
	[o] ADD
	[o] SBX
	[o] SBY
	[o] SUB
	[o] JMP
	[o] PNT
	[o] BNE
	[o] CMP
	[o] LBL
*/

#include <iostream> // This is the "default" library that everything needs
#include <fstream>  // For File handling to actually read the file
#include <vector>   // For using a vector to store the file into
#include <map>		// For using what is essentially a Dict

using namespace std;


// This is my function to check if a line starts with a string, as there is no built-in method
bool startsWith(string mainStr, string toMatch) {
	if (mainStr.find(toMatch) == 0) {
		return true;
	} else {
		return false;
	}
}

void splitTwo(string stringToSplit, string array[]) {
	string tempString3;
	string tempString4;
	unsigned first = stringToSplit.find(" ");
	unsigned last = stringToSplit.find_last_of(" ");
	tempString3 = stringToSplit.substr(first+1, (last-first)-1); // Split at the space, which is the 4th character
	tempString4 = stringToSplit.substr(last+1);
	array[0] = tempString3;
	array[1] = tempString4;
}

int main(int argc, char *argv[])
{	
	// All the variables and data for actually holding everything... not the most efficient, but it works
	vector <string> file_contents;
	map <string, string> variables;
	map <string, int> labels;
	map <string, int> registers;
	registers["X"] = 0;
	registers["Y"] = 0;
	fstream file;
	string tempString1;
	string tempString2;
	string letters[2];
	int tempNum;
	bool isEqual = false;

	file.open(argv[1], ios::in);
	if (file) {
		cout << "Starting file reading\n";
		string ch;
		while (getline(file, ch)) {
			file_contents.push_back(ch);
		}
	} else {
		cout << "Error Loading File\n";
	}
	file.close();
	for (int i=0; i<file_contents.size(); i++){

		// INcrement X, add one to the X register
		if (startsWith(file_contents.at(i), "INX")) {
			registers["X"]++; // Increment X
		}
		
		// INcrement Y, add one to the Y register
		else if (startsWith(file_contents.at(i), "INY")) {
			registers["Y"]++; // Increment Y
		}
		
		// STore to X, store the int given to the X register
		else if (startsWith(file_contents.at(i), "STX")) {
			tempString1 = file_contents.at(i).substr(4); // Split at the space, which is the 4th character
			registers["X"] = stoi(tempString1); // change that to an int and store it back into X
		}
		
		// STore to Y, store the int given to the Y register
		else if (startsWith(file_contents.at(i), "STY")) {
			tempString1 = file_contents.at(i).substr(4); // Split at the space, which is the 4th character
			registers["Y"] = stoi(tempString1); // change that to an int and store it back into Y
		}
		
		// STOre to variable, store the second argument to the first argument
		else if (startsWith(file_contents.at(i), "STO")) {
			splitTwo(file_contents.at(i), letters);
			variables[letters[0]] = letters[1]; // Assign tempString1 with tempString2
		}

		// Store a Variable to X, whatever int is in the variable is saved to X
		else if (startsWith(file_contents.at(i), "SVX")) {
			tempString1 = file_contents.at(i).substr(4);
			tempNum = stoi(variables[tempString1]);
			registers["X"] = tempNum;
		}

		// Store a Variable to Y, whatever int is in the variable is saved to Y
		else if (startsWith(file_contents.at(i), "SVY")) {
			tempString1 = file_contents.at(i).substr(4);
			tempNum = stoi(variables[tempString1]);
			registers["Y"] = tempNum;
		}
		
		// JuMP to line, move "i" to the line -1 for "JuMPing"
		else if (startsWith(file_contents.at(i), "JMP")) {
			tempString1 = file_contents.at(i).substr(4); // Grab the line to jump as a string
			try {
				tempNum = stoi(tempString1); // Turn it to an int
			} catch (const std::invalid_argument& e) {
				tempNum = labels[tempString1];
			}
			i = tempNum - 1; // set the for loop iterable to that minus one, because indexing
		}
		
		// CoMPare variable to string, see if the variable given is equal to string given
		else if (startsWith(file_contents.at(i), "CMP")) {
			splitTwo(file_contents.at(i), letters);
			
			isEqual = variables[letters[0]] == letters[1];
		}
		
		// Branch if Not Equal, Checks the flag that CMP set
		else if (startsWith(file_contents.at(i), "BNE")) {
			if (!isEqual) {
				tempString1 = file_contents.at(i).substr(4); // Grab the line to jump as a string
				try {
					tempNum = stoi(tempString1); // Turn it to an int
				} catch (const std::invalid_argument& e) {
					tempNum = labels[tempString1];
				}
				i = tempNum - 1; // set the for loop iterable to that minus one, because indexing
			}
		}

		// Branch if EQual to, Checks the flag that CMP sets
		else if (startsWith(file_contents.at(i), "BE!")) {
			if (isEqual) {
				tempString1 = file_contents.at(i).substr(4); // Grab the line to jump as a string
				try {
					tempNum = stoi(tempString1); // Turn it to an int
				} catch (const std::invalid_argument& e) {
					tempNum = labels[tempString1];
				}
				i = tempNum - 1; // set the for loop iterable to that minus one, because indexing
			}
		}
		
		// ADD to variable, add the second arg to the variable at first arg; only works if variable is an int
		else if (startsWith(file_contents.at(i), "ADD")) {
			splitTwo(file_contents.at(i), letters);
			try {
				tempNum = stoi(letters[1]);
			} catch (...) {break;}
			variables[letters[0]] = to_string(tempNum + stoi(variables[letters[0]]));
		}

		// SUBtract to variable, subtract the second arg from the variable at first arg; only works if variable is an int
		else if (startsWith(file_contents.at(i), "SUB")) {
			splitTwo(file_contents.at(i), letters);
			try {
				tempNum = stoi(letters[1]);
			} catch (...) {break;}
			variables[letters[0]] = to_string(tempNum - stoi(variables[letters[0]]));
		}

		// SuBtract from X, subtract the given number from X
		else if (startsWith(file_contents.at(i), "SBX")) {
			tempString1 = file_contents.at(i).substr(4);
			try {
				tempNum = stoi(tempString1);
				registers["X"] = registers["X"] - tempNum;
			} catch (...) {break;}

		}

		// SuBtract from Y, subtract the given number from Y
		else if (startsWith(file_contents.at(i), "SBY")) {
			tempString1 = file_contents.at(i).substr(4);
			try {
				tempNum = stoi(tempString1);
				registers["Y"] = registers["Y"] - tempNum;
			} catch (...) {break;}

		}
		
		// PriNT the variable to the screen
		else if (startsWith(file_contents.at(i), "PNT")) {
			tempString1 = file_contents.at(i).substr(4);
			cout << variables[tempString1] << "\n";
		}


		// Create a LaBeL, allows for JMP/BNE/BEQ later... however, you must set it before jumping
		else if (startsWith(file_contents.at(i), "LBL")) {
			tempString1 = file_contents.at(i).substr(4);
			labels[tempString1] = i;
		}



		//cout << file_contents.at(i) << "\n";
	}
/*
	// Debug things
	cout << "X: " << registers["X"] << "\n";
	cout << "Y: " << registers["Y"] << "\n";
	cout << "isEqual: " << isEqual << "\n";
	auto iter = variables.begin();
	while (iter!=variables.end()) {
		cout << iter->first << ":" << iter->second << "\n";
		++iter;
	}
*/
	return 0;
}