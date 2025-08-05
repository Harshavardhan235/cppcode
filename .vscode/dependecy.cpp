#include <iostream>

#include <string>
 
// High-level module depends on this abstract interface

class IMessageService {

public:

    virtual void sendMessage(const std::string& msg) = 0;

};
 
// Low-level module: Email

class EmailService : public IMessageService {

public:

    void sendMessage(const std::string& msg) override {

        std::cout << "Email sent: " << msg << std::endl;

    }

};
 
// High-level module

class Notification {

    IMessageService* service;

public:

    Notification(IMessageService* service_) : service(service_) {}

    void notifyUser(const std::string& msg) {

        service->sendMessage(msg);

    }

};
 
int main() {

    EmailService emailService;

    Notification notify(&emailService);
 
    notify.notifyUser("Your report is ready!");
 
    return 0;

}

 