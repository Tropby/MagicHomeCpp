
#include <EBObject.hpp>
#include <socket/tcp/EBTcpSocket.hpp>

class MagicHomeProtocol : public EBCpp::EBObject<MagicHomeProtocol>
{
public:
    typedef enum
    {
        RGB,
        RGB_WW,
        RGB_WW_CW,
        BLUB_V4,
        BLUB_V3
    } DEVICE_TYPE;

    MagicHomeProtocol(DEVICE_TYPE deviceType) : deviceType(deviceType)
    {
    }

    void connect()
    {
        EB_LOG_DEBUG("Connect to LED Strip!");

        socket.connected.connect(this, &MagicHomeProtocol::tcpConnected);
        socket.disconnected.connect(this, &MagicHomeProtocol::tcpDisconnected);
        socket.error.connect(this, &MagicHomeProtocol::tcpError);
        socket.readReady.connect(this, &MagicHomeProtocol::tcpReadReady);

        socket.setFileName("tcp://192.168.222.151:5577");

        if (!socket.open(EBCpp::EBTcpSocket::READ_WRITE))
        {
            EB_LOG_DEBUG("Can not connect!");
        }
    }

    void turnOff()
    {
        uint8_t buffer[] = {0x71, 0x24, 0x0F, 0xA4, 0xCC, 0x24, 0x33};

        switch (deviceType)
        {
        case RGB:
        case RGB_WW:
        case RGB_WW_CW:
        case BLUB_V4:
            socket.write((char *)buffer, 7);
            break;
        case BLUB_V3:
            socket.write((char *)buffer, 4);
            break;
        }
    }

    void turnOn()
    {
        uint8_t buffer[] = {0x71, 0x23, 0x0F, 0xA4, 0xCC, 0x23, 0x33};

        switch (deviceType)
        {
        case RGB:
        case RGB_WW:
        case RGB_WW_CW:
        case BLUB_V4:
            socket.write((char *)buffer, 7);
            break;
        case BLUB_V3:
            socket.write((char *)buffer, 4);
            break;
        }
    }

    void requestStatus()
    {
        uint8_t buffer[] = {0x81, 0x8A, 0x8B, 0x96};
        socket.write((char *)buffer, 4);
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t ww, uint8_t cw)
    {
        switch (deviceType)
        {

        case RGB:
        case RGB_WW:
        {
            uint8_t buffer[] = {0x31, r, g, b, ww, 0x00, 0x0f, 0x00};
            buffer[7] = checksum(buffer, 7);
            socket.write((char *)buffer, 8);
            break;
        }

        case RGB_WW_CW:
        {
            uint8_t buffer[] = {0x31, r, g, b, ww, cw, 0x0f, 0x0f, 0x00};
            buffer[8] = checksum(buffer, 8);
            socket.write((char *)buffer, 9);
            break;
        }

        case BLUB_V4:
        {
            uint8_t buffer[] = {0x31, r, g, b, ww, 0x00, 0x0f, 0x0f, 0x00};
            buffer[8] = checksum(buffer, 8);
            socket.write((char *)buffer, 9);
            break;
        }

        case BLUB_V3:
        {
            uint8_t buffer[] = {0x56, r, g, b, 0x00, 0xf0, 0xaa, 0x00};
            buffer[7] = checksum(buffer, 7);
            socket.write((char *)buffer, 8);
            break;
        }
        
        }
    }

    /**
     * @brief EB_SIGNAL connected
     *
     * Emitted if the connection was established
     */
    EB_SIGNAL(connected);

    /**
     * @brief EB_SIGNAL diconnected
     *
     * Emitted if the connection was terminated
     */
    EB_SIGNAL(disconnected);

    /**
     * @brief EB_SIGNAL readReady
     *
     * Emitted if new data is available
     */
    EB_SIGNAL(readReady);

    /**
     * @brief EB_SIGNAL error
     *
     * Emitted if an error occures on the TCP connection
     */
    EB_SIGNAL_WITH_ARGS(error, std::string);

private:
    uint8_t checksum(uint8_t *buffer, uint8_t size)
    {
        uint8_t result = 0;
        for (uint8_t i = 0; i < size; i++)
        {
            result += buffer[i];
        }
        return result;
    }

    EB_SLOT(tcpConnected)
    {
        EB_EMIT(connected);
    }

    EB_SLOT(tcpDisconnected)
    {
        EB_EMIT(disconnected);
    }

    EB_SLOT(tcpReadReady)
    {
        char buffer[1024];
        EBCpp::EBObjectPointer<EBCpp::EBTcpSocket> socket = sender->cast<EBCpp::EBTcpSocket>();
        int nbytes = socket->read(buffer, 1024);
        if (nbytes > 0)
        {
            EB_LOG_DEBUG("read Ready: " << EBCpp::EBUtils::binToHex(buffer, nbytes));
        }
    }

    EB_SLOT_WITH_ARGS(tcpError, std::string message)
    {
        EB_EMIT_WITH_ARGS(error, message);
    }

    EBCpp::EBTcpSocket socket;
    DEVICE_TYPE deviceType;
};
