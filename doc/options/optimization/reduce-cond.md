## 选项 `--reduce-cond`

**功能**

移除掉最后一个 pattern 的条件判断。

**其它**

Isabelle/HOL 中由 `fun` 和 `function` 定义的函数要求是 total 的，但是 `primrec` 定义的函数可以不是 total 函数，但是会报警告，Isabelle/HOL 中可以通过选项 `nonexhaustive` 声明函数非 total 避免警告。因此，现在 `--reduce-cond` 开启的时候，会假设没有标记 `nonexhaustive` 的函数是 total 的。就可以移除掉最后一个 pattern 的条件。
