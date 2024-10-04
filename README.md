
# ENTRANCE-MANAGEMENT-SYSTEM

## Overview
The ENTRANCE-MANAGEMENT-SYSTEM is a C language-based program designed to streamline the management of entrance exams for universities, colleges, and jobs. The program helps store and manage candidate data, minimize errors, and ensure a smooth, accurate, and fast process for exam results and candidate selection.
## Key Objectives:
- Data Management: Store candidate data based on their submitted forms.
- Candidate Selection: Select qualified candidates for universities, colleges, and job applications.
- Error Reduction: Minimize errors and prevent corruption in the selection process.
- Efficiency: Maximize accuracy and speed when publishing results.


## Features:
- Form Publishing: Publishes and collects candidate information for validation.
- Data Storage: Stores candidate information for future use.
- Data Modification: Allows candidates to modify their details before the exam.
- Candidate Withdrawal: Candidates can withdraw their application by deleting their data.
- Timed Exam: A 20-minute entrance exam with a total score of 10 marks.
- Question Format: Displays 10 questions, each worth 1 mark, with a 10% negative marking per incorrect answer.
- Final Submission Lock: Once submitted, the answers cannot be modified.
- Instant Results: Displays the candidate’s marks immediately after submission.
- Ranked Results: Displays rank-wise results once all candidates have completed the exam.

## Installation

1. Prerequisites:

- Install a C compiler (e.g., gcc for Linux/Mac or MinGW for Windows).

- Ensure your system is set up to run C programs.

-  Clone the Repository: Clone the project repository to your local machine using:


```bash
 git clone https://github.com/heyitsprashant/Entrance-management-system

```
3. Compile the Program: Navigate to the project directory and compile the program with the following command:

```
gcc -o entrance_management_system entrance_management_system.c
```
4. Run the Program: After compiling, you can run the program by executing:

```
./entrance_management_system
```
5. Dependencies: This program does not require any external libraries or dependencies beyond a C compiler.

## Usage/Examples

Once the program is running, it will guide the user through the entrance exam process. Candidates will:

- Submit their personal information through a form.
- Be presented with 10 questions.
- Submit their answers for evaluation.
- Receive instant feedback on their score, including any deductions for incorrect answers.
- After all candidates have completed the exam, a rank-wise result list will be displayed.


## Contributing

If you'd like to contribute to this project, follow these steps:

- Fork the repository.
- Create a new feature branch (`git checkout -b feature-branch`).
* Commit your changes (`git commit -m 'Add new feature'`).
* Push to the branch (`git push origin feature-branch`).
* Submit a Pull Request.


## License


This project is licensed under the [MIT](https://choosealicense.com/licenses/mit/) License. See the LICENSE file for more information.



