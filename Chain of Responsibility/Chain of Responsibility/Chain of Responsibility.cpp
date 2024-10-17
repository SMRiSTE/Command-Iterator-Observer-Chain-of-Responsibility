#include <iostream>
#include <string>
#include <fstream>

class LogMessage {
public:
    virtual void type(std::string& type) = 0;
    virtual void message() = 0;
    virtual void NextHendler(LogMessage* handler) = 0;
protected:
    std::string type_;
    std::string message_;
};

class ForWarning : public LogMessage {
public:
    explicit ForWarning() {
        this->type_ = "warning";
    }

    void NextHendler(LogMessage* handler) override {
        NextType = handler;
    }
    void type(std::string& type) override {
        if (type == this->type_) {
            this->message();
        }
        else if (NextType != nullptr) {
            NextType->type(type);
        }
        else {
            std::cout << "Нет подходящего типа" << std::endl;
        }
    }
    void message() override {
        std::cout << message_ << std::endl;
    }
private:
    std::string message_ = "warning";
    LogMessage* NextType = nullptr;
};

class ForError : public LogMessage {
public:
    explicit ForError() {
        this->type_ = "error";
    }

    void NextHendler(LogMessage* handler) override {
        NextType = handler;
    }
    void type(std::string& type) override {
        if (type == this->type_) {
            this->message();
        }
        else if (NextType != nullptr) {
            NextType->type(type);
        }
        else {
            std::cout << "Нет подходящего типа" << std::endl;
        }
    }
    void message() override {
        std::ofstream file("filename.txt");
        if (!file) {
            std::cout << "Ошибка открытия файла!" << std::endl;
        }
        else {
            file << message_ << std::endl;
        }
    }
private:
    std::string message_ = "error";
    LogMessage* NextType = nullptr;
};

class FatError : public LogMessage {
public:
    explicit FatError() {
        this->type_ = "FatError";
    }

    void NextHendler(LogMessage* handler) override {
        NextType = handler;
    }
    void type(std::string& type) override {
        if (type == this->type_) {
            this->message();
        }
        else if (NextType != nullptr) {
            NextType->type(type);
        }
        else {
            std::cout << "Нет подходящего типа" << std::endl;
        }
    }
    void message() override {
        throw std::runtime_error(message_);
    }
private:
    std::string message_ = "FatError";
    LogMessage* NextType = nullptr;
};

class Unknown : public LogMessage {
public:
    explicit Unknown() = default;
    void NextHendler(LogMessage* handler) override {

    }
    void type(std::string& type) override {
        if (NextType != nullptr) {
            NextType->type(type);
        }
        else {
            this->message();
        }
    }
    void message() override {
        throw std::runtime_error(message_);
    }
private:
    std::string message_ = "Unknown message";
    LogMessage* NextType = nullptr;
};

int main()
{
    auto ForWar = std::make_unique<ForWarning>();
    auto ForEr = std::make_shared<ForError>();
    auto ForFatEr = std::make_shared<FatError>();
    auto Unknow = std::make_shared<Unknown>();

    ForWar->NextHendler(ForEr.get());
    ForEr->NextHendler(ForFatEr.get());
    ForFatEr->NextHendler(Unknow.get());

    std::string str = "alala";
    try{
        ForWar->type(str);
    }
    catch (const std::exception& e){
        std::cout << e.what() << std::endl;
    }
}