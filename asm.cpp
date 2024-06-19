
// ------------------------------------------------------------------
// Declaration of Authorship
// Name : Kashika Aggarwal
// Roll No. 2201CS35
// --------------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;
// Custom implementation of find_if
template <typename InputIt, typename UnaryPredicate>
InputIt custom_find_if(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (p(*first)) {
            return first;
        }
    }
    return last;
}

// Custom implementation of not1
template <typename Predicate>
struct custom_not1 {
    Predicate pred;
    custom_not1(Predicate p) : pred(p) {}
    bool operator()(typename Predicate::argument_type const &arg) const {
        return !pred(arg);
    }
};
// Custom implementation for push_back
void custom_push_back(string& str, char c) {
    str += c;
}


// Define structure for symbols
struct symbol
{
    string name;
    int address;
    bool set;
    int ValueOfSet;
};

// Define structure for literals
struct literal
{
	int literal;
	int address;
};

// Symbol table
vector<symbol> sym_table;

// Literal table
vector<literal> TableOfLiterals;

// Custom implementation for s.erase
void custom_erase(string &s, string::iterator first, string::iterator last) {
    s.erase(first, last);
}

// Custom implementation for s.begin()
string::iterator custom_begin(string &s) {
    return s.begin();
}

// Custom implementation for s.end()
string::iterator custom_end(string &s) {
    return s.end();
}

// Utility function to deal with white spaces
static inline string &trim(string &s) 
{
    custom_erase(s, find_if(s.rbegin(), s.rend(),
            not1(ptr_fun<int, int>(isspace))).base(), custom_end(s));
    custom_erase(s, custom_begin(s), find_if(s.begin(), s.end(),
            not1(ptr_fun<int, int>(isspace))));
    return s;
}


// Custom implementation for s.length()
size_t custom_length(const string& s) {
    size_t len = 0;
    const char* ptr = s.c_str();
    while (*ptr != '\0') {
        ++len;
        ++ptr;
    }
    return len;
}

// Custom implementation for s.substr()
string custom_substr(const string& s, size_t pos, size_t len) {
    string result;
    const char* start = s.c_str() + pos;
    for (size_t i = 0; i < len && start[i] != '\0'; ++i) {
       custom_push_back(result,start[i]);
    }
    return result;
}

// Custom implementation for s.find()
size_t custom_find(const string& s, const string& str) {
    size_t len = custom_length(s);
    size_t strLen = custom_length(str);
    if (strLen == 0) return 0;
    for (size_t i = 0; i < len - strLen + 1; ++i) {
        bool found = true;
        for (size_t j = 0; j < strLen; ++j) {
            if (s[i + j] != str[j]) {
                found = false;
                break;
            }
        }
        if (found) return i;
    }
    return string::npos;
}

// Utility function to check if a string is a number
bool is_number(const string& s)
{
    int i = 0;
    string temp;
    temp = s;
    if (s.front() == '-' or s.front() == '+') 
    {
        temp = custom_substr(s, 1, custom_length(s) - 1);
    }
    else if(custom_find(s, "0x") == 0) {
        temp = custom_substr(s, 2, custom_length(s) - 2);
    }
    string::const_iterator it = temp.begin();
    while (it != temp.end() && isdigit(*it)) ++it;
    return !temp.empty() && it == temp.end();
}

// Custom implementation for locating a substring
size_t locate2(const string& str, const string& sub) {
    size_t found = str.find(sub);
    return (found != string::npos) ? found : string::npos;
}
// Utility function for String in any base to decimal
int tonum(string s) {
    s = trim(s);
    if(locate2(s, "0x") == 0) {
        return stoul(s, nullptr, 16);
    } else if(locate2(s, "0") == 0) {
        return stoul(s, nullptr, 8);
    } else if(locate2(s, "-") == 0) {
        return -stoul(s.substr(1, s.length()), nullptr, 10);
    } else if(locate2(s, "+") == 0) {
        return stoul(s.substr(1, s.length()), nullptr, 10);
    } else {
        return stoul(s, nullptr, 10);
    }
}


// Utility function to check for elements in symbol table
bool consists(string name)
{
   auto iter = find_if(sym_table.begin(), sym_table.end(), 
               [&](const symbol& ts){return ts.name == name;});
   return iter != sym_table.end();
}

// Utility function to convert int to hex
string IntToHexa(int i)
{
  stringstream stream;
  stream << setfill ('0') << setw(8) 
         << hex << i;
  return stream.str();
}

// Custom implementation for isdigit
bool dizit(char c) {
    return (c >= '0' && c <= '9');
}

// Utility function for checking correct label name format
int LabelIsValid(string label) {
    if (!((label[0] >= 'a' && label[0] <= 'z') || (label[0] >= 'A' && label[0] <= 'Z')))
        return false;

    for (int i = 0; i < label.length(); i++) {
        if (!(dizit(label[i]) || (label[0] >= 'a' && label[0] <= 'z') || (label[0] >= 'A' && label[0] <= 'Z'))) {
            return false;
        }
    }

    return true;
}
//meumonic table, opcode/operand
map<string, string> MOT_TableEntries;
void MOT_TableEntries_init()
{
	MOT_TableEntries["ldc"] = string("00");
	MOT_TableEntries["adc"] = string("01");
	MOT_TableEntries["ldl"] = string("02");
	MOT_TableEntries["stl"] = string("03");
	MOT_TableEntries["ldnl"] = string("04");
	MOT_TableEntries["stnl"] = string("05");
	MOT_TableEntries["add"] = string("06");
	MOT_TableEntries["sub"] = string("07");
	MOT_TableEntries["shl"] = string("08");
	MOT_TableEntries["shr"] = string("09");
	MOT_TableEntries["adj"] = string("0a");
	MOT_TableEntries["a2sp"] = string("0b");
	MOT_TableEntries["sp2a"] = string("0c");
	MOT_TableEntries["call"] = string("0d");
	MOT_TableEntries["return"] = string("0e");
	MOT_TableEntries["brz"] = string("0f");
	MOT_TableEntries["brlz"] = string("10");
	MOT_TableEntries["br"] = string("11");
	MOT_TableEntries["HALT"] = string("12");
	MOT_TableEntries["data"] = string("13");
	MOT_TableEntries["SET"] = string("14");
}

// Custom implementation for substr
string custom_substr2(const string& str, size_t pos, size_t len) {
    string result;
    const char* start = str.c_str() + pos;
    for (size_t i = 0; i < len && start[i] != '\0'; ++i) {
        custom_push_back(result,start[i]);
    }
    return result;
}

// Custom implementation for length
size_t custom_length2(const string& str) {
    size_t len = 0;
    const char* ptr = str.c_str();
    while (*ptr != '\0') {
        ++len;
        ++ptr;
    }
    return len;
}
// Custom implementation for find
size_t custom_find2(const string& str, const string& substr, size_t pos = 0) {
    size_t len = custom_length(str);
    size_t subLen = custom_length(substr);
    if (subLen == 0) return pos;
    for (size_t i = pos; i < len - subLen + 1; ++i) {
        bool found = true;
        for (size_t j = 0; j < subLen; ++j) {
            if (str[i + j] != substr[j]) {
                found = false;
                break;
            }
        }
        if (found) return i;
    }
    return string::npos;
}

string InstrToTable(string instr, int* location_pointer, int line)
{
    int loc = *location_pointer;
    string mistakes = "";

    // Identify label and variables
    if(custom_find(instr, ":") != string::npos)
    {
        int colon = custom_find2(instr, ":", 0);

        if(consists(custom_substr2(instr, 0, colon)))
        {
            cout << "ERROR: Duplicate Label at line " << line << endl;
            mistakes += "ERROR: Duplicate Label at line " + to_string(line) + "\n";
        }

        if(!LabelIsValid(custom_substr(instr, 0, colon)))
        {
            cout << "WARNING: Incorrect label format at line " << line << endl;
            mistakes += "WARNING: Incorrect label format at line " + to_string(line) + "\n";
        }

        // Instruction could be present after the colon
        if(colon != custom_length2(instr) - 1)
        {
            string subs = custom_substr(instr, colon + 1, custom_length2(instr));
            subs = trim(subs);
            InstrToTable(subs, &loc, line);
            int space = custom_find2(subs, " ", 0);
            string sub_op = custom_substr(subs, 0, space);
            string sub_val = custom_substr(subs, space + 1, custom_length2(subs));
            sub_op = trim(sub_op);
            sub_val = trim(sub_val);

            // Dealing with set instructions
            if(sub_op == "SET")
            {
                sym_table.push_back({custom_substr(instr, 0, colon), loc, 1, stoi(sub_val)});
            }
            else
            {
                sym_table.push_back({custom_substr2(instr, 0, colon), loc, 0, -1});
            }
        }
        else
        {
            *location_pointer = *location_pointer - 1;
            sym_table.push_back({custom_substr(instr, 0, colon), loc, 0, -1});
        }
    }

    // Identify literals and constants
    else
    {
        int space = custom_find2(instr, " ", 0);
        string subs = custom_substr2(instr, space + 1, custom_length(instr));
        subs = trim(subs);
        if(is_number(subs))
        {
            TableOfLiterals.push_back({tonum(subs), -1});
        }
    }
    return(mistakes);
}

// Custom implementation for subs2
string subs2(const string& str, size_t pos, size_t len) {
    string result;
    const char* start = str.c_str() + pos;
    for (size_t i = 0; i < len && start[i] != '\0'; ++i) {
       custom_push_back(result, start[i]);
    }
    return result;
}

// Custom implementation for locate
size_t locate(const string& str, const string& substr, size_t pos = 0) {
    size_t len = custom_length(str);
    size_t subLen = custom_length(substr);
    if (subLen == 0) return pos;
    for (size_t i = pos; i < len - subLen + 1; ++i) {
        bool found = true;
        for (size_t j = 0; j < subLen; ++j) {
            if (str[i + j] != substr[j]) {
                found = false;
                break;
            }
        }
        if (found) return i;
    }
    return string::npos;
}

// Custom implementation for ifstream
bool custom_open_file(ifstream& file, const string& filename) {
    file.open(filename);
    return file.is_open();
}

// Custom implementation for getline
bool ObtainLine(ifstream& file, string& line) {
    if (file.eof()) {
        return false; 
    }
    getline(file, line);
    return true;
}
void analysis(string file, ofstream& LogFile)
{
    string line;
    int loc = 0;
    int line_count = 1;

    ifstream PersFile;
    if (!custom_open_file(PersFile, file)) {
        cerr << "Error: Not able to open file: " << file << endl;
        return;
    }


    // individual lines
   while (ObtainLine(PersFile, line)){
        // trim extra spaces
        string instr;
        instr = subs2(line, 0, locate(line, ";", 0));
        instr = trim(instr);

        // Skip empty lines
        if (instr == "")
        {
            line_count++;
            continue;
        }

        LogFile << InstrToTable(instr, &loc, line_count++);
        loc++;
    }

    // LTORG loop for literals
    for (int i = 0; i < TableOfLiterals.size(); i++)
    {
        if (TableOfLiterals[i].address == -1)
        {
            TableOfLiterals[i].address = loc++;
        }
    }
    PersFile.close();
}


// Custom implementation for make_tuple
tuple<string, string, string> FuncForTouple(const string& EncodedStuff, const string& mistakes, const string& MachineCode) {
    return make_tuple(EncodedStuff, mistakes, MachineCode);
}
tuple<string, string, string> InstrToCode(string instr, int* location_pointer, int line)
{
	// PC
	int loc = *location_pointer;

	// Hex Code of PC
	string EncodedStuff = IntToHexa(loc) + " ";

	// Warnings and mistakes
    string EncodedStuff_ = "";
	string mistakes = "";
	string MachineCode = "";

    // label and variables identification
    if(instr.find(':') != string::npos)
    {
        int colon = instr.find(":", 0);

        // Instruction CAN be present after the colon
        if(colon != instr.length() - 1)
        {
        	string subs = instr.substr(colon + 1, instr.length());
        	subs = trim(subs);
        	tie(EncodedStuff_, mistakes, MachineCode) = InstrToCode(subs, &loc, line);
        	string temp = EncodedStuff_;
        	temp = temp.substr(9, 9);
        	EncodedStuff += temp;
        }
        // If no instruction after colon, PC shouldn't change
        else
        {
        	EncodedStuff += "         ";
        	*location_pointer = *location_pointer - 1;
        }
        EncodedStuff += instr + "\n";
    }

    // Identify literals and constants
    else
    {
        int space = instr.find(" ", 0);

        // Temp variable for operand EncodedStuff
        string HexadecString;

        // String with operation
        string OperationStr = custom_substr2(instr,0, space);

        // String with operand
        string StrWithOperand = custom_substr2(instr,space + 1, instr.length());

        StrWithOperand = trim(StrWithOperand);
        OperationStr = trim(OperationStr);

        //invalid mnemonics
        if(MOT_TableEntries[OperationStr] == "")
        {
        	mistakes += "ERROR: The Mnemonic not defined at line " + to_string(line) + "\n";
        	cout << "ERROR: The Mnemonic not defined at line " << line << endl;
        }

        //No operand instructions
        else if(OperationStr == "add" || OperationStr == "sub"
        		|| OperationStr == "shl"|| OperationStr == "shr"
        		|| OperationStr == "a2sp"|| OperationStr == "sp2a"
        		|| OperationStr == "return"|| OperationStr == "HALT")
        {
        	EncodedStuff += "000000" + MOT_TableEntries[OperationStr] + " "; 
        	MachineCode += "000000" + MOT_TableEntries[OperationStr] + " "; 
        	if(OperationStr.length() != instr.length())
			{
				mistakes += "ERROR: Operand not expected at line " + to_string(line) + "\n";
				cout << "ERROR: Operand not expected at line " << line << endl;
			}
        }
        //numeral operand to encode directly
        else if(is_number(StrWithOperand))
        {
        	HexadecString = IntToHexa(tonum(StrWithOperand));
        	EncodedStuff += HexadecString.substr(HexadecString.length() - 6, HexadecString.length()) + MOT_TableEntries[OperationStr] + " "; 
        	MachineCode += HexadecString.substr(HexadecString.length() - 6, HexadecString.length()) + MOT_TableEntries[OperationStr] + " "; 
        }
        //variable operand to encode address
        else
        {
        	int kashu = 0;
        	for(int i=0; i < sym_table.size(); i++)
        	{
        		if(sym_table[i].name.compare(StrWithOperand) == 0)
        		{
        			kashu = 1;
        			// SET variables considered numeral
        			if(sym_table[i].set)
        			{
        				HexadecString = IntToHexa(sym_table[i].ValueOfSet);
        			}
        			// Operands which need offset from PC
        			else if(OperationStr == "call" || OperationStr == "brz"
        				|| OperationStr == "brlz"|| OperationStr == "br")
        			{
        				HexadecString = IntToHexa(sym_table[i].address - loc - 1);
        			}
        			// in Normal case encode address
        			else
        			{
        				HexadecString = IntToHexa(sym_table[i].address);
        			}
                    EncodedStuff += HexadecString.substr(HexadecString.length() - 6, HexadecString.length()) + MOT_TableEntries[OperationStr] + " "; 
        			MachineCode += HexadecString.substr(HexadecString.length() - 6, HexadecString.length()) + MOT_TableEntries[OperationStr] + " "; 
        			break;
        		}
        	}
        	if(OperationStr.length() == instr.length())
        	{
        		mistakes += "ERROR: An Operand expected at line " + to_string(line) + "\n";
        		cout << "ERROR: An Operand expected at line " << line << endl;
        	}
        	else if(!kashu)
        	{
        		mistakes += "ERROR: An Unknown Symbol as operand at line " + to_string(line) + "\n";
        		cout << "ERROR: An Unknown Symbol as operand at line " << line << endl;
        	}
        }
        EncodedStuff += instr + "\n";
    }
    return FuncForTouple(EncodedStuff, mistakes, MachineCode);

}

void synthesize(string file, ofstream& OutFile, ofstream& LogFile, ofstream& ObjectFile)
{
    string line;
    // PC
    int loc = 0;
    // Line counter
    int line_count = 1;
    // Read input file
    ifstream PersFile(file);
    // Read individual lines
    while (getline (PersFile, line)) 
    {
        //  trim extra spaces
        string instr;
        instr = line.substr(0, line.find(";", 0));
        instr = trim(instr);
        // Skip empty lines
        if(instr == "")
        {
            line_count++;
            continue;
        }

        // Writing Encoded instruction into listing file
        string EncodedStuff, mistakes, MachineCode;
        tie(EncodedStuff, mistakes, MachineCode) = InstrToCode(instr, &loc, line_count++);
        OutFile << EncodedStuff;
        if(MachineCode != "")
        {
        	uint32_t temp = stoul("0x" + trim(MachineCode), nullptr, 16);
        	ObjectFile.write((char *)&temp, sizeof(temp));
        }	
        LogFile << mistakes;
        loc++;
    }
}

int main(int argc, char* argv[]) 
{
	// Initalize Machine operation table(mot)
	MOT_TableEntries_init();

	// arg for input file
	string InFile = argv[1];

	// arg for output file
	string out_file = InFile.substr(0, InFile.find(".", 0)) + ".L";

	// arg for log file
	string log_file = InFile.substr(0, InFile.find(".", 0)) + ".log";

	// arg for object file
	string obj_file = InFile.substr(0, InFile.find(".", 0)) + ".o";

	// Defining output listing, log file
    ofstream OutFile(out_file);
    ofstream LogFile(log_file);
    ofstream ObjectFile(obj_file,ios::out | ios::binary);

    // Pass-1 of assembly, analysis phase
    analysis(InFile, LogFile);

    // Pass-2 of assembly, synthesis phase
    synthesize(InFile, OutFile, LogFile, ObjectFile);

    // Close files
    OutFile.close();
    LogFile.close();
    ObjectFile.close();
}