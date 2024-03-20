This repository contains solutions to two problems assigned in the Theory of Computation course. The tasks involve the conversion of regular expressions to Nondeterministic Finite Automata (NFA) and simulating the behavior of an NFA on a given input string.

`build.cpp`: This file contains the implementation for converting a regular expression to an NFA. It reads a regular expression from standard input and prints a representation of the converted NFA to standard output.

`run.cpp`: This file contains the implementation for simulating the behavior of an NFA on a given input string. It reads an NFA and an input string from standard input and outputs whether the input string is accepted by the NFA.

## Implementation details:

### Regular Expression to NFA Conversion
- Tokenization and Conversion: The regular expression is tokenized and converted to postfix notation using the Shunting Yard algorithm.
- NFA Construction: NFAs are constructed based on the postfix notation using Thompson's Construction algorithm.
- Null Transitions and Epsilon Closure: Null transitions are handled, and epsilon closure is computed to optimize the NFA.
- Reduction: Redundant states and transitions are removed from the NFA to optimize its size.
- Printing NFA: The resulting NFA is printed for visualization.
### Usage
- Compilation: Compile `build.cpp`.
- Execution: Run the compiled executable, providing a regular expression as input. The program outputs the corresponding NFA representation.


### NFA Simulation
- Input Parsing: The program reads the input string and NFA description from standard input.
- NFA Initialization: An NFA instance is initialized based on the provided description.
- Simulation: The NFA is simulated on the input string using breadth-first search (BFS).
- Result Generation: The simulation generates a result string indicating acceptance or rejection at each position in the input string.
### Usage
- Compilation: Compile `run.cpp`.
- Execution: Run the compiled executable, providing the input string and NFA description as input. The program outputs the result string indicating acceptance or rejection.

