#include <string>
#include <iostream>
#include <fstream>
#include <vector>

typedef uint8_t byte;

enum OutputType {
  BINFILE,
  CARRAYFILE
} outType;

enum Opcode {
  LDI   = 0b0000,
  LDM   = 0b0001,
  ST    = 0b0010,
  ADR   = 0b0011,
  DIR   = 0b0100,
  ADM   = 0b0101,
  DIM   = 0b0110,
  PCR   = 0b0111,
  HALT  = 0b1000
};

enum ArgType {
  IMMEDIATE,
  OPERAND,
  NONE
};

struct OpDescription {
  Opcode opcode;
  std::string mnemonic;
  ArgType arg1Type;
  ArgType arg2Type;
};

struct RegDescription {
  byte id;
  std::string name;
};

struct Instruction {
  OpDescription* opDesc;
  byte arg1;
  byte arg2;
};

OpDescription operations[] = {
  { LDI,  "LDI",  OPERAND,    IMMEDIATE },
  { LDM,  "LDM",  OPERAND,    IMMEDIATE },
  { ST,   "ST",   IMMEDIATE,  OPERAND   },
  { ADR,  "ADR",  OPERAND,    NONE      },
  { DIR,  "DIR",  OPERAND,    NONE      },
  { ADM,  "ADM",  IMMEDIATE,  NONE      },
  { DIM,  "DIM",  IMMEDIATE,  NONE      },
  { PCR,  "PCR",  NONE,       NONE      },
  { HALT, "HALT", NONE,       NONE      }
};
const int numOps = sizeof(operations) / sizeof(OpDescription);

OpDescription* findOp(const std::string& mnemonic) {
  for(int i = 0; i <= numOps; i++) {
    if(mnemonic == operations[i].mnemonic) {
      return operations + i;
    }
  }
  return nullptr;
}

size_t findFirstOfEither(const std::string& str, const std::string& characters) {
  size_t pos = std::string::npos;
  for(size_t i = 0; i < str.length(); i++) {
    for(size_t j = 0; j < characters.length(); i++) {
      if(str[i] == characters[j]) return i;
    }
  }
  return pos;
}

int main(int argc, char *argv[]) {
  if(argc < 3) {
    std::cout << "Usage: easm [type] [file]" << std::endl;
    return 1;
  }

  std::string type = argv[1];
  if(type == "C") {
    outType = CARRAYFILE;
  } else if(type == "BIN") {
    outType = BINFILE;
  } else {
    std::cerr << "type must be one of C or BIN" << std::endl;
    return 1;
  }

  std::string fileName = argv[2];
  std::ifstream infile(fileName);
  if(!infile) {
    std::cerr << "Error opening " << fileName << std::endl;
  }

  std::vector<Instruction> program;

  unsigned int lineNum = 0;
  std::string line, mnemonic;
  while(std::getline(infile, line)) {
    lineNum++;
    
    if(line[0] == '#' || line.size() == 0) continue;

    size_t mnemonicEnd = findFirstOfEither(line, " \n");
    if(mnemonicEnd == std::string::npos) {
      std::cerr << "Couldn't find end of mnemonic on line " << lineNum << std::endl;
      return 1;
    }

    mnemonic = line.substr(0, mnemonicEnd);
    OpDescription* op = findOp(mnemonic);
    if(op == nullptr) {
      std::cerr << "Unknown mnemonic: \"" << mnemonic <<  "\"" << std::endl;
      return 1;
    }

    unsigned char numArgs = op->arg1Type == NONE ? 0 : (op->arg2Type == NONE ? 1 : 2);
    if(numArgs == 0) {
      program.push_back({op, 0, 0});
    }

    if(numArgs == 2) {
      
    }
  }
}