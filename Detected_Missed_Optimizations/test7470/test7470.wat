(module
  (import "External" "external_function" (func $external_function))
  (func $_start (param $0 i64) (param $1 i32) (param $2 i32) (param $3 i32)
    (local $4 i32) (local $5 i32)
    i32.const 0
    call $foo
    drop
  )
  (func $foo (param $0 i32) (result i32)
    (local $1 i32) (local $2 i32) (local $4 i32) (local $5 i32) (local $scratch i32) (local $3 i64) (local $6 i64)
      i32.const 77986
      i32.load16_u
      i32.const 16
      local.tee $2
      i32.shl
      local.get $2
      i32.shr_s
      i32.const 65533
      i32.eq
      i32.const 77986
      i32.load16_u
      i32.const 16
      i32.shl
      i32.and
      if (result i32)  ;; label = @4
        call $external_function
        i32.const 0
      else
        i32.const 1
      end
    )
  (memory $0 258 258)
  (export "_start" (func $_start)))