# 🚗 Car Black Box using PIC18F4580

## 📖 Overview

A **Car Black Box** is an automotive event data recording system that continuously monitors and stores important vehicle information. In the event of an incident, the recorded data can be used to analyze vehicle behavior and understand what occurred before the event.

This project is a simplified implementation of a **Car Black Box** developed using the **PIC18F4580 Microcontroller** as part of the **Advanced Embedded Systems Certification (ECEP)** at **Emertxe Information Technologies**.

The system monitors and records essential vehicle parameters such as **Time**, **Gear Position**, and **Speed**, while providing a menu-driven interface to manage the recorded logs.

---

## ✨ Features

- Real-time monitoring of Time, Gear and Speed
- Automatic event logging during gear changes
- DS1307 RTC integration for timestamp generation
- External EEPROM for non-volatile event storage
- UART-based log download
- Menu-driven CLCD interface
- Set Time functionality
- View stored logs
- Clear stored logs
- Event logs retained after power cycling using External EEPROM

---

## 🛠 Hardware Used

- PIC18F4580 Microcontroller
- DS1307 Real-Time Clock (RTC)
- External EEPROM (I²C)
- Character LCD (16x2)
- Matrix Keypad
- Potentiometer (Speed Simulation)
- UART Interface

---

## 💻 Software Used

- MPLAB X IDE
- XC8 Compiler
- Embedded C

---

## 🔌 Communication Protocols

- I²C
  - DS1307 RTC
  - External EEPROM

- UART
  - Log Download using Tera Term

- ADC
  - Speed Simulation

---

## 📂 Project Structure

```
Car-BlackBox-PIC18F4580
│
├── adc.c
├── adc.h
├── clcd.c
├── clcd.h
├── ds1307.c
├── ds1307.h
├── ext_eeprom.c
├── ext_eeprom.h
├── i2c.c
├── i2c.h
├── uart.c
├── uart.h
├── matrix_keypad.c
├── matrix_keypad.h
├── main.c
└── ...
```

---

## 🎥 Project Demonstration

A complete demonstration of the project is available on my LinkedIn profile.

🔗 **Watch the project demonstration here:**
//Post link

The video showcases:
- Real-time Time, Gear, and Speed Monitoring
- Automatic Event Logging
- Menu-driven Interface
- View Log
- Download Log (UART)
- Set Time
- Clear Log
- External EEPROM Data Retention after Power Cycling

## ⚙ Working Principle

The system continuously monitors:

- Current Time from DS1307 RTC
- Gear Position
- Vehicle Speed

Whenever a gear change occurs:

- Current Time
- Gear Position
- Speed

are recorded into the External EEPROM.

The user can:

- View Logs
- Download Logs through UART
- Clear Logs
- Set RTC Time

using the menu-driven interface.

---

## 🚀 Future Improvements

- CAN Bus Integration
- GPS Module
- SD Card Data Logging
- Accident Detection
- Temperature Monitoring
- Wireless Data Transfer

---

## 📚 Learning Outcomes

Through this project, I gained practical experience in:

- Embedded C Programming
- Peripheral Interfacing
- I²C Communication
- UART Communication
- EEPROM Memory Management
- RTC Interfacing
- Menu-driven Embedded Applications
- Embedded Firmware Debugging

---

## 🙏 Acknowledgment

This project was developed as part of the **Advanced Embedded Systems Certification (ECEP)** at **Emertxe Information Technologies**.

I sincerely thank my mentors at Emertxe Information Technologies for their continuous guidance, encouragement, and valuable support throughout the development of this project.

---

## 👨‍💻 Author

**Mohammed Maaz**

Embedded C | Embedded Systems

LinkedIn:
www.linkedin.com/in/mdm-mohammedmaaz

GitHub:
https://github.com/mohammedmaaz17-63

---

⭐ If you found this project interesting, feel free to star the repository.
