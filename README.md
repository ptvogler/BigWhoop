<p align="center">
  <a href="https://github.com/ptvogler/BigWhoop">
    <img alt="BigWhoop" src="https://github.com/ptvogler/BigWhoop/blob/main/docs/img/Logo.svg"/>
  </a>
</p>
<h1 align="center">Compression library for numerical datasets</h1>


BigWhoop is a compression library for numerical datasets that was developed as part of the EU projects ExaFLOW and EXCELLERAT, as well as the BMFTR Project TopIO. It aims to give scientists and egineers a tool to drastically reduce the size of their simulation data while minimizing the distortion introduced by a lossy compression scheme.


<h1 align="center">Building BigWhoop</h1>

### Dependencies

* make
* cmake (>= 3.5.1)
* gcc (>= 8.5.0)

### Building

clone the sources from HLRS' gitea instance (note: default branch is `main`):
```
git clone https://code.hlrs.de/TOPIO/BigWhoop.git
```

Now change into the source directory and run the following commands.

**for a simple build**
```
make clean
make
```
**for debugging**
```
make debug
```
**for a full build (including command line tool)**
```
make cmdl
```
**for debugging (including command line tool)**
```
make cldebug
```

This builds the library and places the associated files in the `build/lib` or `build/lib64` folders. If a full build was attempted, the command line tool binaries are placed in the `build/bin` folder.

### Credits

This project exists thanks to all the people who contribute.

- Patrick Vogler patrick.vogler@hlrs.de (principal author)
- Gregor Weiss gregor.weiss@hlrs.de 
