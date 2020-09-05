# Background reading

To perform these exercises most effectively, we recommend first building a
working knowledge of CHERI.  The most critical references will be the
*Introduction to CHERI* and *CHERI C/C++ Programming Guide*, but there is a
broad variety of other reference material available regarding CHERI:

- [An Introduction to CHERI](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-941.pdf) - An overview of the CHERI architecture, security model, and programming models.
- [CHERI C/C++ Programming Guide](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-947.pdf) - This use of CHERI capabilities to represent C/C++ pointers requires modest changes to the way C and C++ are used. This document describes those changes.
- [CheriABI: Enforcing Valid Pointer Provenance and Minimizing Pointer Privilege in the POSIX C Run-time Environment](https://www.cl.cam.ac.uk/research/security/ctsrd/pdfs/201904-asplos-cheriabi.pdf) - This paper describes the CheriABI pure-capability process environment these exercises are expected to use. An extended [technical report](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-932.pdf) is also available.
- [Cornucopia: Temporal Safety for CHERI Heaps](https://www.cl.cam.ac.uk/research/security/ctsrd/pdfs/2020oakland-cornucopia.pdf) - The temporal-safety exercises require the use of Cornucopia's quarantine and revocation infrastructure.
- [Capability Hardware Enhanced RISC Instructions:
CHERI Instruction-Set Architecture (version 7)](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-927.pdf) - Instruction reference and design discussion.
- [Complete spatial safety for C and C++ using CHERI capabilities](https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-949.pdf) This PhD dissertation provides an extensive overview of the CHERI-MIPS linking model (also relevant to the current CHERI-RISC-V model), an implementation of opportunistic subobject bounds, and general C/C++ compatibility issues.
