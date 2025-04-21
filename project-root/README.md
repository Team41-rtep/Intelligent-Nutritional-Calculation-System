## Project Overview

Intelligent-Nutritional-Calculation-System is a real-time embedded system that identifies fruits using a camera and a lightweight neural network model, measures their weight using a serial-connected weight sensor, and calculates their nutritional content instantly. It provides a visual and intuitive interface to help users monitor nutritional intake in daily life.

This project combines computer vision, serial communication, and nutrition analysis into a single user-friendly solution, running on a Raspberry Pi with real-time data acquisition and processing.

## Real-Time Requirements

To ensure responsive and interactive operation, the system is designed with the following real-time features:

Live video feed from a camera is captured and processed frame-by-frame for fruit classification using a TensorFlow Lite model.

Serial communication with a weight sensor is handled via a dedicated background thread, ensuring continuous data flow without blocking the main UI.

Real-time feedback is provided by overlaying predictions and nutritional information onto the live video stream using OpenCV.

Non-blocking execution is achieved through multithreading and event-driven architecture, avoiding delays or unresponsiveness in user interaction.

## System Requirements

Raspberry Pi 5

USB Camera 

Serial Weight Sensor (connected via /dev/ttyAMA0 or /dev/ttyUSB0)

C++17 or newer

Dependencies:
TensorFlow Lite C++ API
OpenCV
Boost.Asio

##  Install Dependencies

## Usage

## Run the app

## Repository Structure

project-root/
├── src/                         # Source code               
│   ├── weight_sensor.cc        
│   ├── fruit_classifier         
│   ├── fruit_classifier.runfiles_manifest
│   ├── fruit_classifier-2.params
│   ├── weight_sensor            
│   ├── weight_sensor.runfiles_manifest
│   ├── weight_sensor-2.params
├── include/                     # Header files 
│   ├── fruit_classifier.cc 
├── models/                      # AI models
│   └── fruit_model.tflite      
├── data/                        # Dataset or CSV files
│   └── fruits.csv               
├── doc/                         # Design docs, architecture diagrams
│   └──weight_sensor.jpg   
│   └── Structure.png 
├── media/                       # Images, screenshots, demo videos
│   └── demo_screenshot.jpg   
├── README.md                    # Project overview and guide
├── LICENSE                      # Open source license
└── .github/                     # GitHub templates
    ├── ISSUE_TEMPLATE.md
    └── PULL_REQUEST_TEMPLATE.md



## Team Contribution
Name | GitHub Branch | Main Responsibilities | Description

Ge Zhou | GeZhou-hardware | Hardware communication & weight sensor | Developed the WeightSensor class, implemented serial communication, data reading, command sending, and weight acquisition thread 
Zhenyu Xu | ZhenyuXu-ai | AI model integration & fruit classification | Integrated TensorFlow Lite model, handled image preprocessing, inference execution, and label decoding 
Junze Mao | JunzeMao-nutrition | Nutrition database & calculation module | Built the NutritionDatabase class from CSV file, implemented nutritional value calculation based on predicted fruit and weight 
Ziyan Zeng | ZiyanZeng-ui-doc | UI display, documentation, and promotion | Designed OpenCV real-time UI output, managed testing and documentation, updated GitHub README, created demo video and handled social media promotion
