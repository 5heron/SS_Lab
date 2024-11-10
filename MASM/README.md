## Content

- **MASM**
  - [Sample Programs](Programs)
  - [Programs.pdf](masam_programs.pdf)
  - [Tools for execution](For_execution)
  
## Guide

Here's a guide to install DOSBox and set up MASM:

### Step 1: Install DOSBox
1. **Download DOSBox**: Go to [www.dosbox.com](https://www.dosbox.com/) and download the version for your OS.
2. **Install DOSBox**: Run the installer and follow the on-screen steps.

### Step 2: Set Up MASM
1. **Create a Folder**: Make a folder called `C:\MASM` and place the MASM files there.

### Step 3: Run DOSBox and Mount MASM
1. **Start DOSBox**.
2. **Mount the Folder**:
   - Type the following:
     ```bash
     MOUNT C C:\MASM
     C:
     ```
3. **Use MASM**:
   - To assemble: `MASM program.asm;`
   - To link: `LINK program.obj;`
   - Run your program: `program.exe`

### Optional: Automate Mounting
- Add `MOUNT C C:\MASM` to the end of `dosbox-<version>.conf` to mount automatically on startup.

You’re now set up to use MASM in DOSBox!

