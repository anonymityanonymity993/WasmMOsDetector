(module
  (import "External" "external_function" (func $external_function))
  (func $_start 
    (param $0 i32) (param $1 i32)  (param $4 i32) (result i32)
    (local $5 i32) (local $6 i32)
    i32.const 0
    call $foo
    local.set $6
    i32.const -1
    local.set $5
    local.get $5
    local.get $6
    i32.or
    local.set $1
    i32.const 0
    local.set $0
    block  ;; label = @1
      local.get $1
      br_if 0 (;@1;)
      call $external_function
    end
    block  ;; label = @1
      local.get $0
      br_if 0 (;@1;)
    end
    unreachable)
  (func $foo (param $0 i32) (result i32)
    i32.const 0
    i32.load
    return)
  (memory 1)
  (export "_start" (func $_start)))