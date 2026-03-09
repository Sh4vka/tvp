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
    GetInfo(bool createChild = false) : parentPid(static_cast<T>(getpid())) {
        if (createChild) {
            T childPid = fork();
            switch (childPid) {
                case -1:
                    std::cerr <<"Fork failed" << std::endl;
                    isChild = false;
                    break;

                case 0:
                    isChild = true;
                    myPid = parentPid;
                    break;

                default:
                    childPid = -1;
                    isChild = false;
                    myPid = parentPid;
            }
        }
    }

    bool is_chiled_process() const {return isChild;}
    T get_current_pid() const {return myPid;}

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
    std::string getfile() { return "/proc/" + std::to_string(static_cast<int>(myPid)); }
    T myPid;
    T parentPid;
    bool isChild;
};
}

int main() {
    infoproc::GetInfo<pid_t> proc(true);
    if (proc.is_chiled_process()) {
        std::cout << proc.getproc() << std::endl;
        exit(0);
    } else {
        std::cout << proc.getproc() << std::endl;
        wait(nullptr);
    }
    return 0;
}
