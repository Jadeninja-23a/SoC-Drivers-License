# Assignment Week 1 


---

## 1. /proc

- **Processor:** Virtual core capable of running one process at a time.  
- **Core:** Physical CPU core, may contain one or more logical processors.

- **Number of Cores:** 6 (from a virtual machine, physical machine has 24 cores)  
- **Number of Processors:** 6 (processor IDs range from 0 to 5, physical machine has 32 processors)  
- **CPU Frequency:** 2419.2 MHz (from `/proc/cpuinfo`)  
- **Architecture:** x86_64 (64-bit), from `lscpu` output  
- **Total Memory:** 3.9 GB (from `free -h`)  
- **Available Memory:** 3.2 GB (difference used by cache for performance)  
- **Context Switches since Boot:** 182,545  
- **Process Clones:** 2,838  
- (Context switches and clones from `/proc/stat`)

---

## 2. Top

- **Process ID (PID):** 2063  
- **CPU Usage:** 99.7% - 100% (observed with `top`, no other significant CPU usage)  
- **Memory Usage:** 792 KB  
- **Process State:** Running

---

## 3. Memory

| Program  | Virtual Memory (KB) | Physical Memory (KB) | Description                                   |
|----------|--------------------|---------------------|-----------------------------------------------|
| memory_1 | 8,296              | 792                 | Declares a large array but does not initialize it; minimal physical memory usage. |
| memory_2 | 8,304              | 3,240               | Declares and initializes half the array; increased physical memory usage. |

### Explanation

- Both programs allocate the same virtual memory space for the large array.  
- Physical memory differs due to **lazy allocation** by the OS — physical RAM is assigned only when memory pages are accessed (written).  
- In `memory_1`, the array is untouched, so physical memory usage is low, reflecting just the program’s basic runtime needs.  
- In `memory_2`, half the array is written to, causing the OS to commit physical pages, increasing RAM usage.

---

## 6. Process? Which Process?
![image](https://github.com/user-attachments/assets/8d9f9783-4b1a-44a5-8d80-2023ff079a6b)

