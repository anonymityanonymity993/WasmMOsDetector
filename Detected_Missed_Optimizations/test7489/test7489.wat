(module
  (import "External" "external_function" (func $external_function))
  (func $_start 
    i32.const 0
    i32.const 0
    call $foo)
  (func $foo (param $0 i32) (param $2 i32) 
    block ;; label = @1
      i32.const 0
      i32.load
      drop
      i32.const 1
      i32.load
      local.set $2
      i32.const 1
      local.set $0
      local.get $2
      br_if 0 (;@1;)
      i32.const 1
      local.set $0
      local.get $0
      br_if 0 (;@1;)
      i32.const 0
      local.set $0
    end
    block ;; label = @1
      local.get $0
      br_if 0 (;@1;)
      call $external_function
    end)
  (memory $0 258 258)
  (export "_start" (func $_start)))