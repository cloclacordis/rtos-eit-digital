## FreeRTOS Course Assignment 2

> Completed as part of the *Development of Real-Time Systems* course (EIT Digital).

A minimal POSIX‑based FreeRTOS demo implementing three tasks and dynamic priority management:

- `matrix_task`: performs heavy CPU‑intensive matrix multiplication every cycle (plus dummy delay), at initial priority 3  
- `communication_task`: simulates I/O by printing *“Sending data…”* then *“Data sent!”* with 100 ms delays (total 200 ms period), at initial priority 1  
- `priority_set_task`: monitors `communication_task` execution time via `vApplicationTickHook()` and adjusts its priority — raises to 4 if > 1000 ms, lowers to 2 if < 200 ms

### Building & Running

This assignment reuses the project structure from [Module 1](https://github.com/cloclacordis/rtos-eit-digital/tree/e84920b77fee42bcd3ca450efcebd4cdbd2e5602/Module1), with some modified files.

To prepare the project copy all files and folders from `Module1` into `Module2` **except**:

- `main.c`
- `port.c`
- `README.md`
- `assets/`

> These four elements must be the original ones from the `Module2` folder.

After replacement, **the `Module2` directory structure** should look like this:

  
`assets/` <- original `Module2`, *not copied* from `Module1`  
`croutine.c`  
`event_groups.c`  
`FreeRTOS-simulator-for-Linux.url`  
`heap_3.c`  
`include/`  
`list.c`  
`main.c` <- original `Module2`  
`Makefile`  
`port.c` <- original `Module2`  
`queue.c`  
`README.md` <- original `Module2`  
`stream_buffer.c`  
`tasks.c`  
`timers.c`  
`utils/`

**Build & Run:**

```bash
# cd Module2
make clean
make
./freertos_app
```

In the terminal will see the output:

![Freehand Drawing.svg](assets/assignment2.png)

### Assignment Questions & Answers

As part of the assignment requirements, the following questions were posed. Below are answers based on the implemented system behavior:

1. **Why is `matrix_task` using most of the CPU utilization?**
> Because it performs heavy matrix multiplications and a large dummy delay loop, consuming most CPU time.

2. **Why must the priority of `communication_task` increase in order for it to work properly?**
> Since `matrix_task` has higher priority and runs frequently, it blocks `communication_task`. Increasing `communication_task` priority ensures it gets CPU time to meet its 200 ms deadline.

3. **What happens to the completion time of `matrix_task` when the priority of `communication_task` is increased?**
> The completion time of `matrix_task` increases because it is preempted more often by the higher-priority `communication_task`.

4. **How many seconds is the period of `matrix_task`?**
> The period of `matrix_task` is approximately 100 ms, controlled by `vTaskDelay(100)` calls and measurable via `vApplicationTickHook()`.
