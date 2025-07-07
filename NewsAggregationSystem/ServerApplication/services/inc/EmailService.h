#ifndef EMAIL_SERVICE_H
#define EMAIL_SERVICE_H

#include <string>
#include <vector>

class EmailService {
public:
    EmailService();
    
    // Send notification email to a single user
    bool sendNotificationEmail(const std::string& toEmail, 
                              const std::string& subject, 
                              const std::string& message);
    
    // Send notification email to multiple users
    bool sendBulkNotificationEmail(const std::vector<std::string>& toEmails, 
                                  const std::string& subject, 
                                  const std::string& message);
    
    // Send news article notification
    bool sendNewsNotification(const std::string& toEmail,
                             const std::string& articleTitle,
                             const std::string& articleDescription,
                             const std::string& articleUrl,
                             const std::string& categoryName);

private:
    std::string smtpServer;
    int smtpPort;
    std::string smtpUsername;
    std::string smtpPassword;
    std::string fromEmail;
    
    void loadEmailConfig();
    std::string createNewsEmailTemplate(const std::string& articleTitle,
                                       const std::string& articleDescription,
                                       const std::string& articleUrl,
                                       const std::string& categoryName);
};

#endif 