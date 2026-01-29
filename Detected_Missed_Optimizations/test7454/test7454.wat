(module
  (import "External" "external_function" (func $external_function))
  (func $_start (result i32)
    (local i32 i32 i32 i32 i32 i32 i32 i32 i32)
    call $foo
    local.set 1
    i32.const 1
    local.set 2
    local.get 1
    local.get 2
    i32.and
    local.set 3
    i32.const -556321873
    local.set 4
    local.get 3
    local.get 4
    i32.ge_u
    local.set 5
    i32.const 0
    i32.load
    local.set 6
    local.get 5
    local.get 6
    i32.and
    local.set 7
    i32.const 0
    local.set 0
    block  ;; label = @1
      local.get 7
      i32.eqz
      br_if 0 (;@1;)
      call $external_function
      i32.const 0
      local.set 0
    end
    local.get 0
    local.set 8
    i32.const 0
    local.get 8
    i32.store
    unreachable)
  (func $foo (result i32)
    i32.const 0
    i32.const 0
    i32.store8
    i32.const 0)
  (memory (;0;) 258 258)
  (export "_start" (func $_start)))
