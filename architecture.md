# Architecture  

This document describes the high-layer architecture of win-vind. If you would like to contribute, a read will help you understand the files you need to edit.  


## KeyEvent Overview  

<img src="https://github.com/pit-ray/pit-ray.github.io/blob/master/win-vind/imgs/keyevt_overview.jpg?raw=true" >  


## KeyAbsorber  

**Location:** `core/src/common/key_absorber.cpp`  
KeyAbsorber has a low-level hook function to absorb key events, and keeps the received key codes as the key state.  


## KeyLogger  

**Location:** `core/src/common/key_logger.cpp`, `core/src/common/key_log.cpp`  
KeyLogger is actually a std::vector of KeyLog class, but its utility functions manages when to keep a log.  


## KeyBinder  

**Location:** `core/src/key_binder.cpp`  
KeyBinder holds a list of BindedFunc, and has functions to parse bindings.json to assign hotkeys and to scan a list.  


## BindedFunc  

**Location:** `core/src/common/binded_func.cpp`, `core/src/common/key_matcher.cpp`  

BindedFunc is a base class for polymorphism, which has a mechanism to determine if the KeyLogger matches the assigned bindings.  


## Derived classes from BindedFunc  

**Location:** `core/src/*_bindings`  
All pre-defined functions of win-vind are defined in derived classes from BindedFunc or BindedFuncWithCreator. If you want to add a new pre-defined function, create a new derived class.   
