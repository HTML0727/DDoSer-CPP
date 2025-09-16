# DDoS Attack Tool (DDoSer)

A network penetration testing framework designed for security research and educational purposes. Implemented in C++, it provides various types of network attack simulations to help understand and defend against DDoS threats.

中文版本: [中文README文件](ZH.README.md)

## Project Structure
```

DDoSer-cpp/
├──CMakeLists.txt
├──README.md
├──ZH.README.md
├──.gitignore
├──src/
│├── main.cpp
│├── ui/
││   ├── UIManager.cpp
││   ├── ThemeManager.cpp
││   ├── InputHandler.cpp
││   └── PageManager.cpp
│├── attack/
││   ├── AttackBase.cpp
││   ├── AttackFactory.cpp
││   ├── ARPAttack.cpp
││   ├── ICMPAttack.cpp
││   ├── UDPAttack.cpp
││   ├── TCPAttack.cpp
││   └── DNSAttack.cpp
│├── network/
││   ├── NetworkManager.cpp
││   ├── PacketBuilder.cpp
││   ├── SocketWrapper.cpp
││   └── RawSocket.cpp
│├── logic/
││   ├── LogicCore.cpp
││   └── StatsCollector.cpp
│├── config/
││   ├── ConfigManager.cpp
││   └── PresetManager.cpp
│├── util/
││   ├── Logger.cpp
││   ├── Randomizer.cpp
││   ├── Helpers.cpp
││   ├── ThreadPool.cpp
││   └── Validator.cpp
│├── Application.cpp
│├── Constants.cpp
│└── Exception.cpp
├──include/
│├── ui/
││   ├── UIManager.h
││   ├── ThemeManager.h
││   ├── InputHandler.h
││   └── PageManager.h
│├── attack/
││   ├── AttackBase.h
││   ├── AttackFactory.h
││   ├── ARPAttack.h
││   ├── ICMPAttack.h
││   ├── UDPAttack.h
││   ├── TCPAttack.h
││   └── DNSAttack.h
│├── network/
││   ├── NetworkManager.h
││   ├── PacketBuilder.h
││   ├── SocketWrapper.h
││   └── RawSocket.h
│├── logic/
││   ├── LogicCore.h
││   └── StatsCollector.h
│├── config/
││   ├── ConfigManager.h
││   └── PresetManager.h
│├── util/
││   ├── Logger.h
││   ├── Randomizer.h
││   ├── Helpers.h
││   ├── ThreadPool.h
││   └── Validator.h
│├── Application.h
│├── Constants.h
│└── Exception.h
└──preconfigs/
├── mainpage.json
└── function.json

```

## Features

- Multiple attack types including:
  - ARP-based attacks
  - ICMP Flood attacks
  - TCP SYN Flood attacks
  - UDP Flood attacks
  - DNS Amplification attacks
  - Extensible for more attack types
- Intelligent address detection and randomization
- Configurable attack targets and parameters
- Real-time statistics monitoring
- Preset attack configuration management
- User-friendly terminal interface using ncurses
- Multi-theme support
- Comprehensive logging system

## Requirements

- GCC compiler (C++17 support)
- CMake (3.12 or higher)
- ncurses library
- cJSON library
- Unix-like environment (Linux/macOS)
- Root privileges (for sending raw packets)

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/DDoSer.git
   cd DDoSer
```

1. Install dependencies:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install build-essential cmake libncurses5-dev libcjson-dev
   
   # Arch Linux
   sudo pacman -S base-devel cmake ncurses cjson
   
   # Gentoo Linux
   sudo emerge sys-devel/gcc dev-util/cmake sys-libs/ncurses dev-libs/cjson
   
   # macOS (using Homebrew)
   brew install cmake ncurses cjson
   ```
2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

Usage

1. Run the tool (requires root privileges):
   ```bash
   sudo ./ddoser
   ```
2. Use the following keyboard shortcuts:
   · R : Run/Stop attack
   · F1: Add/Edit target
   · F2: Network configuration
   · F3: View statistics
   · F4: Save configuration
   · F5: Load preset
   · Q: Quit
   · ? : Show help

Command Line Options

```bash
sudo ./ddoser [options]
Options:
  -d, --daemon         Run as daemon
  -c, --config FILE    Use specified config file
  -l, --log FILE       Use specified log file
  -h, --help           Show help message
  -v, --version        Show version information
```

Configuration

The tool uses JSON configuration files, with the default location at ~/.ddoser/config.json. The configuration file includes:

· Attack configuration (type, target, intensity, etc.)
· Target list
· Network interface settings
· Theme preferences

Preset files are stored in the ~/.ddoser/presets/ directory, allowing you to save and load commonly used attack configurations.

Disclaimer

This tool is for educational and research purposes only. Unauthorized use of this tool to attack networks is illegal and unethical. The developers are not responsible for any misuse of this software.

Before using this tool, please ensure:

1. You have explicit authorization for the target system
2. You understand the relevant local laws and regulations
3. You only use it in isolated test environments

License

This project uses the NC-OSL License - see the LICENSE file for details.

Contributing

We welcome issues and pull requests to help improve this project. Before contributing code, please ensure:

1. Code follows the project's coding style
2. Includes appropriate test cases
3. Updates relevant documentation

Support

If you encounter issues while using this tool, you can get help through:

1. Checking the project Wiki pages
2. Submitting issues to the GitHub repository
3. Contacting the development team (for educational institutions and security researchers only)