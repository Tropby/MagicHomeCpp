
#include <iostream>
#include <EBEventLoop.hpp>
#include "ExampleMagicHomeProtocol.hpp"

void printHelp()
{
    std::cout << "Help:" << std::endl
              << "MagicHomeProtocol {DEVICE_TYPE} {IP} {COMMAND}*" << std::endl
              << std::endl
              << "DEVICE_TYPE:" << std::endl
              << "\t0: RGB" << std::endl
              << "\t1: RGB+WW" << std::endl
              << "\t2: RGB+WW+CW" << std::endl
              << "\t3: Blub (v.4+)" << std::endl
              << "\t4: Blub (v.3-)" << std::endl
              << std::endl
              << "IP-Address:" << std::endl
              << "\tip address of the led controller" << std::endl
              << "\tExample: tcp://192.168.222.151:5577" << std::endl
              << std::endl
              << "COMMAND:" << std::endl
              << "\tON: turn on a device" << std::endl
              << "\tOFF: turn off a device" << std::endl
              << "\tCOLOR=RRGGBBWWCW: set the color" << std::endl
              << "\t\tRR = red channel," << std::endl
              << "\t\tGG = green channel," << std::endl
              << "\t\tBB = blue channel," << std::endl
              << "\t\tWW = warm white channel," << std::endl
              << "\t\tCW = cold white channel" << std::endl
              << std::endl
              << std::endl;
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printHelp();
        return -1;
    }

    std::list<std::string> argList = EBCpp::EBUtils::getArgList(argc, argv);
    int i = 0;

    std::list<std::string>
        commandList;
    std::string ip;
    int type;
    for (std::string arg : argList)
    {
        switch (i)
        {
        case 0:
            break;
        case 1:
            type = std::atoi(arg.c_str());
            break;
        case 2:
            ip = arg;
            break;
        default:
            commandList.push_back(arg);
            break;
        }
        i++;
    }

    ExampleMagicHomeProtocol ex(type, ip, commandList);

    EBCpp::EBEventLoop::getInstance()->exec();
}
