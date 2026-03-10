#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>

namespace infoproc {

template<typename T>
class GetInfo {
public:
    GetInfo() {pid = static_cast<T>(getpid());}
    
    std::string getproc() {
        std::string filename = getfile();
        std::cout << filename << std::endl;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error " << filename << std::endl;
            return "";
        }
        std::stringstream buf;
        buf << file.rdbuf();

        file.close();
        return buf.str();
    }

private:
    std::string getfile() { return "/proc/" + std::to_string(static_cast<int>(pid)) + "/status"; }
    T pid;
};
}

int main() {
    infoproc::GetInfo<pid_t> proc;
    std::cout << proc.getproc() << std::endl;
    return 0;
}
