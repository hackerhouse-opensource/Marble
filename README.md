# Marble Framework

In 2017, Wikileaks published incomplete source code for the Marble Framework. Using AI/ML and a human with
information learned from the Vault7 leaks, we re-constructed the missing code libraries and have patched the
StringScrambling solution to compile for build-time String Obfuscation using 106 "Marbles" with Visual Studio. 
This Framework was created for use by the Central Intelligence Agency (CIA) Applied Engineering Department (AED),
however a number of malware samples have been identified in the wild using these algorithms which may indicate
use by others. By releasing our modifications, we aim to improve detections of the framework and identification 
of additional samples using these techniques in the wild. The tool description from CIA internal wiki follows: 

> "The Marble Framework is designed to allow for flexible and easy-to-use obfuscation when developing tools. 
> When signaturing tools, string obfuscation algorithms (especially those that are unique) are often used to 
> link malware to a specific developer or development shop. This framework is intended to help us (AED) to 
> improve upon our current process for string/data obfuscation in our tools." The framework utilizes pre and
> post-build execution steps to apply obfuscation to the tool. If the tool breaks the build, the post build will
> always be able to repair it. The pre-build execuion step will store clean copies of the code before making
> modifications. The post build execution step restores the files to a clean-copy state. The framework allows for
> obfuscation to be chosen randomly from a pool of techniques. These techniques can be filtered based upon the
> project needs. If desired, a user may also, select a specific technique to use for obfuscation. A receipt file
> is generated on run (and replaces any previous receipts). The receipt file identifes the algorithm used as well
> as all of the strings/data that was obfuscated. The post-build step will also double check to make sure none of
> the obfuscated data appears in the binary.

The framework's integration into the EDG Project Wizard will set up the appropriate project and solution properties 
needed to run. Currently, the obfuscation framework will only be set for release builds. If it is so desired to debug 
the obfuscated strings you may manually set the pre and post build events.

This framework can be used to dynamically obfuscate and handle common string operations as an anti-forensics'
capability.

## Documentation

To use, compile the solution which will produce `Mibster.exe`, `Mender.exe` and `Validator.exe`. These tools
can be added into the Pre-Build and Post-Build steps of a Visual Studio file to obfuscate the following
data types by including `Marble.h` and your chosen deobfuscator from the `Shared` folder in your project file:

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

Documentation can also be found in the `docs` folder which has been created from the link above. 

## Malware Detected

The following malware samples have been identified in the wild as using code components from the Marble Framework:

* Worm:Win32/Takc!pz (Microsoft)
* TROJAN-DROPPER.WIN32.DAPATO (Kaspersky)

# License

Hacker House code additions are available under a Attribution-NonCommercial-NoDerivatives 4.0 International license.
