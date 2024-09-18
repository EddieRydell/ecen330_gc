import os
import re


def get_c_files():
    """Retrieve all .c and .h files in the current directory and subdirectories."""
    c_files = []
    for root, dirs, files in os.walk('.'):
        for file in files:
            if file.endswith('.c') or file.endswith('.h'):
                c_files.append(os.path.join(root, file))
    return c_files


def remove_comments(content):
    """Remove all comments from the content."""
    pattern = r'//.*?$|/\*.*?\*/'
    content_no_comments = re.sub(pattern, '', content, flags=re.MULTILINE | re.DOTALL)
    return content_no_comments


def count_commented_out_code(content, filename, errors):
    """
    Count the number of lines of commented-out code in the file.

    Comments are allowed, but lines where code has been commented out are limited to 5 per file.
    """
    # Pattern to detect commented-out code
    code_line_pattern = re.compile(r'^\s*//\s*(?:[a-zA-Z_].*;?|{|\})\s*$', re.MULTILINE)
    code_block_pattern = re.compile(r'/\*.*?\*/', re.DOTALL)

    code_line_comments = code_line_pattern.findall(content)
    code_block_comments = code_block_pattern.findall(content)

    # Count lines in single-line commented-out code
    total_code_comment_lines = len(code_line_comments)

    # Count lines in block commented-out code
    for block in code_block_comments:
        # Check if the block contains code-like patterns
        if re.search(r'[;{}=]', block):
            lines = block.count('\n') + 1
            total_code_comment_lines += lines

    if total_code_comment_lines > 5:
        error_msg = f"{filename}: Error: More than 5 lines of commented-out code ({total_code_comment_lines} lines)."
        errors.append(error_msg)


def check_no_cpp_code(content, filename, errors):
    """Check for C++ specific keywords to ensure the code is pure C."""
    cpp_keywords = [
        'class', 'namespace', 'template', 'operator', 'new', 'delete',
        'private', 'public', 'protected', 'friend', 'virtual', 'throw',
        'catch', 'try', 'using', 'this', 'nullptr', 'constexpr', 'decltype'
    ]
    # Remove comments before checking for C++ keywords
    content_no_comments = remove_comments(content)
    lines = content_no_comments.split('\n')
    for i, line in enumerate(lines, start=1):
        # Remove string literals to avoid false positives inside strings
        line_no_strings = re.sub(r'".*?"', '', line)
        for keyword in cpp_keywords:
            if re.search(r'\b' + keyword + r'\b', line_no_strings):
                error_msg = f"{filename}:{i}: Error: C++ keyword '{keyword}' found."
                errors.append(error_msg)


def check_h_file_no_executable_code(filename, content, errors):
    """Ensure header files do not contain executable code or variable definitions."""
    # Remove comments and strings
    content_no_comments = remove_comments(content)
    content_no_strings = re.sub(r'".*?"', '', content_no_comments, flags=re.DOTALL)
    lines = content_no_strings.split('\n')
    for i, line in enumerate(lines, start=1):
        # Skip empty lines
        if line.strip() == '':
            continue
        # Look for function definitions
        if re.match(r'.*\)\s*\{', line):
            error_msg = f"{filename}:{i}: Error: Executable code (function definition) found in header file."
            errors.append(error_msg)
        # Look for variable definitions outside of functions
        if re.match(r'^\s*(?:static\s+)?[a-zA-Z_][a-zA-Z0-9_]*\s+[a-zA-Z_][a-zA-Z0-9_]*(\s*=\s*[^;]+)?;', line):
            error_msg = f"{filename}:{i}: Error: Variable definition found in header file."
            errors.append(error_msg)


def check_h_file_naming(filename, content, errors):
    """Check that function names and constants in header files are properly prefixed."""
    base_name = os.path.basename(filename)
    base_name_no_ext = os.path.splitext(base_name)[0]
    base_name_upper = base_name_no_ext.upper()
    # Remove comments and strings
    content_no_comments = remove_comments(content)
    content_no_strings = re.sub(r'".*?"', '', content_no_comments, flags=re.DOTALL)
    lines = content_no_strings.split('\n')
    for i, line in enumerate(lines, start=1):
        # Skip empty lines
        if line.strip() == '':
            continue
        # Find function declarations
        func_decl_match = re.match(
            r'^\s*[a-zA-Z_][a-zA-Z0-9_\s\*]*\s+(\*?\s*[a-zA-Z_][a-zA-Z0-9_]*)\s*\([^;]*\);',
            line)
        if func_decl_match:
            func_name = func_decl_match.group(1).strip()
            if not func_name.startswith(base_name_no_ext + '_'):
                error_msg = f"{filename}:{i}: Error: Function '{func_name}' should be prefixed with '{base_name_no_ext}_'"
                errors.append(error_msg)
        # Find constants (#define)
        define_match = re.match(
            r'^\s*#define\s+([a-zA-Z_][a-zA-Z0-9_]*)\s+',
            line)
        if define_match:
            const_name = define_match.group(1)
            if not const_name.startswith(base_name_upper + '_'):
                error_msg = f"{filename}:{i}: Error: Constant '{const_name}' should be prefixed with '{base_name_upper}_'"
                errors.append(error_msg)
            if not const_name.isupper():
                error_msg = f"{filename}:{i}: Error: #define '{const_name}' is not all uppercase."
                errors.append(error_msg)


def check_function_comments(content, filename, errors):
    """Ensure each function definition is preceded by a comment."""
    # Remove strings
    content_no_strings = re.sub(r'".*?"', '', content, flags=re.DOTALL)
    # Remove comments to prevent misidentifying function comments
    content_no_comments = remove_comments(content_no_strings)
    lines = content_no_comments.split('\n')
    original_lines = content.split('\n')  # For accurate line numbers with comments
    for i in range(len(lines)):
        line = lines[i]
        if re.match(r'^\s*[a-zA-Z_][a-zA-Z0-9_\s\*]*\s+.*\)\s*\{', line):
            # Function definition found
            has_comment = False
            j = i - 1
            while j >= 0 and lines[j].strip() == '':
                j -= 1
            if j >= 0 and (original_lines[j].strip().startswith('//') or original_lines[j].strip().startswith('/*')):
                has_comment = True
            if not has_comment:
                # Try to get function name
                func_line = line.strip()
                func_name_match = re.match(r'.*?([a-zA-Z_][a-zA-Z0-9_]*)\s*\(.*', func_line)
                func_name = func_name_match.group(1) if func_name_match else 'unknown'
                # Calculate actual line number
                line_number = i + 1
                error_msg = f"{filename}:{line_number}: Error: Function '{func_name}' is not preceded by a comment."
                errors.append(error_msg)


def check_const_usage(content, filename, errors):
    """Check that 'const' is used only for defining arrays of constants."""
    # Remove comments and strings
    content_no_comments = remove_comments(content)
    content_no_strings = re.sub(r'".*?"', '', content_no_comments, flags=re.DOTALL)
    lines = content_no_strings.split('\n')
    for i, line in enumerate(lines, start=1):
        # Skip empty lines
        if line.strip() == '':
            continue
        # Find const declarations
        const_decl_match = re.match(r'^\s*const\s+[^;]+;', line)
        if const_decl_match:
            decl = const_decl_match.group(0)
            if '[' not in decl:
                error_msg = f"{filename}:{i}: Error: 'const' used for non-array variable: '{decl.strip()}'"
                errors.append(error_msg)


def check_c_files(c_files):
    """Main function to check all C files against the coding standards."""
    all_errors = []
    for file in c_files:
        errors = []
        with open(file, 'r') as f:
            content = f.read()

        # Check for C++ code
        check_no_cpp_code(content, file, errors)

        # Check commented-out code lines
        count_commented_out_code(content, file, errors)

        # Specific checks for .h files
        if file.endswith('.h'):
            check_h_file_no_executable_code(file, content, errors)
            check_h_file_naming(file, content, errors)

        # Specific checks for .c files
        if file.endswith('.c'):
            base_name = os.path.basename(file)
            base_name_no_ext = os.path.splitext(base_name)[0]
            if base_name != 'main.c':
                header_file = os.path.join(os.path.dirname(file), base_name_no_ext + '.h')
                if not os.path.exists(header_file):
                    error_msg = f"{file}: Error: No corresponding header file '{header_file}' found."
                    errors.append(error_msg)
            # Check function comments
            check_function_comments(content, file, errors)
            # Check 'const' usage
            check_const_usage(content, file, errors)

        # Print errors for the current file
        if errors:
            print(f"\nErrors in file: {file}")
            for error in errors:
                print(error)
            all_errors.extend(errors)

    # Print summary
    if all_errors:
        print("\nSummary of Errors:")
        for error in all_errors:
            print(error)
    else:
        print("No errors found. All files comply with the coding standards.")


if __name__ == '__main__':
    c_files = get_c_files()
    check_c_files(c_files)

