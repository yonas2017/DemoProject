
RUNNING

	$ ./build/debug/bin/client 127.0.0.1 1234 // to run the client.
	$ ./build/debug/bin/server src/server/jobfiles/alice_short.job 1234 // to run the server.

# Configure the Project Using a Preset
	## For the Debug build:
		$ cmake --preset=user-debug
	## For the Release build:
		$ cmake --preset=user-release

# Build the Project:
Once the project is configured, you can build it using:
	## For Debug build:
		$ cmake --build --preset=user-debug
	## For Release build:
		$ cmake --build --preset=user-release

# Clean the project
	## For Debug build:
		$ cmake --build --preset=user-debug --target=clean
	## For Release build:
		$ cmake --build --preset=user-release --target=clean

# Install the Project
	## For Debug build:
		$ cmake --install build/debug 
	## For Release build:
		$ cmake --install build/release

# UnitTest the Project
	$ ctest --test-dir build/debug/test --verbose

# Package the project
	$ cpack --preset=release

	# check content of package without extructing
	$ cd build/release/package && tar -tzvf DemoProject-1.0.0.tar.gz

# formatting
	$ cd build/debug && ninja format

# doc generation
	$ cd build/debug && ninja doc
	$ xdg-open docs/html/index.html  # On Linux

# Profiling: Using Valgrind's Callgrind for Profiling
$ valgrind --tool=callgrind ./your_program

	This will generate a file called callgrind.out.<pid>
	## Visualize the Profiling Data
	$ kcachegrind callgrind.out.<pid>

	## Alternatively, you can use the callgrind_annotate command-line tool to get a text-based report:
	$ callgrind_annotate callgrind.out.<pid> > profiling_report.txt

# Valgrind Tools:
## Memcheck: Detects memory errors (leaks, uninitialized memory, invalid access).
	$ valgrind --leak-check=full ./your_program

## Callgrind: Analyzes performance and identifies bottlenecks.
	$ valgrind --tool=callgrind ./your_program

## Massif: Heap profiler for memory usage analysis.
	$ valgrind --tool=massif ./your_program

## Helgrind: Detects thread-related errors (race conditions).
	$ valgrind --tool=helgrind ./your_program

## DRD: Another race detection tool for multi-threading.
	$ valgrind --tool=drd ./your_program

## Cachegrind: Analyzes cache usage to optimize memory access patterns.
	$ valgrind --tool=cachegrind ./your_program

## Lackey and Indigo: Low-level memory and thread analysis tools.
	$ valgrind --tool=lackey ./your_program
	$ valgrind --tool=indigo ./your_program
