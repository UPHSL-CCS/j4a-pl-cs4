# 📦 Concurrency Models Activity

## 📚 Overview
This repository contains our group’s submission for the **Concurrency Models Activity**. The primary **objective** was:
> "To analyze and demonstrate concurrency models using a programming language of choice."

We implemented and explored the following concepts in **Python** and **C++** (as per the group's current work):

- **Concurrency:** Running multiple tasks seemingly at the same time.
- **Parallel Tasks:** Implementing at least two tasks (functions) to execute concurrently.

The code is located in the `/concurrency/` folder. Each member contributed code or a reflection to meet the activity requirements, including the mandatory **incremental Git commit** structure.

---

## ✨ Reflection

### Angela (Python)

* **Explanation of the concurrency model used.**
This program uses Python’s threading model to make two tasks run at the same time. One thread is in charge of the timer countdown, while the main thread waits for the player to press ENTER to catch the fish. Both tasks happen together, which means the timer continues even while the program waits for user input. This shows how concurrency works by allowing different parts of a program to run in parallel without stopping each other.

* **Challenges faced when implementing concurrency.**
At first, I found it hard to understand concurrency because I rarely use threads in my programs. I had to learn how to make two things happen at the same time and still work correctly. I got the idea for my project from Animal Crossing’s fishing game, where you only have a few seconds to catch a fish, so I used that concept here. I also found it a bit challenging to do Git merges at first, but I’m slowly getting used to it and learning along the way.

### Adam (C++)

* **Explanation of the concurrency model used.**
For C++, the concurrency model is used through std::thread. The main function starts and would start to call two threads under each function. The first function will print out the odd numbers while the second prints out even the even number. When both task are finished, they will be joined in the main function to be printed. This shows of two task in parallel combined to count from 1-21

* **Challenges faced when implementing concurrency.**
When testing out concurrency, I had a hard time for the result to look organize since the result is fusing one output to another , making it look like a mess. Another challenge I observed is that I wanted the results from both task to interleave each other, but only resulted to either the 1st task would always be the first to print out or they're all jumbled randomly

### Michaela (Documentation & Git)

* **Explanation of the concurrency model used.**
[Leave blank for Michaela's answer]

* **Challenges faced when implementing concurrency.**
[Leave blank for Michaela's answer]

---

## 👥 Group Members & Contributions

- Angela Cabanes – **Python** (Threading Implementation)
- Kelvin Adam Aninang – **C++** (Threading Implementation)
- Michaela Jornales – README consolidation and Git workflow management
