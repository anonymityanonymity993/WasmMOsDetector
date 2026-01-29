(module
  (import "External" "external_function" (func $external_function))
  (func $_start
    (local i32)
    global.get $g
    i32.const 0
    i32.load
    if
      unreachable
    end
    if
      call $external_function
    end
    unreachable)
  (memory (;0;) 256 256)
  (global $g (mut i32) (i32.const 0))
  (export "_start" (func $_start)))