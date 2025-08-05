# Note-Taking Workflow for This Repository

This document outlines a systematic workflow for studying the Linux kernel source code within this forked repository. The goal is to create a structured, context-aware, and version-controlled "notebook" for each topic of study.

The core philosophy is to use **Git branches** for topic isolation and **GitHub Pull Requests** as rich, self-contained research logs.

---

## The Workflow

### Step 1: Sync with Upstream

Before starting a new study session, ensure your local `master` branch is up-to-date with the official Linux repository (`upstream`).

```bash
# Add the official repository as 'upstream' (only needs to be done once)
git remote add upstream https://github.com/torvalds/linux.git

# Fetch the latest changes from upstream
git fetch upstream

# Switch to your master branch
git checkout master

# Rebase your master branch on top of the official master
git rebase upstream/master

# Force push to update your fork's master branch on GitHub
# (Use --force with caution; it's safe here as master is a mirror of upstream)
git push origin master --force
```

### Step 2: Create a Study Branch

For each new topic you want to investigate, create a dedicated branch from your up-to-date `master` branch. Use a consistent naming convention.

**Example:** To study the `fork()` system call.
```bash
# Create and switch to a new branch for your study topic
git checkout -b study/fork-syscall-implementation
```

### Step 3: Add Notes Directly in the Code

As you navigate the source code, add your notes and analysis directly as comments. To distinguish your notes from the original code comments, use a personalized prefix.

**Example (`kernel/fork.c`):**
```c
/*
 * NOTE-MyName: This is the core function for fork(). It handles the heavy
 * lifting of duplicating a process's resources.
 */
long _do_fork(unsigned long clone_flags,
              unsigned long stack_start,
              /* ... */)
{
    struct task_struct *p;
    /* ... */

    /*
     * NOTE-MyName: The first major step is duplicating the task_struct.
     * copy_process() is where most of the magic happens.
     */
    p = copy_process(clone_flags, stack_start, stack_size,
                     parent_tidptr, child_tidptr, tls, NUMA_NO_NODE,
                     &args);
    
    /* ... */
}
```

### Step 4: Commit Your Progress

Periodically commit your notes to the study branch with clear, descriptive messages.

```bash
# Stage the files you've modified
git add kernel/fork.c

# Commit your notes
git commit -m "docs(fork): Add initial notes on _do_fork implementation"
```

### Step 5: Create a Pull Request as Your "Notebook"

This is the core of the workflow. You will create a Pull Request (PR) that targets your own repository. **This PR is for documentation and review, not for merging.**

1.  Push your study branch to your fork on GitHub:
    ```bash
    git push origin study/fork-syscall-implementation
    ```
2.  Go to your GitHub repository page. A prompt to create a Pull Request from the new branch will appear. Click it.
3.  **Configure the Pull Request:**
    *   **Base repository:** `your-username/linux`
    *   **Base branch:** `master`
    *   **Compare branch:** `study/fork-syscall-implementation`

    > **CRITICAL:** Ensure the PR targets your own fork, **NOT** the upstream `torvalds/linux` repository.

4.  **Write a Detailed Description:** Use the PR description as your high-level notebook. Include:
    *   A clear title (e.g., `Study Log: Deep Dive into the fork() System Call`).
    *   Your overall goal for this topic.
    *   A checklist to track your progress.
    *   High-level architecture diagrams (Mermaid syntax is supported).
    *   Questions, hypotheses, and links to external resources (e.g., LWN articles).

5.  Click **"Create pull request"**.

### Step 6: Iterate and Review

Use the created PR as your interactive study environment:

*   **Files Changed Tab:** Review your notes in a clean diff view. This is excellent for seeing your thoughts in context.
*   **Line Comments:** Add comments directly on code lines within the PR to ask yourself questions, add further details, or connect different parts of the code.
*   **Continuous Updates:** As you discover more, simply add more notes, commit, and push to the same branch. The PR will update automatically.

---

## Lifecycle of a Study Topic

Once you feel you have sufficiently explored a topic, you can **Close** the Pull Request **without merging it**. This keeps your `master` branch clean and pristine.

Then, you can go back to Step 1 to sync your repository and start a new study branch for your next topic of interest.

This workflow provides a powerful, organized, and motivating way to tackle a codebase as massive and complex as the Linux kernel.

Happy exploring!
