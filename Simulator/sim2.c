#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

#define RESET "\x1B[0m"
#define BOLD "\x1B[1m"
#define UNDERLINE "\x1B[4m"
#define ITALIC "\x1B[3m"
#define CLR_WHITE "\x1B[37m"
#define CLR_RED "\x1B[31m"
#define CLR_GREEN "\x1B[32m"
#define CLR_YELLOW "\x1B[33m"
#define CLR_BLUE "\x1B[34m"
#define CLR_MAGENTA "\x1B[35m"
#define CLR_CYAN "\x1B[36m"
#define CLR_WHITE "\x1B[37m"
#define CLR_MAGENTA_BG "\x1B[45m"
#define CLR_CYAN_BG "\x1B[46m"
#define CLR_WHITE_BG "\x1B[47m"

char *banner_text = "\n"
                    " ██████╗███████ ██████╗╗ ██╗ ██████╗ ██████╗\n"
                    "██╔════╝██╔════╝╚════██╗███║██╔═████╗╚════██╗\n"
                    "██║     ███████╗ █████╔╝╚██║██║██╔██║ █████╔╝\n"
                    "██║     ╚════██║██╔═══╝  ██║████╔╝██║██╔═══╝\n"
                    "╚██████╗███████║███████╗ ██║╚██████╔╝███████╗\n"
                    " ╚═════╝╚══════╝ ╚══════╝ ╚═╝ ╚═════╝ ╚══════╝\n"
                    "\n"
                    "██████╗ ██████╗  ██████╗      ██╗███████╗ ██████╗████████╗\n"
                    "██╔══██╗██╔══██╗██╔═══██╗     ██║██╔════╝██╔════╝╚══██╔══╝\n"
                    "██████╔╝██████╔╝██║   ██║     ██║█████╗  ██║        ██║\n"
                    "██╔═══╝ ██╔══██╗██║   ██║██   ██║██╔══╝  ██║        ██║\n"
                    "██║     ██║  ██║╚██████╔╝╚█████╔╝███████╗╚██████╗   ██║\n"
                    "╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚════╝ ╚══════╝ ╚═════╝   ╚═╝\n"
                    "\n"
                    "\n";

#define NUM_GAMES 8
#define MAX_LINES 1000
#define LINES_PER_PAGE 10
#define MAX_LINE_LENGTH 256

typedef struct
{
  char **lines;
  int totalLines;
  int currentLine;
} FileContent;

typedef struct
{
  unsigned int pc;
  unsigned int sp;
  unsigned int a;
  unsigned int b;
  char instruction[20];
  unsigned int operand;
} TraceEntry;
TraceEntry *trace;
int trace_count;

typedef struct
{
  char **entries;
  int num_entries;
} MemoryDump;
MemoryDump memoryDumps;

void printPreText()
{
  system("cls");
  printf("%s%s%s", CLR_CYAN, banner_text, RESET);
  printf("%s%s%s%s", CLR_GREEN, BOLD, "Two Pass Assembler Emulator by Hemant Kumar\n\n", RESET);
}
FileContent *storeFileContents(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    printf("%sError opening file!%s\n", CLR_RED, RESET);
    return NULL;
  }

  FileContent *content = (FileContent *)malloc(sizeof(FileContent));
  if (content == NULL)
  {
    fclose(file);
    return NULL;
  }

  content->lines = (char **)malloc(MAX_LINES * sizeof(char *));
  content->totalLines = 0;
  content->currentLine = 0;

  if (content->lines == NULL)
  {
    free(content);
    fclose(file);
    return NULL;
  }

  char buffer[MAX_LINE_LENGTH];
  while (fgets(buffer, sizeof(buffer), file) && content->totalLines < MAX_LINES)
  {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
    {
      buffer[len - 1] = '\0';
    }

    content->lines[content->totalLines] = (char *)malloc(strlen(buffer) + 1);
    if (content->lines[content->totalLines] == NULL)
    {
      for (int i = 0; i < content->totalLines; i++)
      {
        free(content->lines[i]);
      }
      free(content->lines);
      free(content);
      fclose(file);
      return NULL;
    }

    strcpy(content->lines[content->totalLines], buffer);
    content->totalLines++;
  }

  fclose(file);
  return content;
}
void displayFromLine(FileContent *content, int start_line)
{
  system("cls");
  printf("%s%s%s", CLR_CYAN, "File Viewer (Use Up/Down arrows to scroll, ESC to exit)\n", RESET);

  int current_page = (start_line / LINES_PER_PAGE) + 1;
  int total_pages = (content->totalLines + LINES_PER_PAGE - 1) / LINES_PER_PAGE;
  printf("%s%s%d/%d%s\n\n", CLR_YELLOW, "Page: ", current_page, total_pages, RESET);

  if (start_line < 0)
    start_line = 0;
  if (start_line >= content->totalLines)
  {
    start_line = content->totalLines - LINES_PER_PAGE;
    if (start_line < 0)
      start_line = 0;
  }

  for (int i = 0; i < LINES_PER_PAGE; i++)
  {
    int lineIndex = start_line + i;
    if (lineIndex < content->totalLines)
    {
      printf("%s%3d | %s%s\n", (i == 0 ? CLR_MAGENTA_BG : CLR_GREEN), lineIndex + 1, content->lines[lineIndex], RESET);
    }
    else
    {
      printf("\n");
    }
  }

  printf("\n%s%s%s", CLR_YELLOW, "Press ESC to exit\n\n\n", RESET);
}
void displayRegisters(int A, int B, int SP, int PC, int modifA, int modifB, int modifSP, int modifPC)
{
  printf("-------------------------------------------------------------------\n");
  printf(RESET);
  printf("%sA:%d    %s", (modifA ? CLR_MAGENTA_BG : CLR_YELLOW), A, RESET);
  printf("%sB:%d    %s", (modifB ? CLR_MAGENTA_BG : CLR_YELLOW), B, RESET);
  printf("%sSP:%d    %s", (modifSP ? CLR_MAGENTA_BG : CLR_YELLOW), SP, RESET);
  printf("%sPC:%d    %s", (modifPC ? CLR_MAGENTA_BG : CLR_YELLOW), PC, RESET);
  printf("\n-------------------------------------------------------------------\n");
}
void displayMemory(int index)
{
  printf("\n-------------------------------------------------------------------\n");
  printf(memoryDumps.entries[index]);
  printf("\n-------------------------------------------------------------------\n");
}
void freeFileContent(FileContent *content)
{
  if (content)
  {
    for (int i = 0; i < content->totalLines; i++)
    {
      free(content->lines[i]);
    }
    free(content->lines);
    free(content);
  }
}

TraceEntry *exec_emu_trace(const char *filename, int *num_entries)
{
  char command[256];
  snprintf(command, sizeof(command), "emu -trace %s.o", filename);

  FILE *pipe = popen(command, "r");
  if (!pipe)
  {
    printf("Error executing command\n");
    return NULL;
  }

  int capacity = 10;
  TraceEntry *entries = malloc(sizeof(TraceEntry) * capacity);
  *num_entries = 0;

  char line[256];
  while (fgets(line, sizeof(line), pipe))
  {
    if (*num_entries >= capacity)
    {
      capacity *= 2;
      entries = realloc(entries, sizeof(TraceEntry) * capacity);
    }

    TraceEntry *entry = &entries[*num_entries];
    int matches = sscanf(line,
                         "PC=%08X SP=%08X A=%08X B=%08X %s %08X",
                         &entry->pc, &entry->sp, &entry->a, &entry->b,
                         entry->instruction, &entry->operand);

    if (matches >= 5)
    {
      (*num_entries)++;
    }
  }

  pclose(pipe);

  entries = realloc(entries, sizeof(TraceEntry) * *num_entries);
  return entries;
}

MemoryDump *exec_emu_memory_dump(const char *filename)
{
  char command[256];
  snprintf(command, sizeof(command), "emu -write %s.o", filename);

  FILE *pipe = popen(command, "r");
  if (!pipe)
  {
    printf("Error executing command\n");
    return NULL;
  }

  MemoryDump *dump = malloc(sizeof(MemoryDump));
  dump->entries = NULL;
  dump->num_entries = 0;
  int capacity = 10;
  dump->entries = malloc(sizeof(char *) * capacity);

  char line[2048];
  char *current_entry = NULL;
  size_t entry_capacity = 0;
  size_t entry_size = 0;

  while (fgets(line, sizeof(line), pipe))
  {
    if (strstr(line, "-----") != NULL)
    {
      if (current_entry && entry_size > 0)
      {
        if (dump->num_entries >= capacity)
        {
          capacity *= 2;
          dump->entries = realloc(dump->entries, sizeof(char *) * capacity);
        }

        if (current_entry[entry_size - 1] == '\n')
        {
          current_entry[entry_size - 1] = '\0';
        }

        dump->entries[dump->num_entries] = current_entry;
        dump->num_entries++;

        current_entry = NULL;
        entry_capacity = 0;
        entry_size = 0;
      }
      continue;
    }

    size_t line_len = strlen(line);
    char colored_line[2048];

    if (line_len > 0)
    {
      snprintf(colored_line, sizeof(colored_line),
               "%s%.8s%s%s",
               CLR_BLUE, line, RESET, line + 8);
    }
    else
    {
      colored_line[0] = '\0';
    }

    size_t colored_line_len = strlen(colored_line);

    if (entry_size + colored_line_len + 1 > entry_capacity)
    {
      entry_capacity = (entry_size + colored_line_len + 1) * 2;
      current_entry = realloc(current_entry, entry_capacity);
    }

    strcpy(current_entry + entry_size, colored_line);
    entry_size += colored_line_len;
  }

  pclose(pipe);

  dump->entries = realloc(dump->entries, sizeof(char *) * dump->num_entries);

  return dump;
}

void viewFileWithScroll(FileContent *content)
{
  char key;
  int running = 1;
  int lineNum = 0;
  int ind = 0;
  int lastA = 0, lastB = 0, lastSP = 0, lastPC = 0;
  while (running)
  {
    if (ind != 0)
    {
      lineNum = trace[ind - 1].pc;
    }
    displayFromLine(content, lineNum);
    int A = 0, B = 0, SP = 0, PC = 0;
    if (ind > 0)
    {
      A = trace[ind - 1].a;
      B = trace[ind - 1].b;
      PC = trace[ind - 1].pc;
      SP = trace[ind - 1].sp;
    }
    displayRegisters(A, B, SP, PC, (A != lastA), (B != lastB), (SP != lastSP), (PC != lastPC));
    lastA = A;
    lastB = B;
    lastSP = SP;
    lastPC = PC;

    displayMemory(ind);

    key = getch();

    if (key == -32)
    {
      key = getch();
      switch (key)
      {
      case 72: // Up arrow
        ind--;
        if (ind <= 0)
        {
          ind = 0;
          lineNum = 0;
        }

        break;
      case 80: // Down arrow
        ind++;
        if (ind > trace_count)
        {
          ind = trace_count;
        }

        break;
      }
    }
    else if (key == 27)
    { // ESC key
      running = 0;
    }
  }
}

int main()
{
  setlocale(LC_ALL, "en_US.UTF-8");
  printPreText();

  char *filename = (char *)malloc(256 * sizeof(char));
  if (filename == NULL)
  {
    printf("%sMemory allocation failed!%s\n", CLR_RED, RESET);
    return 1;
  }

  printf("Enter file path: ");
  scanf("%255s", filename);

  char *lst_filename = (char *)malloc(strlen(filename) + 5);
  if (lst_filename == NULL)
  {
    printf("%sMemory allocation failed!%s\n", CLR_RED, RESET);
    free(filename);
    return 1;
  }

  char *dot = strrchr(filename, '.');
  if (dot != NULL)
  {
    *dot = '\0';
  }

  sprintf(lst_filename, "%s.lst", filename);

  // Check if .lst file exists
  FILE *test_file = fopen(lst_filename, "r");
  if (test_file != NULL)
  {
    fclose(test_file);
    system("cls");
    FileContent *content = storeFileContents(lst_filename);
    trace = exec_emu_trace(filename, &trace_count);
    memoryDumps = *exec_emu_memory_dump(filename);
    // printf("%d\n", memoryDumps.num_entries);
    if (content != NULL)
    {
      viewFileWithScroll(content);
      freeFileContent(content);
    }
  }
  else
  {
    printf("%sFile Not Found!%s\n", CLR_RED, RESET);
  }
  printf(RESET);
  free(filename);
  free(lst_filename);

  return 0;
}