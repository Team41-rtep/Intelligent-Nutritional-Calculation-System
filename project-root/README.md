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
1. Download and Flash Standard Raspberry Pi OS Image  
Go to the official Raspberry Pi OS download page:  
👉 https://www.raspberrypi.com/software/operating-systems/    

Download Raspberry Pi OS (64-bit) with Desktop  

Use Win32DiskImager or Raspberry Pi Imager to flash the image:  

Insert your SD card into your computer  

Launch Win32DiskImager  
  
Select the .img file and your SD card device  

Click Write to burn the OS image  

2. First Boot and Basic Configuration  
Insert the SD card into the Raspberry Pi and boot  

Connect to Wi-Fi, enable SSH (optional), and expand filesystem if prompted  

Run:  

sudo apt update && sudo apt upgrade -y

3. Install TensorFlow Lite Runtime
Install dependencies:
sudo apt install -y python3-pip libatlas-base-dev

Install TFLite runtime (C++ development libraries):  
Download the precompiled .deb for arm64 from TensorFlow GitHub  
Or build from source (optional, slower)  

4. Clone the Project
git clone https://github.com/Team41-rtep/Intelligent-Nutritional-Calculation-System.git  
cd Intelligent-Nutritional-Calculation-System  

5. Build the Project (CMake)
mkdir build  
cd build  
cmake ..  
make  

## Usage
 1. Hardware Setup  
Before running the program, make sure all required hardware is properly connected:  


Component	Port/Connection Type  
HDMI Monitor	Connect to Raspberry Pi HDMI port  
USB Camera	Plug into a USB port on the Pi  
Load Sensor	Connect via serial port (e.g., /dev/ttyAMA0) using USB-TTL or GPIO  
Power Supply	Standard Raspberry Pi power adapter  

2. Boot and Access the Terminal  
Power on the Raspberry Pi.  

Wait for the desktop interface or terminal to appear.  

Open a terminal window (or connect via SSH if running headless).   

3. Navigate to Program Directory  
In the terminal, navigate to the program directory where the compiled executable is stored:  

cd ~/work/mytflite/tensorflow/bazel-bin/tensorflow/lite/work/  

4. Run the Fruit Classifier
5. 
Make sure the camera and sensor are connected, then execute:  
./fruit_classifier  
This will:  
Start capturing video from the camera  
Classify the fruit in real time using a TensorFlow Lite model  
Read the weight from the load sensor  
Display nutritional values (protein, carbs, fat, calories) overlaid on the video feed using OpenCV  



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

## Download

[🔗 Click here to download v1.0 release](https://github.com/Team41-rtep/Intelligent-Nutritional-Calculation-System/releases/tag/v1.0)  
[🔗 Click here to download v2.0 release](https://github.com/Team41-rtep/Intelligent-Nutritional-Calculation-System/releases/tag/v2.0)  

## License

This project is licensed under the MIT License.    
See the [LICENSE](./LICENSE) file for more details.    

## Follow & Share  
Platform	Link  
Tik-Tok(CN)    https://v.douyin.com/e2VfmxN4hlU/ daA:/ S@y.GV 09/28   
Instagram	https://www.instagram.com/reel/DIsNyOcIzj-/?igsh=a3Bzc2doYTR3N3p2  
Tik-Tok   https://vm.tiktok.com/ZNdYf6r1x/   


## Team Contribution  
Name | GitHub Branch | Main Responsibilities | Description  

Ge Zhou | GeZhou-hardware | Hardware communication & weight sensor | Developed the WeightSensor class, implemented serial communication, data reading, command sending, and weight acquisition thread   
Zhenyu Xu | ZhenyuXu-ai | AI model integration & fruit classification | Integrated TensorFlow Lite model, handled image preprocessing, inference execution, and label decoding   
Junze Mao | JunzeMao-nutrition | Nutrition database & calculation module | Built the NutritionDatabase class from CSV file, implemented nutritional value calculation based on predicted fruit and weight   
Ziyan Zeng | ZiyanZeng-ui-doc | UI display, documentation, and promotion | Designed OpenCV real-time UI output, managed testing and documentation, updated GitHub README, created demo video and handled social media promotion  
