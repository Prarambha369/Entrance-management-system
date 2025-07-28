# Entrance Management System (EMS)

## 📋 Overview
A comprehensive C-based Entrance Management System designed for educational institutions to manage student records, conduct entrance examinations, and process results efficiently. The system features a clean, text-based interface and supports both Windows and Unix-like operating systems.

## ✨ Key Features

### 🎓 Student Management
- Complete student registration with personal details
- Easy modification of student records
- Quick search functionality by ID
- Secure data storage in a binary file

### 📝 Examination System
- Interactive multiple-choice questions
- Random question selection from a pool
- Real-time answer validation
- Negative marking (10% deduction for wrong answers)
- Option to skip questions
- Timer support (20-minute exam duration)

### 📊 Results & Ranking
- Automatic score calculation
- Student ranking system
- Detailed result display
- Comprehensive result analysis

## 🛠️ Technical Implementation
- **Language**: C (C99 standard)
- **Dependencies**: Standard C Library, Math Library (-lm)
- **Compatibility**: Windows, Linux, macOS
- **Storage**: Binary file-based storage (`student.txt`)

## 🚀 Getting Started

### Prerequisites
- GCC or any C99 compatible compiler
- Terminal/Command Prompt
- 10MB free disk space

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/Prarambha369/Entrance-management-system.git
   cd Entrance-management-system
   ```

2. Compile the program:
   ```bash
   gcc -o ems ems.c -lm
   ```

3. Run the application:
   ```bash
   ./ems
   ```

## 📋 Usage Guide

### Main Menu Options
1. **Form for Student**: Register new students
2. **Modify Student Record**: Update existing student information
3. **Entrance Examination**: Take the entrance test
4. **Display Student Record**: View all student records
5. **Search Record**: Find specific student records
6. **Rank of Students**: View student rankings based on exam scores
0. **Exit**: Close the application

### Taking the Exam
- Use A/B/C/D to select answers
- Press 'S' to skip a question (cannot be revisited)
- Negative marking applies to wrong answers (-10%)
- Results are saved automatically

## 🛠️ Troubleshooting

### Common Issues
1. **File Permission Errors**:
   - Ensure write permissions in the current directory
   - The program creates/updates `student.txt` in the same directory

2. **Compilation Warnings**:
   - Use `-Wall -Wextra` for detailed warnings
   - Ensure all standard libraries are available

3. **Input Issues**:
   - Follow on-screen instructions carefully
   - Enter data in the correct format as prompted

4. **Cross-Platform Notes**:
   - On Windows, use Command Prompt or PowerShell
   - On Unix-like systems, ensure terminal supports ANSI escape codes

## 📝 Notes
- Data is stored in `student.txt` in the program's directory
- The program handles all file operations automatically
- No internet connection required

## 🤝 Contributing
Contributions are welcome! Please feel free to submit issues and pull requests.

## 📄 License
This project is open source and available under the MIT License.

---
*Created by Mister Bashyal*
1. Launch the program
2. Choose options from the main menu:
   - Option 1: Register new students
   - Option 2: Modify student records
   - Option 4: View all records
   - Option 6: View rankings

### For Students
1. Register using Option 1
2. Note down your ID number
3. Take exam using Option 3
4. View results immediately after completion
5. Check rankings using Option 6

## Key Features
- **Secure Storage**: All student data is stored locally
- **User-Friendly Interface**: Simple menu-driven system
- **Real-Time Processing**: Instant result calculation
- **Data Persistence**: Records maintained between sessions

## Contributing
1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to your branch
5. Create a Pull Request

## Author
- Prarambha Bashyal

## License
This project is licensed under the MIT License - see the LICENSE file for details.
