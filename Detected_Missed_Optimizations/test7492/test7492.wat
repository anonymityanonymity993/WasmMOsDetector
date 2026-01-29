(module
  (import "External" "external_function" (func $external_function))
  (func $foo (result i32) i32.const 0)
  (func $bar (result i32) i32.const 0)
  (func $_start (param $0 i32) (param $1 i32) (result i32)
    (local $2 i32) (local $3 i32) (local $5 i32) (local $6 i32) (local $7 i32) (local $8 i32) (local $9 i32) (local $10 i32) (local $11 i32) (local $12 i32) (local $13 i32) (local $14 i32) (local $15 i32) (local $16 i32) (local $4 i32)
    call $bar
    local.set $6
    i32.const 1
    local.set $2
    block  ;; label = @1
      local.get $6
      br_if 0 (;@1;)
      call $foo
      local.set $8
      i32.const 0
      local.set $9
      local.get $8
      local.get $9
      i32.le_u
      local.set $11
      i32.const 0
      local.set $3
      block  ;; label = @2
        local.get $11
        br_if 0 (;@2;)
        call $external_function
        i32.const 1
        local.set $3
      end
      local.get $3
      drop
      i32.const 0
      local.set $2
    end
    local.get $2
    drop
    i32.const 0
    i32.load align=1
    local.set $13
    block  ;; label = @1
      local.get $13
      br_if 0 (;@1;)
      block  ;; label = @2
        i32.const 0
        br_if 0 (;@2;)
        i32.const 0
        i32.const 0
        i32.store
        i32.const 1
        local.set $14
      end
      local.get $14
      local.set $16
    end
    local.get $16
    drop
    unreachable)
  (memory $0 258 258)
  (export "_start" (func $_start)))