# Cmorse
Terminal program for decoding and incoding morse in c.

## Usage
morse MODE STRING

where MODE is either:
                      1 - for decoding
                      2 - for encoding

and STRING is:        a string containing the caracters/morse codes to be en/decoded, seperated by spaces

## Example usage

morse.exe 1 ".... . .-.. .-.. ---" 
-> H E L L O

morse.exe 2 "W O R L D"
-> .-- --- .-. .-.. -..
