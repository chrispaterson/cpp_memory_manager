# Memory Management Simulation

Write a C++ program that simulates a simplified Memory Management system.  When the program starts there will be 1 Meg (2<sup>20</sup>) of available memory. Memory will be allocated in 512 byte memory blocks.  Therefore, if a job requires 4 bytes of memory, it will allocate 1 block of memory (512 bytes), and if a job requires 513 bytes of memory it will allocate 2 blocks of memory (1024 bytes).  Each job&#39;s memory must be a **CONTIGUOUS** block.  If there isn&#39;t a block large enough available for a job, then the job doesn&#39;t allocate memory and the job is terminated. You will need to implement dynamic memory allocation algorithm using the &quot; **best-fit**&quot; for locating the block of memory to use for loading the new job. Best fit is using the smallest available memory block that will satisfy the request. I strongly suggest (not require) that you use a binary search to find the best available block.

The input file (input.txt) will consist of 3 fields.

***Field 1:  Action (char) - action to perform.***

 * `A` = allocate memory block
 * `P` = write all the AllocatedMemBlock&#39;s to the output file in order from smallest to largest in size.
 * `R` = release memory block
 * `S` = stops the simulation
 * `Z` = simulates a reboot of the computer.  All allocated memory is released. 1 Meg of contiguous memory is available.

***Field 2:  Process Id (unsigned int) – indicates the process id of the job***

This field will be 0 if the action is P, S or Z

***Field 3: Memory required (unsigned int) – amount of memory required by the job.***

This field will be 0 if the action is P, R, S or Z

When your program first starts you need to dynamically allocate a memory allocation structure that indicates the available memory.

```C++
struct AllocatedMemBlock
{

   unsigned int ProcessId;
   unsigned int MemBlocks;
   unsigned int MemorySize;

};
```

*Note*: A Process Id of 0 indicates this block of memory is not being used by a job.

So after startup you should have a single AllocatedMemBlock with the following values:
```
Process Id = 0
MemBlocks = 2048   (1 Meg / 512 bytes/block)
MemorySize = 1048576 (1 Meg)
```
When you release a block of memory, DO NOT join your released block with other available blocks of memory.

Example input file.
```
P 0 0
A 1 2000
P 0 0
A 2 4016
P 0 0
R 1 0
P 0 0
A 3 500
P 0 0
Z 0 0
P 0 0
S 0 0
```
Example output file (output.txt).
```
AllocatedMemBlock 1            
   Process Id = 0              
   MemBlocks = 2048            
   MemorySize = 1048576

AllocatedMemBlock 1
   Process Id = 1              
   MemBlocks = 4
   MemorySize = 2000           
AllocatedMemBlock 2            
   Process Id = 0
   MemBlocks = 2044
   MemorySize = 1046528

AllocatedMemBlock 1
   Process Id = 1
   MemBlocks = 4
   MemorySize = 2000
AllocatedMemBlock 2
   Process Id = 2
   MemBlocks = 8
   MemorySize = 4016
AllocatedMemBlock 3
   Process Id = 0
   MemBlocks = 2036
   MemorySize = 1042432

AllocatedMemBlock 1
   Process Id = 0
   MemBlocks = 4
   MemorySize = 2048
AllocatedMemBlock 2
   Process Id = 2
   MemBlocks = 8
   MemorySize = 4016
AllocatedMemBlock 3
   Process Id = 0
   MemBlocks = 2036
   MemorySize = 1042432

AllocatedMemBlock 1
   Process Id = 3
   MemBlocks = 1
   MemorySize = 500
AllocatedMemBlock 2
   Process Id = 0
   MemBlocks = 3
   MemorySize = 1536
AllocatedMemBlock 3
   Process Id = 2
   MemBlocks = 8
   MemorySize = 4016
AllocatedMemBlock 4
   Process Id = 0
   MemBlocks = 2036
   MemorySize = 1042432

AllocatedMemBlock 1
   Process Id = 0
   MemBlocks = 2048
   MemorySize = 1048576
```

Additional Requirements:

1. Your program must not leak memory.  Your program should release all previous allocated memory blocks on a reboot action (Z).
2. Your program must use functions.
3. You are allowed to use C style arrays, std::arrays and vectors.  You are NOT allowed to use linked list, queue or any other type of container.
4. You are allowed (encouraged) to use unique\_ptr&#39;s.
5. You must include a header file with your function prototypes and include your header in your .cpp file.  Header file must contain the minimum includes required for your header to compile on its own.
