#include <boost/asio.hpp>
#include <iostream>
#include <cstdint>
#include <thread>  // 包含线程头文件
#include <chrono>  // 包含时间头文件
#include <boost/asio/serial_port.hpp>
class WeightSensor {
public:
    WeightSensor(const std::string& device, int baudRate)
        : io(), serial(io, device) {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
    }

    // 读取重量数据
    float readWeight() {
        uint8_t buffer[10];
        boost::asio::read(serial, boost::asio::buffer(buffer, 10));  // 读取 10 字节数据
        // 解析重量数据
        uint32_t weight = (buffer[4] << 16) | (buffer[5] << 8) | buffer[6];
        return static_cast<float>(weight);  // 转换为千克
    }

    // 发送命令函数，支持发送16进制数据
    void sendCommand(const std::vector<uint8_t>& command) {
        size_t sent;
        try {
            boost::asio::write(serial, boost::asio::buffer(command.data(), command.size()));
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 等待100毫秒
        } catch (const std::exception& e) {
            std::cerr << "Error sending command: " << e.what() << std::endl;
        }
    }



private:
    boost::asio::io_context io;
    boost::asio::serial_port serial;
};

int main() {
    try {
        // 打开串口 ttyAMA0
        WeightSensor weightSensor("/dev/ttyAMA0", 9600);

        // 发送一个16进制命令
        std::vector<uint8_t> command = {0xA5};
        weightSensor.sendCommand(command);
       
        std::vector<uint8_t> command2 = {0xAA};
        weightSensor.sendCommand(command2);
        std::vector<uint8_t> command3 = {0xA4};
        weightSensor.sendCommand(command3);
        std::vector<uint8_t> command4 = {0xA4};
        weightSensor.sendCommand(command4);
        // 读取重量数据
        while (true) {
            float weight = weightSensor.readWeight();
            std::cout << "Weight: " << weight << " g" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
