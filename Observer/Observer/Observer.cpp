#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Observer {
public:
    virtual void update(std::string message) = 0;
    virtual void onWarning(const std::string& message) = 0;
    virtual void onError(const std::string& message) = 0;
    virtual void onFatalError(const std::string& message) = 0;
protected:
    std::string message_ = "";
    std::string FileName_ = "";
};

class Observable {
public:
    explicit Observable() = default;
    void warning(const std::string& message) const {

    }
    void error(const std::string& message) const {

    }
    void fatalError(const std::string& message) const {

    }

    void setVal(std::string message) {
        this->message_ = message;
    }
    void addObs(Observer* ObserverClass) {
        Observers.push_back(ObserverClass);
    }
    void removeObs(Observer* ObserverClass) {
        auto it = std::remove(Observers.begin(), Observers.end(), ObserverClass);
        Observers.erase(it, Observers.end());
    }
    void Notify() {
        if (!message_.empty()) {
            for (Observer* obs : Observers) {
               obs->update(message_);
            }
        }
        else {
            std::cout << "Добавьте значение переменной с помощью функции addObs" << std::endl;
        }
    }
private:
    std::string message_;
    std::vector<Observer*> Observers = {};
};

class ForWar : public Observer {
public:
    void onWarning(const std::string& message) override {
        message_ = message;
        std::cout << message << std::endl;
    }
    void update(std::string message) override {
        this->onWarning(message);
    }
    void onError(const std::string& message) override {

    }
    void onFatalError(const std::string& message) override {

    }
};

class ForError : public Observer {
public:
    explicit ForError(std::string FileName) {
        this->FileName_ = FileName;   
    }
    void onError(const std::string& message) override {
        this->message_ = message;
        std::ofstream file(FileName_ + ".txt");
        if (!file) {
            std::cout << "Ошибка открытия файла!" << std::endl;
        }
        else {
            file << message_ << std::endl;
        }
        file.close();
    }
    void update(std::string message) override {
        this->onError(message);
    }
    void onWarning(const std::string& message) override {

    }

    void onFatalError(const std::string& message) override {

    }
};

class ForFatError : public Observer {
public:
    explicit ForFatError(std::string FileName) {
        this->FileName_ = FileName;
    }
    void onFatalError(const std::string& message) override {
        this->message_ = message;
        std::cout << message_ << std::endl;
        std::ofstream file(FileName_ + ".txt");
        if (!file) {
            std::cout << "Ошибка открытия файла!" << std::endl;
        }
        else {
            file << message_ << std::endl;
        }
        file.close();

    }
    void update(std::string message) override {
        this->onFatalError(message);
    }
    void onWarning(const std::string& message) override {

    }

    void onError(const std::string& message) override {

    }
};

int main()
{
    auto Obser = std::make_unique<Observable>();
    auto F_W = std::make_shared<ForWar>();
    auto F_E = std::make_shared<ForError>("filename");
    auto F_F_E = std::make_shared<ForFatError>("filename");

    Obser->addObs(F_W.get());
    Obser->addObs(F_E.get());
    Obser->addObs(F_F_E.get());

    Obser->setVal("message");
    Obser->Notify();
}