### Remodeling Bindings Class (9/30)
- pass following structure as argument to sprocess()
|type|name|note|
|---|---|---|
|bool|first_call|as before|
|int|num|for repeating commands|
|const KeyLogger* const|parent_logger|the process is able to have key matching loop|
|const std::string|cmd|caller command|

- has `is_for_moving_caret` function as virtual function with default process for combined commands(e.g. `c`, `d`).
- fuse KeyBinding and Command as one class.
- we can design packaged key matching system.
