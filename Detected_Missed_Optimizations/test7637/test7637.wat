(module
  (import "External" "external_function" (func $external_function))
  (func $_start call $main)
  (func $main
  (local $0 i32)
    (local $1 i32)
    block  ;; label = @1
      block (result i32)  ;; label = @2
        i32.const 1
        local.set $0
        i32.const 0
      end
      br_if 0 (;@1;)
      i32.const 1
      local.set $1
    end
    local.get $1
    local.set $0
    i32.const 1
    local.set $1
    block  ;; label = @1
      local.get $0
      br_if 0 (;@1;)
      i32.const 0
      i32.load8_u
      local.tee $0
      br_if 0 (;@1;)
      call $external_function
      i32.const 0
      local.set $1
    end
    i32.const 0
    local.get $1
    local.tee $0
    i32.store)
  (memory $0 258 258)
  (export "_start" (func $_start)))
