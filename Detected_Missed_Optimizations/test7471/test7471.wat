(module
  (import "External" "external_function" (func $external_function))
  (func $foo (result i32)
    (local i32)
    i32.const 0
    local.get 0 
    i32.div_u
    return)
  (func $_start
    block
      call $foo
      i32.eqz
      br_if 0
      call $external_function
    end)
  (export "_start" (func $_start)))