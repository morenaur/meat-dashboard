#include <iostream>
#include <iomanip>
#include <windows.h>
#include <cstdint>
#include <conio.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\e[0;92m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

uint64_t FILETIME_to_uint64(const FILETIME& ft);
double get_cpu_usage_percentage();

void display_ram();
void display_cpu();
void move_cursor_topleft();
const char* get_color(double percent);

void print_header();
void print_footer();

int main() {
    SetConsoleMode(WriteConsole, ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    system("cls");

    while (true) {
        if (_kbhit()) {
            char pressed_key = _getch();
            if (pressed_key == 'q' || pressed_key == 'Q') {
                system("cls");
                break;
            };
        };

        move_cursor_topleft();
        print_header();
        display_ram();
        display_cpu();
        print_footer();
    }

    return 0;
}

void display_ram() {
   MEMORYSTATUSEX mem_info;
   mem_info.dwLength = sizeof(MEMORYSTATUSEX);
   GlobalMemoryStatusEx(&mem_info);

   DWORDLONG total_ram = mem_info.ullTotalPhys;
   DWORDLONG avail_ram = mem_info.ullAvailPhys;
   
   double used_ram = (total_ram-avail_ram);
   double used_ram_percent = (used_ram/total_ram) * 100;

   double total_gb = total_ram/(1024.0 * 1024*1024);
   double used_gb = used_ram/(1024.0 * 1024*1024);

   std::cout << std::fixed << std::setprecision(1);
   std::cout << "[ CPU ] CPU Usage: " << std::setw(5)
             << get_color(used_ram_percent) << used_ram_percent << RESET << "%\n";
}


void display_cpu() {
    double cpu_percent = get_cpu_usage_percentage();

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "[ CPU ] CPU Usage: " << std::setw(5) 
              << get_color(cpu_percent) << cpu_percent << RESET << "%\n";
}

uint64_t FILETIME_to_uint64(const FILETIME& ft) {
    // Explicitly converts
    return (static_cast<uint64_t>(ft.dwHighDateTime) << 32 | ft.dwLowDateTime);
}

double get_cpu_usage_percentage() {
    FILETIME idle_time1, kernel_time1, user_time1;
    FILETIME idle_time2, kernel_time2, user_time2;

    GetSystemTimes(&idle_time1, &kernel_time1, &user_time1);
    Sleep(1000);
    GetSystemTimes(&idle_time2, &kernel_time2, &user_time2);

    uint64_t idle1 = FILETIME_to_uint64(idle_time1);
    uint64_t idle2 = FILETIME_to_uint64(idle_time2);
    uint64_t kernel1 = FILETIME_to_uint64(kernel_time1);
    uint64_t kernel2 = FILETIME_to_uint64(kernel_time2);
    uint64_t user1 = FILETIME_to_uint64(user_time1);
    uint64_t user2 = FILETIME_to_uint64(user_time2);

    uint64_t idle_dt = idle2 - idle1;
    uint64_t kernel_dt = kernel2 - kernel1;
    uint64_t user_dt = user2 - user1;
    uint64_t total_dt = kernel_dt + user_dt;

    double cpu_percentage = 100.0 * (1.0 - ((double)idle_dt / total_dt));

    return cpu_percentage;
}

void move_cursor_topleft() {
    COORD target = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), target);
}

const char* get_color(double percent) {
    if (percent <= 50) return GREEN;
    else if (percent <= 80) return YELLOW;
    else return RED;
}

void print_header() {
    std::cout << "==================================\n";
    std::cout << GREEN << "             THE MEAT        \n" << RESET;
    std::cout << "==================================\n\n";
}

void print_footer() {
    std::cout << "\n==================================\n\n\n\n";
}