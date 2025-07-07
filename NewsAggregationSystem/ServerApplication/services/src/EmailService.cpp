#include "services/inc/EmailService.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

EmailService::EmailService() {
    loadEmailConfig();
}

void EmailService::loadEmailConfig() {
    // Default configuration - in a real application, these would be loaded from environment variables or config files
    smtpServer = "smtp.gmail.com";
    smtpPort = 587;
    smtpUsername = "your-email@gmail.com";
    smtpPassword = "your-app-password";
    fromEmail = "your-email@gmail.com";
    
    // Try to load from config file if it exists
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
}

bool EmailService::sendNotificationEmail(const std::string& toEmail, 
                                        const std::string& subject, 
                                        const std::string& message) {
    // For development purposes, we'll use a simple approach
    // In production, you would use a proper SMTP library like libcurl or similar
    
    std::cout << "[EMAIL SERVICE] Sending email to: " << toEmail << std::endl;
    std::cout << "[EMAIL SERVICE] Subject: " << subject << std::endl;
    std::cout << "[EMAIL SERVICE] Message: " << message << std::endl;
    
    // Create a temporary email file
    std::string tempFile = "/tmp/news_notification_" + std::to_string(std::time(nullptr)) + ".txt";
    std::ofstream emailFile(tempFile);
    if (emailFile.is_open()) {
        emailFile << "To: " << toEmail << std::endl;
        emailFile << "From: " << fromEmail << std::endl;
        emailFile << "Subject: " << subject << std::endl;
        emailFile << std::endl;
        emailFile << message << std::endl;
        emailFile.close();
        
        // Use system mail command (if available)
        std::string command = "mail -s \"" + subject + "\" " + toEmail + " < " + tempFile;
        int result = system(command.c_str());
        
        // Clean up temp file
        std::remove(tempFile.c_str());
        
        if (result == 0) {
            std::cout << "[EMAIL SERVICE] Email sent successfully to " << toEmail << std::endl;
            return true;
        } else {
            std::cout << "[EMAIL SERVICE] Failed to send email to " << toEmail << std::endl;
            return false;
        }
    }
    
    return false;
}

bool EmailService::sendBulkNotificationEmail(const std::vector<std::string>& toEmails, 
                                            const std::string& subject, 
                                            const std::string& message) {
    bool allSent = true;
    
    for (const auto& email : toEmails) {
        if (!sendNotificationEmail(email, subject, message)) {
            allSent = false;
        }
    }
    
    return allSent;
}

bool EmailService::sendNewsNotification(const std::string& toEmail,
                                       const std::string& articleTitle,
                                       const std::string& articleDescription,
                                       const std::string& articleUrl,
                                       const std::string& categoryName) {
    std::string subject = "Breaking News: " + articleTitle;
    std::string message = createNewsEmailTemplate(articleTitle, articleDescription, articleUrl, categoryName);
    
    return sendNotificationEmail(toEmail, subject, message);
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