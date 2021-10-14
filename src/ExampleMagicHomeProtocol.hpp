
#include <EBObject.hpp>
#include <EBEventLoop.hpp>
#include "MagicHomeProtocol.hpp"

class ExampleMagicHomeProtocol : public EBCpp::EBObject<ExampleMagicHomeProtocol>
{
public:
    ExampleMagicHomeProtocol(int type, std::string ip, std::list<std::string> commandList) : protocol((MagicHomeProtocol::DEVICE_TYPE)type), commandList(commandList)
    {
        protocol.connected.connect(this, &ExampleMagicHomeProtocol::connected);
        protocol.disconnected.connect(this, &ExampleMagicHomeProtocol::disconnected);
        protocol.readReady.connect(this, &ExampleMagicHomeProtocol::readReady);
        protocol.error.connect(this, &ExampleMagicHomeProtocol::error);

        protocol.connect(ip);
    }

    ~ExampleMagicHomeProtocol()
    {

    }

private:
    MagicHomeProtocol protocol;
    std::list<std::string> commandList;

    EB_SLOT(connected)
    {
        EB_LOG_INFO("Connected");
        for (std::string command : commandList)
        {
            EB_LOG_DEBUG("Command: " << command);
            if (command == "ON")
            {
                protocol.turnOn();
            }
            else if (command == "OFF")
            {
                protocol.turnOff();
            }
            else if (command.substr(0, 5) == "COLOR")
            {
            }
        }

        protocol.disconnect();
    }

    EB_SLOT(disconnected)
    {
        EB_LOG_INFO("Disconnected");

        protocol.connected.disconnect(this, &ExampleMagicHomeProtocol::connected);
        protocol.disconnected.disconnect(this, &ExampleMagicHomeProtocol::disconnected);
        protocol.readReady.disconnect(this, &ExampleMagicHomeProtocol::readReady);
        protocol.error.disconnect(this, &ExampleMagicHomeProtocol::error);
        
        EBCpp::EBEventLoop::getInstance()->exit();
    }

    EB_SLOT(readReady)
    {
    }

    EB_SLOT_WITH_ARGS(error, std::string message)
    {
        EB_LOG_WARNING(message);
    }
};
