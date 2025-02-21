1. **Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?**

   **Answer:**  
   Yeah, I believe the reason we use `fork` before calling `execvp` is that `execvp` alone would completely replace the current process image with that of the new program. It works like this: by calling `fork`, the shell creates a separate child process that runs the external command while the parent (our shell) remains intact. This separation is crucial because it allows the shell to continue running, manage multiple commands, and collect the exit status of the child. Essentially, `fork` provides process isolation and control—key for features like job control and ensuring our shell stays responsive.

2. **What happens if the fork() system call fails? How does your implementation handle this scenario?**

   **Answer:**  
   Yeah, I believe if `fork()` fails, it returns a negative value (usually -1) and sets the `errno` variable to indicate the specific error (like a lack of resources). In our implementation, we check for this condition. If `fork()` fails, we use `perror("fork")` to output an error message, free any allocated resources (like our command buffer), and then continue to the next loop iteration. This approach ensures that the shell doesn’t crash and can still process future commands even if a fork fails.

3. **How does execvp() find the command to execute? What system environment variable plays a role in this process?**

   **Answer:**  
   It works like this: `execvp()` looks for the command in the directories listed in the `PATH` environment variable. I believe this variable, which contains a colon-separated list of directories, tells `execvp()` where to search for executables. This allows users to run commands by simply typing their name instead of the full path, as `execvp()` goes through each directory in `PATH` until it finds the executable.

4. **What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?**

   **Answer:**  
   Yeah, I believe the purpose of calling `wait()` (or `waitpid()`) is to make sure the parent process—the shell—pauses and waits for the child process to finish executing. This is important because it allows the shell to capture the exit status of the command and prevent the child from becoming a zombie process. Without calling `wait()`, the parent would continue immediately, possibly leading to resource leaks and the inability to properly track the success or failure of commands.

5. **In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?**

   **Answer:**  
   It works like this: `WEXITSTATUS()` extracts the actual exit code from the status value returned by `wait()` or `waitpid()`. I believe this is important because it tells us whether the child process executed successfully or if it encountered an error. The exit code is then used by our shell (or even by users in scripts) to make decisions based on the success or failure of a command.

6. **Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?**

   **Answer:**  
   Yeah, I believe our `build_cmd_buff()` function is designed to treat any sequence of characters enclosed in double quotes as a single argument. It works like this: when the parser sees a starting double quote, it collects all characters until the closing quote, preserving spaces and other characters inside. This is necessary because many commands require arguments that contain spaces (for example, file names or messages), and without this handling, those arguments would be split incorrectly, leading to errors in command execution.

7. **What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?**

   **Answer:**  
   Yeah, I believe the main change was moving from using a command list (`command_list_t`) to using a single command buffer (`cmd_buff_t`). This required more robust handling of whitespace—trimming leading and trailing spaces, removing duplicate spaces outside quotes, and properly preserving spaces within quotes. An unexpected challenge was ensuring that the parser correctly differentiates between spaces meant to separate arguments and spaces that are part of a quoted argument. Balancing these requirements while keeping the code efficient and error-free was a bit tricky, but ultimately it led to a more streamlined design.

8. **Linux Signals Research**

   - **What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?**

     **Answer:**  
     Yeah, I believe signals are used as a mechanism to asynchronously notify a process about events like interrupts, termination requests, or errors. They differ from other IPC methods—such as pipes or message queues—in that they carry very little information (just a signal number) and are meant for immediate, lightweight notifications rather than large-scale data exchange. Signals are a fast and efficient way to inform processes about events that require urgent attention.

   - **Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?**

     **Answer:**  
     It works like this:
     - **SIGKILL (Signal 9):** This signal forcefully terminates a process immediately and cannot be caught, blocked, or ignored. It’s used as a last resort to stop processes that are unresponsive.
     - **SIGTERM (Signal 15):** This signal requests a process to terminate gracefully, allowing it to perform cleanup operations before shutting down. Unlike SIGKILL, it can be handled by the process.
     - **SIGINT (Signal 2):** Typically generated by pressing Ctrl+C in a terminal, SIGINT interrupts a process and allows it to handle the interruption (such as by saving state or prompting the user) before exiting.

   - **What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?**

     **Answer:**  
     Yeah, I believe when a process receives SIGSTOP, it is immediately suspended by the operating system. Unlike SIGINT, SIGSTOP cannot be caught, blocked, or ignored. This is because SIGSTOP is designed to unconditionally pause a process, which is important for system utilities and debuggers that need to reliably control process execution. This non-catchable behavior ensures that processes can always be halted when necessary, regardless of their internal state.
