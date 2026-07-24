/*
To y'all who wonder how I structure my C++ files
Headers
Classes/Structs/Constants
Function Declarations
Main
Function Definitions
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

class CallStack
{
    private:
        uint16_t data[16] = {0};
        uint8_t top = 0;
    public:
        void push(uint16_t address)
        {
            if (top < 16)
            {
                data[top] = address;
                top++;
            }
            else
            {
                // to any code reader that isn't me, this is a joke.
                std::cout << "Please check Stack Overflow to fix this error.\n You've made too many function calls in your script.\n";

                for (uint8_t i = 0; i < 15; i++)
                {
                    data[i] = data[i + 1];
                }
                top = 15;
                data[top] = address;
                // ^ shifts the addresses due to the stack overflowing, that's how I designed it, ask me why and it's because I made the call stack like that in minecraft.
            }
        }

        uint16_t pop()
        {
            if (top > 0)
            {
                top--;
                return data[top];
            }
            // return 0 cuz the stack is empty
            return 0;
        }
};

std::unordered_map<std::string, std::string> opcodeLUT = {
    {"NOP", "0000"},
    {"HLT", "0001"},
    {"ADD", "0010"},
    {"SUB", "0011"},
    {"NAND", "0100"},
    {"OR", "0101"},
    {"XOR", "0110"},
    {"RSH", "0111"},
    {"LDI", "1000"},
    {"ADI", "1001"},
    {"JMP", "1010"},
    {"BRH", "1011"},
    {"CALL", "1100"},
    {"RET", "1101"},
    {"LOD", "1110"},
    {"STR", "1111"}
};

std::unordered_map<std::string, std::vector<std::string>> operandsLUT = {
    {"NOP", {""}},
    {"HLT", {""}},
    {"ADD", {"reg", "reg", "reg"}},
    {"SUB", {"reg", "reg", "reg"}},
    {"NAND", {"reg", "reg", "reg"}},
    {"OR", {"reg", "reg", "reg"}},
    {"XOR", {"reg", "reg", "reg"}},
    {"RSH", {"reg", "reg"}},
    {"LDI", {"reg", "imm"}},
    {"ADI", {"reg", "imm"}},
    {"JMP", {"Addr10"}},
    {"BRH", {"Cond", "Addr10"}},
    {"CALL", {"Addr10"}},
    {"RET", {""}},
    {"LOD", {"reg", "Addr"}},
    {"STR", {"reg", "Addr"}}
};

std::unordered_map<std::string, std::string> registerLUT = {
    {"r0", "000"},
    {"r1", "001"},
    {"r2", "010"},
    {"r3", "011"},
    {"r4", "100"},
    {"r5", "101"},
    {"r6", "110"},
    {"r7", "111"},
};

std::unordered_map<std::string, std::string> condLUT= {
    {"Z", "00"},
    {"NZ", "01"},
    {"C", "10"},
    {"NC", "11"}
};

void displayState();

// bool validFileType(std::string filename); some old things
// bool fileExists(std::string filename);
std::string fileFormat(std::string filename);
std::string filePath(std::string filename);

bool assemble(const std::string& infile); 
std::string getOperandType(const std::string& operand, const std::string& instruction, const int& operandPos);
bool isANumber(const std::string& inString);

std::string DectoBin(int num, int bitWidth);
std::string BintoHex(const std::string& bin);

void fetch();
void decode();
void execute();

int main()
{
    // Initialize the Components
    uint16_t instruction_memory[1024];
    uint16_t pc = 0;
    CallStack call_stack;
    uint8_t RAM[256];
    uint8_t register_file[8];
    bool zero_flag;
    bool carry_flag;

    // Load in da recipe (load in the program)
    std::cout << "What file do you want to run bro?\n";
    std::string filename;
    std::cin >> filename;

    // Do some validation (i.e. check that the file actually exists, check that it's the correct file type (.hex or .asm))
    /*
    if (!validFileType(filename))
    {
        std::cout << "You sure it's that file bro?\n";
        return 1;
    } // Ignore this :P
    */
    std::string binFile;
    if (fileFormat(filename) == ".asm")
    {
        // check if there's .hex already :P if not then make it
        std::string hexFilename = std::filesystem::path(filename).replace_extension(".hex").string();
        if (std::filesystem::exists(filePath(hexFilename)))
        {
            binFile = hexFilename;
        }
        else
        {
            if (!assemble(filename))
            {
                std::cout << "^ There are some errors pal.\n";
                return 1;
            }
            binFile = hexFilename;
        }
    }
    else if (fileFormat(filename) == ".hex")
    {
        binFile = filename;
    }
    else
    {
        std::cout << "You sure that's the file?\n";
        return 1;
    }

    std::ifstream file(filePath(binFile));

    if (!file.is_open())
    {
        std::cout << "Erm the file didn't open. Idk does it actually exist?\n"; 
        return 1;
    }

    uint16_t instruction;
    uint16_t addr = 0;

    while (file >> std::hex >> instruction)
    {
        if (addr >= 1024)
        {
            std::cout << "Aw, what ashame. The program is too long. Recode it or something\n";
            return 1;
        }
        instruction_memory[addr] = instruction;
        addr++;
    }
    
    return 0;
}

std::string fileFormat(std::string filename)
{
    std::filesystem::path filepath = std::filesystem::absolute(filename); // this file path doesn't exist lol, this is just a dummy path so that .extension() works
    return filepath.extension().string();
}

std::string filePath(std::string filename)
{
    // please make sure the working directory is the main folder
    if (fileFormat(filename) == ".asm") return std::filesystem::absolute("programs/src/" + filename).string();
    else if (fileFormat(filename) == ".hex") return std::filesystem::absolute("programs/bin/" + filename).string();
    else return "";
}

/*
bool isRegister(const std::string& operand)
{
    int regAddr;
    if (operand.find("r") == 0)
    {
        try
        {
            regAddr = std::stoi(operand.substr(1), nullptr, 0);
        }
        catch(const std::exception& e)
        {
            return false;
        }
        
        if (regAddr < 16) return true;
    }
    else
    {
        return false;
    }
}
*/

std::string getOperandType(const std::string& operand, const std::string& instruction, const int& operandPos)
{
    if (registerLUT.find(operand) != registerLUT.end()) return "reg";
    else if ((instruction == "LDI" || instruction == "ADI") && (operandPos == 1) && isANumber(operand)) return "imm";
    else if ((instruction == "JMP" || instruction == "CALL") && (operandPos == 0) && isANumber(operand)) return "Addr10";
    else if ((instruction == "BRH") && (operandPos == 1) && isANumber(operand)) return "Addr10";
    else if ((instruction == "BRH") && (operandPos == 0) && (condLUT.find(operand) != condLUT.end())) return "Cond";
    else if ((instruction == "LOD" || instruction == "STR") && (operandPos == 1) && isANumber(operand)) return "Addr";
    else return "";
}

bool isANumber(const std::string& inString)
{
    try
    {
        int val;
        if (inString[0] == '0' && (inString[1] == 'b' || inString[1] == 'B')) val = std::stoi(inString, nullptr, 2);
        val = std::stoi(inString, nullptr, 0);
        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}

std::string DectoBin(int num, int bitWidth)
{
    std::string bin = "";
    while (num > 0)
    {
        bin = (char)('0' + (num % 2)) + bin;
        num /= 2;
    }
    while (bin.length() < bitWidth)
    {
        bin = "0" + bin;
    }

    return bin;
}

std::string BintoHex(const std::string& bin)
{
    std::string hex = "";

    int pad = (4 - (bin.length() % 4)) % 4;
    std::string padbin = std::string(pad, '0') + bin;

    for (int i = 0; i < padbin.length(); i += 4)
    {
        int nib = std::stoi(padbin.substr(i, 4), nullptr, 2);
        hex += "0123456789ABCDEF"[nib];
    }

    return hex;
}

bool assemble(const std::string& infile)
{
    // some assembler logic (open files, assemble input file, write to output file)
    // clears comments, records labels and definitions, then swaps them, the actually assembles them
    std::string outfile = std::filesystem::path(infile).replace_extension(".hex").string(); 

    std::ifstream srcfile(filePath(infile));

    if (!srcfile.is_open())
    {
        std::cout << "Idk why did the input file didn't open during assembly. Probably the CWD issue, read the README.\n";
        return false;
    }

    std::ofstream asmfile(filePath(outfile));

    if (!asmfile.is_open())
    {
        std::cout << "I'm not sure how the heck did the assembled file didn't open. Idk maybe it's the CWD issue? Read the README bro.\n";
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    std::string assembledLine;
    std::string assembledOperands;
    std::string hexAssembledLine;
    std::vector<std::string> hexAssembledLines;

    std::unordered_map<std::string, int> labelDict;
    std::unordered_map<std::string, int> defDict;

    while (std::getline(srcfile, line))
    {
        // clean the code (remove comments)
        size_t commentPos = line.find(";");
        if (commentPos != std::string::npos) line = line.substr(0, commentPos);
        if (!line.empty()) lines.push_back(line);
    }

    for (int i = 0; i < lines.size(); i++)
    {
        // find da labels and definitions
        line = lines[i];

        if (line[0] == '.' && line.substr(0, 7) != ".define")
        {
            std::string name = line.substr(1);

            size_t start = name.find_first_not_of(" \t");
            if (start != std::string::npos) name = name.substr(start);
            size_t end = name.find_last_not_of(" \t");
            if (end != std::string::npos) name = name.substr(0, end + 1);

            if (name.empty())
            {
                std::cout << "What am I supposed to do with this?\n";
                std::cout << line;
                return false;
            }

            labelDict[name] = i;
            lines.erase(lines.begin() + i);
            i--;
        }

        else if (line.substr(0, 7) == ".define")
        {
            size_t equalPos = line.find("=");
            if (equalPos == std::string::npos)
            {
                std::cout << "Hey buddy, there's no equals sign over here: " << line << std::endl;
                std::cout << "Fix the code pal.\n";
                return false;
            }
            std::string name = line.substr(7, equalPos - 7);
            int val;
            try
            {
                if (line[0] == '0' && (line[1] == 'b' || line[1] == 'B')) val = std::stoi(line.substr(equalPos + 1), nullptr, 2);
                val = std::stoi(line.substr(equalPos + 1), nullptr, 0);
            }
            catch (const std::invalid_argument&)
            {
                std::cout << "Who invited someone's NaN?\n";
                std::cout << "This ain't a number pal: " << line;
                return false;
            }

            size_t start = name.find_first_not_of(" \t");
            if (start != std::string::npos) name = name.substr(start);
            size_t end = name.find_last_not_of(" \t");
            if (end != std::string::npos) name = name.substr(0, end + 1);

            defDict[name] = val;
            lines.erase(lines.begin() + i);
            i--;
        }
    }

    for (int i = 0; i < lines.size(); i++)
    {
        // replace da labels and defs
        line = lines[i];

        size_t labelStart = line.find(".");
        if (labelStart != std::string::npos)
        {
            std::string name = line.substr(labelStart + 1);

            if (labelDict.find(name) == labelDict.end())
            {
                std::cout << "Is this supposed to be a label?\n" << line << std::endl;
                return false;
            }

            line = line.substr(0, labelStart) + std::to_string(labelDict[name]);
            lines[i] = line;
        }

        for (const auto& [name, val] : defDict)
        {
            size_t defPos = line.find(name);
            if (defPos != std::string::npos)
            {
                line = line.substr(0, defPos) + std::to_string(val) + line.substr(defPos + name.length());
                lines[i] = line;
            }
        }
    }

    for (const std::string& line : lines)
    {
        // finally, assemble it
        std::istringstream iss(line);
        std::string instruction;
        iss >> instruction;

        if (opcodeLUT.find(instruction) == opcodeLUT.end())
        {
            std::cout << "Have you read the ISA yet?\n";
            std::cout << "You should read it, idk what this is: " << instruction << std::endl;
            return false;
        }

        std::string opcode = opcodeLUT[instruction];

        std::vector<std::string> operands;
        std::string operand;

        assembledOperands = "";

        if (instruction == "NOP" || instruction == "HLT" || instruction == "RET") assembledOperands += "000000000000";
        if (instruction == "JMP" || instruction == "CALL") assembledOperands += "00";

        while (iss >> operand) operands.push_back(operand);
        for (int i = 0; i < operands.size(); i++)
        {
            std::vector<std::string> expectedOperandTypes = operandsLUT.at(instruction);
            if (operands.size() > operandsLUT.at(instruction).size())
            {
                std::cout << "Hey pal I think you've put too many operands here.\n";
                std::cout << line;
                return false;
            }
            if (operands.size() < operandsLUT.at(instruction).size())
            {
                std::cout << "I think there's not enough operands there buddy.\n";
                std::cout << line;
                return false;
            }
            if (getOperandType(operands[i], instruction, i) != expectedOperandTypes.at(i)) 
            {
                std::cout << "This is not how you use this instruction bro.\n";
                std::cout << line;
                return false;
            }

            if (getOperandType(operands[i], instruction, i) == "reg")
            {
                if (i == 2 && (instruction == "ADD" || instruction == "SUB" || instruction == "NAND" || instruction == "OR" || instruction == "XOR")) assembledOperands += registerLUT.at(operands[i]) + "000";
                else if (i == 1 && instruction == "RSH") assembledOperands += registerLUT.at(operands[i]) + "000000";
                else assembledOperands += registerLUT.at(operands[i]);
            }

            if (getOperandType(operands[i], instruction, i) == "imm")
            {
                if (operands[i][0] == '0' && (operands[i][1] == 'b' || operands[i][1]== 'B'))
                {
                    assembledOperands += DectoBin(std::stoi(operands[i].substr(2), nullptr, 2), 8) + "0";
                }
                else
                {
                    assembledOperands += DectoBin(std::stoi(operands[i], nullptr, 0), 8) + "0";
                }
            }

            if (getOperandType(operands[i], instruction, i) == "Addr10")
            {
                if (operands[i][0] == '0' && (operands[i][1] == 'b' || operands[i][1] == 'B'))
                {
                    assembledOperands += DectoBin(std::stoi(operands[i].substr(2), nullptr, 2), 10);
                }
                else
                {
                    assembledOperands += DectoBin(std::stoi(operands[i], nullptr, 0), 10);
                }
            }

            if (getOperandType(operands[i], instruction, i) == "Cond")
            {
                assembledOperands += condLUT.at(operands[i]);
            }

            if (getOperandType(operands[i], instruction, i) == "Addr")
            {
                if (operands[i][0] == '0' && (operands[i][1] == 'b' || operands[i][1] == 'B'))
                {
                    assembledOperands += DectoBin(std::stoi(operands[i].substr(2), nullptr, 2), 8) + "0";
                }
                else
                {
                    assembledOperands += DectoBin(std::stoi(operands[i], nullptr, 0), 8) + "0";
                }
            }
        }

        assembledLine = opcode + assembledOperands;

        hexAssembledLine = BintoHex(assembledLine);
        hexAssembledLines.push_back(hexAssembledLine);
    }
    
    for (const std::string& hexline : hexAssembledLines)
    {
        asmfile << hexline << "\n";
    }

    return true;
}