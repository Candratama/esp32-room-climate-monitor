# Contributing to ESP32 Room Climate Monitor

Thank you for your interest in contributing to this project! We welcome contributions from the community.

## How to Contribute

### Reporting Bugs

1. **Check existing issues** first to avoid duplicates
2. **Use the bug report template** when creating new issues
3. **Include detailed information**:
   - Hardware setup
   - Software versions
   - Serial output logs
   - Steps to reproduce

### Suggesting Features

1. **Check existing feature requests** to avoid duplicates
2. **Describe the feature** in detail
3. **Explain the use case** and benefits
4. **Consider implementation impact**

### Code Contributions

1. **Fork the repository**
2. **Create a feature branch** from `main`
3. **Follow the code style** (see below)
4. **Add tests** if applicable
5. **Update documentation** as needed
6. **Submit a pull request**

## Code Style Guidelines

### C++ Code Standards

- **Naming Convention**:
  - Functions: `camelCase` (e.g., `readSensorData()`)
  - Variables: `camelCase` (e.g., `sensorConnected`)
  - Constants: `UPPER_CASE` (e.g., `SENSOR_TIMEOUT`)
  - Private members: `_underscore` prefix

- **Documentation**:
  - All functions must have header comments
  - Complex logic needs inline comments
  - Use descriptive variable names

- **Function Design**:
  - Single responsibility principle
  - Keep functions under 50 lines
  - Use meaningful parameter names

### Documentation Standards

- **README files**: Use clear headers and examples
- **Code comments**: Explain *why*, not just *what*
- **API documentation**: Include parameters and return values

## Pull Request Process

1. **Update documentation** for any new features
2. **Ensure all tests pass** (if applicable)
3. **Follow commit message format**:
   ```
   type(scope): description
   
   - Detailed explanation of changes
   - Why the change was needed
   - Any breaking changes
   ```

4. **Request review** from maintainers
5. **Address feedback** promptly

## Development Setup

1. **Install required tools**:
   - [PlatformIO](https://platformio.org/)
   - [VS Code](https://code.visualstudio.com/)

2. **Clone and setup**:
   ```bash
   git clone https://github.com/YOUR_USERNAME/esp32-room-climate-monitor.git
   cd esp32-room-climate-monitor
   pio run
   ```

3. **Test your changes**:
   ```bash
   pio run --target upload
   pio device monitor
   ```

## Testing Guidelines

### Hardware Testing

- Test on real ESP32 hardware
- Verify all sensor connections
- Check OLED display functionality
- Validate RS485 communication

### Software Testing

- Compile without warnings
- Test error handling paths
- Verify memory usage
- Check serial output

## Community Guidelines

- **Be respectful** and inclusive
- **Help others** learn and contribute
- **Share knowledge** and experiences
- **Follow the code of conduct**

## Getting Help

- **Documentation**: Check existing docs first
- **Issues**: Search existing issues
- **Discussions**: Use GitHub discussions for questions
- **Contact**: Reach out to maintainers for urgent matters

## Recognition

Contributors will be recognized in:
- Repository contributors list
- Release notes
- Project documentation

Thank you for contributing to the ESP32 Room Climate Monitor project! 🚀
