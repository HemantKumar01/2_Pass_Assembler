#include <bits/stdc++.h>
#include <windows.h>
#include <cstdio>

using namespace std;

class TerminalUI
{
private:
  HANDLE hConsole;
  HANDLE hInput;
  COORD cursorPosition;
  vector<string> assembly_code;
  vector<string> memory_segment;
  struct Registers
  {
    uint32_t A;
    uint32_t B;
    uint32_t SP;
    uint32_t PC;
  } registers;

  int current_line;
  int terminal_height;
  int terminal_width;

  // Color constants for Windows console
  enum ConsoleColors
  {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHTGRAY = 7,
    DARKGRAY = 8,
    LIGHTBLUE = 9,
    LIGHTGREEN = 10,
    LIGHTCYAN = 11,
    LIGHTRED = 12,
    LIGHTMAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
  };

  void setCursorPosition(int x, int y)
  {
    cursorPosition.X = x;
    cursorPosition.Y = y;
    SetConsoleCursorPosition(hConsole, cursorPosition);
  }

  void setConsoleColor(int textColor, int bgColor = BLACK)
  {
    SetConsoleTextAttribute(hConsole, textColor | (bgColor << 4));
  }

  void loadAssemblyCode(const string &filename)
  {
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
      assembly_code.push_back(line);
    }
  }

  void loadMemorySegment(const string &filename)
  {
    // For demonstration, loading some dummy memory data
    memory_segment.push_back("0x0000: 48 65 6C 6C 6F 20 57 6F");
    memory_segment.push_back("0x0008: 72 6C 64 21 00 00 00 00");
    // Add more dummy data as needed
  }

  void clearScreen()
  {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD written;
    DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
    COORD home = {0, 0};
    FillConsoleOutputCharacter(hConsole, ' ', cells, home, &written);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cells, home, &written);
    SetConsoleCursorPosition(hConsole, home);
  }

  bool handleInput()
  {
    INPUT_RECORD inputRecord;
    DWORD numEvents;

    ReadConsoleInput(hInput, &inputRecord, 1, &numEvents);

    if (numEvents > 0 && inputRecord.EventType == KEY_EVENT &&
        inputRecord.Event.KeyEvent.bKeyDown)
    {

      switch (inputRecord.Event.KeyEvent.wVirtualKeyCode)
      {
      case VK_ESCAPE:
      case 'Q':
        return false;
      case VK_UP:
        if (current_line > 0)
          current_line--;
        break;
      case VK_DOWN:
        if (current_line < (int)assembly_code.size() - 1)
          current_line++;
        break;
      }
    }
    return true;
  }

public:
  TerminalUI(const string &filename) : current_line(0), registers{0, 0, 0, 0}
  {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    hInput = GetStdHandle(STD_INPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(hInput, &mode);
    SetConsoleMode(hInput, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    // Configure console cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // Hide cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // Get console dimensions
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    terminal_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    terminal_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    loadAssemblyCode(filename);
    loadMemorySegment(filename);

    // Set console title
    SetConsoleTitle("Assembly Debugger");
  }

  void display()
  {
    clearScreen();

    // Display assembly section (top third)
    int assembly_height = terminal_height / 3;
    for (int i = 0; i < assembly_height && i < (int)assembly_code.size(); i++)
    {
      setCursorPosition(0, i);
      if (i == current_line)
      {
        setConsoleColor(BLACK, LIGHTGRAY); // Highlighted line
        cout << (assembly_code[i] + string(terminal_width - assembly_code[i].length(), ' '));
        setConsoleColor(WHITE, BLACK); // Reset colors
      }
      else
      {
        setConsoleColor(WHITE, BLACK);
        cout << assembly_code[i];
      }
    }

    // Display registers (middle section)
    int reg_start = assembly_height + 1;
    setCursorPosition(0, reg_start);
    setConsoleColor(YELLOW, BLACK);
    printf("A: 0x%08X    B: 0x%08X", registers.A, registers.B);
    setCursorPosition(0, reg_start + 1);
    printf("SP: 0x%08X    PC: 0x%08X", registers.SP, registers.PC);

    // Display memory segment (bottom third)
    setConsoleColor(LIGHTGREEN, BLACK);
    int mem_start = reg_start + 3;
    for (int i = 0; i < (terminal_height - mem_start - 1) && i < (int)memory_segment.size(); i++)
    {
      setCursorPosition(0, mem_start + i);
      cout << memory_segment[i];
    }

    // Display controls help
    setConsoleColor(LIGHTCYAN, BLACK);
    setCursorPosition(0, terminal_height - 1);
    cout << "Controls: Up/Down Arrow: Navigate  Q/ESC: Quit";
  }

  void run()
  {
    bool running = true;
    while (running)
    {
      display();

      // Process all pending input events
      DWORD numEvents;
      GetNumberOfConsoleInputEvents(hInput, &numEvents);
      while (numEvents > 0)
      {
        running = handleInput();
        GetNumberOfConsoleInputEvents(hInput, &numEvents);
      }

      // Small sleep to prevent high CPU usage
      Sleep(50);
    }
  }

  void updateRegisters(uint32_t A, uint32_t B, uint32_t SP, uint32_t PC)
  {
    registers = {A, B, SP, PC};
  }

  ~TerminalUI()
  {
    // Restore console cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // Reset colors
    setConsoleColor(WHITE, BLACK);

    // Clear screen on exit
    clearScreen();
  }
};

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " <assembly_file>" << endl;
    return 1;
  }

  try
  {
    TerminalUI ui(argv[1]);
    ui.run();
  }
  catch (const exception &e)
  {
    cerr << "Error: " << e.what() << endl;
    return 1;
  }

  return 0;
}