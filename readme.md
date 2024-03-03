# Text Search and Replace ReadMe

## Overview

The `text_search_replace` program is a C-based utility designed to perform text search and replace operations within text files. It accepts command-line arguments to specify search criteria, replacement text, file input, and file output. This program is particularly useful for automating text manipulation tasks, such as batch find-and-replace operations in large text documents.

## Features

- **Search and Replace:** Search for occurrences of a specified text pattern within the input text files and replace them with the desired replacement text.
- **Wildcard Support:** Supports wildcard characters (*) for flexible search patterns.
- **Line Range Specification:** Allows users to specify a range of lines within which the search and replace operations will be performed.
- **Input and Output Files:** Accepts input from one text file and writes the modified text to another output file.
- **Error Handling:** Provides informative error messages for various error conditions, such as missing arguments, invalid input files, and unwritable output files.

## Usage

To use the `text_search_replace` program, follow these steps:

1. Compile the program using a C compiler compatible with the C99 standard.
2. Execute the compiled binary from the command line, providing the necessary arguments in the specified format.
3. The program accepts the following command-line options:

   - `-s <search_text>`: Specifies the text pattern to search for.
   - `-r <replacement_text>`: Specifies the replacement text to use.
   - `-w`: Enables wildcard mode, allowing wildcard character (*) usage in the search text.
   - `-l <start_line>,<end_line>`: Specifies the range of lines within which to perform the search and replace operations.
   - `<input_file>`: Specifies the input text file.
   - `<output_file>`: Specifies the output text file to write the modified text.

4. Upon execution, the program will perform the search and replace operations as specified and write the modified text to the output file.
5. Error messages will be displayed if any issues occur during execution, such as missing arguments or file-related errors.

## Dependencies

The program is written in C and relies on standard C libraries for file I/O operations and string manipulation.
