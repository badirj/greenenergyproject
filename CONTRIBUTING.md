# Contributing to Smart Hybrid Energy Harvesting System

First off, thank you for considering contributing to this project! It's people like you that make this project such a great learning resource for the community.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Getting Started](#getting-started)
- [Development Process](#development-process)
- [Style Guidelines](#style-guidelines)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)

## Code of Conduct

This project and everyone participating in it is governed by common sense and respect. By participating, you are expected to uphold these principles:

- **Be respectful** of differing viewpoints and experiences
- **Accept constructive criticism** gracefully
- **Focus on what is best** for the community
- **Show empathy** towards other community members

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When you create a bug report, include as many details as possible:

**Bug Report Template:**
```markdown
**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. See error

**Expected behavior**
What you expected to happen.

**Screenshots**
If applicable, add screenshots.

**Hardware/Software:**
 - ESP32 board version
 - Arduino IDE version
 - Android version
 - Firebase SDK version

**Additional context**
Any other context about the problem.
```

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, include:

**Enhancement Template:**
```markdown
**Is your feature request related to a problem?**
A clear description of the problem.

**Describe the solution you'd like**
What you want to happen.

**Describe alternatives you've considered**
Other solutions you've thought about.

**Additional context**
Mockups, diagrams, or examples.

**Implementation complexity**
Your assessment of how difficult this would be.
```

### Your First Code Contribution

Unsure where to begin? Start with issues labeled:
- `good first issue` - Simple issues for beginners
- `help wanted` - Issues that need community help
- `documentation` - Improvements to docs

### Areas for Contribution

#### ESP32 Firmware
- Battery management improvements
- Power optimization
- Additional sensor support
- Error recovery mechanisms
- Sleep mode implementation

#### Android Application
- UI/UX improvements
- Additional chart types
- Notification system
- Historical data analysis
- Settings and preferences
- Dark mode
- Tablet support

#### Documentation
- Tutorial improvements
- Translation to other languages
- Video guides
- Circuit diagrams
- Troubleshooting tips
- FAQ section

#### Hardware
- PCB design
- 3D-printable enclosure
- Alternative component suggestions
- Power optimization circuits
- Additional sensor modules

#### Automation
- Additional N8N workflows
- Home Assistant integration
- MQTT support
- Voice assistant integration
- Automated backup systems

## Getting Started

### Prerequisites

**For ESP32 Development:**
- Arduino IDE 1.8.x or later
- ESP32 board support package
- All required libraries (see README.md)

**For Android Development:**
- Android Studio Electric Eel or later
- JDK 11 or later
- Android SDK API 25+

**For Documentation:**
- Markdown editor
- Basic Git knowledge

### Development Setup

1. **Fork the repository**
   ```bash
   # Click "Fork" on GitHub
   ```

2. **Clone your fork**
   ```bash
   git clone https://github.com/YOUR_USERNAME/greenenergyproject.git
   cd greenenergyproject
   ```

3. **Create a branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

4. **Make your changes**
   - Follow style guidelines below
   - Test thoroughly
   - Update documentation

5. **Commit your changes**
   ```bash
   git add .
   git commit -m "Add feature: your feature description"
   ```

6. **Push to your fork**
   ```bash
   git push origin feature/your-feature-name
   ```

7. **Create Pull Request**
   - Go to original repository on GitHub
   - Click "New Pull Request"
   - Select your branch
   - Fill in PR template

## Development Process

### Branch Naming

- `feature/` - New features
- `bugfix/` - Bug fixes
- `docs/` - Documentation changes
- `refactor/` - Code refactoring
- `test/` - Test additions/changes

Examples:
- `feature/add-mqtt-support`
- `bugfix/battery-percentage-calculation`
- `docs/improve-wiring-diagram`

### Testing Requirements

**ESP32 Code:**
1. Compile without errors
2. Test on actual hardware if possible
3. Verify Serial Monitor output
4. Check memory usage
5. Test edge cases (low battery, WiFi disconnect, etc.)

**Android Code:**
1. Build successfully
2. No lint errors
3. Test on emulator and real device
4. Test different screen sizes
5. Test with slow/no internet
6. Check for memory leaks

**Documentation:**
1. Check spelling and grammar
2. Verify all links work
3. Test code examples
4. Ensure markdown renders correctly
5. Check for consistency

### Code Review Process

All contributions go through code review:
1. Automated checks (if set up)
2. Manual review by maintainer
3. Discussion and feedback
4. Approval and merge

Be patient! Reviews may take time, especially for large changes.

## Style Guidelines

### ESP32 (C++) Style

```cpp
// Use descriptive variable names
float batteryVoltage = 0.0;  // Good
float bv = 0.0;               // Bad

// Comment complex logic
// Calculate battery percentage using voltage curve
int percentage = calculateBatteryPercentage(voltage);

// Constants in UPPER_CASE
#define MAX_VOLTAGE 4.2
const int BATTERY_PIN = 36;

// Functions: camelCase
void readBatteryVoltage() {
    // Implementation
}

// Indentation: 2 spaces
if (condition) {
  doSomething();
}
```

### Java (Android) Style

```java
// Follow Android conventions
public class MainActivity extends AppCompatActivity {
    // Private fields with 'm' prefix (optional)
    private FirebaseFirestore mDb;
    
    // Method names: camelCase
    private void updateBatteryUI() {
        // Implementation
    }
    
    // Constants: UPPER_CASE
    private static final int UPDATE_INTERVAL = 30000;
    
    // Annotations before declarations
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
}
```

### Documentation Style

```markdown
# Main Heading (H1)

## Section Heading (H2)

### Subsection (H3)

- Use bullet points for lists
- Keep lines under 100 characters when possible

**Bold** for emphasis
*Italic* for notes

`code` for inline code

\`\`\`language
code blocks
\`\`\`

[Links](https://example.com) should be descriptive
```

### Commit Message Guidelines

Format:
```
<type>: <subject>

<body>

<footer>
```

**Types:**
- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation changes
- `style:` Code style changes (formatting)
- `refactor:` Code refactoring
- `test:` Test additions/changes
- `chore:` Build process or auxiliary tool changes

**Examples:**

```
feat: Add MQTT broker support for local communication

Implement MQTT publish/subscribe for local network
communication without internet dependency. Includes
configuration options in ESP32 code.

Closes #123
```

```
fix: Correct battery percentage calculation

Previous calculation didn't account for voltage curve
of 18650 cells. Updated to use proper LiPo voltage
to percentage mapping.

Fixes #456
```

```
docs: Update wiring diagram with new piezo configuration

Added clearer schematic for piezo rectifier circuit
and corrected voltage divider resistor values.
```

### Pull Request Template

When creating a PR, include:

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Refactoring

## How Has This Been Tested?
- [ ] ESP32 hardware testing
- [ ] Android emulator
- [ ] Android device (specify model)
- [ ] Manual testing

## Checklist
- [ ] My code follows the project's style guidelines
- [ ] I have commented my code where necessary
- [ ] I have updated the documentation
- [ ] My changes generate no new warnings
- [ ] I have tested my changes
- [ ] Related issues are linked

## Screenshots (if applicable)

## Additional Notes
```

## Pull Request Process

1. **Ensure PR is focused**
   - One feature/fix per PR
   - Keep changes minimal
   - Split large changes into multiple PRs

2. **Update documentation**
   - README.md if adding features
   - Code comments for complex logic
   - Relevant .md files

3. **Test thoroughly**
   - Test all affected functionality
   - Include test results in PR

4. **Wait for review**
   - Address feedback promptly
   - Be open to suggestions
   - Discuss major disagreements respectfully

5. **After approval**
   - Squash commits if requested
   - Wait for maintainer to merge

## Recognition

Contributors will be recognized in:
- GitHub contributors page
- Project documentation (if significant contribution)
- Release notes

## Questions?

- Open an issue with the `question` label
- Check existing documentation
- Review closed issues for similar questions

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Thank You!

Your contributions make this project better for everyone. Whether it's code, documentation, bug reports, or suggestions - every contribution is valuable!

---

**Happy Contributing! 🚀**
