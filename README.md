# bottled-ai
Storyteller


## Description

This is a chatbot that runs locally using your GPU (GPTQ models) or CPU (GGML models). 
It supports several models, and more models are going to be added soon.

*Main window*   
![Ubuntu screenshot](https://github.com/rodjjo/bottled-ai/raw/main/docs/images/preview.jpg)


*Model downloader*   
![Ubuntu screenshot](https://github.com/rodjjo/bottled-ai/raw/main/docs/images/preview2.jpg)

## Installation


Platforms:
* Windows - There is an installer here: [Latest Release (windows)](https://github.com/rodjjo/bottled-ai/releases/download/v0.2.0/Bottled-AI-Setup.exe)
* Linux - Work In Progress
* Mac - Future (Maybe)

**Note**: Do not install inside `Program files` or other directory that requires admin access to write. The program download models inside its directory.

[Other Release](https://github.com/rodjjo/bottled-ai/releases)

## Build and run 

Before start make sure you have 7zip, cmake, python 3.10 and Conan 1.59.0 installed.  
The build.py script will download the toolset and all the dependencies.

Building the project
```bash
python devtools/build.py
```

Clear the build files and build it again
```bash
python devtools/rebuild.py
```

Run the project
```bash
# run:
python devtools/run.py --copy-stuff
# build and run:
python devtools/run.py --build --copy-stuff
```

## Credits
  
**FLTK**
* [User-Interface](https://www.fltk.org/doc-1.3/)
