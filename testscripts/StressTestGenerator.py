import random

# Define some basic parameters
num_lines = int(input("Enter the number of lines to stress test: "))  # Target number of lines for the stress test file
registers = ['r' + str(i) for i in range(10)]  # List of registers r0-r9
operations = ['add', 'sub', 'multiply', 'divide', 'move', 'load', 'store']  # etc.
datatypes = ['integer', 'float', 'boolean', 'character', 'memory', 'instruction']

with open('../examples/StressTest.sasm', 'w') as file:
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

print("Generated sample stress test at " + str(num_lines) + " lines of code.")
print("Make sure to add code/StressTest.sasm as the target when running the compiler! Also recommended to run using the --timings flag")
