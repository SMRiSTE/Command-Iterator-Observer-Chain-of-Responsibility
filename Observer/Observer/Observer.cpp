#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Observer {
public:
    virtual void onWarning(const std::string& message) = 0;
    virtual void onError(const std::string& message) = 0;
    virtual void onFatalError(const std::string& message) = 0;
protected:
    std::string FileName_ = "";
};

class Observable {
public:
    explicit Observable() = default;
    void warning(const std::string& message) const {
        for (auto Obs : Observers) {
            Obs->onWarning(message);
        }
    }
    void error(const std::string& message) const {
        for (auto Obs : Observers) {
            Obs->onError(message);
        }
    }
    void fatalError(const std::string& message) const {
        for (auto Obs : Observers) {
            Obs->onFatalError(message);
        }
    }
    void addObs(Observer* ObserverClass) {
        Observers.push_back(ObserverClass);
    }
    void removeObs(Observer* ObserverClass) {
        auto it = std::remove(Observers.begin(), Observers.end(), ObserverClass);
        Observers.erase(it, Observers.end());
    }
private:
    std::string message_;
    std::vector<Observer*> Observers = {};
};

class ForWar : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << message << std::endl;
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
        std::ofstream file(FileName_ + ".txt");
        if (!file) {
            std::cout << "Ошибка открытия файла!" << std::endl;
        }
        else {
            file << message << std::endl;
        }
        file.close();
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
        std::cout << message << std::endl;
        std::ofstream file(FileName_ + ".txt");
        if (!file) {
            std::cout << "Ошибка открытия файла!" << std::endl;
        }
        else {
            file << message << std::endl;
        }
        file.close();

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

    Obser->fatalError("fatalError");
}