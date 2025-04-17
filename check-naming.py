#!/usr/bin/env python3
import re
import sys
import os
from pathlib import Path

# Regex rules per context
NAMING_RULES = {
    'namespace':       r'^[A-Z][a-zA-Z0-9]*$',
    'class':           r'^[A-Z][a-zA-Z0-9]*$',
    'struct':          r'^[A-Z][a-zA-Z0-9]*$',
    'enum':            r'^[A-Z][a-zA-Z0-9]*$',
    'enum_member':     r'^[a-z][a-zA-Z0-9]*$',
    'macro':           r'^[A-Z][A-Z0-9_]*$',
    'global_variable': r'^[a-z][a-zA-Z0-9]*$',
    'public_var':      r'^[a-z][a-zA-Z0-9]*$',
    'private_var':     r'^_[a-z][a-zA-Z0-9]*$',
    'public_func':     r'^[A-Z][a-zA-Z0-9]*$',
    'private_func':    r'^[a-z][a-zA-Z0-9]*$',
}

# Simplified access modifier guess (very rough, based on context)
ACCESS_CONTEXT = {'public': 'public', 'private': 'private', 'protected': 'private'}

# File extensions to scan
FILE_EXTENSIONS = ['.cpp', '.h', '.hpp']

# Matchers (simplified)
PATTERNS = [
    # Macros
    ('macro', re.compile(r'^\s*#define\s+([A-Z_][A-Z0-9_]*)')),

    # Namespaces
    ('namespace', re.compile(r'\bnamespace\s+([a-zA-Z_][a-zA-Z0-9_]*)')),

    # Class/Struct/Enum
    ('class', re.compile(r'\bclass\s+([a-zA-Z_][a-zA-Z0-9_]*)')),
    ('struct', re.compile(r'\bstruct\s+([a-zA-Z_][a-zA-Z0-9_]*)')),
    ('enum', re.compile(r'\benum\s+(class\s+)?([a-zA-Z_][a-zA-Z0-9_]*)')),

    # Enum members (within enum block)
    ('enum_member', re.compile(r'^\s*([a-zA-Z_][a-zA-Z0-9_]*)\s*(=|,)?')),

    # Global variables (very naive: static or non-class lines ending with ;)
    ('global_variable', re.compile(r'^\s*(?:static\s+)?(?:int|float|double|char|bool|auto|std::[a-z_]+)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*[;=]')),

    # Function declarations (naive, doesn't support templates well)
    ('public_func', re.compile(r'\b(?:void|int|float|auto|bool|double|char|std::[a-z_]+)\s+([A-Z][a-zA-Z0-9_]*)\s*\(')),
    ('private_func', re.compile(r'\b(?:void|int|float|auto|bool|double|char|std::[a-z_]+)\s+([a-z][a-zA-Z0-9_]*)\s*\(')),

    # Class member vars (based on _ prefix)
    ('private_var', re.compile(r'\b(?:int|float|double|bool|char|auto|std::[a-z_]+)\s+(_[a-zA-Z][a-zA-Z0-9_]*)\s*[;=]')),
    ('public_var', re.compile(r'\b(?:int|float|double|bool|char|auto|std::[a-z_]+)\s+([a-z][a-zA-Z0-9_]*)\s*[;=]')),
]

def scan_file(path):
    violations = []
    current_context = None
    inside_enum = False

    with open(path, 'r', encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()

    for lineno, line in enumerate(lines, 1):
        stripped = line.strip()

        if re.match(r'^\s*(public|private|protected)\s*:', stripped):
            current_context = ACCESS_CONTEXT[stripped.split(':')[0]]
            continue

        if 'enum' in stripped:
            inside_enum = True
        if inside_enum and '}' in stripped:
            inside_enum = False

        for ctx, pattern in PATTERNS:
            if ctx == 'enum_member' and not inside_enum:
                continue
            match = pattern.search(line)
            if match:
                name = match.group(1) if ctx != 'enum' else match.group(2)
                rule = NAMING_RULES[ctx]
                if not re.match(rule, name):
                    violations.append((path, lineno, ctx, name, rule))
                break

    return violations

def scan_directory(directory):
    all_violations = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if any(file.endswith(ext) for ext in FILE_EXTENSIONS):
                path = os.path.join(root, file)
                all_violations.extend(scan_file(path))
    return all_violations

def main():
    root = Path(".")
    violations = scan_directory(root)

    if not violations:
        print("Naming convention check passed.")
        return 0

    print("Naming convention violations found:")
    for file, lineno, ctx, name, rule in violations:
        print(f"{file}:{lineno} [{ctx}] '{name}' doesn't match rule: {rule}")
    return 1

if __name__ == "__main__":
    sys.exit(main())
