# Submit an idea
You can submit an idea by creating a pull request or by sending me an mail
or adding directly the idea in the improvement ideas project white board in the List column
(if you have the right to do so).

# Development guidelines
- Try to respect the coding style
- Simply issue the command make to compile a debug build
- Don't install/uninstall the library of your system to make sure to use the modified version
- Always prefix functions and ngi data types with ngi *See Prefix exceptions*
- Modify the test suite by adding new tests and fixing existing tests if necessary
- Run the test suite before validation and see the result in the test configuration file
- Copy the license header if a new file is created
- Add documentation for the file and the functions using the same formating style and add tags if necessary
- Avoid to do breaking changes or state them in the top of the pull request if no workaround is possible
- Do not expose untested/untrusted symbols in the API *See API*
- If the symbol is used in another file, make it internal *See Internal*
- If the symbol is only used in the file, make it private *See Private*

## API
A symbol is in the API if he is in libngi.h or in a header included by libngi.h
The symbol is accessible everywhere

## Internal
A symbol is internal if he is in libngi_internal.h or in a header included by libngi_internal.h
The symbol is accessible only within the library

## Private
A symbol is private if he is only in a file and/or marked as static
The symbol is accessible only within the file

## Prefix exceptions
- If you have multiple variables using the same data type, the ngi prefix can be omitted
- If the symbol is private *See Private*, the ngi prefix can be omitted
