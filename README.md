# GM-OBD1

What is it, and what does it do.
================================
1. An implemetation to communicate with old GM ECM's specifically 1227165.
2. it runs on a BLACKPILL 10$ board, but any STM 32 would do.
3. The file config.h specifies the pin to be used for ECM communication (PB10 rx and PB3 tx),
Earlier version (last commit: 82b610d) used one wire with a serial 10K resistor between ALDL line and MCU. But a small
2 transistor level adaptor was created to better control voltage levels. (ECM is 5 volts. STM is 3.3).
![OBD1 connector with adaptor and schematic](./pics/20220803_140636.jpg?raw=true "OBD1 connector with adaptor")

4. on the other side it will publish an usb-serial interface. Here a raw byte stream will presented to a laptop running a scanner software.
5. the Interface will start by reading the 160 baud 25 bytes, frames.
This can be used to get most of the information needed.
6. It  will try to initiate 8192 Baud mode, with 64 bytes frames.


How to build
============

this software is implemented as an application on top of the RTScheduler OS (also available on my github account).

1. Checkout the RTScheduler and build it. An arm compiler will be needed. but that should be documented in the RTScheduler OS repo.
2. Modify the Makefile macro KREL to point into to the build directory of the
RTScheduler.
3. make

the result will be a loadfile myCore. use the st-link program and arm-(xxx)-gdb to burn it to the micro controller.


Project Architecture
====================

This is a sophisticated embedded diagnostic tool that transforms a BLACKPILL microcontroller into a professional-grade OBD1 scanner for legacy GM vehicles.

## Core Components

**obd1_drv.c/h** - Core OBD1 Protocol Driver (91% of codebase)
- Implements low-level bit-banged serial communication
- Manages GPIO interrupt handling for the ALDL line
- Implements high-resolution timer-based sampling for accurate baud rate timing
- Provides driver abstraction for hardware access (GPIO, timers, LEDs)
- Handles both 160 baud and 8192 baud communication modes
- Manages receive/transmit buffers with circular queuing (supports up to 4 buffered messages)

**main.c** - Application Orchestration
- Initializes the OBD1 interface threads
- Manages mode switching between 160B and 8192B protocols
- Handles serial port communication and command routing
- Registers event handlers and timers
- Provides CLI command interface for diagnostic requests (help, sendreq commands)

**menu.c/h** - UI Menu System
- Implements rotatable display panels for data visualization
- Four panel views: SensorData, RawData, FlagData, ErrorData
- Terminal-based UI rendering using panel library
- Real-time updates of ECM diagnostic information
- Supports menu navigation via serial commands ('t'=toggle, 'm'=menu, 'r'=raw, 's'=speed switch)

**asynchio.c/h** - Asynchronous Event-Driven I/O
- Event registration system for file descriptors
- Timer management for high-resolution timing
- Non-blocking event dispatcher (do_event loop)
- Event types: EV_READ, EV_WRITE, EV_STATE

**system_params.c** - Configuration Management
- System parameter storage and retrieval

## Communication Modes

**160 Baud Mode (Default)**
- Entry point for all connections
- 25-byte diagnostic frames transmitted every ~1.5 seconds
- Data includes: RPM, coolant temperature, throttle position, intake air temp, oxygen sensor readings, malfunction flags
- Uses synchronization pattern detection (9 consecutive '1' bits) to frame alignment
- Provides interframe spacing monitoring (~1 second between frames)
- Bit timing: 6.25ms per bit with 2500-4000µs tolerance window

**8192 Baud Mode (High-Speed)**
- 64-byte frames for enhanced real-time data
- Initiated with special packet: {0x80, 0x56, 0x01, 0x29}
- Supports continuous data streaming
- Uses standard start (0) and stop (1) bits
- Periodic mode 0x10 requests to maintain link (every 500ms)
- Bit timing: 122µs per bit with ~40µs tolerance window

## Key Features

**Multi-Mode Data Display**
- Raw sensor data: Temperature conversions, RPM calculations, throttle percentages
- Flag data: Diagnostic malfunction codes with bit-level interpretations
- Error data: ECM fault codes and system status flags
- Raw mode: Hexadecimal byte stream for advanced diagnostics

**USB Serial Bridge**
- Exposes all diagnostic data as a USB serial interface
- External scanner software can connect to the interface
- Raw and formatted data transmission modes
- Non-blocking asynchronous data streaming

**Hardware Abstraction**
- GPIO input monitoring for ALDL line (pull-down configuration)
- GPIO output control for transmission (open-drain/push-pull design)
- High-resolution timer access for bit-level timing
- LED status indicators for operational feedback

**Robust Protocol Implementation**
- Bit-level serial reception at 160 baud with tolerance windows
- Bit-level serial transmission at 8192 baud with tight timing
- Automatic timeout handling and protocol recovery
- Message buffering with 4-message circular queue
- Interframe spacing detection and validation

## Code Distribution

- **C (91.2%)**: Core driver logic, application code, protocol implementation
- **Smarty (7.3%)**: Menu templates and UI rendering
- **Makefile (1.5%)**: Build system configuration

## Operational Workflow

1. **Initialization**: Device boots in 160 Baud mode
2. **Connection**: Waits for ECM sync pattern (9 consecutive '1' bits)
3. **Data Reception**: Receives 25-byte frames via bit-banged GPIO interrupt (EV_READ events)
4. **Display**: Formats and displays data in selected panel view
5. **Mode Switching**: User can send 's' command to switch to 8192 Baud mode
6. **High-Speed Operation**: Once in 8192B mode, sends periodic mode requests to maintain link
7. **USB Bridge**: All data simultaneously transmitted to external scanner software
8. **State Management**: Automatic timeout recovery if link is lost

## Data Decoded from 160B Frames

Each 25-byte frame contains:
- **Byte 0**: Mode word 2 with flags (road speed pulse, ref pulse, test mode indicators, high battery voltage, air meter pulse)
- **Bytes 1-2**: PROM ID
- **Byte 3**: IAC stepper position
- **Byte 4**: Coolant temperature (converted: (value*75)-4000 °C)
- **Byte 5**: Vehicle speed (MPH)
- **Byte 6**: EGR duty cycle
- **Byte 7**: Engine RPM (converted: value*25)
- **Byte 8**: Throttle position (converted: (value*100)>>8 %)
- **Byte 9**: Pulse width correction (closed loop)
- **Byte 10**: Oxygen sensor reading
- **Bytes 11-14**: Malfunction flags 1-4 (speed sensor errors, temp sensor faults, TPS faults, emissions sensor problems, etc.)
- **Byte 15**: Fuel/Air ratio
- **Byte 16**: Intake air temperature (MAT) (converted: (value*75)-4000 °C)
- **Byte 17**: MCU status flags (Park/Neutral, cruise control, TCC, power steering pressure)
- **Byte 18**: LV8 status
- **Byte 19**: Block Learned Multiplier (BLM)
- **Byte 20**: ALDL counter
- **Byte 21**: Display flow data 1
- **Byte 22**: Display flow data 2 (converted: (value*100)>>8)
- **Bytes 23-24**: Injector pulse width in milliseconds (byte 23 = integer, byte 24 = decimal)

## Using Raw Output for AI-Based Fault Analysis

This project excels at capturing raw diagnostic data that can be analyzed by AI tools for advanced fault tracing and engine diagnostics of GM TPI engines. This demonstrates why elegant, simple interfaces are superior to complex GUIs.

### Workflow:

1. **Start the Device**: Open a terminal emulator (PuTTY, Tera Term, etc.) configured for serial communication
2. **Switch to 8kbaud Raw Mode**: Send the 's' command to switch from 160 baud to 8192 baud mode
3. **Enable Terminal Logging**: Configure your terminal emulator to log/capture all output to a file with timestamps
4. **Run Recording**: Let the interface capture data while the engine operates under various conditions (idle, acceleration, load, etc.)
5. **Stop and Save**: Terminate logging once you have sufficient timestamped data
6. **Analyze with AI**: Copy the entire raw timestamped byte stream and paste it into your preferred AI tool (Claude, ChatGPT, etc.)
7. **Get Diagnostic Insights**: The AI will:
   - Format the raw data for readability
   - Identify patterns and anomalies
   - Trace fault codes and root causes
   - Provide detailed engine performance analysis
   - Suggest corrective actions

### Why This Approach is Superior:

- ✅ **Simple raw text format** is portable and universally shareable
- ✅ **No proprietary tools needed** - works with any AI model
- ✅ **Leverages AI's pattern recognition** for advanced fault analysis
- ✅ **Timestamps enable temporal correlation** of events and sensor changes
- ✅ **Future-proof design** that will work for decades to come
- ✅ **Elegant simplicity beats complex GUIs** - demonstrates the power of well-designed interfaces
- ✅ **Easy to document and share** diagnostic sessions with others
- ✅ **Enables secondary use cases** that fancy graphical interfaces could never achieve

This architecture provides a complete bridge between legacy GM ECMs and modern diagnostic tools, enabling detailed real-time monitoring and fault diagnosis of vehicles from the 1980s-1990s era.