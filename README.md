<p align="center">
  <a href="https://code.hlrs.de/hpcpvogl/BigWhoop">
    <img alt="BigWhoop" src="https://code.hlrs.de/TOPIO/BigWhoop/raw/branch/main/docs/img/Logo.svg"/>
  </a>
</p>
<h1 align="center">Compression library for numerical datasets</h1>


BigWhoop is a compression library for numerical datasets that has been developed as part of the EU Projects ExaFLOW and EXCELLERAT. It aims to give scientists and egineers a tool to drastically reduce the size of their simulation data while minimizing the distortion introduced by a lossy compression scheme.


<h1 align="center">Building BigWhoop</h1>

### Dependencies

* make
* cmake (>= 3.5.1)
* gcc (>= 8.5.0)

### Building

pull the sources from HLRS' gitea instance (note: default branch is `main`):
```
git pull https://code.hlrs.de/hpcpvogl/BigWhoop.git
```

Now change into the source directory and run the following commands.

**for a simple build**
```
make clean
make
```
**for a full build (including command line tool)**
```
make full
```
**for debugging**
```
make clean
make debug
```
This builds the library and places the associated files in the `/lib` or `/lib64` folders. If a full build was attempted, the command line tool binaries are placed in the `/bin` folder.
