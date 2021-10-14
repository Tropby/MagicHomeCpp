
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
                uint8_t r = 0;
                uint8_t g = 0;
                uint8_t b = 0;
                uint8_t ww = 0;
                uint8_t cw = 0;

                if( command.size() >= 8 )
                    r = EBCpp::EBUtils::hexToInt(command.substr(6, 2));
                if (command.size() >= 10)
                    g = EBCpp::EBUtils::hexToInt(command.substr(8, 2));
                if (command.size() >= 12)
                    b = EBCpp::EBUtils::hexToInt(command.substr(10, 2));
                if (command.size() >= 14)
                    ww = EBCpp::EBUtils::hexToInt(command.substr(12, 2));
                if (command.size() >= 16)
                    cw = EBCpp::EBUtils::hexToInt(command.substr(14, 2));

                protocol.setColor(r, g, b, ww, cw);
            }
        }

        protocol.disconnect();
    }

    EB_SLOT(disconnected)
    {
        EB_LOG_INFO("Disconnected");
        exit();
    }

    EB_SLOT(readReady)
    {
    }

    EB_SLOT_WITH_ARGS(error, std::string message)
    {
        EB_LOG_WARNING("ERROR: " << message);
        exit();
    }

    void exit()
    {
        protocol.connected.disconnect(this, &ExampleMagicHomeProtocol::connected);
        protocol.disconnected.disconnect(this, &ExampleMagicHomeProtocol::disconnected);
        protocol.readReady.disconnect(this, &ExampleMagicHomeProtocol::readReady);
        protocol.error.disconnect(this, &ExampleMagicHomeProtocol::error);
        EBCpp::EBEventLoop::getInstance()->exit();
    }
};
