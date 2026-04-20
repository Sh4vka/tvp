#include <iostream>
#include <string>
#include <deque>

namespace shell {
class CommandParse {
public:
    CommandParse(std::string line_) : line(line_) {};

private:
    std::string line;

    struct Command {
        std::string cmd;
        std::deque<std::string> args;
        std::string ;
};

class Shell {
public:


private:
    std::string visit = "mysh > ";
};
}
