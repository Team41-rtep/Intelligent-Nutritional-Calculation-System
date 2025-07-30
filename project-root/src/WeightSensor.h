#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H

#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>

class WeightSensor {
public:
    WeightSensor(const std::string& device, int baudRate);
    ~WeightSensor();
    
    void requestWeight();
    float getWeight();
    void sendCommand(const std::vector<uint8_t>& command);
    void sendCommandNoDelay(const std::vector<uint8_t>& command);

private:
    boost::asio::io_context io;
    boost::asio::serial_port serial;
    std::thread io_thread;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard;
    
    std::mutex mutex;
    std::condition_variable cv;
    uint8_t buffer[10];
    float current_weight = 0.0f;
    bool weight_ready = false;
    bool reading_in_progress = false;
};

#endif // WEIGHTSENSOR_H