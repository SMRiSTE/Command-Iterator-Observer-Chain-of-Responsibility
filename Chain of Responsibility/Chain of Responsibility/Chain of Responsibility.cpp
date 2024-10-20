#include <iostream>
#include <string>
#include <fstream>

enum class MessageType {
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage {
public:
    LogMessage(MessageType type, const std::string& message)
        : type_(type), message_(message) {}

    MessageType type() const { return type_; }
    const std::string& message() const { return message_; }

private:
    MessageType type_;
    std::string message_;
};

class LogHandler {
public:
    virtual ~LogHandler() = default;

    void receiveMessage(const LogMessage& msg) {
        if (canHandle() == msg.type()) {
            handleMessage(msg);
        }
        else if (next_) {
            next_->receiveMessage(msg);
        }
        else {
            throw std::runtime_error("Error: no handler for this message was found!");
        }
    }

    void setNext(LogHandler* next) {
        next_ = next;
    }

protected:
    virtual void handleMessage(const LogMessage& msg) = 0;
    virtual MessageType canHandle() const = 0; 

private:
    LogHandler* next_ = nullptr;
};

class WarningHandler : public LogHandler {
protected:
    void handleMessage(const LogMessage& msg) override {
        std::cout << "Warning: " << msg.message() << std::endl;
    }

    MessageType canHandle() const override {
        return MessageType::Warning;
    }
};

class ErrorHandler : public LogHandler {
protected:
    void handleMessage(const LogMessage& msg) override {
        std::ofstream file("filename.txt");
        if (!file) {
            std::cout << "Ошибка открытия файла!" << std::endl;
        }
        else {
            file << msg.message() << std::endl;
        }
    }

    MessageType canHandle() const override {
        return MessageType::Error;
    }
};

class FatalErrorHandler : public LogHandler {
protected:
    void handleMessage(const LogMessage& msg) override {
        throw std::runtime_error("Fatal Error: " + msg.message());
    }

    MessageType canHandle() const override {
        return MessageType::FatalError;
    }
};

int main() {
    FatalErrorHandler fatalHandler;
    ErrorHandler errorHandler;
    WarningHandler warningHandler;

    warningHandler.setNext(&errorHandler);
    errorHandler.setNext(&fatalHandler);

    try {
        LogMessage warningMsg(MessageType::Warning, "This is a warning message.");
        warningHandler.receiveMessage(warningMsg);

        LogMessage errorMsg(MessageType::Error, "This is an error message.");
        warningHandler.receiveMessage(errorMsg);

        LogMessage fatalMsg(MessageType::FatalError, "This is a fatal error message.");
        warningHandler.receiveMessage(fatalMsg);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}