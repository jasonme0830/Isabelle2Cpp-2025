`reduce-cond`

```
primrec (nonexhaustive) nth :: "'a list => nat => 'a" (infixl "!" 100) where
nth_Cons: "(x # xs) ! n = (case n of 0 \<Rightarrow> x | Suc k \<Rightarrow> xs ! k)"
```

非全函数似乎进行了标注。

`use-deque`

使用 `deque` 替换 `list`
