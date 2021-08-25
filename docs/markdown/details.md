# Configuration file
A basic configuration file looks like this:
```
sample section ->
property: value
property 2: 2

sample_section 2 ->
property: value
property_2: "value 2"
```
The configuration file is structured with sections (this is like a paragraph)
and with properties (using a key: value format) contained in a section.
Each section name needs to be unique and each property name needs to be unique **only** within a section.

# Using the library
This is a sample C program using the library:
```C
/* Include only this header */
#include <libngi/libngi.h>

int main(int argc, char** argv)
{
    /* Open a file and parses it */
    ngi_header_t* ngi_header = ngi_open("config.ngi", "r+");

    /* Do others things here */

    /* Close the file when finished */
    ngi_close(ngi_header);

    return 0;
}
```
The header contains all the informations about the file.
To see what the others functions of the library are, generate the documentation.
