#include <iostream>
#include <fstream>
#include <string>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class Console : public LogCommand {
public:
    explicit Console() = default;
    void print(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

class File : public LogCommand {
public:
    explicit File(std::string filename) : filename_(filename) {}
    void print(const std::string& message) override {
        std::ofstream file(filename_ + ".txt");
        if (!file) {
            std::cout << "Ошибка открытия файла!" << std::endl;
        }
        else {
            file << message << std::endl;
        }
    }
private:
    std::string filename_ = nullptr;
};

void print(LogCommand& command, std::string& message) {
    command.print(message);
}

int main()
{
    std::string message = "Hello, World!";

    Console console;
    File file("file");
    
    print(console, message);
    print(file, message);
}