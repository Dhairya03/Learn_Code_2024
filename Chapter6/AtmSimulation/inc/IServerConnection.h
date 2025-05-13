#ifndef ISERVERCONNECTION_H
#define ISERVERCONNECTION_H

class IServerConnection {
    public:
        virtual bool connect() = 0;
        virtual ~IServerConnection() = default;
    };

#endif 