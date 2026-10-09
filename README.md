# Markdown-to-HTML-Converter
## 1. Project Description

The Markdown to HTML Converter is a C++ application that converts Markdown
files (.md) into properly formatted HTML files (.html).

It reads Markdown syntax from an input file, parses the content, and generates
an HTML document that can be opened in a web browser.
The project demonstrates Object-Oriented Programming concepts, file handling,
string manipulation, and state machine design.

## 2. Objectives

- Convert Markdown files into HTML documents.
- Support common Markdown formatting.
- Generate structured HTML output.
- Apply Object-Oriented Programming concepts.
- Handle file operations and errors.
- Manage memory safely using modern C++.

## 3. Features

- Headings of different levels
- Bold and italic text
- Ordered and unordered lists
- Paragraphs and line breaks
- Hyperlinks and images
- Code blocks
- Horizontal rules
- HTML document generation
- File input and output
- Exception handling

## 4. Technologies Used

- Programming Language: C++
- File Handling: ifstream and ofstream
- Compiler: GCC (g++)
- Concepts: OOP, Templates, STL, and Exception Handling

## 5. Project Structure

MarkdownToHTML/
│
├── main.cpp
├── MarkdownParser.h
├── MarkdownParser.cpp
├── HTMLDocument.h
├── HTMLDocument.cpp
├── FileHandler.h
├── FileHandler.cpp
├── input.md
├── output.html
└── README.md
│
├── input/
│   ├── index.md
│   ├── about.md
│   └── contact.md
│
└── output/
    ├── index.html
    ├── about.html
    └── contact.html
## 6. Object-Oriented Programming Concepts

### Classes and Objects,Encapsulation, Inheritance,Polymorphism ,Templates ,Exception Handling ,Operator Overloading ,Memory Management ,State Machine ,File Handling

## 7. Installation and Execution

### Step 1: Install the Compiler

Install GCC if it is not already available.

On Ubuntu:

    sudo apt update
    sudo apt install g++

### Step 2: Compile the Program

Open a terminal in the project directory and execute:

    g++ -std=c++14 main.cpp MarkdownParser.cpp HTMLDocument.cpp FileHandler.cpp -o converter

### Step 3: Run the Program

    ./converter

The program reads input.md and generates output.html.

## 8. Sample Input

Create a file named input.md:

    # Welcome to My Website

    ## Introduction

    This is a **Markdown to HTML Converter**.

    Markdown makes *formatting* simple.

    ### Features

    - Easy to use
    - Fast conversion
    - HTML output

    Visit [Google](https://www.google.com).

## 9. Sample Output

The generated output.html contains a complete HTML document.

For example:

    <h1>Welcome to My Website</h1>

    <h2>Introduction</h2>

    <p>This is a <strong>Markdown to HTML Converter</strong>.</p>

    <p>Markdown makes <em>formatting</em> simple.</p>

    <h3>Features</h3>

    <ul>
    <li>Easy to use</li>
    <li>Fast conversion</li>
    <li>HTML output</li>
    </ul>

    <p>Visit <a href="https://www.google.com">Google</a>.</p>

The HTML file can be opened in any modern web browser.

## 10. Error Handling

The application handles errors such as:

- Input file not found
- Output file creation failure
- File writing errors
- Invalid or incomplete Markdown structures

Exceptions are caught and displayed as error messages.


## 11. Conclusion

The Markdown to HTML Converter demonstrates how C++ can be used to build
a file-processing application using Object-Oriented Programming.

The project combines string manipulation, parsing, file handling, exception
handling, and memory management to transform Markdown documents into
browser-readable HTML pages.
