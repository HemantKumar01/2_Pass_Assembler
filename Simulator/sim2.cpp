#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <conio.h>
#include <iomanip>
#include <memory>

// ANSI escape codes for colors and formatting
namespace Colors
{
  const std::string RESET = "\x1B[0m";
  const std::string BOLD = "\x1B[1m";
  const std::string UNDERLINE = "\x1B[4m";
  const std::string ITALIC = "\x1B[3m";
  const std::string WHITE = "\x1B[37m";
  const std::string RED = "\x1B[31m";
  const std::string GREEN = "\x1B[32m";
  const std::string YELLOW = "\x1B[33m";
  const std::string BLUE = "\x1B[34m";
  const std::string MAGENTA = "\x1B[35m";
  const std::string CYAN = "\x1B[36m";
  const std::string MAGENTA_BG = "\x1B[45m";
  const std::string CYAN_BG = "\x1B[46m";
  const std::string WHITE_BG = "\x1B[47m";
}

// Banner text as a const string
const std::string BANNER_TEXT = R"(
 ██████╗███████╗ ██╗ ██████╗ ██████╗
██╔════╝██╔════╝███║██╔═████╗╚════██╗
██║     ███████╗╚██║██║██╔██║ █████╔╝
██║     ╚════██║ ██║████╔╝██║██╔═══╝
╚██████╗███████║ ██║╚██████╔╝███████╗
 ╚═════╝╚══════╝ ╚═╝ ╚═════╝ ╚══════╝

██████╗ ██████╗  ██████╗      ██╗███████╗ ██████╗████████╗
██╔══██╗██╔══██╗██╔═══██╗     ██║██╔════╝██╔════╝╚══██╔══╝
██████╔╝██████╔╝██║   ██║     ██║█████╗  ██║        ██║
██╔═══╝ ██╔══██╗██║   ██║██   ██║██╔══╝  ██║        ██║
██║     ██║  ██║╚██████╔╝╚█████╔╝███████╗╚██████╗   ██║
╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚════╝ ╚══════╝ ╚═════╝   ╚═╝

)";

class MenuItem
{
private:
  std::string gameName;
  std::string gamePath;
  std::string creator;
  bool isGraphics;

public:
  MenuItem(const std::string &name, const std::string &path,
           const std::string &creator, bool graphics)
      : gameName(name), gamePath(path), creator(creator), isGraphics(graphics) {}

  std::string getOptionString() const
  {
    std::string optionString = gameName;
    int spacesRemaining = 20 - gameName.length();
    optionString += std::string(spacesRemaining, ' ');

    optionString += Colors::YELLOW;
    std::string type = isGraphics ? "Graphics" : "Terminal";
    optionString += type;

    spacesRemaining = 10 - type.length();
    optionString += std::string(spacesRemaining, ' ');

    optionString += Colors::WHITE + creator;
    return optionString;
  }

  const std::string &getGamePath() const { return gamePath; }
};

class GameMenu
{
private:
  std::vector<MenuItem> menuItems;
  int currentlySelectedItem;

  void printPreText() const
  {
    system("cls");
    std::cout << Colors::CYAN << BANNER_TEXT << Colors::RESET;
    std::cout << Colors::GREEN << Colors::BOLD
              << "Project Group 20\n\n"
              << Colors::RESET;
  }

public:
  GameMenu() : currentlySelectedItem(0)
  {
    // Initialize menu items
    menuItems = {
        MenuItem("Pong Game", "pong.exe", "Hemant Kumar", true),
        MenuItem("Breakout Game", "breakout.exe", "Sasmit Shaswat", true),
        MenuItem("Snake Game", "snake_g.exe", "Vidhan Bansal", true),
        MenuItem("Snake Game", "snake_t.exe", "Prasad Mhaske", false),
        MenuItem("Tic-Tac-Toe", "tictactoe.exe",
                 "Namrata Patel & Aravind Chowdary", true),
        MenuItem("Rock Paper Scissor", "rockpaper.exe", "Priyanshu", false),
        MenuItem("Catch The Apple", "apple.exe", "Korak Basu", true),
        MenuItem("Hangman", "hangman.exe", "Anish and Sandeep", false)};
  }

  void show()
  {
    while (true)
    {
      printPreText();
      std::cout << Colors::YELLOW << "Which Game Do You Want to Play?"
                << Colors::RESET << "\n\n";

      for (size_t i = 0; i < menuItems.size(); i++)
      {
        if (i == currentlySelectedItem)
        {
          std::cout << Colors::WHITE << Colors::MAGENTA_BG
                    << menuItems[i].getOptionString()
                    << Colors::RESET << "\n";
        }
        else
        {
          std::cout << Colors::CYAN << menuItems[i].getOptionString()
                    << Colors::RESET << "\n";
        }
      }

      char key = _getch();
      if (key == -32)
      { // Special key
        char key2 = _getch();
        if (key2 == 80)
        { // Down arrow
          currentlySelectedItem = std::min(static_cast<int>(menuItems.size() - 1),
                                           currentlySelectedItem + 1);
        }
        else if (key2 == 72)
        { // Up arrow
          currentlySelectedItem = std::max(0, currentlySelectedItem - 1);
        }
      }
      else if (key == 13)
      { // Enter key
        std::string command = "cd dist && " +
                              menuItems[currentlySelectedItem].getGamePath();
        system(command.c_str());
      }
      else if (key == 27)
      { // Escape key
        return;
      }
    }
  }
};

int main()
{
  std::locale::global(std::locale("en_US.UTF-8"));
  GameMenu menu;
  menu.show();
  return 0;
}