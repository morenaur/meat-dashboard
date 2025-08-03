#include <iostream>
#include <iomanip>
#include <windows.h>

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
   std::cout << "Memory Usage: " 
             << used_gb << " GB /" 
             << total_gb << " GB (" 
             << used_ram_percent << "%)\n\n";
}

int main() {
    std::cout << "RAM INFO\n\n";

    display_ram();
    return 0;
}