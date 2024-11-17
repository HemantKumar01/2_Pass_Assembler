/*****************************************************************************
TITLE: Claims
AUTHOR: Hemant Kumar (2301CS20)
Declaration of Authorship
This emu.cpp file, is part of the miniproject assignment of course CS2102 at CSE Dept. IIT Patna
*****************************************************************************/
#include <bits/stdc++.h>
using namespace std;
class EmulatorState
{
private:
  vector<uint32_t> program_memory;
  unordered_map<string, pair<uint8_t, uint8_t>> instruction_set;
  uint32_t accumulator;
  uint32_t backup_reg;
  uint32_t stack_ptr;
  uint32_t program_counter;
  size_t instruction_count;

  struct MemoryOperation
  {
    uint32_t address;
    uint32_t old_value;
    MemoryOperation() : address(0), old_value(0) {} // Initialize to prevent warnings
  } last_mem_op;

public:
  EmulatorState() : accumulator(0),
                    backup_reg(0),
                    stack_ptr(0),
                    program_counter(0),
                    instruction_count(0)
  {
    initialize_instruction_set();
  }

  void initialize_instruction_set()
  {
    // Instruction format: name, opcode, operand_type
    // operand_type: 0=none, 1=value, 2=offset
    const vector<tuple<string, uint8_t, uint8_t>> instructions = {
        make_tuple("push_const", 0x00, 1),
        make_tuple("add_const", 0x01, 1),
        make_tuple("load_local", 0x02, 2),
        make_tuple("store_local", 0x03, 2),
        make_tuple("load_indirect", 0x04, 2),
        make_tuple("store_indirect", 0x05, 2),
        make_tuple("sum", 0x06, 0),
        make_tuple("difference", 0x07, 0),
        make_tuple("shift_left", 0x08, 0),
        make_tuple("shift_right", 0x09, 0),
        make_tuple("stack_adjust", 0x0A, 1),
        make_tuple("acc_to_stack", 0x0B, 0),
        make_tuple("stack_to_acc", 0x0C, 0),
        make_tuple("procedure_call", 0x0D, 2),
        make_tuple("procedure_return", 0x0E, 0),
        make_tuple("branch_zero", 0x0F, 2),
        make_tuple("branch_negative", 0x10, 2),
        make_tuple("branch_always", 0x11, 2),
        make_tuple("terminate", 0x12, 0)};

    for (const auto &inst : instructions)
    {
      instruction_set[get<0>(inst)] = make_pair(get<1>(inst), get<2>(inst));
    }
  }

  bool load_program(const string &filename)
  {
    ifstream input(filename.c_str(), ios::binary); // Use c_str() for C++11
    if (!input)
      return false;

    uint32_t instruction;
    while (input.read(reinterpret_cast<char *>(&instruction), sizeof(uint32_t)))
    {
      program_memory.push_back(instruction);
    }
    return true;
  }

  void execute_instruction(uint32_t instruction, bool trace_mode)
  {
    uint8_t opcode = instruction & 0xFF;
    int32_t operand = static_cast<int32_t>(instruction) >> 8;
    instruction_count++;

    switch (opcode)
    {
    case 0x00: // push_const
      backup_reg = accumulator;
      accumulator = static_cast<uint32_t>(operand);
      break;
    case 0x01: // add_const
      accumulator = static_cast<uint32_t>(static_cast<int32_t>(accumulator) + operand);
      break;
    case 0x02: // load_local
      backup_reg = accumulator;
      accumulator = program_memory[stack_ptr + static_cast<uint32_t>(operand)];
      last_mem_op.address = stack_ptr + static_cast<uint32_t>(operand);
      last_mem_op.old_value = 0;
      break;
    case 0x03: // store_local
    {
      uint32_t addr = stack_ptr + static_cast<uint32_t>(operand);
      last_mem_op.address = addr;
      last_mem_op.old_value = program_memory[addr];
      program_memory[addr] = accumulator;
      accumulator = backup_reg;
    }
    break;
    case 0x04: // load_indirect
    {
      uint32_t addr = accumulator + static_cast<uint32_t>(operand);
      accumulator = program_memory[addr];
      last_mem_op.address = addr;
      last_mem_op.old_value = 0;
    }
    break;
    case 0x05: // store_indirect
    {
      uint32_t addr = accumulator + static_cast<uint32_t>(operand);
      last_mem_op.address = addr;
      last_mem_op.old_value = program_memory[addr];
      program_memory[addr] = backup_reg;
    }
    break;
    case 0x06: // sum
      accumulator = static_cast<uint32_t>(static_cast<int32_t>(accumulator) + static_cast<int32_t>(backup_reg));
      break;
    case 0x07: // difference
      accumulator = static_cast<uint32_t>(static_cast<int32_t>(backup_reg) - static_cast<int32_t>(accumulator));
      break;
    case 0x08:                                          // shift_left
      accumulator = backup_reg << (accumulator & 0x1F); // Prevent undefined behavior
      break;
    case 0x09:                                          // shift_right
      accumulator = backup_reg >> (accumulator & 0x1F); // Prevent undefined behavior
      break;
    case 0x0A: // stack_adjust
      stack_ptr = static_cast<uint32_t>(static_cast<int32_t>(stack_ptr) + operand);
      break;
    case 0x0B: // acc_to_stack
      stack_ptr = accumulator;
      accumulator = backup_reg;
      break;
    case 0x0C: // stack_to_acc
      backup_reg = accumulator;
      accumulator = stack_ptr;
      break;
    case 0x0D: // procedure_call
      backup_reg = accumulator;
      accumulator = program_counter;
      program_counter = static_cast<uint32_t>(static_cast<int32_t>(program_counter) + operand);
      break;
    case 0x0E: // procedure_return
      program_counter = accumulator;
      accumulator = backup_reg;
      break;
    case 0x0F: // branch_zero
      if (accumulator == 0)
      {
        program_counter = static_cast<uint32_t>(static_cast<int32_t>(program_counter) + operand);
      }
      break;
    case 0x10: // branch_negative
      if (static_cast<int32_t>(accumulator) < 0)
      {
        program_counter = static_cast<uint32_t>(static_cast<int32_t>(program_counter) + operand);
      }
      break;
    case 0x11: // branch_always
      program_counter = static_cast<uint32_t>(static_cast<int32_t>(program_counter) + operand);
      break;
    case 0x12: // terminate
      program_counter = static_cast<uint32_t>(program_memory.size());
      break;
    default:
      break;
    }

    if (trace_mode)
    {
      print_trace(opcode, operand);
    }
  }

  string format_hex(uint32_t value) const
  {
    ostringstream ss;
    ss << uppercase << setfill('0') << setw(8) << hex << value;
    return ss.str();
  }

  void print_trace(uint8_t opcode, int32_t operand) const
  {
    cout << "PC=" << format_hex(program_counter - 1)
              << " SP=" << format_hex(stack_ptr)
              << " A=" << format_hex(accumulator)
              << " B=" << format_hex(backup_reg) << " ";

    for (const auto &pair : instruction_set)
    {
      if (pair.second.first == opcode)
      {
        cout << pair.first;
        if (pair.second.second > 0)
        {
          cout << " " << format_hex(static_cast<uint32_t>(operand));
        }
        break;
      }
    }
    cout << endl;
  }

  void memory_dump() const
  {
    for (size_t i = 0; i < program_memory.size(); i += 4)
    {
      cout << format_hex(static_cast<uint32_t>(i)) << " ";
      for (size_t j = i; j < min(program_memory.size(), i + 4); j++)
      {
        cout << format_hex(program_memory[j]) << " ";
      }
      cout << endl;
    }
  }

  void execute(const string &mode)
  {
    bool trace = (mode == "-trace");
    bool read_mode = (mode == "-read");
    bool write_mode = (mode == "-write");

    while (program_counter < program_memory.size())
    {
      uint32_t current = program_memory[program_counter++];
      execute_instruction(current, trace);

      uint8_t opcode = current & 0xFF;
      if (read_mode && (opcode == 0x02 || opcode == 0x04))
      {
        cout << "Reading memory[" << format_hex(last_mem_op.address)
                  << "] finds " << format_hex(accumulator) << endl;
      }
      if (write_mode && (opcode == 0x03 || opcode == 0x05))
      {
        cout << "Writing memory[" << format_hex(last_mem_op.address)
                  << "] was " << format_hex(last_mem_op.old_value)
                  << " now " << format_hex(program_memory[last_mem_op.address]) << endl;
      }
      if (opcode >= 0x12)
        break;
    }
  }

  void reset()
  {
    accumulator = 0;
    backup_reg = 0;
    stack_ptr = 0;
    program_counter = 0;
  }

  size_t get_instruction_count() const
  {
    return instruction_count;
  }
};

int main(int argc, char *argv[])
{
  if (argc <= 2)
  {
    cout << "Usage: " << argv[0] << " [mode] filename.o\n"
              << "Modes:\n"
              << "  -trace   Show instruction trace\n"
              << "  -read    Show memory reads\n"
              << "  -write   Show memory writes\n"
              << "  -before  Show memory dump before execution\n"
              << "  -after   Show memory dump after execution\n"
              << "  -wipe    Reset registers before execution\n";
    return 1;
  }

  EmulatorState emu;
  string mode(argv[1]);
  string filename(argv[2]);

  if (!emu.load_program(filename))
  {
    cout << "Failed to load program file\n";
    return 1;
  }

  if (mode == "-before")
  {
    cout << "memory dump before execution\n";
    emu.memory_dump();
  }
  else if (mode == "-wipe")
  {
    emu.reset();
  }
  else
  {
    emu.execute(mode);
  }

  if (mode == "-after")
  {
    cout << "memory dump after execution\n";
    emu.memory_dump();
  }

  cout << emu.get_instruction_count() << " instructions executed\n";
  return 0;
}