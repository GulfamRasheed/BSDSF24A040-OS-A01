# BSDSF24A040-OS-A01 — Analysis Report

**Author:** Gulfam Rasheed — `gulfamrasheed06@gmail.com`

This report answers all the analysis questions from the assignment, feature by feature.

---

## Feature 2: Multi-file Project using Make Utility

### Q1. Explain the linking rule in this part's Makefile: `$(TARGET): $(OBJECTS)`. How does it differ from a Makefile rule that links against a library?

`$(TARGET): $(OBJECTS)` is a Make rule in which the executable target depends **directly** on every object file (`main.o`, `mystrfunctions.o`, `myfilefunctions.o`). When the rule runs, the recipe calls the compiler once with all of those objects, e.g.:

```
gcc main.o mystrfunctions.o myfilefunctions.o -o client
```

This is **direct linking** — the linker receives every `.o` file explicitly and copies all their code into the final executable.

A rule that links **against a library** is different because the target depends on the library file (`.a` or `.so`) instead of the individual function objects. The recipe does not list the function objects; it uses the `-L` (library search path) and `-l` (library name) flags:

```
gcc main.o -L../lib -lmyutils -o client
```

Here the linker is only told *where to search* (`-L`) and *which library to pull symbols from* (`-lmyutils`, which expands to `libmyutils.a` or `libmyutils.so`). So the difference is the packaging of the code: with plain objects the functions are separate `.o` files passed to the linker one by one, while with a library the functions are bundled into a single archive/shared file and the linker resolves them from there.

### Q2. What is a git tag and why is it useful in a project? What is the difference between a simple tag and an annotated tag?

A **git tag** is a named pointer to a specific commit. It is used to mark meaningful, stable points in the project history — such as a versioned release (`v0.1.1-multifile`, `v0.4.1-final`, ...). Tags are useful because they give a permanent, easy-to-remember name to a snapshot, so anyone (or any release process) can reproduce that exact state of the code even after many new commits.

The two kinds are:

- **Simple (lightweight) tag** — just a pointer stored directly at a commit hash. It carries no extra information (no tagger name, date, or message).
- **Annotated tag** — created with `git tag -a -m "message"`. Git stores a full tag object with the tagger's name, email, date, and a message, and it is signed-capable. For project releases an annotated tag is preferred because it records who created the tag and why.

### Q3. What is the purpose of creating a "Release" on GitHub? What is the significance of attaching binaries (like your client executable) to it?

A **GitHub Release** turns a tag into a distributable package: it adds a title, release notes/description, and downloadable assets. It gives users and the instructor an easy, official place to download a stable version without browsing the source or building it themselves.

Attaching the compiled **binary** (e.g. `bin/client`) is significant because it ships the ready-to-run program. Not every user has the build tools or wants to compile from source; attaching the executable means the release is immediately usable, and it proves the project builds and runs as a genuine, distributable artifact.

---

## Feature 3: Creating and using Static Library

### Q4. Compare the Makefile from Part 2 and Part 3. What are the key differences in the variables and rules that enable the creation of a static library?

In **Part 2** the Makefile compiled every `.c` file and linked them **directly** into one executable using the rule `$(TARGET): $(OBJECTS)`, where the recipe listed all object files in one `gcc` command.

In **Part 3** the Makefile was modified to build the library first and then link against it:

- New variables were introduced, mainly `STATIC_LIB = $(LIB_DIR)/libmyutils.a` and `LIB_OBJECTS` (the list of function objects).
- A new **archive rule** was added:

  ```
  $(STATIC_LIB): $(LIB_OBJECTS)
  ar rcs $(STATIC_LIB) $(LIB_OBJECTS)
  ```

  This bundles the function objects into `libmyutils.a` using `ar`.
- The executable rule changed to depend on the library and to link with the library flags:

  ```
  $(STATIC_TARGET): $(OBJ_DIR)/main.o $(STATIC_LIB)
  gcc $(OBJ_DIR)/main.o -L$(LIB_DIR) -lmyutils -o $(STATIC_TARGET)
  ```

So the three key differences are: (1) the `ar` archive rule that creates `libmyutils.a`, (2) the target now depends on the library instead of on all objects, and (3) the `-L`/`-l` flags that make the linker search and link against the archive.

### Q5. What is the purpose of the `ar` command? Why is `ranlib` often used immediately after it?

`ar` (**a**rchive) is the GNU utility that creates and manages archives — for our purpose, it bundles several object files into one static library file `libmyutils.a`. In our command `ar rcs libmyutils.a mystrfunctions.o myfilefunctions.o`:

- `r` — replace/insert members (object files) into the archive,
- `c` — create the archive if it does not exist,
- `s` — write a **symbol index** into the archive.

`ranlib` adds (or refreshes) that symbol index table inside an existing archive. The linker uses the index to find which object member contains a requested symbol without scanning every member. Many older toolchains do not put the index in with `r` alone, which is why `ranlib` is run right after `ar` — it guarantees the archive is indexed and linkable. (`ar s` is equivalent, which is why some Makefiles use `ar rcs` and skip a separate `ranlib`.)

### Q6. When you run `nm` on your `client_static` executable, are the symbols for functions like `mystrlen` present? What does this tell you about how static linking works?

Yes. Running `nm bin/client_static | grep mystrlen` shows an entry like `T mystrlen` in the executable's symbol table (studying the output of `nm` and `readelf` during the build confirmed the function is present).

This happens because **static linking copies the library code into the executable at link time**. The linker extracted the needed object members from `libmyutils.a` and physically embedded `mystrlen`, `mystrcpy`, `wordCount`, etc. into `client_static`. The executable therefore contains everything it needs and runs by itself — no library lookup is required at run time.

---

## Feature 4: Creating and using Dynamic Library

### Q7. What is Position-Independent Code (`-fPIC`) and why is it a fundamental requirement for creating shared libraries?

`-fPIC` (**P**osition-**I**ndependent **C**ode) tells `gcc` to generate machine code that can be loaded and executed at **any** memory address, without relocation/recompilation for a fixed address.

A shared library (`.so`) is not loaded at a predictable address: the operating system maps it into different addresses for different processes (typically sharing one physical copy among many processes). If the code used fixed absolute addresses, it could not be reused at a different location. `-fPIC` makes the compiler emit code that accesses its own global data/functions through indirection (GOT/PLT - Global Offset Table / Procedure Linkage Table), so the dynamic loader can relocate the library at load time wherever it wants. This is why building a shared object without `-fPIC` fails (or produces fragile, non-shareable code) — position independence is a fundamental requirement for shared libraries.

### Q8. Explain the difference in file size between your static and dynamic clients. Why does this difference exist?

On our build, `bin/client_static` was **16624** bytes while `bin/client_dynamic` was **16328** bytes — the dynamic client is noticeably smaller than the static one.

The reason is what each executable contains:

- **Static client**: the linker copied the whole needed library code from `libmyutils.a` **into** the executable. All the string and file functions are physically present in `client_static`, which is why it is bigger.
- **Dynamic client**: the executable contains **only a reference** to `libmyutils.so` (a `DT_NEEDED` entry) — the library code is *not* embedded. The functions are loaded into memory at run time by the dynamic loader from the single shared copy.

So the size difference exists because static linking embeds the library code into each executable, while dynamic linking defers the code to a shared file loaded at run time.

### Q9. What is the `LD_LIBRARY_PATH` environment variable? Why was it necessary to set it for your program to run, and what does this tell you about the responsibilities of the operating system's dynamic loader?

`LD_LIBRARY_PATH` is an environment variable that lists the directories in which the dynamic loader (`ld.so` / `ld-linux`) searches for shared libraries **at run time**.

When we first ran `./bin/client_dynamic` we got the expected error `cannot open shared object file` because, by default, the loader only checks the standard system paths (`/usr/lib`, `/lib`, etc.) — our custom `lib/` directory was not among them, so it could not find `libmyutils.so`.

Setting the variable fixed it:

```
export LD_LIBRARY_PATH=$PWD/lib:$LD_LIBRARY_PATH
./bin/client_dynamic
```

Running `ldd bin/client_dynamic` then showed that `libmyutils.so` resolved from our project's `lib/` directory. This demonstrates a key responsibility of the OS's **dynamic loader**: at run time it must locate every needed shared library (using the executable's recorded `DT_NEEDED` entries plus the user/system library search paths), map it into the process address space, perform relocations, and only then start the program — and if it cannot find a library, it refuses to run the program.

---

## Feature 5: Man Pages & Installation

For this feature a `man/man3/` directory was created containing a man page for each library function (`mystrlen.3`, `mystrcpy.3`, `mystrncpy.3`, `mystrcat.3`, `wordCount.3`, `mygrep.3`), written in groff format with the standard `.TH`, `.SH NAME`, `.SH SYNOPSIS`, `.SH DESCRIPTION`, and `.SH AUTHOR` sections. An `install` target was added to the Makefile which copies the dynamically linked client and the shared library into `/usr/local/bin` and `/usr/local/lib`, refreshes the linker cache with `ldconfig`, and installs the man pages into `/usr/local/share/man/man3`.

---

## Final Notes

All work from Features 1–6 has been merged into the `main` branch, the analysis report is completed, and every development branch (`multifile-build`, `static-build`, `dynamic-build`, `man-pages`) has been pushed to GitHub so the full history of the project is visible for grading.