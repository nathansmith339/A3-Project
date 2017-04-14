# Vague Instructions
1. Start up VS 2015 (or whatever version)
2. If your Team Explorer tab does not have github integration, follow the next steps
    * Under **Tools > Extensions and Updates > Online (left panel)**, search for the Github extension.
    * Install and restart VS
    * Open the Github window under **View > Other Windows > Github** and log in if necessary.
3. **Clone** the project into a local repo on your PC using Team Explorer.
4. The .sln file should show up in the tab to open. When you open that, VS should function as normal.

--

### Using Team Explorer
- When new files are added, we must **fetch** them from the server, and **pull** them into our project.
- When making changes, create a new **branch** (to be merged when finished). **Changing the master branch is not a great idea.**
- **Commit**ting changes should be straightforward, submit **commits** and **pull** requests as needed.
- If you have odd issues committing changes, **ensure .gitignore is present.**

### Adding files
- Make a file like normal, do whatever, then you can **commit** the changes. 
- Ensure you **push** outgoing changes and **sync** to add it to the server.
