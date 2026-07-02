# Work Queues and Task Scheduling

## Work Queue

In client - server based application, when you want to ask server to do something, client will request to server, server do the job for you, client waiting until the job is finished.

But imagine if the job is a long running job, for example processing a big image, it will take time like in minutes, will the client want to wait that long?

It will ruin the UX of course.

Instead, we can just make client tell server what to do, and then not waiting the server do the job, it's like fire and forget

```
Main Thread

Task A
Task B
Task C
Task D
   |
   V

+-------------------+
|    Work Queue     |
+-------------------+
| A                 |
| B                 |
| C                 |
| D                 |
+-------------------+
```

Workers continuously pull tasks from this queue.

```c++
while (true)
{
    Task task = queue.pop();
    task();
}
```

## Task Scheduling

Task Scheduling makes basically these 3 decision:
- Which task should run next
- How long the task should be run
- When to interrupt a running task for something more important

Usually this mostly on OS side, there so many Task Scheduling algorithm out there, each of them has their own pros and cons, there's no best task scheduling overall, each of them has their own use cases.

## Source

https://medium.com/@coderoyalty/why-you-need-a-job-queue-d1f50df5bbe1

https://www.geeksforgeeks.org/operating-systems/cpu-scheduling-in-operating-systems/