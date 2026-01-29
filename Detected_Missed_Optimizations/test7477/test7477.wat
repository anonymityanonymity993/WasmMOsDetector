(module
  (import "External" "external_function" (func $external_function ))
  (func $_start  (result i32)
    i32.const 0
    i32.const 0
    call $func
    )
  (func $func(param $1 i32) (param $2 i32) (result i32)
    call $foo
    i32.const 1
    i32.and
    local.set $2
    i32.const 0
    local.set $1
    local.get $2
    i32.const 1
    i32.or
    if (result i32)  ;; label = @1
      local.get $1
    else
      call $external_function
      i32.const 0
    end)
  (func $foo (result i32)
    i32.const 0
    i32.load)
  (memory $0 258 258)
  (export "_start" (func $_start)))