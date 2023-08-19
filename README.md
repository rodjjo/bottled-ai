# bottled-ai
Storyteller

## Description

This is a chatbot that runs locally using your GPU. 
It supports several models, and more models are going to be added soon.

*Main window*   
![Ubuntu screenshot](https://github.com/rodjjo/bottled-ai/raw/main/docs/images/preview.jpg)


*Model downloader*   
![Ubuntu screenshot](https://github.com/rodjjo/bottled-ai/raw/main/docs/images/preview2.jpg)

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
