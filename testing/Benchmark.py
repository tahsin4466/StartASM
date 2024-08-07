import random
import os
import subprocess
import argparse

# Set up argument parsing
parser = argparse.ArgumentParser(description='Generate and run a StartASM stress test.')
parser.add_argument('num_lines', type=int, help='Number of lines to include in the stress test file')
args = parser.parse_args()

# Define the StartASM executable path
executable_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'startasm')

# Define the full path for the stress test file
stress_test_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'StressTest.sasm')

# Command-line arguments
cli_args = ["compile", stress_test_path, "--timings"]

# Define some basic parameters
num_lines = args.num_lines  # Target number of lines for the stress test file
registers = ['r' + str(i) for i in range(10)]  # List of registers r0-r9
operations = ['add', 'sub', 'multiply', 'divide', 'move', 'load', 'store']  # etc.
datatypes = ['integer', 'float', 'boolean', 'character', 'memory', 'instruction']

print("Preparing test unit")
# Create the stress test file
with open(stress_test_path, 'w') as file:
    file.write("comment \"This is the automatically generated stress test script to test the front-end compilation performance of the compiler\"\n")
    for _ in range(num_lines-2):
        reg1 = random.choice(registers)
        reg2 = random.choice(registers)
        operation = random.choice(operations)
        datatype = random.choice(datatypes)
        value = random.randint(0, 1000)  # Adjust range as needed

        # Construct a line of StartASM code
        if operation in ['add', 'sub', 'multiply', 'divide']:
            line = f"{operation} {reg1} with {reg2} to {random.choice(registers)}"
        elif operation == 'move':
            line = f"move {reg1} to {reg2}"
        elif operation == 'load':
            line = f"load m<{value}> to {reg1}"
        elif operation == 'store':
            line = f"store {reg1} to m<{value}>"
        # Add more operation types as needed

        # Write the line to the file
        file.write(line + '\n')
    file.write("stop")

print("Starting benchmark")
# Change the working directory to the root directory
root_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(root_dir)

# Use subprocess to call and run the stress test
result = subprocess.run([executable_path] + cli_args, capture_output=True, text=True)

# Remove the stress test file
try:
    os.remove(stress_test_path)
except FileNotFoundError:
    print("File not found error deleting stress test file")
except PermissionError:
    print("Permission error deleting stress test file")
except Exception as e:
    print(f"Other error deleting stress test file: {e}")

# Print the output of the compiler
print(result.stdout)
