
#include <iostream>
#include <EBEventLoop.hpp>
#include "MagicHomeProtocol.hpp"

int main(int, char**) 
{
    MagicHomeProtocol mhp(MagicHomeProtocol::RGB);
    mhp.connect();

    EBCpp::EBEventLoop::getInstance()->exec();
}
 