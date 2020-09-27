# Exploit vulnerability FreeBSD-SA-09:06.ktimer: kernel buffer overflow

This mission depends on the pure-capability CheriBSD FETT kernel included in FETT CHERI-RISC-V Mission 2 (kernel spatial memory safety).

The objective of this mission is to demonstrate arbitrary code execution in a pure-capability kernel.
This must be achieved via a reintroduced past FreeBSD security vulnerability, [FreeBSD-SA-09:06.ktimer](https://www.freebsd.org/security/advisories/FreeBSD-SA-09:06.ktimer.asc).
We have reintroduced this via change [69bb6a5e55fc94dd7338e22492971edbf55f8393](https://github.com/CTSRD-CHERI/cheribsd/commit/69bb6a5e55fc94dd7338e22492971edbf55f8393) in the pure-capability kernel branch of the CheriBSD repository.
In this vulnerability, an integer system-call argument is not properly bounds checked, allowing an out-of-bounds access that on a vanilla non-CHERI system is exploitable to gain kernel privilege.

Successful completion of this mission requires demonstrating that the kernel function `flag_captured(9)` has executed with the integer argument `0xfe77c0de` using one of the `ktimer(2)` system calls triggered as an unprivileged (non-root) user.
Use of privileged kernel manipulation mechanisms, such as reconfiguration of the boot-time environment, use of the kernel debugger, kernel module loading, and access to `/dev/mem`, is considered out-of-scope in this mission.
If `flag_captured(9)` is called, the sysctl `security.flags_captured` counter will be incremented.
This corresponds to a partially successful exploit.
If the function is called with the designated argument, the sysctl `security.flags_captured_key` counter will be incremented.
This corresponds to a fully successful exploit.
