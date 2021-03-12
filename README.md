# Olet onnistuneesti hakenut päivitykset upstream repositorysta

# You have successfully pulled updates from the upstream repository

# NOTE! About available test files in prg1

Short explanation of test files:
- Files named "...-in.txt" are input files containing commands for different tests.
- Files named "...-out.txt" are output files containing expected output of the corresponding input.

You can either just run the input file with command 'read "...-in.txt"' or compare your own output with the expected output with command 'testread "...-in.txt" "...-out.txt"' (the graphical UI's file selector allows you to select both files at once by control-clicking with mouse).

Files named "perftest-....txt" are performance tests of various types of operations. They can be run with command
'read "perftest-....txt"'.

Correctness tests:
- simpletest-compulsory/all-in/out.txt: Really simple test to find most obvious bugs
- example-compulsory/all-in/out.txt: The example in the assignment description, showing basic operation of all commands
- kintulammi-test-compulsory/all-in/out.txt: More tests using real Kintulammi nature reserve data from OpenStreetmap

Additionally, the following data files exist as data for your own testing:
- kintulammi-places/areas.txt: Real Kintulammi nature reserve data from OpenStreetmap (~51 places, ~40 areas)
- hervanta-south-places/areas.txt: Real Openstreetmap data south of Hervanta (~75 places, ~63 areas)
- helv-places/areas.txt: Real OpenStreetmap data of Helvetinjärvi National Park (~240 places, ~320 areas)
