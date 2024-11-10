/*****************************************************************************
TITLE: Claims
AUTHOR: Hemant Kumar (2301CS20)
Declaration of Authorship
This asm.cpp file, is part of the miniproject assignment of course CS2102 at CSE Dept. IIT Patna
*****************************************************************************/

#include <bits/stdc++.h>

std::string file_initial = "test";

class ProcessorLogic
{
private:
  class LineItem
  {
  public:
    int pc;
    std::string tag;
    std::string cmd;
    std::string arg;
    std::string origArg;

    LineItem(int p, std::string t, std::string c, std::string a, std::string o) : pc(p), tag(t), cmd(c), arg(a), origArg(o) {}
  };

  class IssueItem
  {
  public:
    int pos;
    std::string text;

    IssueItem(int p, std::string t) : pos(p), text(t) {}

    bool operator<(const IssueItem &other) const
    {
      return pos < other.pos;
    }
  };

  std::map<std::string, std::pair<int, int>> symTable;
  std::map<std::string, std::string> varTable;
  std::map<std::string, std::vector<int>> refTable;
  std::map<int, std::string> noteTable;
  std::map<std::string, std::pair<std::string, int>> cmdTable;

  std::vector<LineItem> codeLines;
  std::vector<std::string> binOutput;
  std::vector<std::tuple<std::string, std::string, std::string>> listOutput;
  std::vector<IssueItem> errorList;
  std::vector<IssueItem> warnList;

  bool verifyChar(char c, bool first) const
  {
    if (first)
      return std::isalpha(c) != 0;
    return std::isalnum(c) != 0 || c == '_';
  }

  bool verifyTag(const std::string &str) const
  {
    if (str.empty())
      return false;
    if (!verifyChar(str[0], true))
      return false;
    for (size_t i = 1; i < str.size(); ++i)
    {
      if (!verifyChar(str[i], false))
        return false;
    }
    return true;
  }

  std::string processValue(const std::string &val)
  {
    if (val.empty())
      return "";

    size_t start = 0;
    std::string prefix;
    if (val[0] == '+' || val[0] == '-')
    {
      prefix = val.substr(0, 1);
      start = 1;
      if (val.length() == 1)
        return "";
    }

    std::string num = val.substr(start);
    if (num[0] == '0')
    {
      if (num.length() == 1)
        return val;
      if (num.length() > 1 && (num[1] == 'x' || num[1] == 'X'))
      {
        return convertBase(num.substr(2), 16, prefix);
      }
      return convertBase(num.substr(1), 8, prefix);
    }

    for (char c : num)
    {
      if (!std::isdigit(c))
        return "";
    }
    return val;
  }

  std::string convertBase(const std::string &num, int base, const std::string &prefix)
  {
    int result = 0;
    for (char c : num)
    {
      if (base == 16)
      {
        if (!std::isxdigit(c))
          return "";
        result = result * 16 + (std::isdigit(c) ? c - '0' : (std::tolower(c) - 'a' + 10));
      }
      else
      {
        if (c < '0' || c > '7')
          return "";
        result = result * 8 + (c - '0');
      }
    }
    return prefix + std::to_string(result);
  }

  std::string toHexStr(int val) const
  {
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(8) << std::uppercase << std::hex
       << static_cast<unsigned int>(val);
    return ss.str();
  }

  void logError(int line, const std::string &msg)
  {
    errorList.push_back(IssueItem(line, msg));
  }

  void logWarning(int line, const std::string &msg)
  {
    warnList.push_back(IssueItem(line, msg));
  }

public:
  ProcessorLogic()
  {
    setupCommands();
  }

  void setupCommands()
  {
    const std::vector<std::tuple<std::string, std::string, int>> cmds = {
        {"data", "", 1}, {"ldc", "00", 1}, {"adc", "01", 1}, {"ldl", "02", 2}, {"stl", "03", 2}, {"ldnl", "04", 2}, {"stnl", "05", 2}, {"add", "06", 0}, {"sub", "07", 0}, {"shl", "08", 0}, {"shr", "09", 0}, {"adj", "0A", 1}, {"a2sp", "0B", 0}, {"sp2a", "0C", 0}, {"call", "0D", 2}, {"return", "0E", 0}, {"brz", "0F", 2}, {"brlz", "10", 2}, {"br", "11", 2}, {"HALT", "12", 0}, {"SET", "", 1}};

    for (const auto &cmd : cmds)
    {
      cmdTable[std::get<0>(cmd)] = std::make_pair(std::get<1>(cmd), std::get<2>(cmd));
    }
  }

  std::vector<std::string> tokenizeLine(const std::string &line)
  {
    std::vector<std::string> tokens;
    std::string current;
    bool inComment = false;

    for (size_t i = 0; i < line.length(); ++i)
    {
      char c = line[i];
      if (c == ';')
      {
        if (!current.empty())
        {
          tokens.push_back(current);
          current.clear();
        }
        inComment = true;
        continue;
      }

      if (inComment)
        continue;

      if (std::isspace(c))
      {
        if (!current.empty())
        {
          tokens.push_back(current);
          current.clear();
        }
        continue;
      }

      if (c == ':')
      {
        current += ':';
        tokens.push_back(current);
        current.clear();
        continue;
      }

      current += c;
    }

    if (!current.empty())
      tokens.push_back(current);
    return tokens;
  }

  void analyzeCode(const std::vector<std::string> &code)
  {
    int lineNum = 0;
    int pc = 0;

    for (const auto &line : code)
    {
      ++lineNum;
      auto tokens = tokenizeLine(line);
      if (tokens.empty())
        continue;

      std::string tag, cmd, param;
      size_t pos = 0;

      if (tokens[0].back() == ':')
      {
        tag = tokens[0];
        tag.pop_back();
        ++pos;
      }

      if (pos < tokens.size())
        cmd = tokens[pos++];
      if (pos < tokens.size())
        param = tokens[pos++];

      bool validCmd = false;
      std::string processedParam = param;

      if (!tag.empty())
      {
        if (!verifyTag(tag))
        {
          logError(lineNum, "Invalid label format");
        }
        else if (symTable.count(tag) && symTable[tag].first != -1)
        {
          logError(lineNum, "Duplicate label");
        }
        else
        {
          symTable[tag] = std::make_pair(pc, lineNum);
        }
      }

      if (!cmd.empty())
      {
        if (cmdTable.count(cmd))
        {
          int paramType = cmdTable[cmd].second;
          if (paramType > 0)
          {
            if (param.empty())
            {
              logError(lineNum, "Missing parameter");
            }
            else if (pos < tokens.size())
            {
              logError(lineNum, "Extra tokens");
            }
            else
            {
              std::string processed = verifyTag(param) ? param : processValue(param);

              if (processed.empty())
              {
                logError(lineNum, "Invalid parameter");
              }
              else
              {
                processedParam = processed;
                validCmd = true;
              }
            }
          }
          else
          {
            if (!param.empty())
            {
              logError(lineNum, "Unexpected parameter");
            }
            else
            {
              validCmd = true;
            }
          }
        }
        else
        {
          logError(lineNum, "Unknown command");
        }
      }

      codeLines.push_back(LineItem(pc, tag, cmd, processedParam, param));
      if (validCmd)
      {
        if (cmd == "SET")
        {
          if (tag.empty())
          {
            logError(lineNum, "SET requires label");
          }
          else
          {
            varTable[tag] = processedParam;
          }
        }
        ++pc;
      }
    }
  }

  void generateCode()
  {
    for (const auto &line : codeLines)
    {
      std::string machineCode = "        ";

      if (!line.cmd.empty() && cmdTable.count(line.cmd))
      {
        int type = cmdTable[line.cmd].second;
        std::string opcode = cmdTable[line.cmd].first;

        if (type == 2)
        {
          int offset = symTable.count(line.arg) ? symTable[line.arg].first - (line.pc + 1) : std::stoi(line.arg);
          machineCode = toHexStr(offset).substr(2) + opcode;
        }
        else if (type == 1 && line.cmd != "data" && line.cmd != "SET")
        {
          int value = symTable.count(line.arg) ? symTable[line.arg].first : std::stoi(varTable.count(line.arg) ? varTable[line.arg] : line.arg);
          machineCode = toHexStr(value).substr(2) + opcode;
        }
        else if (type == 1 && (line.cmd == "data" || line.cmd == "SET"))
        {
          machineCode = toHexStr(std::stoi(line.arg));
        }
        else if (type == 0)
        {
          machineCode = "000000" + opcode;
        }
      }

      binOutput.push_back(machineCode);
      std::string statement = (line.tag.empty() ? "" : line.tag + ": ") +
                              (line.cmd.empty() ? "" : line.cmd + " ") +
                              line.origArg;
      listOutput.push_back(std::make_tuple(toHexStr(line.pc), machineCode, statement));
    }
  }

  void writeOutput()
  {
    std::ofstream listFile(file_initial + ".lst");
    for (const auto &entry : listOutput)
    {
      listFile << std::get<0>(entry) << " " << std::get<1>(entry) << " "
               << std::get<2>(entry) << std::endl;
    }
    listFile.close();

    std::ofstream binFile(file_initial + ".o", std::ios::binary);
    for (const auto &code : binOutput)
    {
      if (code == "        ")
        continue;
      unsigned int value;
      std::istringstream(code) >> std::hex >> value;
      binFile.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }
    binFile.close();

    std::ofstream logFile(file_initial + ".log");
    if (errorList.empty())
    {
      logFile << "No errors!" << std::endl;
      for (const auto &warn : warnList)
      {
        logFile << "Line: " << warn.pos << " WARNING: "
                << warn.text << std::endl;
      }
    }
    else
    {
      for (const auto &err : errorList)
      {
        logFile << "Line: " << err.pos << " ERROR: "
                << err.text << std::endl;
      }
    }
    logFile.close();
  }

  bool execute(const std::string &filename)
  {
    std::ifstream input(filename);
    if (!input)
    {
      std::cout << "Failed to open input file" << std::endl;
      return false;
    }

    std::vector<std::string> sourceCode;
    std::string line;
    while (std::getline(input, line))
    {
      sourceCode.push_back(line);
    }
    input.close();

    analyzeCode(sourceCode);

    if (errorList.empty())
    {
      generateCode();
      writeOutput();
      std::cout << "Generated: lst, .o, and .log files" << std::endl;
      return true;
    }

    writeOutput();
    return false;
  }
};

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::string filename = argv[1];
  file_initial = filename.substr(0, filename.find('.'));
  ProcessorLogic processor;
  processor.execute(filename);

  return 0;
}