(module
  (import "External" "external_function" (func $external_function))
  (func $_start
    (local $0 i32) (local $3 i32) (local $5 i32) (local $6 i32) (local $11 i32) (local $1 i32) (local $2 i32) (local $7 i32)
    loop ;; label = @1
      block ;; label = @2
        i32.const 0
        i32.load
        br_if 0 (;@2;)
        i32.const 0
        local.get $11
        i32.store
        i32.const 0
        local.set $5
        i32.const 1
        local.set $0
        i32.const 0
        local.get $0
        i32.store
        i32.const -346457217
        local.set $6
        block ;; label = @3
          local.get $0
          local.get $6
          i32.ne
          br_if 0 (;@3;)
          call $external_function
        end
        local.get $5
        i32.const 0
        i32.load
        i32.lt_s
        i32.const 0
        local.set $1
        local.get $1
        i32.shl
        drop
        local.get $1
        unreachable
      end
      i32.const 0
      i32.load
      local.set $2
      local.get $2
      local.set $3
      local.get $2
      local.get $3
      i32.store
      br 0 (;@1;)
    end)
  (memory $0 1)
  (export "_start" (func $_start)))