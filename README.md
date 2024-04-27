# ThingsInNet-Wi-Fi-Provisioning

This code is to streamline the Wi-Fi provisioning process for ThingsInNet ESP32 IoT devices. Note that the program code and instructions provided in here are also applicable to other ESP32 devkits available in the market.

Features:

Connects to a WiFi network using stored credentials (EEPROM).
Creates a temporary access point (AP) for configuration if no WiFi connection is found.
Configures WiFi settings through a web interface accessible on the temporary AP.

Hardware Requirements:

    ESP32 Development Board:
        Any ThingsInNet ESP32 devkit board is compatible.
        Alternatively, use any ESP32 device with a pushbutton connected to pin D15 (pin 34).

Software Requirements:

    Arduino IDE
    WiFi library
    HTTPClient library (not directly used for credentials)
    WebServer library
    EEPROM library

Getting Started:

    Replace default SSID and password with your actual WiFi credentials in the code.
    Upload the code to your ESP32.
    If it doesn't connect automatically, connect to the temporary AP named "Master_AP001" (without password).
    Open a web browser on a connected device and navigate to the ESP32's IP address (shown on serial monitor).
    Enter your desired WiFi network's SSID and password, then click "Submit".
    The ESP32 will save the credentials, restart, and connect to the specified WiFi network on future boots.

The web interface allows you to:

    View the temporary AP's IP address.
    Scan for available WiFi networks.
    Enter and save WiFi credentials for automatic connection.
    This program simplifies WiFi configuration for your ESP32 projects and provides a user-friendly interface for managing credentials.

