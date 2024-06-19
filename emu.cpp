/*****************************************************************************																														
AUTHOR: Kashika Aggarwal
ROLL NO.: 2201CS35
Declaration of Authorship
This cpp file, emu.cpp, is part of the assignment of CS210 Comp Architecture
*****************************************************************************/

#include <bits/stdc++.h>

using namespace std;
//endl function
void enter() {
    cout << '\n';
}
//cout function
void out(const string& msg) {
    cout << msg;
}

//ofstream write function
void writee(ofstream& file, const string& data) {
    file << data;
}
// Utility function to check of a string is a number
bool is_number(const string& s)
{
    int i = 0;
    string temp;
    temp = s;
    if (s.front() == '-' or s.front() == '+') 
    {
        temp = s.substr(1, s.length());
    }
    else if(s.find("0x") == 0) {
        temp = s.substr(2),16;
    }
    string::const_iterator it = temp.begin();
    while (it != temp.end() && isdigit(*it)) ++it;
    return !temp.empty() && it == temp.end();
}
//find function
template<typename ForwardIterator, typename T>
bool locate(ForwardIterator first, ForwardIterator last, const T& value) {
    // Your custom implementation of find
    for (auto it = first; it != last; ++it) {
        if (*it == value) {
            return true;
        }
    }
    return false;
}
// Utility function to convert integer to hexadecimal
string IntToHexa(int i) {
    stringstream stream;
    // Convert integer to hexadecimal string
    string hex_str = "00000000"; // 8 characters long

    // Convert integer to hexadecimal and store in hex_str
    for (int j = 7; j >= 0; --j) {
        int nibble = (i >> (j * 4)) & 0xF; 
        hex_str[7 - j] = nibble < 10 ? '0' + nibble : 'A' + (nibble - 10);
    }
    // Append hexadecimal string to the stream
    stream << hex_str;

    return stream.str();
}


// GLobal variable for registers
int32_t A1, B1, PC, SP;//register names
int32_t memory[10000];//load memory pf machine
vector<int> PC_of_DataVar;//vector to store programme counter of variables

// Utility function to retrieve operand and opcode from machine code
tuple<int32_t, int32_t> inst_to_operand_opcode(int32_t instr)
{
	int32_t opcode, operand;
	// masking first 6 bits
	opcode = instr & 0xff;
	// masking last 2 bits
	operand = instr & 0xffffff00;
	// arth Shift by 2
	operand >>= 8;
	return(make_tuple(operand, opcode));
}

// Utility function for usage instructions
void prompt() {
    cout << "Usage: ./emu.exe [option] file.o" << endl;
    cout << "Options:" << endl;
    cout << "\t-Trace_func\tshow instruction Trace_func" << endl;
    cout << "\t-before\tshow memory dump before execution" << endl;
    cout << "\t-after\tshow memory dump after execution" << endl;
    cout << "\t-isa\tdisplay InsSetArch" << endl;
}

// Utility function for memory dump 
void Dump_Mem(int poc, ofstream& trcfile)
{
	cout << "Dump from memory";
	trcfile << "Dump from memory";

	int cat = 0;
while (cat < poc) {
    if (!(cat % 4)) {
        cout << endl << endl << IntToHexa(cat) << "\t" << IntToHexa(memory[cat]) << " ";
        trcfile << endl << endl << IntToHexa(cat) << "\t" << IntToHexa(memory[cat]) << " ";
    } else {
        cout << IntToHexa(memory[cat]) << " ";
        trcfile << IntToHexa(memory[cat]) << " ";
    }
    cat++;
}

	cout << endl;
}

// Print Instruction set architecture
void InsSetArch() {
    cout << "Opcode Mnemonic Operand\n";
    cout << "       data     value\n";
    cout << "0      ldc      value\n";
    cout << "1      adc      value\n";
    cout << "2      ldl      value\n";
    cout << "3      stl      value\n";
    cout << "4      ldnl     value\n";
    cout << "5      stnl     value\n";
    cout << "6      add\n";
    cout << "7      sub\n";
    cout << "8      shl\n";
    cout << "9      shr\n";
    cout << "10     adj      value\n";
    cout << "11     a2sp\n";
    cout << "12     sp2a\n";
    cout << "13     call     offset\n";
    cout << "14     return\n";
    cout << "15     brz      offset\n";
    cout << "16     brlz     offset\n";
    cout << "17     br       offset\n";
    cout << "18     HALT\n";
    cout << "       SET      value\n";
}

// MOT table(operand-opcode table)
map<int, string> MOT_TableEntries;
void MOT_TableEntries_init()
{
	MOT_TableEntries[0] = string("ldc");
	MOT_TableEntries[1] = string("adc");
	MOT_TableEntries[2] = string("ldl");
	MOT_TableEntries[3] = string("stl");
	MOT_TableEntries[4] = string("ldnl");
	MOT_TableEntries[5] = string("stnl");
	MOT_TableEntries[6] = string("add");
	MOT_TableEntries[7] = string("sub");
	MOT_TableEntries[8] = string("shl");
	MOT_TableEntries[9] = string("shr");
	MOT_TableEntries[10] = string("adj");
	MOT_TableEntries[11] = string("a2sp");
	MOT_TableEntries[12] = string("sp2a");
	MOT_TableEntries[13] = string("call");
	MOT_TableEntries[14] = string("return");
	MOT_TableEntries[15] = string("brz");
	MOT_TableEntries[16] = string("brlz");
	MOT_TableEntries[17] = string("br");
	MOT_TableEntries[18] = string("HALT");
	MOT_TableEntries[19] = string("data");
	MOT_TableEntries[20] = string("SET");
}

// Function to Trace individual instructions
int Trace_func(int PC, int poc, ofstream& trcfile) {
    //poc=no of instr in the programme
    // Number of executed instructions
    int count = 0;
    // Loop to execute until HALT
    while (true) {
        // Bookkeeping for infinite loop
        int old_pc = PC;
        // Int32 for operand and opcode
        int32_t operand, operation;
        // Instruction at particular PC
        int32_t instr = memory[PC];

        // Retrieve operand and opcode
        tie(operand, operation) = inst_to_operand_opcode(instr);

        // Print statement for tracing
        out("PC: " + IntToHexa(PC) + "\tSP: " + IntToHexa(SP) + "\tA1: " 
                    + IntToHexa(A1) + "\tB1: " + IntToHexa(B1) + "\t" + MOT_TableEntries[operation] 
                    + " " + to_string(operand));
        enter();

        // Write to file
        writee(trcfile, "PC: " + IntToHexa(PC) + "\tSP: " + IntToHexa(SP) + "\tA1: " 
                            + IntToHexa(A1) + "\tB1: " + IntToHexa(B1) + "\t" + MOT_TableEntries[operation] 
                            + " " + to_string(operand) + "\n");

        // Check if PC is in PC_of_DataVar
        if (locate(PC_of_DataVar.begin(), PC_of_DataVar.end(), PC)) {
            PC++;
            continue;
        }

switch (operation) {
    case 0:
        B1 = A1;
        A1 = operand;
        break;
    case 1:
        A1 += operand;
        break;
    case 2:
        B1 = A1;
        A1 = memory[SP + operand];
        break;
    case 3:
        memory[SP + operand] = A1;
        A1 = B1;
        break;
    case 4:
        A1 = memory[A1 + operand];
        break;
    case 5:
        memory[A1 + operand] = B1;
        break;
    case 6:
        A1 += B1;
        break;
    case 7:
        A1 = B1 - A1;
        break;
    case 8:
        A1 = B1 << A1;
        break;
    case 9:
        A1 = B1 >> A1;
        break;
    case 10:
        SP += operand;
        break;
    case 11:
        SP = A1;
        A1 = B1;
        break;
    case 12:
        B1 = A1;
        A1 = SP;
        break;
    case 13:
        B1 = A1;
        A1 = PC;
        PC += operand;
        break;
    case 14:
        PC = A1;
        A1 = B1;
        break;
    case 15:
        if (A1 == 0)
            PC += operand;
        break;
    case 16:
        if (A1 < 0)
            PC += operand;
        break;
    case 17:
        PC += operand;
        break;
    case 18:
        out(to_string(count) + " number of instructions executed\n");
        return 0;
    case 20:
        A1 = operand;
        break;
    default:
        // Handle unknown operation
        out("Unknown operation\n");
        break;
}
  // Increment PC
        PC++;
  // Check for infinite loop anomaly
        if (old_pc == PC) {
            out("Infinite loop detected\n");
            break;
        }
        count++;
    }
    // Total number of executed instructions
    out(to_string(count) + " number of instructions executed\n");
}


int main(int Arg_C, char* Arg_V[])
{
	// Correct input command should be given
	if(Arg_C < 3)
	{
		// Print prompt for usage
		prompt();
		return(0);
	}

	// Initialize the machine operand table
	MOT_TableEntries_init();

	// Argument for input file
	string in_file = Arg_V[2];

	// Argument for output file
	string Trace_func_file = in_file.substr(0, in_file.find(".", 0)) + ".Trace_func";

	// Input object file
	ifstream infile;

	// Output Trace_func file
	ofstream trcfile; 

	// Open file
	infile.open(in_file, ios::out | ios::binary);
	trcfile.open(Trace_func_file);

	// 32 bit integer for machine code
	int32_t instr_code;

	// Variable for number of instructions
	int poc = 0;

	// Loop to store mchine code into main memory
	while (infile.read((char*) &instr_code, sizeof(instr_code)))
	{
		int32_t operand, operation;
		tie(operand, operation) = inst_to_operand_opcode(instr_code);
		
		// If instruction is data or set then store the value into memory
		if(operation == 19 || operation == 20)
		{
			memory[poc] = operand;
			PC_of_DataVar.push_back(poc++);
		}

		// Other instructions stored in memory
		else if(operation >= 0 && operation < 20)
		{
			memory[poc++] = instr_code;
		}

		// Identify invalid instruction
		else
		{
			cout << "Invalid instruction\n";
			return(0);
		}
	}

	A1 = 0;
	B1 = 0;
	PC = 0;
	SP = sizeof(memory)/sizeof(memory[0])-1;
	
	// Memory dump before execution
	if(string(Arg_V[1]) == "-before")
		Dump_Mem(poc, trcfile);

	// Tracing for each executed instruction
	if(string(Arg_V[1]) == "-Trace_func")
		Trace_func(0, poc, trcfile);

	// Memory dump after execution
	if(string(Arg_V[1]) == "-after")
{
    Trace_func(0, poc, trcfile);
    Dump_Mem(poc, trcfile);

    // Write sorted array to output file
    ofstream sortedFile("sorted_output.txt");
    if (sortedFile.is_open()) {
        sortedFile << "Sorted Array:\n";
        for (int i = 0; i < 10; ++i) {
            sortedFile << memory[i] << " ";
        }
        sortedFile << endl;
        sortedFile.close();
    } else {
        cout << "Unable to open sorted output file." << endl;
    }
}
	// Close file
	infile.close();
}
