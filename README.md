# A simple and fast C++ implementation of maximum cost matching
(also known as Hungarian algorithm of Kuhn algorithm)

It is designed to be used first from other lanuguages, particularly ones where
the implementation of this approach is much slower, notably, from python.

## Using from python

```python
import numpy as np
import system

cost_matrix = np.random.rand(100, 100)
np.savetxt(open('input.txt', 'w'), cost_matrix)
os.system('bin/hungarian_solver %d %d < input.txt > output.txt' % cost_matrix.shape)
assignment = np.int32(np.loadtxt(open('output.txt', 'r')))
solution_cost = np.sum(cost_matrix[assignment[:, 0], assignment[:, 1]])
```

## Using from the command line
```
bin/hungarian_solver matrix_row_num matrix_col_num [< input_file [> output_file]]
```

## Compiling

make

## Speed
You can also call 'bin/test' to see the speed of the approach. For example, for
300x300 matrices, it computes the result in ~1s., while the python 'munkres'
requires 176s.

## Configuration
If you need matrices bigger than 6kx6k, or values bigger than 1e6 in magnitude,
modify the appropriate constants inthe src/hungarian_assignment.h.

####NOTE
Traditionally, minimum cost matching is performed. This approach
computes maximum cost mapping. Multiply your matrix by -1 if you need the
former.
