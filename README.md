# Ronsor Privacy Utilities

Building: gcc -s -lm rsa.c rpu.c -o rpu

Tested: Windows, Linux.

Usage:

    Error: Usage: rsu [gen|encrypt|decrypt] [-f infile] [-o outfile] [-k pubkey] [-p privkey]
    Exiting: End usage.

Warning: This is not cryptographically secure. RSA keys are only 64-bits!
