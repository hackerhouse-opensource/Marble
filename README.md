# Marble Framework

In 2017, Wikileaks published incomplete source code for the Marble Framework. Using AI/ML and a human with
information learned from the Vault7 leaks, we re-constructed the missing code libraries and have patched the
StringScrambling solution to compile for build-time String Obfuscation using 106 "Marbles" with Visual Studio. 
This Framework was created for use by the CIA, however a number of malware samples have been identified in
the wild using these algorithms. By releasing our modifications, we aim to improve detections of the framework
and identification of additional samples using these techniques in the wild. 

> "The Marble Framework is designed to allow for flexible and easy-to-use obfuscation when developing tools. 
> When signaturing tools, string obfuscation algorithms (especially those that are unique) are often used to 
> link malware to a specific developer or development shop. This framework is intended to help us (AED) to 
> improve upon our current process for string/data obfuscation in our tools."

This framework can be used to dynamically obfuscate and handle common string operations as an anti-forensics'
capability.

## Documentation

To use, compile the solution which will produce `Mibster.exe`,`Mender.exe` and `Validator.exe`. These tools
can be added into the Pre-Build and Post-Build steps of a Visual Studio file to obfuscate the following
data types by including `Marble.h` from the `Shared` folder in your project file:

 * CARBLE - char[] / char*
 * WARBLE - wchar[] / wchar*
 * BARBLE - byte[] / byte* 
 
Running the `Mibster.exe` will use options specified in `Marble.h` (such as which XOR / RXOR algorithm to use,
wether strings should be cleared after use, chooses at random by default etc.) and obfuscate your strings. You
can then run `Validator.exe` to ensure no strings are present and `Mender.exe` to restore the original source
code (recommended post-build). The deobfuscator (Marble) chosen should be added from the Shared folder into your
project. File states and changes are stored in ".marble" files and it supports a total of 106 algorithms (known as
"Marbles") to be chosen from. More information and documentation can be found on the Wikileaks Vault7 wiki.

[Marble Framework (wikileaks)](https://wikileaks.org/ciav7p1/cms/page_14588467.html)

## Malware Detected

The following malware samples have been identified in the wild as using code components from the Marble Framework:

* Worm:Win32/Takc!pz
* TROJAN-DROPPER.WIN32.DAPATO

# License

Hacker House code additions are available under a Attribution-NonCommercial-NoDerivatives 4.0 International license.
