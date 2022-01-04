# spkd
PC Speaker Driver Program

## Synopsis
This repository contains the MS-DOS driver source code for my DIY [ISA PC Speaker Driver](http://www.alexandrugroza.ro/microelectronics/system-design/isa-pc-speaker-driver/index.html) card that you can find on the [Microelectronics](http://www.alexandrugroza.ro/microelectronics/index.html) page on my site.

The program allows the control of the output relay, switches between amplified and non-amplified PC Speaker modes, and ajdusts the digital volume control in amplified PC Speaker mode.

Since this is a very small program, I chose to write it entirely in x86 assembly language. Besides execution speed, which is out of question anyway on such a simple program, the main benefit is that the output executable file is only around 2.5 Kb. And since I like optimized code, you get the idea. If I were to write this program in Pascal, only the runtime code would've been about twice the size of my entire assembled program.

In other words, I also wanted to de-rust my x86 assembly skills. Naturally I had a lot of fun programming this driver and I had to reference to Michael Abrash's book a few times. I read it in the past but a lot of sections wiped from my memory throughout time.

Given that I designed the PC Speaker Driver hardware as an 8-bit ISA card, it might be replicated by XT-class machine lovers out there. And these computers often have low storage space and low code execution speed. So I think an assembly program would be a better idea in this case.

I am using the PC Speaker Driver hardware and software with my [32-bit 80386DX ISA Single Board Microcomputer](http://www.alexandrugroza.ro/microelectronics/system-design/isa-80386dx-sbmc/index.html). Thus code execution speed is trivial in my case. Also I have plenty of solid-state storage attached to this machine.

I am releasing my work under the GNU General Public License v3.0 terms and conditions, for educational and documentation purposes.

### Program Usage

The following lines are taken directly from the commandline help screen.

```
Usage is: spkd.com [-?|-h(elp)] [-a=300-31F] [-c=0-7] [-q]

Where:
  -?|-h   shows this screen
  -a=XXX  hexadecimal ISA card I/O address
  -c=X    decimal hardware command number
  -q      reduces text verbosity

Commands reference:
  0 = output relay flip
  1 = output relay on
  2 = output relay off
  3 = mode relay flip
  4 = mode relay on
  5 = mode relay off
  6 = volume down
  7 = volume up
```
