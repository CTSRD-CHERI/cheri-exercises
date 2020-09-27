# Exploit vulnerability FreeBSD-SA-18:13.nfs: out-of-bounds access

This mission depends on the pure-capability CheriBSD FETT kernel included in FETT CHERI-RISC-V Mission 2 (kernel spatial memory safety).

The objective of this mission is to demonstrate arbitrary code execution in a pure-capability kernel.
This must be achieved via a reintroduced past FreeBSD security vulnerability, [FreeBSD-SA-18:13.nfs](FreeBSD-SA-18:13.nfs).
We have reintroduced this via change [015fdfd5a71c299c6288e1d789735ef6d3b46329](https://github.com/CTSRD-CHERI/cheribsd/commit/015fdfd5a71c299c6288e1d789735ef6d3b46329) in the pure-capability kernel branch of the CheriBSD repository.
In this vulnerability, an out-of-bounds access is performed during received NFS packet processing, which is exploitable on a vanilla non-CHERI system to gain kernel privilege.
More information on the NFSv4 packet format may be found in [RFC7530](https://tools.ietf.org/html/rfc7530).

Successful completion of this mission requires demonstrating that the kernel function `flag_captured(9)` has executed with the integer argument `0xfe77c0de` using the use of an NFS packet exploiting this vulnerability.
Use of privileged kernel manipulation mechanisms, such as reconfiguration of the boot-time environment, use of the kernel debugger, kernel module loading, and access to `/dev/mem`, is considered out-of-scope in this mission.
If `flag_captured(9)` is called, the sysctl `security.flags_captured` counter will be incremented.
This corresponds to a partially successful exploit.
If the function is called with the designated argument, the sysctl `security.flags_captured_key` counter will be incremented.
This corresponds to a fully successful exploit.
