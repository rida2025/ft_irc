#include "Commands.hpp"

std::string getRandomLine(const std::vector<std::string>& lines) {
    int index = rand() % lines.size();
    return lines[index];
}

void    HAKIM_COMMAND(int fd){
    std::string buffer;
    std::ifstream inputFile;
    inputFile.open("Commands/info.txt");
    if (!inputFile.is_open()){
        std::cerr << "Error opening file." << std::endl;
        return ;
    }
    std::vector<std::string> lines;
    std::string line;
    
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();
    if (lines.empty()) {
        std::cerr << "The file is empty." << std::endl;
        return;
    }
    srand(static_cast<unsigned int>(time(0)));
    std::string randomLine = getRandomLine(lines);
    buffer = ":Bot el-hakim say: " + randomLine + "";
    buffer = ":el-hakim!~elHakim@somewhere PRIVMSG - :"+ buffer + "\r\n";
    send(fd, buffer.c_str(), buffer.length(), 0);
}
