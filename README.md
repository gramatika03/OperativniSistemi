This project aims to develop a small yet fully functional kernel for a multithreaded operating system with time-sharing capabilities. Key features include:

Memory Allocator: Efficient management of memory resources.
Thread Management: Support for threads and semaphores.
Time-Sharing: Implementation of asynchronous context switching and preemption via timer and keyboard interrupts.
The kernel is designed as a "library," enabling the user program (application) and the kernel to share the same address space, functioning as a statically linked program loaded into memory. Concurrent processes within the application will operate as lightweight threads, typical of embedded systems that run pre-installed programs alongside the operating system.

The kernel will be implemented for the RISC-V processor architecture using assembly language and C/C++. It will execute in a virtual environment, specifically a RISC-V processor emulator.
