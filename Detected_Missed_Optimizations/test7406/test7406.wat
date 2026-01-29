(module
  (import "External" "external_fucntion" (func $external_function))
  (func $_start (result i32) (local i32 i32 i32)
    ;; v0 = load(offset=12)
    local.get 0
    i32.load offset=12
    local.set 0
    ;; v1 = const(0)
    i32.const 0
    local.set 1
    ;; if v0 == 0:
    ;;    v1 = const(1)
    block ;; label = @1
      local.get 0
      br_if 0 (;@1;)
      i32.const 1
      local.set 1
    end
    ;; v2 = const(0)
    ;; v2 = v1 && v2
    i32.const 0
    local.set 2
    local.get 1
    local.get 2
    i32.and
    ;; Regardless of local 1 (which is 0 or 1),
    ;; local 2 is set to 0 after the i32.and operation.
    local.set 2 ;; Now the local 2 is zero
    ;; v0 = const(0)
    i32.const 0
    local.set 0
    ;; if v2 != 0:
    ;;    call
    block ;; label = @1
      ;; Since local 2 is always 0, so i32.eqz evaluates to 1 (true).
      ;; Thus the branch skips the call instruction
      local.get 2
      i32.eqz
      br_if 0 (;@1;)
      call $external_function
    end
    ;; Return local 0 const(0)
    local.get 0)
  (memory (;0;) 1)
  (export "_start" (func $_start)))