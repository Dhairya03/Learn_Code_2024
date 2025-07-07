#include "services/inc/EmailService.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

EmailService::EmailService() {
    std::cout << "[EmailService] Constructor called" << std::endl;
    try {
        loadEmailConfig();
        std::cout << "[EmailService] Config loaded" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[EmailService] Error loading config: " << e.what() << std::endl;
    }
}

void EmailService::loadEmailConfig() {
    std::cout << "[EmailService] loadEmailConfig called" << std::endl;
    try {
        smtpServer = "smtp.gmail.com";
        smtpPort = 587;
        smtpUsername = "your-email@gmail.com";
        smtpPassword = "your-app-password";
        fromEmail = "your-email@gmail.com";
        
        std::ifstream configFile("email_config.txt");
        if (configFile.is_open()) {
            std::string line;
            while (std::getline(configFile, line)) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    
                    if (key == "smtp_server") smtpServer = value;
                    else if (key == "smtp_port") smtpPort = std::stoi(value);
                    else if (key == "smtp_username") smtpUsername = value;
                    else if (key == "smtp_password") smtpPassword = value;
                    else if (key == "from_email") fromEmail = value;
                }
            }
            configFile.close();
        }
        std::cout << "[EmailService] loadEmailConfig success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[EmailService] loadEmailConfig error: " << e.what() << std::endl;
    }
}

bool EmailService::sendNotificationEmail(const std::string& toEmail, 
                                        const std::string& subject, 
                                        const std::string& message) {
    std::cout << "[EmailService] sendNotificationEmail called for: " << toEmail << std::endl;
    try {
        std::cout << "[EMAIL SERVICE] Sending email to: " << toEmail << std::endl;
        std::cout << "[EMAIL SERVICE] Subject: " << subject << std::endl;
        std::cout << "[EMAIL SERVICE] Message: " << message << std::endl;
        
        std::string tempFile = "/tmp/news_notification_" + std::to_string(std::time(nullptr)) + ".txt";
        std::ofstream emailFile(tempFile);
        if (emailFile.is_open()) {
            emailFile << "To: " << toEmail << std::endl;
            emailFile << "From: " << fromEmail << std::endl;
            emailFile << "Subject: " << subject << std::endl;
            emailFile << std::endl;
            emailFile << message << std::endl;
            emailFile.close();
            
            std::string command = "mail -s \"" + subject + "\" " + toEmail + " < " + tempFile;
            int result = system(command.c_str());
            
            std::remove(tempFile.c_str());
            
            if (result == 0) {
                std::cout << "[EmailService] Email sent successfully to " << toEmail << std::endl;
                return true;
            } else {
                std::cerr << "[EmailService] Failed to send email to " << toEmail << std::endl;
                return false;
            }
        }
        std::cerr << "[EmailService] Could not open temp file for email" << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "[EmailService] sendNotificationEmail error: " << e.what() << std::endl;
        return false;
    }
}

bool EmailService::sendBulkNotificationEmail(const std::vector<std::string>& toEmails, 
                                            const std::string& subject, 
                                            const std::string& message) {
    std::cout << "[EmailService] sendBulkNotificationEmail called" << std::endl;
    bool allSent = true;
    
    for (const auto& email : toEmails) {
        if (!sendNotificationEmail(email, subject, message)) {
            allSent = false;
        }
    }
    
    std::cout << "[EmailService] sendBulkNotificationEmail finished" << std::endl;
    return allSent;
}

bool EmailService::sendNewsNotification(const std::string& toEmail,
                                       const std::string& articleTitle,
                                       const std::string& articleDescription,
                                       const std::string& articleUrl,
                                       const std::string& categoryName) {
    std::cout << "[EmailService] sendNewsNotification called for: " << toEmail << std::endl;
    try {
        std::string subject = "Breaking News: " + articleTitle;
        std::string message = createNewsEmailTemplate(articleTitle, articleDescription, articleUrl, categoryName);
        
        return sendNotificationEmail(toEmail, subject, message);
    } catch (const std::exception& e) {
        std::cerr << "[EmailService] sendNewsNotification error: " << e.what() << std::endl;
        return false;
    }
}

std::string EmailService::createNewsEmailTemplate(const std::string& articleTitle,
                                                 const std::string& articleDescription,
                                                 const std::string& articleUrl,
                                                 const std::string& categoryName) {
    std::stringstream email;
    
    email << "Hello!\n\n";
    email << "You have a new news notification based on your preferences.\n\n";
    email << "Category: " << categoryName << "\n\n";
    email << "Title: " << articleTitle << "\n\n";
    email << "Description: " << articleDescription << "\n\n";
    email << "Read more: " << articleUrl << "\n\n";
    email << "Best regards,\n";
    email << "News Aggregation System\n\n";
    email << "To unsubscribe from these notifications, please update your settings in the application.";
    
    return email.str();
} 