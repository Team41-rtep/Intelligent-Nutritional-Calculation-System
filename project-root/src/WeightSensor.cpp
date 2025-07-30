#include "WeightSensor.h"

WeightSensor::WeightSensor(const std::string& device, int baudRate)
    : io(), serial(io, device), work_guard(boost::asio::make_work_guard(io)) {
    serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
    // 启动IO服务线程
    io_thread = std::thread([this]() { io.run(); });
}

WeightSensor::~WeightSensor() {
    io.stop();
    if (io_thread.joinable()) {
        io_thread.join();
    }
}

void WeightSensor::requestWeight() {
    std::lock_guard<std::mutex> lock(mutex);
    if (reading_in_progress) return;
    
    reading_in_progress = true;
    sendCommandNoDelay({0xA3, 0x00, 0xA2, 0xA4, 0xA5});
    
    // 异步读取数据
    boost::asio::async_read(serial, boost::asio::buffer(buffer),
        [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
            std::lock_guard<std::mutex> lock(mutex);
            reading_in_progress = false;
            
            if (!ec && bytes_transferred == 10) {
                // 解析重量数据
                uint32_t weight = (buffer[4] << 16) | (buffer[5] << 8) | buffer[6];
                current_weight = static_cast<float>(weight);
                weight_ready = true;
                cv.notify_one(); // 通知主线程重量已准备好
            }
        });
}

float WeightSensor::getWeight() {
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this] { return weight_ready; });
    weight_ready = false;
    return current_weight;
}

void WeightSensor::sendCommand(const std::vector<uint8_t>& command) {
    boost::asio::write(serial, boost::asio::buffer(command.data(), command.size()));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void WeightSensor::sendCommandNoDelay(const std::vector<uint8_t>& command) {
    boost::asio::write(serial, boost::asio::buffer(command.data(), command.size()));
}